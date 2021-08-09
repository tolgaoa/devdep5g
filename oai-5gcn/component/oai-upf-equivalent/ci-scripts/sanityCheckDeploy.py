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

CI_NETWORK_S11='192.168.28.0/24'
CI_NETWORK_SXN='192.168.29.0/24'
CI_NETWORK_S1U='192.168.30.0/24'

CI_SPGWC_S11_ADDR='192.168.28.2'
CI_SPGWC_SXN_ADDR='192.168.29.2'

CI_SPGWU_S1U_ADDR='192.168.30.3'
CI_SPGWU_SXN_ADDR='192.168.29.3'

class deploySanityCheckTest():
    def __init__(self):
        self.action = 'None'
        self.tag = ''

    def createNetworks(self):
        # first check if already up?
        try:
            res = subprocess.check_output('docker network ls | egrep -c "ci-s11|ci-s1x|ci-s1u"', shell=True, universal_newlines=True)
            if int(str(res.strip())) > 0:
                self.removeNetworks()
        except:
            pass

        subprocess_run_w_echo('docker network create --attachable --subnet ' + CI_NETWORK_S11 + ' --ip-range ' + CI_NETWORK_S11 + ' ci-s11')
        subprocess_run_w_echo('docker network create --attachable --subnet ' + CI_NETWORK_SXN + ' --ip-range ' + CI_NETWORK_SXN + ' ci-sx')
        subprocess_run_w_echo('docker network create --attachable --subnet ' + CI_NETWORK_S1U + ' --ip-range ' + CI_NETWORK_S1U + ' ci-s1u')

    def removeNetworks(self):
        try:
            subprocess_run_w_echo('docker network rm ci-sx ci-s11 ci-s1u')
        except:
            pass

    def deploySPGWC(self):
        res = ''
        # first check if tag exists
        try:
            res = subprocess.check_output('docker image inspect oai-spgwc:' + self.tag, shell=True, universal_newlines=True)
        except:
            sys.exit(-1)

        # check if there is an entrypoint
        entrypoint = re.search('entrypoint', str(res))
        if entrypoint is not None:
            subprocess_run_w_echo('wget --quiet https://raw.githubusercontent.com/OPENAIRINTERFACE/openair-spgwc/develop/ci-scripts/generateConfigFiles.py -O ci-scripts/generateSpgwcConfigFiles.py')
            subprocess_run_w_echo('python3 ci-scripts/generateSpgwcConfigFiles.py --kind=SPGW-C --s11c=eth0 --sxc=eth1 --from_docker_file --env_for_entrypoint')
            # S11 to MME will be on `eth0`
            # SX to SPGWU will be on `eth1`
            # At least we hope
            subprocess_run_w_echo('docker create --privileged --name ci-oai-spgwc --network ci-s11 --ip ' + CI_SPGWC_S11_ADDR + ' --env-file ./spgwc-env.list oai-spgwc:' + self.tag)
            subprocess_run_w_echo('docker network connect --ip ' + CI_SPGWC_SXN_ADDR + ' ci-sx ci-oai-spgwc')
            subprocess_run_w_echo('docker start ci-oai-spgwc')
            # Recovering the config part of the entrypoint execution
            tmpAwkFile=open('tmp.awk', 'w')
            tmpAwkFile.write('BEGIN{ok=1}{if ($0 ~/Options parsed/){ok=0};if(ok==1){print $0}}END{}\n')
            tmpAwkFile.close()
            time.sleep(3)
            subprocess.run('docker logs ci-oai-spgwc 2>&1 | awk -f tmp.awk > archives/spgwc_config.log', shell=True)
            subprocess.run('rm -f tmp.awk', shell=True)

        else:
            # S11 to MME will be on `eth0`
            subprocess_run_w_echo('docker run --privileged --name ci-oai-spgwc --network ci-s11 --ip ' + CI_SPGWC_S11_ADDR + ' -d oai-spgwc:' + self.tag + ' /bin/bash -c "sleep infinity"')
            # SX to SPGWU will be on `eth1`
            subprocess_run_w_echo('docker network connect --ip ' + CI_SPGWC_SXN_ADDR + ' ci-sx ci-oai-spgwc')
            subprocess_run_w_echo('wget --quiet https://raw.githubusercontent.com/OPENAIRINTERFACE/openair-spgwc/develop/ci-scripts/generateConfigFiles.py -O ci-scripts/generateSpgwcConfigFiles.py')
            subprocess_run_w_echo('python3 ci-scripts/generateSpgwcConfigFiles.py --kind=SPGW-C --s11c=eth0 --sxc=eth1 --from_docker_file')
            subprocess_run_w_echo('docker cp ./spgwc-cfg.sh ci-oai-spgwc:/openair-spgwc')
            subprocess_run_w_echo('docker exec -it ci-oai-spgwc /bin/bash -c "cd /openair-spgwc && chmod 777 spgwc-cfg.sh && ./spgwc-cfg.sh" >> archives/spgwc_config.log')
            # If we deploy a SPGW-U w/ entrypoint, SPGW-C SHALL be started
            # This will be removed later on.
            subprocess_run_w_echo('docker exec -d ci-oai-spgwc /bin/bash -c "nohup ./bin/oai_spgwc -o -c ./etc/spgw_c.conf > spgwc_check_run.log 2>&1"')

    def deploySPGWU(self):
        res = ''
        # first check if tag exists
        try:
            res = subprocess.check_output('docker image inspect oai-spgwu-tiny:' + self.tag, shell=True, universal_newlines=True)
        except:
            sys.exit(-1)

        # check if there is an entrypoint
        entrypoint = re.search('entrypoint', str(res))
        if entrypoint is not None:
            subprocess_run_w_echo('python3 ci-scripts/generateConfigFiles.py --kind=SPGW-U --sxc_ip_addr=' + CI_SPGWC_SXN_ADDR + ' --sxu=eth1 --s1u=eth0 --from_docker_file --env_for_entrypoint')
            # S1U to eNB will be on `eth0`
            # SX to SPGWC will be on `eth1`
            subprocess_run_w_echo('docker create --privileged --name ci-oai-spgwu --network ci-s1u --ip ' + CI_SPGWU_S1U_ADDR + ' --env-file ./spgwu-env.list oai-spgwu-tiny:' + self.tag)
            subprocess_run_w_echo('docker network connect --ip ' + CI_SPGWU_SXN_ADDR + ' ci-sx ci-oai-spgwu')
            subprocess_run_w_echo('docker start ci-oai-spgwu')
            # Recovering the config part of the entrypoint execution
            tmpAwkFile=open('tmp.awk', 'w')
            tmpAwkFile.write('BEGIN{ok=1}{if ($0 ~/Options parsed/){ok=0};if(ok==1){print $0}}END{}\n')
            tmpAwkFile.close()
            time.sleep(3)
            subprocess.run('docker logs ci-oai-spgwu 2>&1 | awk -f tmp.awk > archives/spgwu_config.log', shell=True)
            subprocess.run('rm -f tmp.awk', shell=True)

        else:
            # S1U to eNB will be on `eth0`
            subprocess_run_w_echo('docker run --privileged --name ci-oai-spgwu --network ci-s1u --ip ' + CI_SPGWU_S1U_ADDR + ' -d oai-spgwu-tiny:' + self.tag + ' /bin/bash -c "sleep infinity"')
            # SX to SPGWC will be on `eth1`
            subprocess_run_w_echo('docker network connect --ip ' + CI_SPGWU_SXN_ADDR + ' ci-sx ci-oai-spgwu')
            subprocess_run_w_echo('python3 ci-scripts/generateConfigFiles.py --kind=SPGW-U --sxc_ip_addr=' + CI_SPGWC_SXN_ADDR + ' --sxu=eth1 --s1u=eth0 --from_docker_file')
            subprocess_run_w_echo('docker cp ./spgwu-cfg.sh ci-oai-spgwu:/openair-spgwu-tiny')
            subprocess_run_w_echo('docker exec -it ci-oai-spgwu /bin/bash -c "cd /openair-spgwu-tiny && chmod 777 spgwu-cfg.sh && ./spgwu-cfg.sh" >> archives/spgwu_config.log')

    def startSPGWC(self):
        res = ''
        # first check if tag exists
        try:
            res = subprocess.check_output('docker image inspect oai-spgwc:' + self.tag, shell=True, universal_newlines=True)
        except:
            sys.exit(-1)

        # check if there is an entrypoint
        entrypoint = re.search('entrypoint', str(res))
        if entrypoint is not None:
            print('there is an entrypoint -- no need')
        else:
            print('Already started')
            #subprocess_run_w_echo('docker exec -d ci-oai-spgwc /bin/bash -c "nohup ./bin/oai_spgwc -o -c ./etc/spgw_c.conf > spgwc_check_run.log 2>&1"')

    def startSPGWU(self):
        res = ''
        # first check if tag exists
        try:
            res = subprocess.check_output('docker image inspect oai-spgwu-tiny:' + self.tag, shell=True, universal_newlines=True)
        except:
            sys.exit(-1)

        # check if there is an entrypoint
        entrypoint = re.search('entrypoint', str(res))
        if entrypoint is not None:
            print('there is an entrypoint -- no need')
        else:
            subprocess_run_w_echo('docker exec -d ci-oai-spgwu /bin/bash -c "nohup ./bin/oai_spgwu -o -c ./etc/spgw_u.conf > spgwu_check_run.log 2>&1"')

    def stopSPGWC(self):
        res = ''
        # first check if tag exists
        try:
            res = subprocess.check_output('docker image inspect oai-spgwc:' + self.tag, shell=True, universal_newlines=True)
        except:
            sys.exit(-1)

        # check if there is an entrypoint
        entrypoint = re.search('entrypoint', str(res))
        if entrypoint is not None:
            print('there is an entrypoint -- no need')
        else:
            subprocess_run_w_echo('docker exec -it ci-oai-spgwc /bin/bash -c "killall oai_spgwc"')

    def stopSPGWU(self):
        res = ''
        # first check if tag exists
        try:
            res = subprocess.check_output('docker image inspect oai-spgwu-tiny:' + self.tag, shell=True, universal_newlines=True)
        except:
            sys.exit(-1)

        # check if there is an entrypoint
        entrypoint = re.search('entrypoint', str(res))
        if entrypoint is not None:
            print('there is an entrypoint')
        else:
            subprocess_run_w_echo('docker exec -it ci-oai-spgwu /bin/bash -c "killall oai_spgwu"')

    def logsSPGWC(self):
        res = ''
        # first check if tag exists
        try:
            res = subprocess.check_output('docker image inspect oai-spgwc:' + self.tag, shell=True, universal_newlines=True)
        except:
            sys.exit(-1)

        # check if there is an entrypoint
        entrypoint = re.search('entrypoint', str(res))
        if entrypoint is not None:
            # Recovering the run part of the entrypoint execution
            tmpAwkFile=open('tmp.awk', 'w')
            tmpAwkFile.write('BEGIN{ok=0}{if ($0 ~/Options parsed/){ok=1};if(ok==1){print $0}}END{}\n')
            tmpAwkFile.close()
            time.sleep(1)
            subprocess.run('docker logs ci-oai-spgwc 2>&1 | awk -f tmp.awk > archives/spgwc_check_run.log', shell=True)
            subprocess.run('rm -f tmp.awk', shell=True)
        else:
            subprocess_run_w_echo('docker cp ci-oai-spgwc:/openair-spgwc/spgwc_check_run.log archives')

    def logsSPGWU(self):
        res = ''
        # first check if tag exists
        try:
            res = subprocess.check_output('docker image inspect oai-spgwu-tiny:' + self.tag, shell=True, universal_newlines=True)
        except:
            sys.exit(-1)

        # check if there is an entrypoint
        entrypoint = re.search('entrypoint', str(res))
        if entrypoint is not None:
            # Recovering the run part of the entrypoint execution
            tmpAwkFile=open('tmp.awk', 'w')
            tmpAwkFile.write('BEGIN{ok=0}{if ($0 ~/Options parsed/){ok=1};if(ok==1){print $0}}END{}\n')
            tmpAwkFile.close()
            time.sleep(1)
            subprocess.run('docker logs ci-oai-spgwu 2>&1 | awk -f tmp.awk > archives/spgwu_check_run.log', shell=True)
            subprocess.run('rm -f tmp.awk', shell=True)
        else:
            subprocess_run_w_echo('docker cp ci-oai-spgwu:/openair-spgwu-tiny/spgwu_check_run.log archives')

    def removeAllContainers(self):
        try:
            subprocess_run_w_echo('docker rm -f ci-oai-spgwc ci-oai-spgwu')
        except:
            pass

def subprocess_run_w_echo(cmd):
    print(cmd)
    subprocess.run(cmd, shell=True)

def Usage():
    print('----------------------------------------------------------------------------------------------------------------------')
    print('sanityCheckDeploy.py')
    print('   Deploy a Sanity Check Test in the pipeline.')
    print('----------------------------------------------------------------------------------------------------------------------')
    print('Usage: python3 sanityCheckDeploy.py [options]')
    print('  --help  Show this help.')
    print('---------------------------------------------------------------------------------------------- Mandatory Options -----')
    print('  --action={CreateNetworks,RemoveNetworks,...}')
    print('-------------------------------------------------------------------------------------------------------- Options -----')
    print('  --tag=[Image Tag in registry]')
    print('------------------------------------------------------------------------------------------------- Actions Syntax -----')
    print('python3 sanityCheckDeploy.py --action=CreateNetworks')
    print('python3 sanityCheckDeploy.py --action=RemoveNetworks')
    print('python3 sanityCheckDeploy.py --action=DeploySPGWC --tag=[tag]')
    print('python3 sanityCheckDeploy.py --action=DeploySPGWU --tag=[tag]')
    print('python3 sanityCheckDeploy.py --action=StartSPGWC --tag=[tag]')
    print('python3 sanityCheckDeploy.py --action=StartSPGWU --tag=[tag]')
    print('python3 sanityCheckDeploy.py --action=StopSPGWC --tag=[tag]')
    print('python3 sanityCheckDeploy.py --action=StopSPGWU --tag=[tag]')
    print('python3 sanityCheckDeploy.py --action=RetrieveLogsSPGWC --tag=[tag]')
    print('python3 sanityCheckDeploy.py --action=RetrieveLogsSPGWU --tag=[tag]')
    print('python3 sanityCheckDeploy.py --action=RemoveAllContainers')

#--------------------------------------------------------------------------------------------------------
#
# Start of main
#
#--------------------------------------------------------------------------------------------------------

DSCT = deploySanityCheckTest()

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
           action != 'DeploySPGWC' and \
           action != 'DeploySPGWU' and \
           action != 'StartSPGWC' and \
           action != 'StartSPGWU' and \
           action != 'StopSPGWC' and \
           action != 'StopSPGWU' and \
           action != 'RetrieveLogsSPGWC' and \
           action != 'RetrieveLogsSPGWU' and \
           action != 'RemoveAllContainers':
            print('Unsupported Action => ' + action)
            Usage()
            sys.exit(-1)
        DSCT.action = action
    elif re.match('^\-\-tag=(.+)$', myArgv, re.IGNORECASE):
        matchReg = re.match('^\-\-tag=(.+)$', myArgv, re.IGNORECASE)
        DSCT.tag = matchReg.group(1)

if DSCT.action == 'CreateNetworks':
    DSCT.createNetworks()
elif DSCT.action == 'RemoveNetworks':
    DSCT.removeNetworks()
elif DSCT.action == 'DeploySPGWC':
    if DSCT.tag == '':
        print('Missing OAI-SPGWC image tag')
        Usage()
        sys.exit(-1)
    DSCT.deploySPGWC()
elif DSCT.action == 'DeploySPGWU':
    if DSCT.tag == '':
        print('Missing OAI-SPGWU image tag')
        Usage()
        sys.exit(-1)
    DSCT.deploySPGWU()
elif DSCT.action == 'StartSPGWC':
    if DSCT.tag == '':
        print('Missing OAI-SPGWC image tag')
        Usage()
        sys.exit(-1)
    DSCT.startSPGWC()
elif DSCT.action == 'StartSPGWU':
    if DSCT.tag == '':
        print('Missing OAI-SPGWU image tag')
        Usage()
        sys.exit(-1)
    DSCT.startSPGWU()
elif DSCT.action == 'StopSPGWC':
    if DSCT.tag == '':
        print('Missing OAI-SPGWC image tag')
        Usage()
        sys.exit(-1)
    DSCT.stopSPGWC()
elif DSCT.action == 'StopSPGWU':
    if DSCT.tag == '':
        print('Missing OAI-SPGWU image tag')
        Usage()
        sys.exit(-1)
    DSCT.stopSPGWU()
elif DSCT.action == 'RetrieveLogsSPGWC':
    if DSCT.tag == '':
        print('Missing OAI-SPGWC image tag')
        Usage()
        sys.exit(-1)
    DSCT.logsSPGWC()
elif DSCT.action == 'RetrieveLogsSPGWU':
    if DSCT.tag == '':
        print('Missing OAI-SPGWU image tag')
        Usage()
        sys.exit(-1)
    DSCT.logsSPGWU()
elif DSCT.action == 'RemoveAllContainers':
    DSCT.removeAllContainers()

sys.exit(0)

