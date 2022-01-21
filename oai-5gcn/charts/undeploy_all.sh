#!/bin/bash

#helm delete mysql -n oai 

for ((count=10;count<=110;count++));
do
	helm delete smf$count -n oai
	helm delete upf$count -n oai
	helm delete amf$count -n oai 
	helm delete nrf$count -n oai 
	helm delete gnbsim$count -n oai 
	kubectl delete deployment -n oai oai-dnn$count
done
