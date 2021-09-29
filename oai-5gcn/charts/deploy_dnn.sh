#!/bin/bash


for ((j=11;j<=11;j++));
do 
	ipdnn=$(($j+1))
	sed -i "/address\"/c\                        \"address\": \"192.168.20.$ipdnn/16\"," oai-dnn/01_multus.yaml
	sed -i "4s/.*/  name: oai-dnn$j/" oai-dnn/02_deployment.yaml
	sed -i "6s/.*/    app: oai-dnn$j/" oai-dnn/02_deployment.yaml
	sed -i "11s/.*/      app: oai-dnn$j/" oai-dnn/02_deployment.yaml
	sed -i "17s/.*/        app: oai-dnn$j/" oai-dnn/02_deployment.yaml
	kubectl apply -k oai-dnn/
done
