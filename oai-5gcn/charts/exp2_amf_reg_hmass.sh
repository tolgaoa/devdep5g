#!/bin/bash

users=0;
for ((j=1;j<=3;j++));
do
	ngapIP=$(($j+10))
	ngapsubnet=$(($j+20))

	for ((i=10;i<=249;i++));
	do
		var1=$(($j-1))
		deploynum=$(($var1*240+$i))
		if [ $i -le 99 ]
		then

			sed -i "/name/c\name: oai-gnbsim$deploynum" oai-gnbsim/Chart.yaml
			sed -i "/ngapIPadd/c\  ngapIPadd: \"192.168.$ngapIP.$i\"" oai-gnbsim/values.yaml
			sed -i "/gtpIPadd/c\  gtpIPadd: \"192.168.$ngapsubnet.$i\"" oai-gnbsim/values.yaml
			sed -i "/gtpulocaladdr/c\  gtpulocaladdr: \"192.168.$ngapIP.$i\"" oai-gnbsim/values.yaml
			sed -i "/gnbid/c\  gnbid: \"$deploynum\"" oai-gnbsim/values.yaml
			sed -i "/msin/c\  msin: \"${j}0000000$i\"" oai-gnbsim/values.yaml
			sed -i "16s/.*/  name: \"oai-gnbsim-sa$deploynum\"/" oai-gnbsim/values.yaml
			sed -i "/key/c\  key: \"0C${j}A34601D4F07677303652C046250$i\"" oai-gnbsim/values.yaml

		else

			sed -i "/name/c\name: oai-gnbsim$deploynum" oai-gnbsim/Chart.yaml
			sed -i "/ngapIPadd/c\  ngapIPadd: \"192.168.$ngapIP.$i\"" oai-gnbsim/values.yaml
			sed -i "/gtpIPadd/c\  gtpIPadd: \"192.168.$ngapsubnet.$i\"" oai-gnbsim/values.yaml
			sed -i "/gtpulocaladdr/c\  gtpulocaladdr: \"192.168.$ngapIP.$i\"" oai-gnbsim/values.yaml
			sed -i "/gnbid/c\  gnbid: \"$deploynum\"" oai-gnbsim/values.yaml
			sed -i "/msin/c\  msin: \"${j}000000$i\"" oai-gnbsim/values.yaml
			sed -i "16s/.*/  name: \"oai-gnbsim-sa$deploynum\"/" oai-gnbsim/values.yaml
			sed -i "/key/c\  key: \"0C${j}A34601D4F07677303652C04625$i\"" oai-gnbsim/values.yaml

		fi

			helm install gnbsim$deploynum oai-gnbsim/ -n oai
			((users+=1))
			kubectl wait --for=condition=available --timeout=200s deployment/oai-gnbsim$deploynum -n oai

			sleep 15
			amfpod=$(kubectl get pods -n oai  | grep amf | awk '{print $1}')
			data=$(kubectl top pod -n oai $amfpod)
			k="0"
			while [ $k -lt 4 ]
			do
				data=$(echo $data | awk '{$1=""}1' | awk '{$1=$1}1')
				((k++))
			done
			echo "$data " ${users} >> logs/amf_compute_logs.txt

			smfpod=$(kubectl get pods -n oai  | grep smf | awk '{print $1}')
			data=$(kubectl top pod -n oai $smfpod)
			k="0"
			while [ $k -lt 4 ]
			do
			data=$(echo $data | awk '{$1=""}1' | awk '{$1=$1}1')
			((k++))
			done
			echo "$data " ${users} >> logs/smf_compute_logs.txt

			sleep 0.5s

	done
done

