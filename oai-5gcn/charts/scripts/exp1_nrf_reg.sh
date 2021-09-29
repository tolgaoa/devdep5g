#!/bin/bash

###########################################
###AUTOMATED DEPOYMENT OF SMF AND UPF TO###
########TEST FOR CPU CONSUMPTION###########
###########################################
##########AUTHOR: Tolga Atalay#############
###########################################

for ((loop=1;loop<=1;loop++))
do
	vnfc=0
	vnfpernrf=120
	loopmod=$((10+$vnfpernrf-1))
	waitcoeff=20

	for ((count=10;count<=loopmod;count++));
	do
		smfIP=$count
		((upfIP=$count+$vnfpernrf))
		#----------------------------------SMF Modifications-------------------------------------------------------
		sed -i "/name: oai-smf/c\name: oai-smf$count " oai-smf/Chart.yaml
		sed -i "/oai-smf-sa/c\  name: \"oai-smf-sa$count\"" oai-smf/values.yaml 
		sed -i "/n4IPadd/c\  n4IPadd: \"192.168.18.$smfIP\"" oai-smf/values.yaml 
		sed -i "/upfIpv4Address/c\  upfIpv4Address: \"192.168.18.$upfIP\"" oai-smf/values.yaml 

		#----------------------------------UPF Modifications-------------------------------------------------------
		sed -i "/name: oai-spgwu-tiny/c\name: oai-spgwu-tiny$count " oai-spgwu-tiny/Chart.yaml
		sed -i "/oai-spgwu-tiny-sa/c\  name: \"oai-spgwu-tiny-sa$count\"" oai-spgwu-tiny/values.yaml
		#sed -i "/volumePath/c\  volumePath: \"/k8s-data/sharedpvgnb$upfc\"" oai-spgwu-tiny/values.yaml
		sed -i "/sgwS1uIp/c\  sgwS1uIp: \"192.168.18.$upfIP\"" oai-spgwu-tiny/values.yaml
		sed -i "/spgwc0IpAdd/c\  spgwc0IpAdd: \"192.168.18.$smfIP\" #SMF IP address " oai-spgwu-tiny/values.yaml

		helm install smf$count oai-smf/ -n oai 
		kubectl wait --for=condition=available --timeout=200s deployment/oai-smf$count -n oai
		((vnfc+=1))
		
		wt=$(($count / $waitcoeff))
		data=$(kubectl top pod -n oai oai-nrf1-7ff444bcc6-d8d2f)
		i="0"
		while [ $i -lt 4 ]
		do
			data=$(echo $data | awk '{$1=""}1' | awk '{$1=$1}1')
			((i++))
		done
		echo "$data " ${vnfc} >> scripts/nrf_compute_logs$loop.txt
		sleep 0.5s

		helm install upf$count oai-spgwu-tiny/ -n oai
		kubectl wait --for=condition=available --timeout=200s deployment/oai-spgwu-tiny$count -n oai
		((vnfc+=1))

		data=$(kubectl top pod -n oai oai-nrf1-7ff444bcc6-d8d2f)
		i="0"
		while [ $i -lt 4 ]
		do
			data=$(echo $data | awk '{$1=""}1' | awk '{$1=$1}1')
			((i++))
		done
		echo "$data " ${vnfc} >> scripts/nrf_compute_logs$loop.txt   
		sleep 0.5s
	done

	for ((count=120;count>=10;count--));
	do

		helm delete smf$count -n oai
		helm delete upf$count -n oai
		((vnfc-=2))
		wt=$(($count / $waitcoeff))
		data=$(kubectl top pod -n oai oai-nrf1-7ff444bcc6-d8d2f)
		i="0"
		while [ $i -lt 4 ]
		do
			data=$(echo $data | awk '{$1=""}1' | awk '{$1=$1}1')
			((i++))
		done
		echo "$data " ${vnfc} >> scripts/nrf_compute_logs$loop.txt
		sleep 0.5s
	done
done
