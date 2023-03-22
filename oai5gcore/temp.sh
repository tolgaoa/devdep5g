#!/bin/bash

helm install nrf oai-nrf/ -n oai
sleep 8
helm install udr oai-udr/ -n oai
sleep 8
helm install udm oai-udm/ -n oai
sleep 8
helm install ausf oai-ausf/ -n oai
sleep 8
helm install amf oai-amf/ -n oai
sleep 8
helm install smf oai-smf/ -n oai
sleep 8
helm install upf oai-spgwu-tiny_new/ -n oai
sleep 8

amfpod=$(kubectl get pods -n oai  | grep amf | awk '{print $1}')
amfeth0=$(kubectl exec -n oai $amfpod -c amf -- ifconfig | grep "inet 10.42" | awk '{print $2}')

sed -i "/ngappeeraddr/c\  ngappeeraddr: \"$amfeth0\"" oai-gnbsim/values.yaml
sed -i "/ngappeeraddr/c\  ngappeeraddr: \"$amfeth0\"" gnbsim/values.yaml

helm install gnb gnbsim/ -n oai 
sleep 8 
kubectl apply -k oai-dnn/
sleep 5
upfpod=$(kubectl get pods -n oai  | grep spgwu | awk '{print $1}')
upfeth0=$(kubectl exec -n oai $upfpod -c spgwu -- ifconfig | grep "inet 10.42" | awk '{print $2}')
dnnpod=$(kubectl get pods -n oai  | grep oai-dnn | awk '{print $1}')
kubectl exec -it -n oai $dnnpod -- iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE
kubectl exec -it -n oai $dnnpod -- ip route add 12.1.1.0/24 via $upfeth0 dev eth0
