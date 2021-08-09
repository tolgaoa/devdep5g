#!/bin/bash

# Functioning of the script
# 1. Start
# 1.1 Start the core network components (Mysql ---> NRF ---> AMF ---> SMF --> SPGWU)
# 1.2 Check if the components started properly (skip)
# 1.3 Check if the components are healthy, calculate individual time
# 1.4 Check if the components are connected and core network is configured properly
# 1.5 Green light
# 2. Stop

RED='\033[0;31m'
NC='\033[0m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
STATUS=0 #STATUS 0 exit safe STATUS 1 exit with an error

if [[ $1 == 'start' ]]; then
	start_time=$(date +%s%N | cut -b1-13)
	if [[ $2 == 'nrf' ]]; then
		echo -e "${BLUE}Starting 5gcn components in the order nrf, mysql, amf, smf, spgwu${NC}..."
		docker-compose -f docker-compose.yaml -p 5gcn up -d
	else
		echo -e "${BLUE}Starting 5gcn components in the order mysql, amf, smf, spgwu${NC}..."
		docker-compose -f docker-compose-no-nrf.yaml -p 5gcn up -d
	fi
	echo -e "${GREEN}Checking the health status of the containers${NC}..."
	for loop in $(seq 1 25); do
		mysql_health=$(docker inspect --format='{{json .State.Health.Status}}' mysql)
		if [[ $2 == 'nrf' ]]; then
			nrf_health=$(docker inspect --format='{{json .State.Health.Status}}' oai-nrf)
		fi
		amf_health=$(docker inspect --format='{{json .State.Health.Status}}' oai-amf)
		smf_health=$(docker inspect --format='{{json .State.Health.Status}}' oai-smf)
		spgwu_health=$(docker inspect --format='{{json .State.Health.Status}}' oai-spgwu)
		if [[ ${mysql_health} == '"healthy"' && ${amf_health} == '"healthy"' && ${smf_health} == '"healthy"' && ${spgwu_health} == '"healthy"' && $2 != 'nrf' ]]; then
			echo -e "\n${GREEN}All components are healthy${NC}..."
			STATUS=0
			break
		elif [[ ${mysql_health} == '"healthy"' && ${amf_health} == '"healthy"' && ${smf_health} == '"healthy"' && ${spgwu_health} == '"healthy"' && ${nrf_health} == '"healthy"' && $2 == 'nrf' ]]; then
			echo -e "\n${GREEN}All components are healthy${NC}..."
			STATUS=0
			break
		elif [[ $2 != 'nrf' ]]; then
                        echo -ne "mysql : $mysql_health, oai-amf : $amf_health, oai-smf : $smf_health, oai-spgwu : $spgwu_health\033[0K\r"
			STATUS=1
			sleep 2
		else
                        echo -ne "oai-nrf : $nrf_health, mysql : $mysql_health, oai-amf : $amf_health, oai-smf : $smf_health, oai-spgwu : $spgwu_health\033[0K\r"
			STATUS=1
			sleep 2
		fi
	done
	if [[ $2 == 'nrf' && $STATUS == 0 ]]; then
		echo -e "\nChecking if SMF and UPF registered with nrf core network"
		smf_registration_nrf=$(curl -s -X GET http://192.168.70.130/nnrf-nfm/v1/nf-instances?nf-type="SMF" | grep -o '192.168.70.133')
		upf_registration_nrf=$(curl -s -X GET http://192.168.70.130/nnrf-nfm/v1/nf-instances?nf-type="UPF" | grep -o '192.168.66.134')
		sample_registration=$(curl -s -X GET http://192.168.70.130/nnrf-nfm/v1/nf-instances?nf-type="SMF")
		echo -e "\n${BLUE}For example: oai-smf Registration with oai-nrf can be checked on this url /nnrf-nfm/v1/nf-instances?nf-type='SMF' $sample_registration${NC}"
		if [[ -z $smf_registration_nrf && -z $upf_registration_nrf ]]; then
			echo -e "${RED}Registration problem with NRF, check the reason manually${NC}..."
			STATUS=1
		else
			echo -e "${GREEN}SMF and UPF are registered to NRF${NC}..."
		fi
	else
		echo -e "${BLUE}Checking if SMF is able to connect with UPF${NC}"
		upf_logs=$(docker logs oai-spgwu | grep  'Received SX HEARTBEAT RESPONSE')
		if [[ -z $upf_logs && $STATUS == 0 ]]; then
		 	echo -e "\n${RED}UPF not receiving heartbeats from SMF${NC}..."
		 	STATUS=1
		else
		 	echo -e "\n${GREEN}UPF receiving heathbeats from SMF${NC}..."
		fi
	fi
	end_time=$(date +%s%N | cut -b1-13)
	final_time=$(expr $(expr $end_time - $start_time))
	if [[ $STATUS == 0 ]]; then
		echo -e "\n${GREEN}Core network is configured and healthy, total time taken $final_time milli seconds${NC}"
		exit $STATUS
	else
		echo -e "\n${RED}Core network is un-healthy, total time taken $final_time milli seconds\ndebug using docker inspect command...${NC}"
		exit $STATUS
	fi

elif [[ $1 == 'stop' ]]; then
	echo -e "${BLUE}Stopping the core network${NC}..."
	if [[ $2 == 'nrf' ]]; then
		docker-compose -f docker-compose.yaml -p 5gcn down
	else
		docker-compose -f docker-compose-no-nrf.yaml -p 5gcn down
	fi
	echo -e "${GREEN}Core network stopped${NC}"
else
	echo -e "Only use the following options\n\n${RED}start${NC}: start the 5gCN\n${RED}stop${NC}: stops the 5gCN\n${RED}nrf${NC}: nrf should be used\n${RED}no-nrf${NC}: nrf should not be used\nExample: ./core-network.sh start nrf"
fi
