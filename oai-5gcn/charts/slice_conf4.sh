#!/bin/bash


nrfci=10
amfci=10
supfc1i=10
supfc2i=40

nrfc=10
amfc=10
supfc1=10
supfc2=40
gnbsimc=10

for ((nrf=1;nrf<=5;nrf++))
do
	#----------------------------------NRF Deployment----------------------------------------------------------
	sed -i "/name: oai-nrf/c\name: oai-nrf$nrfc " oai-nrf/Chart.yaml
	sed -i "/oai-nrf-sa/c\  name: \"oai-nrf-sa$nrfc\"" oai-nrf/values.yaml
	sed -i "/nrfIpNg/c\  nrfIpNg: \"192.168.11.$nrfc\"" oai-nrf/values.yaml
	helm install nrf$nrfc oai-nrf/ -n oai
	kubectl wait --for=condition=available --timeout=200s deployment/oai-nrf$nrfc -n oai

	for ((amf=1;amf<=2;amf++))
	do
		#----------------------------------AMF Deployment----------------------------------------------------------
		sed -i "/name: oai-amf/c\name: oai-amf$amfc " oai-amf/Chart.yaml
		sed -i "/oai-amf-sa/c\  name: \"oai-amf-sa$amfc\"" oai-amf/values.yaml
		#sed -i "/mySqlServer/c\  mySqlServer: \"$mysqlIP\"" oai-amf/values.yaml
		sed -i "/ngapIPadd/c\  ngapIPadd: \"192.168.12.$amfc\"" oai-amf/values.yaml	
		sed -i "/nrfIpv4Addr/c\  nrfIpv4Addr: \"192.168.11.$nrfc\"" oai-amf/values.yaml	
		sed -i "/smfIpv4Addr0/c\  smfIpv4Addr0: \"192.168.13.$supfc1\"" oai-amf/values.yaml
		sed -i "/smfIpv4Addr1/c\  smfIpv4Addr1: \"192.168.13.$supfc2\"" oai-amf/values.yaml

		helm install amf$amfc oai-amf/ -n oai
		kubectl wait --for=condition=available --timeout=200s deployment/oai-amf$amfc -n oai
		amfpod=$(kubectl get pods -n oai  | grep amf$amfc | awk '{print $1}')
		amfnet1IP=$(kubectl exec -n oai $amfpod -c amf -- ifconfig | grep "inet 192.168" | awk '{print $2}')
		kubectl exec -n oai $amfpod -c amf -- ip route del default
		kubectl exec -n oai $amfpod -c amf -- ip route add default via 169.254.1.1
		kubectl exec -n oai $amfpod -c amf -- ip route del 192.168.0.0/16 via 0.0.0.0
		kubectl exec -n oai $amfpod -c amf -- ip route add 192.168.0.0/16 via $amfnet1IP
	        #kubectl exec -n oai $amfpod -c amf -- ping -c 4 $mysqlIP

		#----------------------------------SMF Deployment----------------------------------------------------------
		sed -i "/name: oai-smf/c\name: oai-smf$supfc1 " oai-smf/Chart.yaml
		sed -i "/oai-smf-sa/c\  name: \"oai-smf-sa$supfc1\"" oai-smf/values.yaml
		sed -i "/n4IPadd/c\  n4IPadd: \"192.168.13.$supfc1\"" oai-smf/values.yaml
		sed -i "/amfIpv4Address/c\  amfIpv4Address: \"192.168.12.$amfc\"" oai-smf/values.yaml
		sed -i "/nrfIpv4Address/c\  nrfIpv4Address: \"192.168.11.$nrfc\"" oai-smf/values.yaml
		sed -i "/upfIpv4Address/c\  upfIpv4Address: \"192.168.14.$supfc1\"" oai-smf/values.yaml
		helm install smf$supfc1 oai-smf/ -n oai
		kubectl wait --for=condition=available --timeout=200s deployment/oai-smf$supfc1 -n oai


		sed -i "/name: oai-smf/c\name: oai-smf$supfc2 " oai-smf/Chart.yaml
		sed -i "/oai-smf-sa/c\  name: \"oai-smf-sa$supfc2\"" oai-smf/values.yaml
		sed -i "/n4IPadd/c\  n4IPadd: \"192.168.13.$supfc2\"" oai-smf/values.yaml
		sed -i "/amfIpv4Address/c\  amfIpv4Address: \"192.168.12.$amfc\"" oai-smf/values.yaml
		sed -i "/nrfIpv4Address/c\  nrfIpv4Address: \"192.168.11.$nrfc\"" oai-smf/values.yaml
		sed -i "/upfIpv4Address/c\  upfIpv4Address: \"192.168.14.$supfc2\"" oai-smf/values.yaml
		helm install smf$supfc2 oai-smf/ -n oai
		kubectl wait --for=condition=available --timeout=200s deployment/oai-smf$supfc2 -n oai
		

		#----------------------------------UPF Deployment----------------------------------------------------------
		sed -i "/name: oai-spgwu-tiny/c\name: oai-spgwu-tiny$supfc1 " oai-spgwu-tiny/Chart.yaml
		sed -i "/oai-spgwu-tiny-sa/c\  name: \"oai-spgwu-tiny-sa$supfc1\"" oai-spgwu-tiny/values.yaml
		sed -i "/sgwS1uIp/c\  sgwS1uIp: \"192.168.14.$supfc1\"" oai-spgwu-tiny/values.yaml
		sed -i "/sgwSxIp/c\  sgwSxIp: \"192.168.15.$supfc1\" #net2 IP address for UPF" oai-spgwu-tiny/values.yaml
		sed -i "/spgwc0IpAdd/c\  spgwc0IpAdd: \"192.168.13.$supfc1\" #SMF IP address" oai-spgwu-tiny/values.yaml
		sed -i "/nrfIpv4Add/c\  nrfIpv4Add: \"192.168.11.$nrfc\" #NRF IP address" oai-spgwu-tiny/values.yaml
		helm install upf$supfc1 oai-spgwu-tiny/ -n oai
		kubectl wait --for=condition=available --timeout=200s deployment/oai-spgwu-tiny$supfc1 -n oai

		sed -i "/name: oai-spgwu-tiny/c\name: oai-spgwu-tiny$supfc2 " oai-spgwu-tiny/Chart.yaml
		sed -i "/oai-spgwu-tiny-sa/c\  name: \"oai-spgwu-tiny-sa$supfc2\"" oai-spgwu-tiny/values.yaml
		sed -i "/sgwS1uIp/c\  sgwS1uIp: \"192.168.14.$supfc2\"" oai-spgwu-tiny/values.yaml
		sed -i "/sgwSxIp/c\  sgwSxIp: \"192.168.15.$supfc2\" #net2 IP address for UPF" oai-spgwu-tiny/values.yaml
		sed -i "/spgwc0IpAdd/c\  spgwc0IpAdd: \"192.168.13.$supfc2\" #SMF IP address" oai-spgwu-tiny/values.yaml
		sed -i "/nrfIpv4Add/c\  nrfIpv4Add: \"192.168.11.$nrfc\" #NRF IP address" oai-spgwu-tiny/values.yaml
		helm install upf$supfc2 oai-spgwu-tiny/ -n oai
		kubectl wait --for=condition=available --timeout=200s deployment/oai-spgwu-tiny$supfc2 -n oai


		for ((sim=1;sim<=8;sim++))
		do

			sed -i "/name/c\name: oai-gnbsim$gnbsimc" oai-gnbsim/Chart.yaml
			sed -i "/ngapIPadd/c\  ngapIPadd: \"192.168.16.$gnbsimc\"" oai-gnbsim/values.yaml
			sed -i "/gtpIPadd/c\  gtpIPadd: \"192.168.17.$gnbsimc\"" oai-gnbsim/values.yaml
			sed -i "/gtpulocaladdr/c\  gtpulocaladdr: \"192.168.16.$gnbsimc\"" oai-gnbsim/values.yaml
			sed -i "/ngappeeraddr/c\  ngappeeraddr: \"192.168.12.$amfc\"" oai-gnbsim/values.yaml
			sed -i "/gnbid/c\  gnbid: \"$gnbsimc\"" oai-gnbsim/values.yaml
			sed -i "/msin/c\  msin: \"10000000$gnbsimc\"" oai-gnbsim/values.yaml
			sed -i "16s/.*/  name: \"oai-gnbsim-sa$gnbsimc\"/" oai-gnbsim/values.yaml
			sed -i "/key/c\  key: \"0C1A34601D4F07677303652C046250$gnbsimc\"" oai-gnbsim/values.yaml

			helm install gnbsim$gnbsimc oai-gnbsim/ -n oai 
			kubectl wait --for=condition=available --timeout=200s deployment/oai-gnbsim$gnbsimc -n oai	

			sleep 15
			for ((ac=$amfci;ac<=$amfc;ac++))
			do
				pod=$(kubectl get pods -n oai  | grep amf$ac | awk '{print $1}')
				data=$(kubectl top pod -n oai $pod)
				k="0"
				while [ $k -lt 4 ]
				do
					data=$(echo $data | awk '{$1=""}1' | awk '{$1=$1}1')
					((k++))
				done
				echo "$data" >> logs/slices/conf4/compute_logs$gnbsimc.txt
			done
			for ((sc=$supfc1i;sc<=supfc1;sc++))
			do
				pod=$(kubectl get pods -n oai  | grep smf$sc | awk '{print $1}')
				data=$(kubectl top pod -n oai $pod)
				k="0"
				while [ $k -lt 4 ]
				do
					data=$(echo $data | awk '{$1=""}1' | awk '{$1=$1}1')
					((k++))
				done
				echo "$data" >> logs/slices/conf4/compute_logs$gnbsimc.txt
				pod=$(kubectl get pods -n oai  | grep spgwu-tiny$sc | awk '{print $1}')
				data=$(kubectl top pod -n oai $pod)
				k="0"
				while [ $k -lt 4 ]
				do
					data=$(echo $data | awk '{$1=""}1' | awk '{$1=$1}1')
					((k++))
				done
				echo "$data" >> logs/slices/conf4/compute_logs$gnbsimc.txt
			done
			for ((sc=$supfc2i;sc<=supfc2;sc++))
			do
				pod=$(kubectl get pods -n oai  | grep smf$sc | awk '{print $1}')
				data=$(kubectl top pod -n oai $pod)
				k="0"
				while [ $k -lt 4 ]
				do
					data=$(echo $data | awk '{$1=""}1' | awk '{$1=$1}1')
					((k++))
				done
				echo "$data" >> logs/slices/conf4/compute_logs$gnbsimc.txt
				pod=$(kubectl get pods -n oai  | grep spgwu-tiny$sc | awk '{print $1}')
				data=$(kubectl top pod -n oai $pod)
				k="0"
				while [ $k -lt 4 ]
				do
					data=$(echo $data | awk '{$1=""}1' | awk '{$1=$1}1')
					((k++))
				done
				echo "$data" >> logs/slices/conf4/compute_logs$gnbsimc.txt
			done
			for ((nc=$nrfci;nc<=$nrfc;nc++))
			do
				pod=$(kubectl get pods -n oai  | grep nrf$nc | awk '{print $1}')
				data=$(kubectl top pod -n oai $pod)
				k="0"
				while [ $k -lt 4 ]
				do
					data=$(echo $data | awk '{$1=""}1' | awk '{$1=$1}1')
					((k++))
				done
				echo "$data" >> logs/slices/conf4/compute_logs$gnbsimc.txt 
			done
			((gnbsimc+=1))
		done
		((supfc1+=1))
		((supfc2+=1))
		((amfc+=1))
	done
	((nrfc+=1))
done
