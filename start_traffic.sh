#!/bin/bash

#---------------------------------------------------------------
#---------------------------------------------------------------
GREEN='\x1b[32m'
BLUE='\x1b[34m'
RED='\x1B[31m'
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
usecase=$2

iperfpodnumber=10
runs=1

rm -r logs/$usecase/compute/
mkdir -p logs/$usecase/compute
rm -r logs/$usecase/throughput/
mkdir -p logs/$usecase/throughput

iperfdnnpod=$(kubectl get pods -n oai | grep oai-dnn$iperfpodnumber | awk '{print $1}')
iperfgnbsimpod=$(kubectl get pods -n oai | grep gnbsim$iperfpodnumber | awk '{print $1}')
upfpod=$(kubectl get pods -n oai  | grep spgwu-tiny | awk '{print $1}')

echo -e "${BLUE} ${bold} Starting iperf server on control gNBSIM ${NC} ${NORMAL}"
kubectl exec -n oai -c gnbsim $iperfgnbsimpod -- iperf3 -s &

tc=0
for ((sim=1;sim<=nousers;sim++))
do
        ((gip=$sim+2))
        ((t=$sim+10))
        ((tc+=1))
	
	avglogsthr=logs/$usecase/throughput/throughput.avg$tc.log.txt
	avglogslat=logs/$usecase/throughput/lat.avg$tc.log.txt

        dnnpod=$(kubectl get pods -n oai  | grep oai-dnn$t | awk '{print $1}')
        #echo -e "${RED} ${bold} DNN pod is $dnnpod ${NC} ${NORMAL}"
        dnneth0=$(kubectl exec -n oai $dnnpod -- ifconfig | grep "inet 10.42" | awk '{print $2}')
        gnbsimpod=$(kubectl get pods -n oai  | grep gnbsim$t | awk '{print $1}')
        #echo -e "${RED} ${bold} GNBSIM pod is $gnbsimpod ${NC} ${NORMAL}"

        echo -e "${BLUE} ${bold} Starting DL/UL for $dnnpod and $gnbsimpod ${NC} ${NORMAL}"
        #------------Downlink traffic----------------
        echo -e "${BLUE} ${bold} Starting DL on 12.1.1.$gip ${NC} ${NORMAL}"
        kubectl exec -n oai $gnbsimpod -c gnbsim -- python3 udpserverclient/server.py 12.1.1.$gip 20001 &
        sleep 1
        kubectl exec -n oai $dnnpod -- python3 udpserverclient/client.py 12.1.1.$gip 20001 1 DL &
        sleep 1
        echo -e "${BLUE} ${bold} DL traffic started ${NC} ${NORMAL}"
        #------------Uplink traffic------------------
        echo -e "${BLUE} ${bold} Starting UL on $dnneth0 ${NC} ${NORMAL}"
        kubectl exec -n oai $dnnpod -- python3 udpserverclient/server.py $dnneth0 20002 &
        sleep 1
        kubectl exec -n oai $gnbsimpod -c gnbsim -- python3 udpserverclient/client.py $dnneth0 20002 1 UL &
        sleep 1
        echo -e "${BLUE} ${bold} UL traffic started ${NC} ${NORMAL}"
        echo "-------------------------------------------------"
        echo "-------------------------------------------------"
        echo "-------------------------------------------------"

	
	if [ $((sim%5)) -eq 1 ]; then

		echo -e "${GREEN} ${bold} Waiting metrics-server resolution window (18s) and gathering compute log  ${NC} ${NORMAL}"

		sleep 17

		for ((ite=0;ite<4;ite++))
		do
			upfdata=$(kubectl top pod -n oai $upfpod)
			k="0"
			while [ $k -lt 4 ]
			do
				upfdata=$(echo $upfdata | awk '{$1=""}1' | awk '{$1=$1}1')
				((k++))
			done
			echo "$upfdata" | sed 's/[A-Za-z]*//g' >> logs/$usecase/compute/compute_logs$tc.txt

			sleep 17
		done
		echo -e "${GREEN} ${bold} Gathered compute logs. Starting throughput test${NC} ${NORMAL}"

		for ((ite=0;ite<10;ite++))
		do
			log=logs/$usecase/throughput/iperf.log

			if [ -f $log ]; then
			  echo removing $log
			  rm $log
			fi

			for run in $(seq 1 $runs); do
			kubectl exec -n oai $iperfdnnpod -- iperf3 -c 12.1.1.2 -t 15 | awk '/[0-9]]/{sub(/.*]/,""); print $1" "$5}'  >> $log
			done

			avg=$(cat $log | awk 'END{print $2}')
			echo $avg >> logs/$usecase/throughput/throughput.avg$tc.log.txt

			sleep 3
		done

		echo -e "${GREEN} ${bold} Finished throughput test. Starting Latency Test ${NC} ${NORMAL}"

		log=logs/$usecase/throughput/lat.log

		if [ -f $log ]; then
		  echo removing $log
		  rm $log
		fi

		kubectl exec -n oai $iperfdnnpod -- ping -c 40 12.1.1.2 | awk -F '[:=]'  'NR!=1 {print $5}' >> $log

		avg=$(cat $log | sed 's/[^.0-9][^.0-9]*/ /g')
		echo $avg >> logs/$usecase/throughput/lat.avg$tc.log.txt

		sleep 3
	fi	
done

echo -e "${GREEN} ${bold} Traffic generation complete for current user set. ${NC} ${NORMAL}"
echo -e "${GREEN} ${bold} Undeploying users for next experiment. ${NC} ${NORMAL}"

/bin/bash ./undeploy.sh $1
sleep 60
