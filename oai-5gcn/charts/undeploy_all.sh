#!/bin/bash

for ((count=10;count<=90;count++));
do
	helm delete smf$count -n oai
	helm delete upf$count -n oai
	helm delete amf$count -n oai 
	helm delete nrf$count -n oai 
	helm delete gnbsim$count -n oai 
done
