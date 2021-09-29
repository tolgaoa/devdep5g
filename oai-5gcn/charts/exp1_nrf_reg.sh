#!/bin/bash

###########################################
###AUTOMATED DEPOYMENT OF SMF AND UPF TO###
########TEST FOR CPU CONSUMPTION###########
###########################################
##########AUTHOR: Tolga Atalay#############
###########################################


for ((j=1;j<=2;j++))
do
	n4subnet=$(($j+10))
	upfsubnet=$(($j+20))

	for ((i=10;i<=249;i++));
	do
		var1=$(($j-1))
		deploynum=$(($var1*240+$i))

	if [ $i -le 99 ]
	then
		#----------------------------------SMF Modifications-------------------------------------------------------
		sed -i "/name: oai-smf/c\name: oai-smf$deploynum " oai-smf/Chart.yaml
		sed -i "/oai-smf-sa/c\  name: \"oai-smf-sa$deploynum\"" oai-smf/values.yaml
		sed -i "/n4IPadd/c\  n4IPadd: \"192.168.$n4subnet.$i\"" oai-smf/values.yaml
		sed -i "/upfIpv4Address/c\  upfIpv4Address: \"192.168.$upfsubnet.$i\"" oai-smf/values.yaml

		#----------------------------------UPF Modifications-------------------------------------------------------
		sed -i "/name: oai-spgwu-tiny/c\name: oai-spgwu-tiny$deploynum " oai-spgwu-tiny/Chart.yaml
		sed -i "/oai-spgwu-tiny-sa/c\  name: \"oai-spgwu-tiny-sa$deploynum\"" oai-spgwu-tiny/values.yaml
		#sed -i "/volumePath/c\  volumePath: \"/k8s-data/sharedpvgnb$upfc\"" oai-spgwu-tiny/values.yaml
		sed -i "/sgwS1uIp/c\  sgwS1uIp: \"192.168.$upfsubnet.$i\"" oai-spgwu-tiny/values.yaml
		sed -i "/spgwc0IpAdd/c\  spgwc0IpAdd: \"192.168.$n4subnet.$i\" #SMF IP address " oai-spgwu-tiny/values.yaml

	else
		#----------------------------------SMF Modifications-------------------------------------------------------
		sed -i "/name: oai-smf/c\name: oai-smf$deploynum " oai-smf/Chart.yaml
		sed -i "/oai-smf-sa/c\  name: \"oai-smf-sa$deploynum\"" oai-smf/values.yaml
		sed -i "/n4IPadd/c\  n4IPadd: \"192.168.$n4subnet.$i\"" oai-smf/values.yaml
		sed -i "/upfIpv4Address/c\  upfIpv4Address: \"192.168.$upfsubnet.$i\"" oai-smf/values.yaml

		#----------------------------------UPF Modifications-------------------------------------------------------
		sed -i "/name: oai-spgwu-tiny/c\name: oai-spgwu-tiny$deploynum " oai-spgwu-tiny/Chart.yaml
		sed -i "/oai-spgwu-tiny-sa/c\  name: \"oai-spgwu-tiny-sa$deploynum\"" oai-spgwu-tiny/values.yaml
		#sed -i "/volumePath/c\  volumePath: \"/k8s-data/sharedpvgnb$upfc\"" oai-spgwu-tiny/values.yaml
		sed -i "/sgwS1uIp/c\  sgwS1uIp: \"192.168.$upfsubnet.$i\"" oai-spgwu-tiny/values.yaml
		sed -i "/spgwc0IpAdd/c\  spgwc0IpAdd: \"192.168.$n4subnet.$i\" #SMF IP address " oai-spgwu-tiny/values.yaml

	fi

	helm install smf$deploynum oai-smf/ -n oai 
	kubectl wait --for=condition=available --timeout=200s deployment/oai-smf$deploynum -n oai
	((vnfc+=1))
	
	data=$(kubectl top pod -n oai oai-nrf1-7ff444bcc6-qdf77)
	k="0"
	while [ $k -lt 4 ]
	do
		data=$(echo $data | awk '{$1=""}1' | awk '{$1=$1}1')
		((k++))
	done
	echo "$data " ${vnfc} >> scripts/nrf_compute_logs.txt
	sleep 1s

	helm install upf$deploynum oai-spgwu-tiny/ -n oai
	kubectl wait --for=condition=available --timeout=200s deployment/oai-spgwu-tiny$deploynum -n oai
	((vnfc+=1))

	data=$(kubectl top pod -n oai oai-nrf1-7ff444bcc6-qdf77)
	k="0"
	while [ $k -lt 4 ]
	do
		data=$(echo $data | awk '{$1=""}1' | awk '{$1=$1}1')
		((k++))
	done
	echo "$data " ${vnfc} >> scripts/nrf_compute_logs.txt   
	sleep 1s
	done
done
