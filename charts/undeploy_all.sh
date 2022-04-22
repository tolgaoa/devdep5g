#!/bin/bash

for ((count=10;count<$1+10;count++))
do
	helm delete nrf$count -n oai
	helm delete udr$count -n oai
	helm delete udm$count -n oai
	helm delete ausf$count -n oai
	helm delete amf$count -n oai
	helm delete smf$count -n oai
	helm delete upf$count -n oai
        helm delete gnbsim$count -n oai
	kubectl delete deployment -n oai oai-dnn$count
done
