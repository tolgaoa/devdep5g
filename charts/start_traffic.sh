#!/bin/bash

gnbsimc=11
gnbsims=11

nousers=$1
aggr=$2
rep=$3

repnet=100

iperfpodnumber=10
runs=1

rm -r logs/gaming/compute/$aggr/
mkdir logs/gaming/compute/$aggr
rm -r logs/gaming/throughput/$aggr/
mkdir logs/gaming/throughput/$aggr

upfc=10
for ((r=1;r<=1;r++))
do
	avglogs=logs/gaming/throughput/$aggr/throughput.avg$r.log.txt

	iperfdnnpod=$(kubectl get pods -n oai | grep oai-dnn$iperfpodnumber | awk '{print $1}')
	iperfgnbsimpod=$(kubectl get pods -n oai | grep oai-gnbsim$iperfpodnumber | awk '{print $1}')
	kubectl exec -n oai $iperfgnbsimpod -- iperf3 -s &

	upfpod=$(kubectl get pods -n oai  | grep spgwu-tiny$upfc | awk '{print $1}')

	gnbsimc=11
	for ((sim=1;sim<=nousers;sim++))
	do
		((gnbsimIP=$sim+2))

		dnnpod=$(kubectl get pods -n oai  | grep oai-dnn$gnbsims | awk '{print $1}')
		gnbsimpod=$(kubectl get pods -n oai  | grep oai-gnbsim$gnbsims | awk '{print $1}')

		echo "Starting Downlink and Uplink for $dnnpod and $gnbsimpod"
		#------------Downlink traffic----------------
		echo "Starting Downlink on 12.1.1.$gnbsimIP"
		kubectl exec -n oai $gnbsimpod -- python3 udpserverclient/server.py 12.1.1.$gnbsimIP 20001 &
		sleep 1	
		kubectl exec -n oai $dnnpod -- python3 udpserverclient/client.py 12.1.1.$gnbsimIP 20001 $aggr DL & 
		sleep 1
		echo "Started Downlink"
		#------------Uplink traffic------------------
		echo Starting Uplink on 10.$repnet.25.$gnbsimc""
		kubectl exec -n oai $dnnpod -- python3 udpserverclient/server.py 10.$repnet.25.$gnbsimc 20002 &
		sleep 1
		kubectl exec -n oai $gnbsimpod -- python3 udpserverclient/client.py 10.$repnet.25.$gnbsimc 20002 $aggr UL & 
		sleep 1
		echo "Started Uplink"

		echo "Started current user. Waiting metrics-server resolution window (15s) and gathering compute logs"
		sleep 15
		
		upfdata=$(kubectl top pod -n oai $upfpod)
		dnndata=$(kubectl top pod -n oai $dnnpod)
		gnbsimdata=$(kubectl top pod -n oai $gnbsimpod)

		k="0"
		while [ $k -lt 4 ]
		do
			upfdata=$(echo $upfdata | awk '{$1=""}1' | awk '{$1=$1}1')
			((k++))
		done
		echo "$upfdata" | sed 's/[A-Za-z]*//g' >> logs/gaming/compute/$aggr/compute_logs$gnbsims.txt

		k="0"
		while [ $k -lt 4 ]
		do
			dnndata=$(echo $dnndata | awk '{$1=""}1' | awk '{$1=$1}1')
			((k++))
		done
		echo "$dnndata" | sed 's/[A-Za-z]*//g' >> logs/gaming/compute/$aggr/compute_logs$gnbsims.txt

		k="0"
		while [ $k -lt 4 ]
		do
			gnbsimdata=$(echo $gnbsimdata | awk '{$1=""}1' | awk '{$1=$1}1')
			((k++))
		done
		echo "$gnbsimdata" | sed 's/[A-Za-z]*//g' >> logs/gaming/compute/$aggr/compute_logs$gnbsims.txt

		echo "Gathered logs. Waiting for 5s and executing throughput test"
		sleep 5

		log=logs/gaming/throughput/$aggr/iperf.log

		if [ -f $log ]; then
		  echo removing $log
		  rm $log
		fi

		for run in $(seq 1 $runs); do
		  kubectl exec -n oai $iperfdnnpod -- iperf3 -c 12.1.1.2 -t 20 | awk '/[0-9]]/{sub(/.*]/,""); print $1" "$5}'  >> $log
		done

		avg=$(cat $log | awk 'END{print $2}')
		echo $avg >> logs/gaming/throughput/$aggr/throughput.avg$r.log.txt
		((gnbsimc+=1))
		((gnbsims+=1))

		echo "Finished throughput test. Waiting for iperf compute to die down"
		sleep 15
	done
	
	echo "Finished current rep of traffic generation"

	((gnbsims+=1))
	((upfc+=1))
	((iperfpodnumber+=$nousers))
	((repnet+=1))
done

echo "Experiment finished. Undeploying 5G core for $aggr aggregated users"
