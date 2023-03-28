#!/bin/bash

helm delete nrf -n oai
helm delete udr -n oai
helm delete udm -n oai
helm delete ausf -n oai
helm delete amf -n oai
helm delete smf -n oai
helm delete upf -n oai

for ((c=10;c<=$1+10;c++))
do
        helm delete gnb$c -n oai
        kubectl delete deployment -n oai oai-dnn$c
done

