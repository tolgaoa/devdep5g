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

usecase=$3

((total=$1*$2))

iperfpodnumber=10
runs=1

rm -r logs/$usecase/compute/
mkdir -p logs/$usecase/compute
rm -r logs/$usecase/throughput/
mkdir -p logs/$usecase/throughput

iperfdnnpod=$(kubectl get pods -n oai | grep oai-dnn$iperfpodnumber | awk '{print $1}')
iperfgnbsimpod=$(kubectl get pods -n oai | grep gnbsim$iperfpodnumber | awk '{print $1}')
upfpod=$(kubectl get pods -n oai  | grep spgwu-tiny10 | awk '{print $1}')
kubectl exec -n oai -c gnbsim $iperfgnbsimpod -- iperf3 -s &

#---------------------------------------------------------------------------------------
#---------------------------------------------------------------------------------------

#if [ $((sim%4)) -eq 0 ]; then

#echo -e "${GREEN} ${bold} Waiting metrics-server resolution window (17s) and gathering compute log  ${NC} ${NORMAL}"

#sleep 17
#
#for ((ite=0;ite<3;ite++))
#do
#        upfdata=$(kubectl top pod -n oai $upfpod)
#        k="0"
#        while [ $k -lt 4 ]
#        do
#                upfdata=$(echo $upfdata | awk '{$1=""}1' | awk '{$1=$1}1')
#                ((k++))
#        done
#        echo "$upfdata" | sed 's/[A-Za-z]*//g' >> logs/$usecase/compute/compute_logs$tc.txt
#
#        sleep 17
#done
#echo -e "${GREEN} ${bold} Gathered compute logs. Starting throughput test over UPF ${NC} ${NORMAL}"

#if [ $5 -eq 1 ]; then
#        for ((ite=0;ite<$4;ite++))
#        do
#
#                log=logs/$usecase/throughput/iperf.log
#                if [ -f $log ]; then
#                  echo removing TCP $log
#                  rm $log
#                fi	
#		measthrTCP
#                value=`cat logs/$usecase/throughput/iperf.log`
#                echo $value | awk '{print $7}' >> logs/$usecase/throughput/throughput.TCP$tc.log.txt        
#		sleep 3
#        done
#        for ((ite=0;ite<$4;ite++))
#        do
#
#                log=logs/$usecase/throughput/iperf.log
#                if [ -f $log ]; then
#                  echo removing UDP $log
#                  rm $log
#                fi	
#		measplUDP
#                value=`cat logs/$usecase/throughput/iperf.log`
#                echo $value | awk '{print $12}' | sed 's/[^0-9.]//g' >> logs/$usecase/throughput/pl.UDP$tc.log.txt               	 
#		sleep 3
#        done
#else
#        for ((ite=0;ite<$4;ite++))
#        do
#                log=logs/$usecase/throughput/iperf.log
#
#                if [ -f $log ]; then
#                  echo removing $log
#                  rm $log
#                fi
#
#                for run in $(seq 1 $runs); do
#                avg=$(cat $log | awk 'END{print $2}')
#                echo $avg >> logs/$usecase/throughput/throughput.avg$tc.log.txt
#                done
#
#                sleep 3
#        done
#fi

#echo -e "${GREEN} ${bold} Finished throughput test. Starting Latency Test ${NC} ${NORMAL}"

#if [ -f $log ]; then
#log=logs/$usecase/throughput/lat.log
#  echo removing $log
#  rm $log
#fi

#kubectl exec -n oai $iperfdnnpod -- ping -c 40 12.1.1.2 | awk -F '[:=]'  'NR!=1 {print $5}' >> $log

#avg=$(cat $log | sed 's/[^.0-9][^.0-9]*/ /g')
#echo $avg >> logs/$usecase/throughput/lat.avg$tc.log.txt

#sleep 3
#fi

#---------------------------------------------------------------------------------------
#---------------------------------------------------------------------------------------


tc=0
gip=1

for ((sim=0;sim<$1;sim++))
do
        ((gip+=1))
        for ((n=0;n<$2;n++))
        do
                ((add=$n*$1))
                ((t=$add+$sim+10))
                ((tc+=1))

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
        done
done

echo -e "${GREEN} ${bold} Traffic generation complete for current user set. ${NC} ${NORMAL}"
# echo -e "${GREEN} ${bold} Undeploying users for next experiment. ${NC} ${NORMAL}"

# sleep 300
# /bin/bash ./undeploy.sh $total
