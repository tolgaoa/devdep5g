#!/bin/bash

rm nrf_compute_logs.txt 
gather=$((10 ** 1))
export vnfc=0
for ((j=1;j<=${gather};j++))
do
	data="test test test test test"
	#data=$(kubectl top pod -n oai oai-nrf1-7ff444bcc6-2f2tv)
	i="0"
	while [ $i -lt 4 ]
	do
		data=$(echo $data | awk '{$1=""}1' | awk '{$1=$1}1')
		((i++))
	done
	echo "$data  " ${vnfc} >> nrf_compute_logs.txt
	#echo "$data" >> nrf_compute_logs.txt
	sleep 0.5s
done


