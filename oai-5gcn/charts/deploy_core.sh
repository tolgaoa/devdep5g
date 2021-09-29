#!/bin/bash

#######################################################################
###################DEPLOY THE CORE NETWORK ############################
##############AND MODIFY THE NETWORK PARAMETERS########################
#######################################################################
#######################################################################

for ((share=1;share<=1;share++))
do 
	#----------------------------------MySQL Deployment--------------------------------------------------------
	sed -i "/volumePath:/c\  volumePath: \"/k8s-data/sharedpvmysql$share\"" mysql/templates/pv.yaml
	helm install mysql$share mysql/ -n oai
	kubectl wait --for=condition=available --timeout=200s deployment/mysql$share -n oai
	mysqlpod=$(kubectl get pods -n oai  | grep mysql$1 | awk '{print $1}')
	mysqlIP=$(kubectl get pods -n oai -o wide | grep mysql$1 | awk '{print $6}')

	#----------------------------------NRF Deployment----------------------------------------------------------
	sed -i "/name: oai-nrf/c\name: oai-nrf$share " oai-nrf/Chart.yaml
	sed -i "/oai-nrf-sa/c\  name: \"oai-nrf-sa$share\"" oai-nrf/values.yaml
	helm install nrf$share oai-nrf/ -n oai 
	kubectl wait --for=condition=available --timeout=200s deployment/oai-nrf$share -n oai

	#----------------------------------AMF Deployment----------------------------------------------------------
	sed -i "/name: oai-amf/c\name: oai-amf$share " oai-amf/Chart.yaml
	sed -i "/oai-amf-sa/c\  name: \"oai-amf-sa$share\"" oai-amf/values.yaml
	sed -i "/mySqlServer/c\  mySqlServer: \"$mysqlIP\"" oai-amf/values.yaml
	helm install amf$share oai-amf/ -n oai
	kubectl wait --for=condition=available --timeout=200s deployment/oai-amf$share -n oai
	amfpod=$(kubectl get pods -n oai  | grep amf$share | awk '{print $1}')
	amfnet1IP=$(kubectl exec -n oai $amfpod -c amf -- ifconfig | grep "inet 192.168" | awk '{print $2}')
	kubectl exec -n oai $amfpod -c amf -- ip route del default
	kubectl exec -n oai $amfpod -c amf -- ip route add default via 169.254.1.1
	kubectl exec -n oai $amfpod -c amf -- ip route del 192.168.10.0/16 via 0.0.0.0
	kubectl exec -n oai $amfpod -c amf -- ip route add 192.168.10.0/16 via $amfnet1IP
	kubectl exec -n oai $amfpod -c amf -- ping -c 4 $mysqlIP

	#----------------------------------SMF Deployment----------------------------------------------------------
	sed -i "/name: oai-smf/c\name: oai-smf$share " oai-smf/Chart.yaml
	sed -i "/oai-smf-sa/c\  name: \"oai-smf-sa$share\"" oai-smf/values.yaml  
	helm install smf$share oai-smf/ -n oai
	kubectl wait --for=condition=available --timeout=200s deployment/oai-smf$share -n oai 

	#----------------------------------UPF Deployment----------------------------------------------------------
	sed -i "/name: oai-spgwu-tiny/c\name: oai-spgwu-tiny$share " oai-spgwu-tiny/Chart.yaml
	sed -i "/oai-spgwu-tiny-sa/c\  name: \"oai-spgwu-tiny-sa$share\"" oai-spgwu-tiny/values.yaml
	sed -i "/volumePath/c\  volumePath: \"/k8s-data/sharedpvgnb$share\"" oai-spgwu-tiny/values.yaml
	#sed -i "/sgwS1uIp/c\  sgwS1uIp: \"192.168.18.16$share\"" oai-spgwu-tiny/values.yaml
	helm install upf$share oai-spgwu-tiny/ -n oai
	kubectl wait --for=condition=available --timeout=200s deployment/oai-spgwu-tiny$share -n oai
	upfpod=$(kubectl get pods -n oai  | grep spgwu$share | awk '{print $1}')
	upfnet1IP=$(kubectl exec -n oai $upfpod -c spgwu -- ifconfig | grep "inet 192.168" | awk '{print $2}')
	#---------------------------------DNN Deployment-----------------------------------------------------------
        #kubectl apply -k oai-dnn/
	#kubectl wait --for=condition=available --timeout=200s deployment/oai-dnn -n oai
	#dnnpod=$(kubectl get pods -n oai | grep "oai-dnn" | awk '{print $1}')
	#kubectl exec -n oai $dnnpod -- sudo apt update; sudo apt install -y iptables iproute2 iperf3 iputils-ping;
	#kubectl exec -n oai $dnnpod -- iptables -t nat -A POSTROUTING -o net1 -j MASQUERADE;
	#kubectl exec -n oai $dnnpod -- ip route add 12.1.1.0/24 via $upfnet1IP dev net1
done



