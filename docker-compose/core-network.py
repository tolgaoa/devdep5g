"""
Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
contributor license agreements.  See the NOTICE file distributed with
this work for additional information regarding copyright ownership.
The OpenAirInterface Software Alliance licenses this file to You under
the OAI Public License, Version 1.1  (the "License"); you may not use this file
except in compliance with the License.
You may obtain a copy of the License at

      http://www.openairinterface.org/?page_id=698

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
------------------------------------------------------------------------------
For more information about the OpenAirInterface (OAI) Software Alliance:
      contact@openairinterface.org

------------------------------------------------------------------------------
"""

import yaml
import re
import subprocess
import time
import logging
import argparse

logging.basicConfig(
    level=logging.DEBUG,
    format="[%(asctime)s] %(name)s:%(levelname)s: %(message)s"
)

# Docker Compose files
MINI_W_NRF = 'docker-compose-mini-nrf.yaml' 
MINI_NO_NRF = 'docker-compose-mini-nonrf.yaml'
BASIC_W_NRF = 'docker-compose-basic-nrf.yaml'
BASIC_NO_NRF = 'docker-compose-basic-nonrf.yaml'
BASIC_VPP_W_NRF = 'docker-compose-basic-vpp-nrf.yaml'
BASIC_VPP_NO_NRF = 'docker-compose-basic-vpp-nonrf.yaml'

def _parse_args() -> argparse.Namespace:
    """Parse the command line args

    Returns:
        argparse.Namespace: the created parser
    """
    example_text = '''example:
        python3 core-network.py --type start-mini
        python3 core-network.py --type start-basic
        python3 core-network.py --type start-basic-vpp
        python3 core-network.py --type stop-mini
        python3 core-network.py --type start-mini --fqdn no --scenario 2
        python3 core-network.py --type start-basic --fqdn no --scenario 2'''

    parser = argparse.ArgumentParser(description='OAI 5G CORE NETWORK DEPLOY',
                                    epilog=example_text,
                                    formatter_class=argparse.RawDescriptionHelpFormatter)

    # 5GCN function TYPE
    parser.add_argument(
        '--type', '-t',
        action='store',
        required=True,
        choices=['start-mini', 'start-basic', 'start-basic-vpp', 'stop-mini', 'stop-basic', 'stop-basic-vpp'],
        help='Functional type of 5g core network ("start-mini"|"start-basic"|"start-basic-vpp"|"stop-mini"|"stop-basic"|"stop-basic-vpp")',
    )
    # Deployment scenario with FQDN/IP based
    parser.add_argument(
        '--fqdn', '-fq',
        action='store',
        choices=['yes', 'no'],
        default='yes',
        help='Deployment scenario with FQDN ("yes"|"no")',
    )
    # Deployment scenario with NRF/ without NRF
    parser.add_argument(
        '--scenario', '-s',
        action='store',
        choices=['1', '2'],
        default='1',
        help='Scenario with NRF ("1") and without NRF ("2")',
    )
    return parser.parse_args()

def deploy(file_name, ct):
    """Deploy the containers using the docker-compose template

    Returns:
        None
    """
    # Before deploy adapting with fqdn/ip
    if args.fqdn == 'no':
        subprocess.run(f'sed -i -e "s#USE_FQDN_DNS=yes#USE_FQDN_DNS=no#g" {file_name}', shell=True)
        subprocess.run(f'sed -i -e "s#USE_FQDN_NRF=yes#USE_FQDN_NRF=no#g" {file_name}', shell=True)
    elif args.fqdn == 'yes':
        subprocess.run(f'sed -i -e "s#USE_FQDN_DNS=no#USE_FQDN_DNS=yes#g" {file_name}', shell=True)
        subprocess.run(f'sed -i -e "s#USE_FQDN_NRF=no#USE_FQDN_NRF=yes#g" {file_name}', shell=True)
    logging.debug('\033[0;34m Starting 5gcn components... Please wait\033[0m....')

    cmd = f'docker-compose -f {file_name} up -d'
    res = run_cmd(cmd)
    if res is None:
        exit(f'\033[0;31m Incorrect/Unsupported executing command {cmd}')
    print(res)
    logging.debug('\033[0;32m OAI 5G Core network started, checking the health status of the containers... takes few secs\033[0m....')
    for x in range(40):
        cmd = f'docker-compose -f {file_name} ps -a'
        res = run_cmd(cmd)
        if res is None:
            exit(f'\033[0;31m Incorrect/Unsupported executing command "{cmd}"')
        time.sleep(2)
        cnt = res.count('(healthy)')
        if cnt == ct:
            logging.debug('\033[0;32m All components are healthy, please see below for more details\033[0m....')
            print(res)
            break
    if cnt != ct:
        logging.error('\033[0;31m Core network is un-healthy, please see below for more details\033[0m....')
        print(res)
        exit(-1)
    check_config(file_name)

def undeploy(file_name):
    """UnDeploy the docker container

    Returns:
        None
    """
    logging.debug('\033[0;34m UnDeploying OAI 5G core components\033[0m....')
    cmd = f'docker-compose -f {file_name} down'
    res = run_cmd(cmd)
    if res is None:
        exit(f'\033[0;31m Incorrect/Unsupported executing command {cmd}')
    print(res)
    logging.debug('\033[0;32m OAI 5G core components are UnDeployed\033[0m....')


def check_config(file_name):
    """Checks the container configurations

    Returns:
        None
    """

    logging.debug('\033[0;34m Checking if the containers are configured\033[0m....')
    # With NRF configuration check
    if args.scenario == '1':
        logging.debug('\033[0;34m Checking if AMF, SMF and UPF registered with nrf core network\033[0m....')
        cmd = 'curl -s -X GET http://192.168.70.130/nnrf-nfm/v1/nf-instances?nf-type="AMF" | grep -o "192.168.70.132"'
        amf_registration_nrf = run_cmd(cmd)
        cmd = 'curl -s -X GET http://192.168.70.130/nnrf-nfm/v1/nf-instances?nf-type="SMF" | grep -o "192.168.70.133"'
        smf_registration_nrf = run_cmd(cmd)
        if file_name == BASIC_VPP_W_NRF:
            cmd = 'curl -s -X GET http://192.168.70.130/nnrf-nfm/v1/nf-instances?nf-type="UPF" | grep -o "192.168.70.202"'
        else:
            cmd = 'curl -s -X GET http://192.168.70.130/nnrf-nfm/v1/nf-instances?nf-type="UPF" | grep -o "192.168.70.134"'
        upf_registration_nrf = run_cmd(cmd)
        cmd = 'curl -s -X GET http://192.168.70.130/nnrf-nfm/v1/nf-instances?nf-type="SMF"'
        sample_registration = run_cmd(cmd)
        logging.debug(f'\033[0;34m For example: oai-smf Registration with oai-nrf can be checked on this url /nnrf-nfm/v1/nf-instances?nf-type="SMF" {sample_registration}\033[0m....')
        if amf_registration_nrf is None or smf_registration_nrf is None or upf_registration_nrf is None:
             logging.error('\033[0;31m Registration problem with NRF, check the reason manually\033[0m....')
        else:
            logging.debug('\033[0;32m AMF, SMF and UPF are registered to NRF\033[0m....')
        if file_name == BASIC_VPP_W_NRF:
            logging.debug('\033[0;34m Checking if SMF is able to connect with UPF\033[0m....')
            cmd1 = 'docker logs oai-smf | grep "Received N4 ASSOCIATION SETUP RESPONSE from an UPF"'
            cmd2 = 'docker logs oai-smf | grep "Node ID Type FQDN: gw1"'
            upf_logs1 = run_cmd(cmd1)
            upf_logs2 = run_cmd(cmd2)
            if upf_logs1 is None or upf_logs2 is None:
                logging.error('\033[0;31m UPF did not answer to N4 Association request from SMF\033[0m....')
                exit(-1)
            else:
                logging.debug('\033[0;32m UPF did answer to N4 Association request from SMF\033[0m....')
            cmd1 = 'docker logs oai-smf | grep "PFCP HEARTBEAT PROCEDURE"'
            upf_logs1 = run_cmd(cmd1)
            if upf_logs1 is None:
                logging.error('\033[0;31m SMF not receiving heartbeats from UPF\033[0m....')
                exit(-1)
            else:
                logging.debug('\033[0;32m SMF receiving heathbeats from UPF\033[0m....')
        else:
            logging.debug('\033[0;34m Checking if SMF is able to connect with UPF\033[0m....')
            cmd1 = 'docker logs oai-spgwu | grep "Received SX HEARTBEAT RESPONSE"'
            cmd2 = 'docker logs oai-spgwu | grep "Received SX HEARTBEAT REQUEST"'
            upf_logs1 = run_cmd(cmd1)
            upf_logs2 = run_cmd(cmd2)
            if upf_logs1 is None or upf_logs2 is None:
                logging.error('\033[0;31m UPF not receiving heartbeats from SMF\033[0m....')
                exit(-1)
            else:
                logging.debug('\033[0;32m UPF receiving heathbeats from SMF\033[0m....')
    # With noNRF configuration checks
    elif args.scenario == '2':
        logging.debug('\033[0;34m Checking if SMF is able to connect with UPF\033[0m....')
        if file_name == BASIC_VPP_NO_NRF:
            cmd1 = 'docker logs oai-smf | grep "Received N4 ASSOCIATION SETUP RESPONSE from an UPF"'
            cmd2 = 'docker logs oai-smf | grep "Node ID Type FQDN: gw1"'
            upf_logs1 = run_cmd(cmd1)
            upf_logs2 = run_cmd(cmd2)
            if upf_logs1 is None or upf_logs2 is None:
                logging.error('\033[0;31m UPF did not answer to N4 Association request from SMF\033[0m....')
                exit(-1)
            else:
                logging.debug('\033[0;32m UPF did answer to N4 Association request from SMF\033[0m....')
        status = 0
        for x in range(4):
            cmd = "docker logs oai-smf | grep  'handle_receive(16 bytes)'"
            res = run_cmd(cmd)
            if res is None:
               logging.error('\033[0;31m UPF not receiving heartbeats from SMF, re-trying\033[0m....')
            else:
                status += 1
        if status > 2:
            logging.debug('\033[0;32m UPF receiving heathbeats from SMF\033[0m....')
    logging.debug('\033[0;32m OAI 5G Core network is configured and healthy\033[0m....')

def run_cmd(cmd):
    result = None
    try:
        res = subprocess.run(cmd,
                        shell=True, check=True,
                        stdout=subprocess.PIPE, 
                        universal_newlines=True)
        result = res.stdout.strip()
    except:
        pass
    return result

if __name__ == '__main__':

    # Parse the arguments to get the deployment instruction
    args = _parse_args()
    if args.type == 'start-mini':
        # Mini function with NRF
        if args.scenario == '1':
            deploy(MINI_W_NRF, 5)
        # Mini function without NRF
        elif args.scenario == '2':
            deploy(MINI_NO_NRF, 4)
    elif args.type == 'start-basic':
        # Basic function with NRF
        if args.scenario == '1':
            deploy(BASIC_W_NRF, 8)
        # Basic function without NRF
        elif args.scenario == '2':
            deploy(BASIC_NO_NRF, 7)
    elif args.type == 'start-basic-vpp':
        if args.fqdn == 'yes':
            logging.error('Configuration not supported yet')
            exit(-1)
        # Basic function with NRF and VPP-UPF
        if args.scenario == '1':
            deploy(BASIC_VPP_W_NRF, 8)
        # Basic function without NRF but with VPP-UPF
        elif args.scenario == '2':
            deploy(BASIC_VPP_NO_NRF, 7)
    elif args.type == 'stop-mini':
        if args.scenario == '1':
            undeploy(MINI_W_NRF)
        elif args.scenario == '2':
            undeploy(MINI_NO_NRF)
    elif args.type == 'stop-basic':
        if args.scenario == '1':
            undeploy(BASIC_W_NRF)
        elif args.scenario == '2':
            undeploy(BASIC_NO_NRF)
    elif args.type == 'stop-basic-vpp':
        if args.scenario == '1':
            undeploy(BASIC_VPP_W_NRF)
        elif args.scenario == '2':
            undeploy(BASIC_VPP_NO_NRF)
