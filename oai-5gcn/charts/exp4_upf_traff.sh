#!/bin/bash

gather=$((10 ** 5))
time=0
for ((j=1;j<=${gather};j++))
do
	((time+=1))
	upfpod=$(kubectl get pods -n oai  | grep spgwu | awk '{print $1}')
	data=$(kubectl top pod -n oai $upfpod)
	k="0"
	while [ $k -lt 4 ]
	do
	data=$(echo $data | awk '{$1=""}1' | awk '{$1=$1}1')
	((k++))
	done
	echo "$data " ${sessions} >> logs/upf_compute_logs.txt
	 
done


