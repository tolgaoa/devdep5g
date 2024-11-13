#!/bin/bash

proxy_version=3.2.0
ndn_proxy_version=2.2.0
init_version=3.0.0

packetloss=0%
proxymode=NONE
proxyon=false
biproxyon=false
ndnproxyon=false
usetls=false


#----NDN Configuration----
nfdserverip="5.100.1.100"
nrfndnIP="5.100.1.10"
udrndnIP="5.100.1.11"
udmndnIP="5.100.1.12"
ausfndnIP="5.100.1.13"
amfndnIP="5.100.1.14"
smfndnIP="5.100.1.15"

st=`date +%s`
#---------------------------------------------------------------
#---------------------------------------------------------------
GREEN='\x1b[32m'
BLUE='\x1b[34m'
NC='\033[0m'

bold=$(tput bold)
NORMAL=$(tput sgr0)
#---------------------------------------------------------------
#---------------------------------------------------------------

# Function to wait for a pod with a specific name prefix to be running
wait_for_pod() {
	echo -e "${BLUE} ${bold} Waiting for pod deployment ${NC} ${NORMAL}"
    local namespace=$1
    local pod_name_prefix=$2

    # Check if the pod is running
    local is_pod_running=false
    while [ "$is_pod_running" = false ]; do
        # Get the status of the pod by name prefix
        local pod_name=$(kubectl get pods -n "$namespace" --field-selector=status.phase!=Succeeded,status.phase!=Failed --no-headers | grep "^$pod_name_prefix" | awk '{print $1}' | head -n 1)
        local pod_status=""
        if [ ! -z "$pod_name" ]; then
            pod_status=$(kubectl get pod "$pod_name" -n "$namespace" -o jsonpath="{.status.phase}")
        fi

        # Check if the status is "Running"
        if [ "$pod_status" = "Running" ]; then
            is_pod_running=true
            echo "Pod $pod_name is running."
        else
            # Wait for a short period before checking again
            sleep 5
        fi
    done
	sleep 5
}

#---------------------------------------------------------------
#---------------------------------------------------------------


gnbsimim=$1
dnnim=$2

((user=$3+10))
((trafficusers=$3-1))
((slice=$4+9))
u=10



echo "-------------------------------------------------"
echo -e "${BLUE} ${bold} gNBSIM image set to $gnbsimim ${NC} ${NORMAL}"
echo -e "${BLUE} ${bold} DNN image is set to $dnnim ${NC} ${NORMAL}"
echo "-------------------------------------------------"


echo "-------------------------------------------------"
echo -e "${BLUE} ${bold} Deploying $4 slice(s) with $3 users each ${NC} ${NORMAL}"
echo "-------------------------------------------------"

echo "-------------------------------------------------"
echo -e "${GREEN} ${bold} Starting 5G Core deployment ${NC} ${NORMAL}"
echo "-------------------------------------------------"

for ((s=10;s<=$slice;s++))
do
    #-----------------------START-----------------------
    #--------------------Proxy Config-------------------

	sed -i "/ndnIP/c\  ndnIP: \"$nrfndnIP\"" oai-nrf/values.yaml
	sed -i "/ndnIP/c\  ndnIP: \"$udrndnIP\"" oai-udr/values.yaml
	sed -i "/ndnIP/c\  ndnIP: \"$udmndnIP\"" oai-udm/values.yaml
	sed -i "/ndnIP/c\  ndnIP: \"$ausfndnIP\"" oai-ausf/values.yaml
	sed -i "/ndnIP/c\  ndnIP: \"$amfndnIP\"" oai-amf/values.yaml
	sed -i "/ndnIP/c\  ndnIP: \"$smfndnIP\"" oai-smf/values.yaml

	sed -i "/initversion/c\  initversion: $init_version" oai-nrf/values.yaml
	sed -i "/initversion/c\  initversion: $init_version" oai-udr/values.yaml
	sed -i "/initversion/c\  initversion: $init_version" oai-udm/values.yaml
	sed -i "/initversion/c\  initversion: $init_version" oai-ausf/values.yaml
	sed -i "/initversion/c\  initversion: $init_version" oai-amf/values.yaml
	sed -i "/initversion/c\  initversion: $init_version" oai-smf/values.yaml
	
	sed -i "/saversion/c\  saversion: $proxy_version" oai-nrf/values.yaml
	sed -i "/saversion/c\  saversion: $proxy_version" oai-udr/values.yaml
	sed -i "/saversion/c\  saversion: $proxy_version" oai-udm/values.yaml
	sed -i "/saversion/c\  saversion: $proxy_version" oai-ausf/values.yaml
	sed -i "/saversion/c\  saversion: $proxy_version" oai-amf/values.yaml
	sed -i "/saversion/c\  saversion: $proxy_version" oai-smf/values.yaml

	sed -i "/ndnpversion/c\  ndnpversion: $ndn_proxy_version" oai-nrf/values.yaml
	sed -i "/ndnpversion/c\  ndnpversion: $ndn_proxy_version" oai-udr/values.yaml
	sed -i "/ndnpversion/c\  ndnpversion: $ndn_proxy_version" oai-udm/values.yaml
	sed -i "/ndnpversion/c\  ndnpversion: $ndn_proxy_version" oai-ausf/values.yaml
	sed -i "/ndnpversion/c\  ndnpversion: $ndn_proxy_version" oai-amf/values.yaml
	sed -i "/ndnpversion/c\  ndnpversion: $ndn_proxy_version" oai-smf/values.yaml

    sed -i "/startproxy/c\  startproxy: $proxyon" oai-nrf/values.yaml
    sed -i "/startproxy/c\  startproxy: $proxyon" oai-udr/values.yaml
    sed -i "/startproxy/c\  startproxy: $proxyon" oai-udm/values.yaml
    sed -i "/startproxy/c\  startproxy: $proxyon" oai-ausf/values.yaml
    sed -i "/startproxy/c\  startproxy: $proxyon" oai-amf/values.yaml
    sed -i "/startproxy/c\  startproxy: $proxyon" oai-smf/values.yaml
    
    sed -i "/startndnproxy/c\  startndnproxy: $ndnproxyon" oai-nrf/values.yaml
    sed -i "/startndnproxy/c\  startndnproxy: $ndnproxyon" oai-udr/values.yaml
    sed -i "/startndnproxy/c\  startndnproxy: $ndnproxyon" oai-udm/values.yaml
    sed -i "/startndnproxy/c\  startndnproxy: $ndnproxyon" oai-ausf/values.yaml
    sed -i "/startndnproxy/c\  startndnproxy: $ndnproxyon" oai-amf/values.yaml
    sed -i "/startndnproxy/c\  startndnproxy: $ndnproxyon" oai-smf/values.yaml

    sed -i "/startbiproxy/c\  startbiproxy: $biproxyon" oai-nrf/values.yaml
    sed -i "/startbiproxy/c\  startbiproxy: $biproxyon" oai-udr/values.yaml
    sed -i "/startbiproxy/c\  startbiproxy: $biproxyon" oai-udm/values.yaml
    sed -i "/startbiproxy/c\  startbiproxy: $biproxyon" oai-ausf/values.yaml
    sed -i "/startbiproxy/c\  startbiproxy: $biproxyon" oai-amf/values.yaml
    sed -i "/startbiproxy/c\  startbiproxy: $biproxyon" oai-smf/values.yaml

    sed -i "/nfdserverip/c\  nfdserverip: \"$nfdserverip\"" oai-nrf/values.yaml
    sed -i "/nfdserverip/c\  nfdserverip: \"$nfdserverip\"" oai-udr/values.yaml
    sed -i "/nfdserverip/c\  nfdserverip: \"$nfdserverip\"" oai-udm/values.yaml
    sed -i "/nfdserverip/c\  nfdserverip: \"$nfdserverip\"" oai-ausf/values.yaml
    sed -i "/nfdserverip/c\  nfdserverip: \"$nfdserverip\"" oai-amf/values.yaml
    sed -i "/nfdserverip/c\  nfdserverip: \"$nfdserverip\"" oai-smf/values.yaml

    sed -i "/opmode/c\  opmode: \"$proxymode\"" oai-nrf/values.yaml
    sed -i "/opmode/c\  opmode: \"$proxymode\"" oai-udr/values.yaml
    sed -i "/opmode/c\  opmode: \"$proxymode\"" oai-udm/values.yaml
    sed -i "/opmode/c\  opmode: \"$proxymode\"" oai-ausf/values.yaml
    sed -i "/opmode/c\  opmode: \"$proxymode\"" oai-amf/values.yaml
    sed -i "/opmode/c\  opmode: \"$proxymode\"" oai-smf/values.yaml
    
	sed -i "/http1tls/c\  http1tls: \"$usetls\"" oai-nrf/values.yaml
	sed -i "/http1tls/c\  http1tls: \"$usetls\"" oai-udr/values.yaml
	sed -i "/http1tls/c\  http1tls: \"$usetls\"" oai-udm/values.yaml
	sed -i "/http1tls/c\  http1tls: \"$usetls\"" oai-ausf/values.yaml
	sed -i "/http1tls/c\  http1tls: \"$usetls\"" oai-amf/values.yaml
	sed -i "/http1tls/c\  http1tls: \"$usetls\"" oai-smf/values.yaml
	
	sed -i "/packetloss/c\  packetloss: \"$packetloss\"" oai-nrf/values.yaml
	sed -i "/packetloss/c\  packetloss: \"$packetloss\"" oai-udr/values.yaml
	sed -i "/packetloss/c\  packetloss: \"$packetloss\"" oai-udm/values.yaml
	sed -i "/packetloss/c\  packetloss: \"$packetloss\"" oai-ausf/values.yaml
	sed -i "/packetloss/c\  packetloss: \"$packetloss\"" oai-amf/values.yaml
	sed -i "/packetloss/c\  packetloss: \"$packetloss\"" oai-smf/values.yaml
    #--------------------Proxy Config-------------------
    #------------------------END------------------------

	((z=$s-9))
	#------------------------NRF-------------------------
	sed -i "/name/c\name: oai-nrf$s" oai-nrf/Chart.yaml

	sed -i "/saname/c\  saname: \"nrf$s\"" oai-nrf/values.yaml
	sed -i "/svcname/c\  svcname: \"oai-nrf$s-sa\"" oai-nrf/values.yaml
    sed -i "/sak8sdns/c\  sak8sdns: \"oai-nrf$s-svc\"" oai-nrf/values.yaml

	helm install nrf$s oai-nrf/ -n oai
	wait_for_pod "oai" "oai-nrf"
	echo -e "${GREEN} ${bold} NRF$s deployed ${NC} ${NORMAL}"

	#------------------------UDR-------------------------
	sed -i "23s/.*/name: oai-udr$s/" oai-udr/Chart.yaml

	sed -i "/nrfFqdn/c\  nrfFqdn: \"oai-nrf$s-svc\"" oai-udr/values.yaml	
	sed -i "/svcname/c\  svcname: \"oai-udr$s-sa\"" oai-udr/values.yaml
    sed -i "/sak8sdns/c\  sak8sdns: \"oai-udr$s-svc\"" oai-udr/values.yaml
	
	helm install udr$s oai-udr/ -n oai
	wait_for_pod "oai" "oai-udr"
	echo -e "${GREEN} ${bold} UDR$s deployed ${NC} ${NORMAL}"
	
	#------------------------UDM-------------------------
	sed -i "23s/.*/name: oai-udm$s/" oai-udm/Chart.yaml

	sed -i "/nrfFqdn/c\  nrfFqdn: \"oai-nrf$s-svc\"" oai-udm/values.yaml
	sed -i "/udrFqdn/c\  udrFqdn: \"oai-udr$s-svc\"" oai-udm/values.yaml
	sed -i "/svcname/c\  svcname: \"oai-udm$s-sa\"" oai-udm/values.yaml
    sed -i "/sak8sdns/c\  sak8sdns: \"oai-udm$s-svc\"" oai-udm/values.yaml

	helm install udm$s oai-udm/ -n oai
	wait_for_pod "oai" "oai-udm"
	echo -e "${GREEN} ${bold} UDM$s deployed ${NC} ${NORMAL}"
	
	#------------------------AUSF------------------------
	sed -i "22s/.*/name: oai-ausf$s/" oai-ausf/Chart.yaml

	sed -i "/nrfFqdn/c\  nrfFqdn: \"oai-nrf$s-svc\"" oai-ausf/values.yaml
	sed -i "/udmFqdn/c\  udmFqdn: \"oai-udm$s-svc\"" oai-ausf/values.yaml
	sed -i "/svcname/c\  svcname: \"oai-ausf$s-sa\"" oai-ausf/values.yaml
    sed -i "/sak8sdns/c\  sak8sdns: \"oai-ausf$s-svc\"" oai-ausf/values.yaml

	helm install ausf$s oai-ausf/ -n oai
	wait_for_pod "oai" "oai-ausf"
	echo -e "${GREEN} ${bold} AUSF$s deployed ${NC} ${NORMAL}"
	
	#------------------------AMF-------------------------
	sed -i "22s/.*/name: oai-amf$s/" oai-amf/Chart.yaml	
	
	sed -i "/nrfFqdn/c\  nrfFqdn: \"oai-nrf$s-svc\"" oai-amf/values.yaml
	sed -i "/smfFqdn/c\  nrfFqdn: \"oai-smf$s-svc\"" oai-amf/values.yaml
	sed -i "/ausfFqdn/c\  ausfFqdn: \"oai-ausf$s-svc\"" oai-amf/values.yaml
	sed -i "/sst0/c\  sst0: \"2$s\"" oai-amf/values.yaml
	sed -i "/svcname/c\  svcname: \"oai-amf$s-sa\"" oai-amf/values.yaml
    sed -i "/sak8sdns/c\  sak8sdns: \"oai-amf$s-svc\"" oai-amf/values.yaml

	helm install amf$s oai-amf/ -n oai
	wait_for_pod "oai" "oai-amf"
	echo -e "${GREEN} ${bold} AMF$s deployed ${NC} ${NORMAL}"
	amfpod=$(kubectl get pods -n oai  | grep amf$s | awk '{print $1}')
	amfeth0=$(kubectl exec -n oai $amfpod -c amf -- ifconfig | grep "inet 10.42" | awk '{print $2}')
	
	#------------------------SMF-------------------------
	sed -i "22s/.*/name: oai-smf$s/" oai-smf/Chart.yaml

	sed -i "/nrfFqdn/c\  nrfFqdn: \"oai-nrf$s-svc\"" oai-smf/values.yaml
	sed -i "/udmFqdn/c\  udmFqdn: \"oai-udm$s-svc\"" oai-smf/values.yaml
	sed -i "/amfFqdn/c\  amfFqdn: \"oai-amf$s-svc\"" oai-smf/values.yaml
	sed -i "/nssaiSst0/c\  nssaiSst0: \"2$s\"" oai-smf/values.yaml
	sed -i "/svcname/c\  svcname: \"oai-smf$s-sa\"" oai-smf/values.yaml
    sed -i "/sak8sdns/c\  sak8sdns: \"oai-smf$s-svc\"" oai-smf/values.yaml

	helm install smf$s oai-smf/ -n oai
	wait_for_pod "oai" "oai-smf"
	echo -e "${GREEN} ${bold} SMF$s deployed ${NC} ${NORMAL}"
	
	#------------------------UPF-------------------------
	sed -i "22s/.*/name: oai-spgwu-tiny$s/" oai-spgwu-tiny/Chart.yaml

	sed -i "/nrfFqdn/c\  nrfFqdn: \"oai-nrf$s-svc\"" oai-spgwu-tiny/values.yaml
	sed -i "/fqdn/c\  fqdn: \"oai-spgwu-tiny$s-svc\"" oai-spgwu-tiny/values.yaml
	sed -i "/oai-spgwu-tiny-sa/c\  name: \"oai-spgwu-tiny$s-sa\"" oai-spgwu-tiny/values.yaml
	sed -i "24s/.*/  name: \"oai-spgwu-tiny$s\"/" oai-spgwu-tiny/values.yaml
	sed -i "/nssaiSst0/c\  nssaiSst0: \"2$s\"" oai-spgwu-tiny/values.yaml
	
	helm install upf$s oai-spgwu-tiny/ -n oai
	wait_for_pod "oai" "oai-spgwu"
	sleep 1
	echo -e "${GREEN} ${bold} UPF$s deployed ${NC} ${NORMAL}"
	upfpod=$(kubectl get pods -n oai  | grep spgwu-tiny$s | awk '{print $1}')
	upfeth0=$(kubectl exec -n oai $upfpod -c spgwu -- ifconfig | grep "inet 10.42" | awk '{print $2}')
	
	echo "-------------------------------------------------"
	echo -e "${GREEN} ${bold} Finished Core VNF deployment. Starting RAN. ${NC} ${NORMAL}"
	echo "-------------------------------------------------"

	ip=2
	for ((ut=0;ut<$3;ut++))
	do
		#-----------------------------GNBSIM Deployment----------------------------------------
		sed -i "2s/.*/name: gnbsim$u/" gnbsim/Chart.yaml
		sed -i "6s/.*/  version: ${gnbsimim}/" gnbsim/values.yaml
		sed -i "28s/.*/  name: \"gnbsim-sa$u\"/" gnbsim/values.yaml
		sed -i "/ngappeeraddr/c\  ngappeeraddr: \"$amfeth0\"" gnbsim/values.yaml
		sed -i "/gnbid/c\  gnbid: \"$u\"" gnbsim/values.yaml
		sed -i "/msin/c\  msin: \"00000000$u\"" gnbsim/values.yaml
		sed -i "/key/c\  key: \"0C0A34601D4F07677303652C046253$u\"" gnbsim/values.yaml
		sed -i "/sst/c\  sst: \"2$s\"" gnbsim/values.yaml

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
		#sed -i "22s/.*/        type: az$z/" oai-dnn/02_deployment.yaml

		kubectl apply -k oai-dnn/
		sleep 2
		echo -e "${BLUE} ${bold} DNN$u deployed ${NC} ${NORMAL}"
		dnnpod=$(kubectl get pods -n oai  | grep oai-dnn$u | awk '{print $1}')
		dnneth0=$(kubectl exec -n oai $dnnpod -- ifconfig | grep "inet 10.42" | awk '{print $2}')
		
		kubectl exec -it -n oai $dnnpod -- iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE
		kubectl exec -it -n oai $dnnpod -- ip route add 12.1.1.0/24 via $upfeth0 dev eth0
		kubectl exec -it -n oai $gnbsimpod -c gnbsim -- ip route replace $dnneth0 via 0.0.0.0 dev eth0 src 12.1.1.$ip
		((ip+=1))
		((u+=1))
		echo "-------------------------------------------------"
	done
done


echo "-------------------------------------------------"
echo -e "${GREEN} ${bold} Finished 5G Deployment ${NC} ${NORMAL}"
echo "-------------------------------------------------"
et=`date +%s`
rt=$((et-st))
hr=$((rt/3600))
echo "-------------------------------------------------"
echo -e "${BLUE} ${bold} Deployment took $hr seconds ${NC} ${NORMAL}"
echo "-------------------------------------------------"
#echo $rt >> depite.txt

#/bin/bash ./ftart_traffic.sh $trafficusers $4 $dnnim
#/bin/bash ./start_traffic.sh $3 $4 $dnnim $5 $6
