#!/bin/bash

rm amf_compute_logs.txt
for ((i=10;i<=99;i++));
do
	sed -i "/name/c\name: oai-gnbsim$i" oai-gnbsim/Chart.yaml
	sed -i "/ngapIPadd/c\  ngapIPadd: \"192.168.18.$i\"" oai-gnbsim/values.yaml
	sed -i "/gtpIPadd/c\  gtpIPadd: \"192.168.18.1$i\"" oai-gnbsim/values.yaml
	sed -i "/gtpulocaladdr/c\  gtpulocaladdr: \"192.168.18.1$i\"" oai-gnbsim/values.yaml	
	sed -i "/gnbid/c\  gnbid: \"$i\"" oai-gnbsim/values.yaml
	sed -i "/msin/c\  msin: \"00000000$i\"" oai-gnbsim/values.yaml
	sed -i "16s/.*/  name: \"oai-gnbsim-sa$i\"/" oai-gnbsim/values.yaml
	sed -i "/key/c\  key: \"0C0A34601D4F07677303652C046250$i\"" oai-gnbsim/values.yaml

	helm install gnbsim$i oai-gnbsim/ -n oai
	kubectl wait --for=condition=available --timeout=200s deployment/oai-gnbsim$i -n oai
	sleep 20
	amfpod=$(kubectl get pods -n oai  | grep amf | awk '{print $1}')
	data=$(kubectl top pod -n oai $amfpod)
	j="0"
	while [ $j -lt 4 ]
	do
		data=$(echo $data | awk '{$1=""}1' | awk '{$1=$1}1')
		((j++))
	done
	echo "$data " ${vnfc} >> scripts/amf_compute_logs.txt
        sleep 0.5s
done
