#/*
# * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
# * contributor license agreements.  See the NOTICE file distributed with
# * this work for additional information regarding copyright ownership.
# * The OpenAirInterface Software Alliance licenses this file to You under
# * the OAI Public License, Version 1.1  (the "License"); you may not use this file
# * except in compliance with the License.
# * You may obtain a copy of the License at
# *
# *       http://www.openairinterface.org/?page_id=698
# *
# * Unless required by applicable law or agreed to in writing, software
# * distributed under the License is distributed on an "AS IS" BASIS,
# * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# * See the License for the specific language governing permissions and
# * limitations under the License.
# *-------------------------------------------------------------------------------
# * For more information about the OpenAirInterface (OAI) Software Alliance:
# *       contact@openairinterface.org
# */
#---------------------------------------------------------------------

import os
import re
import sys
import subprocess
import time

CICD_PRIVATE_NETWORK_RANGE='192.168.68.0/26'
CICD_PUBLIC_NETWORK_RANGE='192.168.61.192/26'

CICD_MYSQL_PUBLIC_ADDR='192.168.61.194'
CICD_AMF_PUBLIC_ADDR='192.168.61.195'
CICD_SMF_PUBLIC_ADDR='192.168.61.196'
CICD_UPF_PUBLIC_ADDR='192.168.61.197'
CICD_DUMMY_SMF_PUBLIC_ADDR='192.168.61.200'

class deployForDsTester():
    def __init__(self):
        self.action = 'None'
        self.tag = ''
        self.mySqlPassword = ''

    def createNetworks(self):
        # first check if already up?
        try:
            res = subprocess.check_output('docker network ls | egrep -c "cicd-oai-public-net|cicd-oai-private-net"', shell=True, universal_newlines=True)
            if int(str(res.strip())) > 0:
                self.removeNetworks()
        except:
            pass

        subprocess_run_w_echo('docker network create --attachable --subnet ' + CICD_PUBLIC_NETWORK_RANGE + ' --ip-range ' + CICD_PUBLIC_NETWORK_RANGE + ' cicd-oai-public-net')
        subprocess_run_w_echo('docker network create --attachable --subnet ' + CICD_PRIVATE_NETWORK_RANGE + ' --ip-range ' + CICD_PRIVATE_NETWORK_RANGE + ' cicd-oai-private-net')

    def removeNetworks(self):
        try:
            subprocess_run_w_echo('docker network rm cicd-oai-public-net cicd-oai-private-net')
        except:
            pass

    def deployMySqlServer(self):
        # first check if already up? If yes, remove everything.
        try:
            res = subprocess.check_output('docker ps -a | grep -c "cicd-mysql-svr"', shell=True, universal_newlines=True)
            if int(str(res.strip())) > 0:
                self.removeAllContainers()
        except:
            pass

        cwd = os.getcwd()
        if not os.path.isfile(cwd + '/component/oai-amf/build/scripts/oai_db.sql'):
            sys.exit(-1)

        subprocess_run_w_echo('docker run --name cicd-mysql-svr --network cicd-oai-public-net --ip ' + CICD_MYSQL_PUBLIC_ADDR + ' -d -e MYSQL_ROOT_PASSWORD=secretPassword mysql/mysql-server:5.7')
        subprocess_run_w_echo('docker cp component/oai-amf/build/scripts/oai_db.sql cicd-mysql-svr:/home')
        subprocess_run_w_echo('sed -e "s@CICD_AMF_PUBLIC_ADDR@' + CICD_AMF_PUBLIC_ADDR + '@" ci-scripts/mysql-script.cmd > ci-scripts/mysql-complete.cmd')
        subprocess_run_w_echo('docker cp ci-scripts/mysql-complete.cmd cicd-mysql-svr:/home')
        # waiting for the service to be properly started
        time.sleep(5)
        doLoop = True
        while doLoop:
            try:
                res = subprocess.check_output('docker logs cicd-mysql-svr 2>&1', shell=True, universal_newlines=True)
                startMessageFound = re.search('Starting MySQL', str(res))
                if startMessageFound is not None:
                    doLoop = False
            except:
                time.sleep(2)
                pass
        time.sleep(2)
        subprocess_run_w_echo('docker exec cicd-mysql-svr /bin/bash -c "mysql -uroot -psecretPassword < /home/mysql-complete.cmd"')

    def deployAMF(self):
        res = ''
        # first check if tag exists
        try:
            res = subprocess.check_output('docker image inspect oai-amf:' + self.tag, shell=True, universal_newlines=True)
        except:
            sys.exit(-1)

        # check if there is an entrypoint
        entrypoint = re.search('entrypoint', str(res))
        if entrypoint is not None:
            subprocess_run_w_echo('docker run --privileged --name cicd-oai-amf --network cicd-oai-public-net --ip ' + CICD_AMF_PUBLIC_ADDR + ' -d --entrypoint "/bin/bash" oai-amf:' + self.tag + ' -c "sleep infinity"')
        else:
            subprocess_run_w_echo('docker run --privileged --name cicd-oai-amf --network cicd-oai-public-net --ip ' + CICD_AMF_PUBLIC_ADDR + ' -d oai-amf:' + self.tag + ' /bin/bash -c "sleep infinity"')
        subprocess_run_w_echo('sed -e "s@CI_NGAP_IF_NAME@eth0@" -e "s@CI_N11_IF_NAME@eth0@" -e "s@CI_SMF0_IP_ADDRESS@' + CICD_SMF_PUBLIC_ADDR + '@" -e "s@CI_SMF1_IP_ADDRESS@' + CICD_DUMMY_SMF_PUBLIC_ADDR + '@" -e "s@CI_MYSQL_IP_ADDRESS@' + CICD_MYSQL_PUBLIC_ADDR + '@" ci-scripts/temp/generate_amf_conf.sh > ci-scripts/temp/ci-generate_amf_conf.sh')
        subprocess_run_w_echo('docker cp ci-scripts/temp/ci-generate_amf_conf.sh cicd-oai-amf:/openair-amf/generate_amf_conf.sh')
        subprocess_run_w_echo('docker exec cicd-oai-amf /bin/bash -c "chmod 755 generate_amf_conf.sh && ./generate_amf_conf.sh" > archives/amf_config.log')

    def deploySMF(self):
        res = ''
        # first check if tag exists
        try:
            res = subprocess.check_output('docker image inspect oai-smf:' + self.tag, shell=True, universal_newlines=True)
        except:
            sys.exit(-1)

        # check if there is an entrypoint
        entrypoint = re.search('entrypoint', str(res))
        if entrypoint is not None:
            subprocess_run_w_echo('docker run --privileged --name cicd-oai-smf --network cicd-oai-public-net --ip ' + CICD_SMF_PUBLIC_ADDR + ' -d --entrypoint "/bin/bash" oai-smf:' + self.tag + ' -c "sleep infinity"')
        else:
            subprocess_run_w_echo('docker run --privileged --name cicd-oai-smf --network cicd-oai-public-net --ip ' + CICD_SMF_PUBLIC_ADDR + ' -d oai-smf:' + self.tag + ' /bin/bash -c "sleep infinity"')
        subprocess_run_w_echo('sed -e "s@CI_N4_IF_NAME@eth0@" -e "s@CI_SBI_IF_NAME@eth0@" -e "s@CI_AMF_IP_ADDR@' + CICD_AMF_PUBLIC_ADDR + '@" -e "s@CI_UPF_IP_ADDR@' + CICD_UPF_PUBLIC_ADDR + '@" ci-scripts/temp/generate_smf_conf.sh > ci-scripts/temp/ci-generate_smf_conf.sh')
        subprocess_run_w_echo('docker cp ci-scripts/temp/ci-generate_smf_conf.sh cicd-oai-smf:/openair-smf/generate_smf_conf.sh')
        subprocess_run_w_echo('docker exec cicd-oai-smf /bin/bash -c "chmod 755 generate_smf_conf.sh && ./generate_smf_conf.sh" > archives/smf_config.log')

    def deployUPF(self):
        res = ''
        # first check if tag exists
        try:
            res = subprocess.check_output('docker image inspect oai-spgwu-tiny:' + self.tag, shell=True, universal_newlines=True)
        except:
            sys.exit(-1)

        # check if there is an entrypoint
        entrypoint = re.search('entrypoint', str(res))
        if entrypoint is not None:
            subprocess_run_w_echo('docker run --privileged --name cicd-oai-upf --network cicd-oai-public-net --ip ' + CICD_UPF_PUBLIC_ADDR + ' -d --entrypoint "/bin/bash" oai-spgwu-tiny:' + self.tag + ' -c "sleep infinity"')
        else:
            subprocess_run_w_echo('docker run --privileged --name cicd-oai-upf --network cicd-oai-public-net --ip ' + CICD_UPF_PUBLIC_ADDR + ' -d oai-spgwu-tiny:' + self.tag + ' /bin/bash -c "sleep infinity"')
        subprocess_run_w_echo('python3 ci-scripts/generate_spgwu-tiny_config_script.py --kind=SPGW-U --sxc_ip_addr=' + CICD_SMF_PUBLIC_ADDR + ' --sxu=eth0 --s1u=eth0 --sgi=eth0 --pdn_list="12.0.0.0/24 12.1.0.0/24" --prefix=/openair-spgwu-tiny/etc --from_docker_file')
        subprocess_run_w_echo('docker cp ./spgw_u.conf cicd-oai-upf:/openair-spgwu-tiny/etc')
        subprocess_run_w_echo('touch archives/spgwu_config.log')

    def removeAllContainers(self):
        try:
            subprocess_run_w_echo('docker rm -f cicd-mysql-svr cicd-oai-amf cicd-oai-smf cicd-oai-upf')
        except:
            pass


def subprocess_run_w_echo(cmd):
    print(cmd)
    subprocess.run(cmd, shell=True)

def Usage():
    print('----------------------------------------------------------------------------------------------------------------------')
    print('dsTestDeployTools.py')
    print('   Deploy for DsTester in the pipeline.')
    print('----------------------------------------------------------------------------------------------------------------------')
    print('Usage: python3 dsTestDeployTools.py [options]')
    print('  --help  Show this help.')
    print('---------------------------------------------------------------------------------------------- Mandatory Options -----')
    print('  --action={CreateNetworks,RemoveNetworks,...}')
    print('-------------------------------------------------------------------------------------------------------- Options -----')
    print('  --tag=[Image Tag in registry]')
    print('------------------------------------------------------------------------------------------------- Actions Syntax -----')
    print('python3 dsTestDeployTools.py --action=CreateNetworks')
    print('python3 dsTestDeployTools.py --action=RemoveNetworks')
    print('python3 dsTestDeployTools.py --action=DeployMySqlServer')
    print('python3 dsTestDeployTools.py --action=DeployAMF --tag=[tag]')
    print('python3 dsTestDeployTools.py --action=DeploySMF --tag=[tag]')
    print('python3 dsTestDeployTools.py --action=DeployUPF --tag=[tag]')
    print('python3 dsTestDeployTools.py --action=RemoveAllContainers')

#--------------------------------------------------------------------------------------------------------
#
# Start of main
#
#--------------------------------------------------------------------------------------------------------

DFDT = deployForDsTester()

argvs = sys.argv
argc = len(argvs)

while len(argvs) > 1:
    myArgv = argvs.pop(1)
    if re.match('^\-\-help$', myArgv, re.IGNORECASE):
        Usage()
        sys.exit(0)
    elif re.match('^\-\-action=(.+)$', myArgv, re.IGNORECASE):
        matchReg = re.match('^\-\-action=(.+)$', myArgv, re.IGNORECASE)
        action = matchReg.group(1)
        if action != 'CreateNetworks' and \
           action != 'RemoveNetworks' and \
           action != 'DeployMySqlServer' and \
           action != 'DeployAMF' and \
           action != 'DeploySMF' and \
           action != 'DeployUPF' and \
           action != 'RemoveAllContainers':
            print('Unsupported Action => ' + action)
            Usage()
            sys.exit(-1)
        DFDT.action = action
    elif re.match('^\-\-tag=(.+)$', myArgv, re.IGNORECASE):
        matchReg = re.match('^\-\-tag=(.+)$', myArgv, re.IGNORECASE)
        DFDT.tag = matchReg.group(1)

if DFDT.action == 'CreateNetworks':
    DFDT.createNetworks()
elif DFDT.action == 'RemoveNetworks':
    DFDT.removeNetworks()
elif DFDT.action == 'DeployMySqlServer':
    DFDT.deployMySqlServer()
elif DFDT.action == 'DeployAMF':
    if DFDT.tag == '':
        print('Missing OAI-AMF image tag')
        Usage()
        sys.exit(-1)
    DFDT.deployAMF()
elif DFDT.action == 'DeploySMF':
    if DFDT.tag == '':
        print('Missing OAI-SMF image tag')
        Usage()
        sys.exit(-1)
    DFDT.deploySMF()
elif DFDT.action == 'DeployUPF':
    if DFDT.tag == '':
        print('Missing OAI-UPF image tag')
        Usage()
        sys.exit(-1)
    DFDT.deployUPF()
elif DFDT.action == 'RemoveAllContainers':
    DFDT.removeAllContainers()

sys.exit(0)

