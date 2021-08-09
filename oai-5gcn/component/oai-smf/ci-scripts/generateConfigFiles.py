#/*
# * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
# * contributor license agreements.  See the NOTICE file distributed with
# * this work for additional information regarding copyright ownership.
# * The OpenAirInterface Software Alliance licenses this file to You under
# * the OAI Public License, Version 1.1  (the "License"); you may not use this file
# * except in compliance with the License.
# * You may obtain a copy of the License at
# *
# *      http://www.openairinterface.org/?page_id=698
# *
# * Unless required by applicable law or agreed to in writing, software
# * distributed under the License is distributed on an "AS IS" BASIS,
# * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# * See the License for the specific language governing permissions and
# * limitations under the License.
# *-------------------------------------------------------------------------------
# * For more information about the OpenAirInterface (OAI) Software Alliance:
# *      contact@openairinterface.org
# */
#---------------------------------------------------------------------

import os
import re
import sys

class smfConfigGen():
	def __init__(self):
		self.kind = ''
		self.sbi_name = ''
		self.sbi_port = '80'
		self.sbi_http2_port = '9090'
		self.sbi_api_version = 'v1'
		self.n4_name = ''
		self.amf_ip_addr = ''
		self.amf_port = '80'
		self.amf_api_version = 'v1'
		self.udm_ip_addr = ''
		self.udm_port = '80'
		self.udm_api_version = 'v2'
#		self.nrf_ip_addr = ''
#		self.nrf_port = '80'
#		self.nrf_api_version = 'v1'
		self.upf_ip_addr = ''
		self.apn = 'carrier.com'
		self.dns1_ip = '192.168.18.129'
		self.dns2_ip = '8.8.4.4'
		self.fromDockerFile = False

	def GenerateSmfConfigurer(self):
		smfFile = open('./smf-cfg.sh', 'w')
		smfFile.write('#!/bin/bash\n')
		smfFile.write('\n')
		if self.fromDockerFile:
			smfFile.write('cd /openair-smf\n')
		else:
			smfFile.write('cd /home\n')
		smfFile.write('\n')
		smfFile.write('INSTANCE=1\n')
		if self.fromDockerFile:
			smfFile.write('PREFIX=\'/openair-smf/etc\'\n')
		else:
			smfFile.write('PREFIX=\'/usr/local/etc/oai\'\n')
		smfFile.write('\n')
		smfFile.write('MY_APN=\'' + self.apn + '\'\n')
		smfFile.write('MY_PRIMARY_DNS=\'' + self.dns1_ip + '\'\n')
		smfFile.write('MY_SECONDARY_DNS=\'' + self.dns2_ip + '\'\n')
		smfFile.write('\n')
		if not self.fromDockerFile:
			smfFile.write('mkdir -p $PREFIX\n')
			smfFile.write('cp etc/spgw_c.conf  $PREFIX\n')
			smfFile.write('\n')
		smfFile.write('declare -A SMF_CONF\n')
		smfFile.write('\n')
		smfFile.write('SMF_CONF[@INSTANCE@]=$INSTANCE\n')
		smfFile.write('SMF_CONF[@PID_DIRECTORY@]=\'/var/run\'\n')
		smfFile.write('SMF_CONF[@SMF_INTERFACE_NAME_FOR_N4@]=\'' + self.n4_name + '\'\n')
		smfFile.write('SMF_CONF[@SMF_INTERFACE_NAME_FOR_SBI@]=\'' + self.sbi_name + '\'\n')
		smfFile.write('SMF_CONF[@SMF_INTERFACE_PORT_FOR_SBI@]=' + self.sbi_port + '\n')
		smfFile.write('SMF_CONF[@SMF_INTERFACE_HTTP2_PORT_FOR_SBI@]=' + self.sbi_http2_port + '\n')		
		smfFile.write('SMF_CONF[@SMF_API_VERSION@]=\'' + self.sbi_api_version + '\'\n')
		smfFile.write('SMF_CONF[@DEFAULT_DNS_IPV4_ADDRESS@]=$MY_PRIMARY_DNS\n')
		smfFile.write('SMF_CONF[@DEFAULT_DNS_SEC_IPV4_ADDRESS@]=$MY_SECONDARY_DNS\n')
		#smfFile.write('SMF_CONF[@DEFAULT_APN@]=$MY_APN\n')
		smfFile.write('SMF_CONF[@AMF_IPV4_ADDRESS@]=\'' + self.amf_ip_addr + '\'\n')
		smfFile.write('SMF_CONF[@AMF_PORT@]=' + self.amf_port + '\n')
		smfFile.write('SMF_CONF[@AMF_API_VERSION@]=\'' + self.amf_api_version + '\'\n')
		smfFile.write('SMF_CONF[@UDM_IPV4_ADDRESS@]=\'' + self.udm_ip_addr + '\'\n')
		smfFile.write('SMF_CONF[@UDM_PORT@]=' + self.udm_port + '\n')
		smfFile.write('SMF_CONF[@UDM_API_VERSION@]=\'' + self.udm_api_version + '\'\n')
#		smfFile.write('SMF_CONF[@NRF_IPV4_ADDRESS@]=\'' + self.nrf_ip_addr + '\'\n')
#		smfFile.write('SMF_CONF[@NRF_PORT@]=' + self.nrf_port + '\n')
#		smfFile.write('SMF_CONF[@NRF_API_VERSION@]=\'' + self.nrf_api_version + '\'\n')		
		smfFile.write('SMF_CONF[@UPF_IPV4_ADDRESS@]=\'' + self.upf_ip_addr + '\'\n')
		smfFile.write('\n')
		smfFile.write('for K in "${!SMF_CONF[@]}"; do \n')
		smfFile.write('  egrep -lRZ "$K" $PREFIX | xargs -0 -l sed -i -e "s|$K|${SMF_CONF[$K]}|g"\n')
		smfFile.write('done\n')
		smfFile.write('\n')
		smfFile.write('exit 0\n')
		smfFile.close()

#-----------------------------------------------------------
# Usage()
#-----------------------------------------------------------
def Usage():
	print('----------------------------------------------------------------------------------------------------------------------')
	print('generateConfigFiles.py')
	print('   Prepare a bash script to be run in the workspace where SMF is being built.')
	print('   That bash script will copy configuration template files and adapt to your configuration.')
	print('----------------------------------------------------------------------------------------------------------------------')
	print('Usage: python3 generateConfigFiles.py [options]')
	print('  --help  Show this help.')
	print('---------------------------------------------------------------------------------------------------- SMF Options -----')
	print('  --kind=SMF')
	print('  --sbi=[SMF SBI Interface Name]')
	print('  --n4=[SMF N4 Interface Name]')
	print('  --amf_ip_addr=[AMF IP Address]')
	print('  --udm_ip_addr=[UDM IP Address]')
#	print('  --nrf_ip_addr=[NRF IP Address]')
	print('  --upf_ip_addr=[UPF IP Address]')
	print('  --from_docker_file')
	print('---------------------------------------------------------------------------------------------- SMF Not Mandatory -----')
	print('  --apn=[Access Point Name]')
	print('  --dns1_ip=[First DNS IP address]')
	print('  --dns2_ip=[Second DNS IP address]')

argvs = sys.argv
argc = len(argvs)
cwd = os.getcwd()

mySmfCfg =  smfConfigGen()

while len(argvs) > 1:
	myArgv = argvs.pop(1)
	if re.match('^\-\-help$', myArgv, re.IGNORECASE):
		Usage()
		sys.exit(0)
	elif re.match('^\-\-kind=(.+)$', myArgv, re.IGNORECASE):
		matchReg = re.match('^\-\-kind=(.+)$', myArgv, re.IGNORECASE)
		mySmfCfg.kind = matchReg.group(1)
	elif re.match('^\-\-sbi=(.+)$', myArgv, re.IGNORECASE):
		matchReg = re.match('^\-\-sbi=(.+)$', myArgv, re.IGNORECASE)
		mySmfCfg.sbi_name = matchReg.group(1)
	elif re.match('^\-\-n4=(.+)$', myArgv, re.IGNORECASE):
		matchReg = re.match('^\-\-n4=(.+)$', myArgv, re.IGNORECASE)
		mySmfCfg.n4_name = matchReg.group(1)
	elif re.match('^\-\-amf_ip_addr=(.+)$', myArgv, re.IGNORECASE):
		matchReg = re.match('^\-\-amf_ip_addr=(.+)$', myArgv, re.IGNORECASE)
		mySmfCfg.amf_ip_addr = matchReg.group(1)
	elif re.match('^\-\-udm_ip_addr=(.+)$', myArgv, re.IGNORECASE):
		matchReg = re.match('^\-\-udm_ip_addr=(.+)$', myArgv, re.IGNORECASE)
		mySmfCfg.udm_ip_addr = matchReg.group(1)
#	elif re.match('^\-\-nrf_ip_addr=(.+)$', myArgv, re.IGNORECASE):
#		matchReg = re.match('^\-\-nrf_ip_addr=(.+)$', myArgv, re.IGNORECASE)
#		mySmfCfg.nrf_ip_addr = matchReg.group(1)
	elif re.match('^\-\-upf_ip_addr=(.+)$', myArgv, re.IGNORECASE):
		matchReg = re.match('^\-\-upf_ip_addr=(.+)$', myArgv, re.IGNORECASE)
		mySmfCfg.upf_ip_addr = matchReg.group(1)
	elif re.match('^\-\-apn=(.+)$', myArgv, re.IGNORECASE):
		matchReg = re.match('^\-\-apn=(.+)$', myArgv, re.IGNORECASE)
		mySmfCfg.apn = matchReg.group(1)
	elif re.match('^\-\-dns1_ip=(.+)$', myArgv, re.IGNORECASE):
		matchReg = re.match('^\-\-dns1_ip=(.+)$', myArgv, re.IGNORECASE)
		mySmfCfg.dns1_ip = matchReg.group(1)
	elif re.match('^\-\-dns2_ip=(.+)$', myArgv, re.IGNORECASE):
		matchReg = re.match('^\-\-dns2_ip=(.+)$', myArgv, re.IGNORECASE)
		mySmfCfg.dns2_ip = matchReg.group(1)
	elif re.match('^\-\-from_docker_file', myArgv, re.IGNORECASE):
		mySmfCfg.fromDockerFile = True
	else:
		Usage()
		sys.exit('Invalid Parameter: ' + myArgv)

if mySmfCfg.kind == '':
	Usage()
	sys.exit('missing kind parameter')

if mySmfCfg.kind == 'SMF':
	if mySmfCfg.sbi_name == '':
		Usage()
		sys.exit('missing S11 Interface Name on SMF container')
	elif mySmfCfg.n4_name == '':
		Usage()
		sys.exit('missing N4 Interface Name on SMF container')
	elif mySmfCfg.amf_ip_addr == '':
		Usage()
		sys.exit('missing AMF IP address')
	elif mySmfCfg.udm_ip_addr == '':
		Usage()
		sys.exit('missing UDM IP address')
#	elif mySmfCfg.nrf_ip_addr == '':
#		Usage()
#		sys.exit('missing NRF IP address')
	elif mySmfCfg.upf_ip_addr == '':
		Usage()
		sys.exit('missing UPF IP address')
	else:
		mySmfCfg.GenerateSmfConfigurer()
		sys.exit(0)
else:
	Usage()
	sys.exit('invalid kind parameter')
