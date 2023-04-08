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

gnbsimim=$1
dnnim=$2

((user=$3+10))
((trafficusers=$3-1))
slice=10
ip=2


echo "-------------------------------------------------"
echo -e "${BLUE} ${bold} gNBSIM image set to $gnbsimim ${NC} ${NORMAL}"
echo -e "${BLUE} ${bold} DNN image is set to $dnnim ${NC} ${NORMAL}"
echo "-------------------------------------------------"


echo "-------------------------------------------------"
echo -e "${BLUE} ${bold} Deploying $3 users ${NC} ${NORMAL}"
echo "-------------------------------------------------"

echo "-------------------------------------------------"
echo -e "${GREEN} ${bold} Starting 5G Core deployment ${NC} ${NORMAL}"
echo "-------------------------------------------------"

for ((s=10;s<=$slice;s++))
do
	helm install nrf oai-nrf/ -n oai
	sleep 8
	echo -e "${GREEN} ${bold} NRF$s deployed ${NC} ${NORMAL}"

	helm install udr oai-udr/ -n oai
	sleep 8
	echo -e "${GREEN} ${bold} UDR$s deployed ${NC} ${NORMAL}"
	
	helm install udm oai-udm/ -n oai
	sleep 8
	echo -e "${GREEN} ${bold} UDM$s deployed ${NC} ${NORMAL}"
	
	helm install ausf oai-ausf/ -n oai
	sleep 8
	echo -e "${GREEN} ${bold} AUSF$s deployed ${NC} ${NORMAL}"
	
	helm install amf oai-amf/ -n oai
	sleep 8
	echo -e "${GREEN} ${bold} AMF$s deployed ${NC} ${NORMAL}"
	amfpod=$(kubectl get pods -n oai  | grep amf | awk '{print $1}')
	amfeth0=$(kubectl exec -n oai $amfpod -c amf -- ifconfig | grep "inet 10.42" | awk '{print $2}')
	
	helm install smf oai-smf/ -n oai
	sleep 8
	echo -e "${GREEN} ${bold} SMF$s deployed ${NC} ${NORMAL}"
	
	helm install upf oai-spgwu-tiny/ -n oai
	sleep 8
	echo -e "${GREEN} ${bold} UPF$s deployed ${NC} ${NORMAL}"
	upfpod=$(kubectl get pods -n oai  | grep spgwu | awk '{print $1}')
	upfeth0=$(kubectl exec -n oai $upfpod -c spgwu -- ifconfig | grep "inet 10.42" | awk '{print $2}')
	
	echo "-------------------------------------------------"
	echo -e "${GREEN} ${bold} Finished Core VNF deployment. Starting RAN. ${NC} ${NORMAL}"
	echo "-------------------------------------------------"

	for ((u=10;u<$user;u++))
	do
		#-----------------------------GNBSIM Deployment----------------------------------------
		sed -i "2s/.*/name: gnbsim$u/" gnbsim/Chart.yaml
		sed -i "6s/.*/  version: ${gnbsimim}/" gnbsim/values.yaml
		sed -i "28s/.*/  name: \"gnbsim-sa$u\"/" gnbsim/values.yaml
		sed -i "/ngappeeraddr/c\  ngappeeraddr: \"$amfeth0\"" gnbsim/values.yaml
		sed -i "/gnbid/c\  gnbid: \"$u\"" gnbsim/values.yaml
		sed -i "/msin/c\  msin: \"00000000$u\"" gnbsim/values.yaml
		sed -i "/key/c\  key: \"0C0A34601D4F07677303652C046253$u\"" gnbsim/values.yaml

		helm install gnb$u gnbsim/ -n oai 
		sleep 8 
		echo -e "${BLUE} ${bold} GNBSIM$u deployed ${NC} ${NORMAL}"
		gnbsimpod=$(kubectl get pods -n oai  | grep gnbsim$u | awk '{print $1}')
		gnbsimeth0=$(kubectl exec -n oai $gnbsimpod -c gnbsim -- ifconfig | grep "inet 10.42" | awk '{print $2}')


		#-----------------------------DNN Deployment-------------------------------------------
		sed -i "4s/.*/  name: oai-dnn$u/" oai-dnn/02_deployment.yaml
		sed -i "6s/.*/    app: oai-dnn$u/" oai-dnn/02_deployment.yaml
		sed -i "11s/.*/      app: oai-dnn$u/" oai-dnn/02_deployment.yaml
		sed -i "17s/.*/        app: oai-dnn$u/" oai-dnn/02_deployment.yaml
		sed -i "28s/.*/        image: tolgaomeratalay\/oai-dnn:${dnnim}/" oai-dnn/02_deployment.yaml

		kubectl apply -k oai-dnn/
		sleep 5
		echo -e "${BLUE} ${bold} DNN$u deployed ${NC} ${NORMAL}"
		dnnpod=$(kubectl get pods -n oai  | grep oai-dnn$u | awk '{print $1}')
		dnneth0=$(kubectl exec -n oai $dnnpod -- ifconfig | grep "inet 10.42" | awk '{print $2}')
		
		kubectl exec -it -n oai $dnnpod -- iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE
		kubectl exec -it -n oai $dnnpod -- ip route add 12.1.1.0/24 via $upfeth0 dev eth0
		kubectl exec -it -n oai $gnbsimpod -c gnbsim -- ip route replace $dnneth0 via 0.0.0.0 dev eth0 src 12.1.1.$ip
		((ip+=1))
		echo "-------------------------------------------------"
	done
done


echo "-------------------------------------------------"
echo -e "${GREEN} ${bold} Finished 5G Deployment ${NC} ${NORMAL}"
echo "-------------------------------------------------"

/bin/bash ./start_traffic.sh $trafficusers $dnnim

