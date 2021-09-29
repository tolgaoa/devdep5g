#/*
# * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
# * contributor license agreements.  See the NOTICE file distributed with
# * this work for additional information regarding copyright ownership.
# * The OpenAirInterface Software Alliance licenses this file to You under
# * the OAI Public License, Version 1.1  (the "License"); you may not use this file
# * except in compliance with the License.
# * You may obtain a copy of the License at
# *
# *	  http://www.openairinterface.org/?page_id=698
# *
# * Unless required by applicable law or agreed to in writing, software
# * distributed under the License is distributed on an "AS IS" BASIS,
# * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# * See the License for the specific language governing permissions and
# * limitations under the License.
# *-------------------------------------------------------------------------------
# * For more information about the OpenAirInterface (OAI) Software Alliance:
# *	  contact@openairinterface.org
# */
#---------------------------------------------------------------------

import os
import re
import sys
import subprocess

class verifySanityCheckDeployment():
	def __init__(self):
		self.job_name = ''

	def checkLogs(self):
		spgwc_status = self.analyze_check_run_log('SPGW-C')
		spgwu_status = self.analyze_check_run_log('SPGW-U')
		if not spgwc_status:
			print ('SPGW-C did not deploy properly')
		if not spgwu_status:
			print ('SPGW-U did not deploy properly')
		if not spgwc_status or not spgwu_status:
			sys.exit('Sanity Check Deployment went wrong')
		else:
			print ('Sanity Check Deployment is OK')

	def analyze_check_run_log(self, nfType):
		logFileName = nfType.lower().replace('-','') + '_check_run.log'

		cwd = os.getcwd()
		status = False
		if os.path.isfile(cwd + '/archives/' + logFileName):
			nb_pfcp_hb_proc = 0
			nb_sx_hb_resp = 0
			nb_sx_hb_req = 0
			with open(cwd + '/archives/' + logFileName, 'r') as logfile:
				for line in logfile:
					result = re.search('PFCP HEARTBEAT PROCEDURE', line)
					if result is not None:
						nb_pfcp_hb_proc += 1
					result = re.search('SX HEARTBEAT RESPONSE', line)
					if result is not None:
						nb_sx_hb_resp += 1
					result = re.search('SX HEARTBEAT REQUEST', line)
					if result is not None:
						nb_sx_hb_req += 1
				logfile.close()
			if nfType == 'SPGW-C':
				if nb_pfcp_hb_proc > 0:
					status = True
			if nfType == 'SPGW-U':
				if nb_pfcp_hb_proc > 0 and nb_sx_hb_resp > 0 and nb_sx_hb_req > 0:
					status = True

		return status

def Usage():
	print('----------------------------------------------------------------------------------------------------------------------')
	print('verifySanityCheckDeployment.py')
	print('   Verify the Sanity Check Deployment in the pipeline.')
	print('----------------------------------------------------------------------------------------------------------------------')
	print('Usage: python3 verifySanityCheckDeployment.py [options]')
	print('  --help  Show this help.')
	print('---------------------------------------------------------------------------------------------- Mandatory Options -----')
	print('  --job_name=[Jenkins Job name]')
	print('  --job_id=[Jenkins Job Build ID]')

#--------------------------------------------------------------------------------------------------------
#
# Start of main
#
#--------------------------------------------------------------------------------------------------------

argvs = sys.argv
argc = len(argvs)

vscd = verifySanityCheckDeployment()

while len(argvs) > 1:
	myArgv = argvs.pop(1)
	if re.match('^\-\-help$', myArgv, re.IGNORECASE):
		Usage()
		sys.exit(0)
	elif re.match('^\-\-job_name=(.+)$', myArgv, re.IGNORECASE):
		matchReg = re.match('^\-\-job_name=(.+)$', myArgv, re.IGNORECASE)
		vscd.job_name = matchReg.group(1)
	elif re.match('^\-\-job_id=(.+)$', myArgv, re.IGNORECASE):
		matchReg = re.match('^\-\-job_id=(.+)$', myArgv, re.IGNORECASE)
		vscd.job_id = matchReg.group(1)
	else:
		sys.exit('Invalid Parameter: ' + myArgv)

if vscd.job_name == '' or vscd.job_id == '':
	sys.exit('Missing Parameter in job description')

vscd.checkLogs()
