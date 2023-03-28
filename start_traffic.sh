#!/bin/bash

#---------------------------------------------------------------
#---------------------------------------------------------------
GREEN='\x1b[32m'
BLUE='\x1b[34m'
NC='\033[0m'

bold=$(tput bold)
NORMAL=$(tput sgr0)
#---------------------------------------------------------------
#---------------------------------------------------------------

echo "-------------------------------------------------"
echo -e "${GREEN} ${bold} Starting DL/UL Traffic ${NC} ${NORMAL}"
echo "-------------------------------------------------"

gnbsimc=11

nousers=$1
aggr=$2
usecase=$3

iperfpodnumber=10
runs=1

rm -r logs/$usecase/compute/$aggr/
mkdir logs/$usecase/compute/$aggr
rm -r logs/$usecase/throughput/$aggr/
mkdir logs/$usecase/throughput/$aggr

avglogs=logs/$usecase/throughput/$aggr/throughput.avg$r.log.txt

iperfdnnpod=$(kubectl get pods -n oai | grep oai-dnn$iperfpodnumber | awk '{print $1}')
iperfgnbsimpod=$(kubectl get pods -n oai | grep oai-gnbsim$iperfpodnumber | awk '{print $1}')
upfpod=$(kubectl get pods -n oai  | grep spgwu-tiny | awk '{print $1}')

echo -e "${BLUE} ${bold} Starting iperf server on control gNBSIM ${NC} ${NORMAL}"
kubectl exec -n oai $iperfgnbsimpod -- iperf3 -s &


t=11
for ((sim=1;sim<=nousers;sim++))
do
	((gip=$sim+2))

	dnnpod=$(kubectl get pods -n oai  | grep oai-dnn$t | awk '{print $1}')
	dnneth0=$(kubectl exec -n oai $dnnpod -- ifconfig | grep "inet 10.42" | awk '{print $2}')
	gnbsimpod=$(kubectl get pods -n oai  | grep gnbsim$t | awk '{print $1}')

	echo -e "${BLUE} ${bold} Starting DL/UL for $dnnpod and $gnbsimpod ${NC} ${NORMAL}"
	echo "-------------------------------------------------"
	#------------Downlink traffic----------------
	echo -e "${BLUE} ${bold} Starting DL on 12.1.1.$gip ${NC} ${NORMAL}"
	kubectl exec -n oai $gnbsimpod -- python3 udpserverclient/server.py 12.1.1.$gip 20001 &
	sleep 1	
	kubectl exec -n oai $dnnpod -- python3 udpserverclient/client.py 12.1.1.$gip 20001 $aggr DL & 
	sleep 1
	echo -e "${BLUE} ${bold} DL traffic started ${NC} ${NORMAL}"
	echo "-------------------------------------------------"
	#------------Uplink traffic------------------
	echo -e "${BLUE} ${bold} Starting UL on $dnneth0 ${NC} ${NORMAL}"
	kubectl exec -n oai $dnnpod -- python3 udpserverclient/server.py $dnneth0 20002 &
	sleep 1
	kubectl exec -n oai $gnbsimpod -- python3 udpserverclient/client.py $dnneth0 20002 $aggr UL & 
	sleep 1
	echo -e "${BLUE} ${bold} UL traffic started ${NC} ${NORMAL}"
	echo "-------------------------------------------------"

	echo -e "${GREEN} ${bold} Started user on gnbsim$t. Waiting metrics-server resolution window (15s) and gathering compute log  ${NC} ${NORMAL}"

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
	echo "$upfdata" | sed 's/[A-Za-z]*//g' >> logs/$usecase/compute/$aggr/compute_logs$t.txt

	k="0"
	while [ $k -lt 4 ]
	do
		dnndata=$(echo $dnndata | awk '{$1=""}1' | awk '{$1=$1}1')
		((k++))
	done
	echo "$dnndata" | sed 's/[A-Za-z]*//g' >> logs/$usecase/compute/$aggr/compute_logs$t.txt

	k="0"
	while [ $k -lt 4 ]
	do
		gnbsimdata=$(echo $gnbsimdata | awk '{$1=""}1' | awk '{$1=$1}1')
		((k++))
	done
	echo "$gnbsimdata" | sed 's/[A-Za-z]*//g' >> logs/$usecase/compute/$aggr/compute_logs$t.txt
	
	#----------------------------------------------------------------
	flannelpods=$(kubectl get pods -A  | grep flannel | awk '{print $2}')
	for x1 in $(echo $flannelpods);
	do
		pod=$(echo $x1)
		data=$(kubectl top pod -n kube-system $pod)
		k="0"
		while [ $k -lt 4 ]
		do
			data=$(echo $data | awk '{$1=""}1' | awk '{$1=$1}1')
			((k++))
		done
		echo "$data" | sed 's/[A-Za-z]*//g' >> logs/$usecase/compute/$aggr/compute_logs$t.txt
	done

	#----------------------------------------------------------------
	ingresspods=$(kubectl get pods -n ingress-nginx | grep controller | awk '{print $1}')
	for x2 in $(echo $ingresspods);
	do
		pod=$(echo $x2)
		data=$(kubectl top pod -n ingress-nginx $pod)
		k="0"
		while [ $k -lt 4 ]
		do
			data=$(echo $data | awk '{$1=""}1' | awk '{$1=$1}1')
			((k++))
		done
		echo "$data" | sed 's/[A-Za-z]*//g' >> logs/$usecase/compute/$aggr/compute_logs$t.txt
	done


	echo -e "${GREEN} ${bold} Gathered logs. Starting throughput test${NC} ${NORMAL}"
	sleep 2

	log=logs/$usecase/throughput/$aggr/iperf.log

	if [ -f $log ]; then
	  echo removing $log
	  rm $log
	fi

	for run in $(seq 1 $runs); do
	  kubectl exec -n oai $iperfdnnpod -- iperf3 -c 12.1.1.2 -t 20 | awk '/[0-9]]/{sub(/.*]/,""); print $1" "$5}'  >> $log
	done

	avg=$(cat $log | awk 'END{print $2}')
	echo $avg >> logs/$usecase/throughput/$aggr/throughput.avg$r.log.txt
	((t+=1))

	echo -e "${GREEN} ${bold} Finished throughput test. Waiting for iperf compute to die down ${NC} ${NORMAL}"
	sleep 15
done

echo -e "${GREEN} ${bold} Traffic generation complete. ${NC} ${NORMAL}"
