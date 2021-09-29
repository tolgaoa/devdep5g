#!/bin/bash

rm nrf_compute_logs.txt

vnfc=0
users=0
vnfpernrf=2
simperses=2
$((loopmod=10+$vnfpernrf-1))

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

	data=$(kubectl top pod -n oai oai-nrf1-7ff444bcc6-48bm2)
	i="0"
	while [ $i -lt 4 ]
	do
		data=$(echo $data | awk '{$1=""}1' | awk '{$1=$1}1')
		((i++))
	done
	echo "$data " ${vnfc} " " ${users} >> nrf_compute_logs.txt

	helm install upf$count oai-spgwu-tiny/ -n oai
	kubectl wait --for=condition=available --timeout=200s deployment/oai-spgwu-tiny$count -n oai
	((vnfc+=1))
	data=$(kubectl top pod -n oai oai-nrf1-7ff444bcc6-48bm2)
	i="0"
	while [ $i -lt 4 ]
	do
		data=$(echo $data | awk '{$1=""}1' | awk '{$1=$1}1')
		((i++))
	done
	echo "$data " ${vnfc} " " ${users} >> nrf_compute_logs.txt   
done

$((loopmod2=100+$simperses-1))

for ((sim=100;sim<=loopmod2;sim++));
do
	ngapIP=$sim
	((gtpuIP=$sim+$simperses))
	((udmmod=$sim-90))
        #----------------------------------GNBSIM Modifications-------------------------------------------------------
	sed -i "/name/c\name: oai-gnbsim$sim" oai-gnbsim/Chart.yaml
	sed -i "/ngapIPadd/c\  ngapIPadd: \"192.168.18.$ngapIP\"" oai-gnbsim/values.yaml
	sed -i "/gtpulocaladdr/c\  gtpulocaladdr: \"192.168.18.$gtpuIP\"" oai-gnbsim/values.yaml
	sed -i "/gtpIPadd/c\  gtpIPadd: \"192.168.18.$gtpuIP\"" oai-gnbsim/values.yaml
	sed -i "/gnbid/c\  gnbid: \"$sim\"" oai-gnbsim/values.yaml
	sed -i "/msin/c\  msin: \"00000000$udmmod\"" oai-gnbsim/values.yaml

	sed -i "16s/.*/  name: \"oai-gnbsim-sa$sim\"/" oai-gnbsim/values.yaml


	sed -i "/key/c\  key: \"0C0A34601D4F07677303652C046253$udmmod\" " oai-gnbsim/values.yaml

	helm install gnbsim$sim oai-gnbsim/ -n oai
	kubectl wait --for=condition=available --timeout=200s deployment/oai-gnbsim$sim -n oai
	sleep 15
	((users+=1))

	data=$(kubectl top pod -n oai oai-nrf1-7ff444bcc6-48bm2)
	i="0"
	while [ $i -lt 4 ]
	do
		data=$(echo $data | awk '{$1=""}1' | awk '{$1=$1}1')
		((i++))
		done
	echo "$data " ${vnfc} " " ${users} >> nrf_compute_logs.txt
done
