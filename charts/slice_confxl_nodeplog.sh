#!/bin/bash

nousers=$1
aggr=$2
rep=$3
gnbsimim=$4
dnnim=$5

echo "gNBSIM image set to $gnbsimim"
echo "DNN image is set to $dnnim"

#rm logs/deployment/*

for ((a=1;a<=$aggr;a++))
do

repmod=100
nrfc=10
amfc=10
supfc1=10
supfc2=40
gnbsimc=10
gnbsims=10

	echo "Starting Core network deployment"

	for ((nrf=1;nrf<=$rep;nrf++))
	do
		echo $nrf
		if [ $nrf -le 3 ]; then
			sqlc=5
			echo "Chosen mysql$sqlc"
		elif [ $nrf -le 6 ]; then
			sqlc=1
			echo "Chosen mysql$sqlc"
		elif [ $nrf -le 9 ]; then
			sqlc=2
			echo "Chosen mysql$sqlc"
		elif [ $nrf -le 10 ]; then
			sqlc=0
			echo "Chosen mysql$sqlc"
		fi

                kubectl wait --for=condition=available --timeout=200s deployment/mysql$sqlc -n oai
                mysqlIP=$(kubectl get pods -n oai -o wide| grep mysql$sqlc | awk '{print $6}');


		#----------------------------------NRF Deployment----------------------------------------------------------
		sed -i "/name: oai-nrf/c\name: oai-nrf$nrfc " charts/oai-nrf/Chart.yaml
		sed -i "/oai-nrf-sa/c\  name: \"oai-nrf-sa$nrfc\"" charts/oai-nrf/values.yaml
		sed -i "/nrfIpNg/c\  nrfIpNg: \"10.$repmod.11.$nrfc\"" charts/oai-nrf/values.yaml
		helm install nrf$nrfc charts/oai-nrf/ -n oai
		kubectl wait --for=condition=available --timeout=200s deployment/oai-nrf$nrfc -n oai

                #----------------------------------UDR Deployment----------------------------------------------------------
                sed -i "/name: oai-udr/c\name: oai-udr$nrfc " charts/oai-udr/Chart.yaml
                sed -i "/oai-udr-sa/c\  name: \"oai-udr-sa$nrfc\"" charts/oai-udr/values.yaml
                sed -i "/nudrIPmain/c\  nudrIPmain: \"10.$repmod.12.$nrfc\"" charts/oai-udr/values.yaml
		sed -i "/mySqlServer/c\  mySqlServer: \"$mysqlIP\"" charts/oai-udr/values.yaml
                helm install udr$nrfc charts/oai-udr/ -n oai
                kubectl wait --for=condition=available --timeout=200s deployment/oai-udr$nrfc -n oai

                #----------------------------------UDM Deployment----------------------------------------------------------
                sed -i "/name: oai-udm/c\name: oai-udm$nrfc " charts/oai-udm/Chart.yaml
                sed -i "/oai-udm-sa/c\  name: \"oai-udm-sa$nrfc\"" charts/oai-udm/values.yaml
                sed -i "/nudmIPmain/c\  nudmIPmain: \"10.$repmod.13.$nrfc\"" charts/oai-udm/values.yaml
                sed -i "/udrIpAddress/c\  udrIpAddress: \"10.$repmod.12.$nrfc\"" charts/oai-udm/values.yaml
                helm install udm$nrfc charts/oai-udm/ -n oai
                kubectl wait --for=condition=available --timeout=200s deployment/oai-udm$nrfc -n oai

                #----------------------------------AUSF Deployment----------------------------------------------------------
                sed -i "/name: oai-ausf/c\name: oai-ausf$nrfc " charts/oai-ausf/Chart.yaml
                sed -i "/oai-ausf-sa/c\  name: \"oai-ausf-sa$nrfc\"" charts/oai-ausf/values.yaml
                sed -i "/nausfIPmain/c\  nausfIPmain: \"10.$repmod.14.$nrfc\"" charts/oai-ausf/values.yaml
                sed -i "/udmIpAddress/c\  udmIpAddress: \"10.$repmod.13.$nrfc\"" charts/oai-ausf/values.yaml
                helm install ausf$nrfc charts/oai-ausf/ -n oai
                kubectl wait --for=condition=available --timeout=200s deployment/oai-ausf$nrfc -n oai

		for ((amf=1;amf<=1;amf++))
		do
			#----------------------------------AMF Deployment----------------------------------------------------------
			sed -i "/mySqlServer/c\  mySqlServer: \"$mysqlIP\"" charts/oai-amf/values.yaml
			sed -i "/name: oai-amf/c\name: oai-amf$amfc " charts/oai-amf/Chart.yaml
			sed -i "/oai-amf-sa/c\  name: \"oai-amf-sa$amfc\"" charts/oai-amf/values.yaml
			sed -i "/namfIPmain/c\  namfIPmain: \"10.$repmod.15.$amfc\"" charts/oai-amf/values.yaml	
			sed -i "/nrfIpv4Addr/c\  nrfIpv4Addr: \"10.$repmod.11.$nrfc\"" charts/oai-amf/values.yaml	
			sed -i "/ausfIpv4Addr/c\  ausfIpv4Addr: \"10.$repmod.14.$nrfc\"" charts/oai-amf/values.yaml	
			sed -i "/smfIpv4Addr0/c\  smfIpv4Addr0: \"10.$repmod.16.$supfc1\"" charts/oai-amf/values.yaml
			sed -i "/smfIpv4Addr1/c\  smfIpv4Addr1: \"10.$repmod.16.$supfc2\"" charts/oai-amf/values.yaml
			helm install amf$amfc charts/oai-amf/ -n oai
			kubectl wait --for=condition=available --timeout=200s deployment/oai-amf$amfc -n oai
			amfpod=$(kubectl get pods -n oai  | grep amf$amfc | awk '{print $1}')
			amfnet1IP=$(kubectl exec -n oai $amfpod -c amf -- ifconfig | grep "inet 10.$repmod" | awk '{print $2}')

			#----------------------------------SMF Deployment----------------------------------------------------------
			sed -i "/name: oai-smf/c\name: oai-smf$supfc1 " charts/oai-smf/Chart.yaml
			sed -i "/oai-smf-sa/c\  name: \"oai-smf-sa$supfc1\"" charts/oai-smf/values.yaml
			sed -i "/nsmfIPmain/c\  nsmfIPmain: \"10.$repmod.16.$supfc1\"" charts/oai-smf/values.yaml
			sed -i "/amfIpv4Address/c\  amfIpv4Address: \"10.$repmod.15.$amfc\"" charts/oai-smf/values.yaml
			sed -i "/udmIpv4Address/c\  udmIpv4Address: \"10.$repmod.13.$amfc\"" charts/oai-smf/values.yaml
			sed -i "/nrfIpv4Address/c\  nrfIpv4Address: \"10.$repmod.11.$nrfc\"" charts/oai-smf/values.yaml
			sed -i "/upfIpv4Address/c\  upfIpv4Address: \"10.$repmod.17.$supfc1\"" charts/oai-smf/values.yaml
			helm install smf$supfc1 charts/oai-smf/ -n oai
			kubectl wait --for=condition=available --timeout=200s deployment/oai-smf$supfc1 -n oai
                        
			#----------------------------------UPF Deployment----------------------------------------------------------
			sed -i "/name: oai-spgwu-tiny/c\name: oai-spgwu-tiny$supfc1 " charts/oai-spgwu-tiny/Chart.yaml
			sed -i "/oai-spgwu-tiny-sa/c\  name: \"oai-spgwu-tiny-sa$supfc1\"" charts/oai-spgwu-tiny/values.yaml
			sed -i "/nupfIP1main/c\  nupfIP1main: \"10.$repmod.17.$supfc1\"" charts/oai-spgwu-tiny/values.yaml
			sed -i "/nupfIP2main/c\  nupfIP2main: \"10.$repmod.18.$supfc1\"" charts/oai-spgwu-tiny/values.yaml
			sed -i "/spgwc0IpAdd/c\  spgwc0IpAdd: \"10.$repmod.16.$supfc1\" #SMF IP address" charts/oai-spgwu-tiny/values.yaml
			sed -i "/nrfIpv4Add/c\  nrfIpv4Add: \"10.$repmod.11.$nrfc\" #NRF IP address" charts/oai-spgwu-tiny/values.yaml
			helm install upf$supfc1 charts/oai-spgwu-tiny/ -n oai
			kubectl wait --for=condition=available --timeout=200s deployment/oai-spgwu-tiny$supfc1 -n oai
			
			gnbsimc=10
			for ((sim=1;sim<=$nousers;sim++))
			do

				sed -i "/name/c\name: oai-gnbsim$gnbsims" charts/oai-gnbsim/Chart.yaml
				sed -i "/ngapIPadd/c\  ngapIPadd: \"10.$repmod.19.$gnbsimc\"" charts/oai-gnbsim/values.yaml
				sed -i "/gtpIPadd/c\  gtpIPadd: \"10.$repmod.20.$gnbsimc\"" charts/oai-gnbsim/values.yaml
				sed -i "/gtpulocaladdr/c\  gtpulocaladdr: \"10.$repmod.19.$gnbsimc\"" charts/oai-gnbsim/values.yaml
				sed -i "/ngappeeraddr/c\  ngappeeraddr: \"10.$repmod.15.$amfc\"" charts/oai-gnbsim/values.yaml
				sed -i "/gnbid/c\  gnbid: \"$gnbsims\"" charts/oai-gnbsim/values.yaml
				sed -i "/msin/c\  msin: \"00000000$gnbsimc\"" charts/oai-gnbsim/values.yaml
				sed -i "15s/.*/  name: \"oai-gnbsim-sa$gnbsims\"/" charts/oai-gnbsim/values.yaml
				sed -i "/key/c\  key: \"0C0A34601D4F07677303652C046253$gnbsimc\"" charts/oai-gnbsim/values.yaml
				sed -i "/version/c\  version: ${gnbsimim}" charts/oai-gnbsim/values.yaml

				helm install gnbsim$gnbsims charts/oai-gnbsim/ -n oai 
				kubectl wait --for=condition=available --timeout=200s deployment/oai-gnbsim$gnbsims -n oai	
				
				#---------------------------------------------------------------------------------------
				#sleep 12

				gnbsimpod=$(kubectl get pods -n oai  | grep oai-gnbsim$gnbsims | awk '{print $1}')
				((gnbsimIP=$sim+1))
				echo $gnbsimIP
				
				#-----------------------------DNN Deployment--------------------------------------------------------------
				sed -i "/name/c\  name: dnn-net$gnbsims" charts/oai-dnn/01_multus.yaml
				sed -i "/\"address\":/c\                        \"address\": \"10.$repmod.25.$gnbsimc/16\"," charts/oai-dnn/01_multus.yaml
				sed -i "4s/.*/  name: oai-dnn$gnbsims/" charts/oai-dnn/02_deployment.yaml
				sed -i "6s/.*/    app: oai-dnn$gnbsims/" charts/oai-dnn/02_deployment.yaml
				sed -i "11s/.*/      app: oai-dnn$gnbsims/" charts/oai-dnn/02_deployment.yaml
				sed -i "17s/.*/        app: oai-dnn$gnbsims/" charts/oai-dnn/02_deployment.yaml
				sed -i "19s/.*/        k8s.v1.cni.cncf.io\/networks: dnn-net$gnbsims/" charts/oai-dnn/02_deployment.yaml
				sed -i "26s/.*/        image: tolgaomeratalay\/oai-dnn:${dnnim}/" charts/oai-dnn/02_deployment.yaml

				kubectl apply -k charts/oai-dnn/
				kubectl wait --for=condition=available --timeout=200s deployment/oai-dnn$gnbsims -n oai
				dnnpod=$(kubectl get pods -n oai  | grep oai-dnn$gnbsims | awk '{print $1}')
				kubectl exec -it -n oai $dnnpod -- iptables -t nat -A POSTROUTING -o net1 -j MASQUERADE
				kubectl exec -it -n oai $dnnpod -- ip route add 12.1.1.0/24 via 10.$repmod.17.10 dev net1
				kubectl exec -it -n oai $gnbsimpod -- ip route replace 10.$repmod.25.$gnbsimc via 0.0.0.0 dev net1 src 12.1.1.$gnbsimIP
				
				((gnbsimc+=1))
				((gnbsims+=1))
			done
		done
		((repmod+=1))
		((nrfc+=1))
		((amfc+=1))
		((supfc1+=1))
		((supfc2+=1))
	done

	#echo "Finished core network deployment for $a aggregated users. Starting traffic generation"

	((trafficusers=$nousers-1))
	#/bin/bash ./charts/start_traffic_cm.sh $trafficusers $a $rep $dnnim
	((undeployusers=$nousers*$rep+1))
	#/bin/bash ./charts/undeploy_all.sh $undeployusers
	#echo "Finished this round. Waiting 60s for undeployment"
	#sleep 60
done
