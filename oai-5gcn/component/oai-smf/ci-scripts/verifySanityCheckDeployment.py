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
		self.smf_pdu_check = False

	def checkLogs(self):
		if self.smf_pdu_check:
			smf_status = self.analyze_pdu_scenario_log('SMF')
			if not smf_status:
				sys.exit('Local Test went wrong')
			else:
				print ('Local Test is OK')
				return
		smf_status = self.analyze_check_run_log('SMF')
		spgwu_status = self.analyze_check_run_log('SPGW-U')
		if not smf_status:
			print ('SMF did not deploy properly')
		if not spgwu_status:
			print ('SPGW-U did not deploy properly')
		if not smf_status or not spgwu_status:
			sys.exit('Sanity Check Deployment went wrong')
		else:
			print ('Sanity Check Deployment is OK')

	def analyze_pdu_scenario_log(self, nfType):
		if nfType != 'SMF':
			return False
		logFileName = nfType.lower().replace('-','') + '_check_run.log'
		cwd = os.getcwd()
		status = False

		if os.path.isfile(cwd + '/archives/' + logFileName):
			myCmd = 'iconv -f ISO-8859-1 -t UTF-8//TRANSLIT ' + cwd + '/archives/' + logFileName + ' -o ' + cwd + '/archives/' + logFileName + '.conv'
			subprocess.run(myCmd, shell=True)
			myCmd = 'mv ' + cwd + '/archives/' + logFileName + '.conv '  + cwd + '/archives/' + logFileName
			subprocess.run(myCmd, shell=True)

			nb_sm_req_from_amf = 0
			nb_pdu_create_smf_req = 0
			nb_encode_pdu_establish_accept = 0
			nb_pdu_session_pending = 0
			nb_pdu_session_ue_establish_req = 0
			nb_pdu_status_active = 0

			with open(cwd + '/archives/' + logFileName, 'r') as logfile:
				for line in logfile:
					result = re.search('Received a SM context create request from AMF', line)
					if result is not None:
						nb_sm_req_from_amf += 1
					result = re.search('PDU Session Create SM Context Request', line)
					if result is not None:
						nb_pdu_create_smf_req += 1
					result = re.search('Encode PDU Session Establishment Accept', line)
					if result is not None:
						nb_encode_pdu_establish_accept += 1
					result = re.search('Set PDU Session Status to PDU_SESSION_ESTABLISHMENT_PENDING', line)
					if result is not None:
						nb_pdu_session_pending += 1
					result = re.search('PDU_SESSION_ESTABLISHMENT_UE_REQUESTED', line)
					if result is not None:
						nb_pdu_session_ue_establish_req += 1
					result = re.search('Set PDU Session Status to PDU_SESSION_ACTIVE', line)
					if result is not None:
						nb_pdu_status_active += 1
				logfile.close()

			if nb_sm_req_from_amf > 0 and nb_pdu_create_smf_req > 0 and nb_encode_pdu_establish_accept > 0 and nb_pdu_session_pending > 0 and nb_pdu_session_ue_establish_req > 0 and nb_pdu_status_active > 0:
				status = True

		return status
		
	def analyze_check_run_log(self, nfType):
		logFileName = nfType.lower().replace('-','') + '_check_run.log'

		cwd = os.getcwd()
		status = False
		if os.path.isfile(cwd + '/archives/' + logFileName):
			myCmd = 'iconv -f ISO-8859-1 -t UTF-8//TRANSLIT ' + cwd + '/archives/' + logFileName + ' -o ' + cwd + '/archives/' + logFileName + '.conv'
			subprocess.run(myCmd, shell=True)
			myCmd = 'mv ' + cwd + '/archives/' + logFileName + '.conv '  + cwd + '/archives/' + logFileName
			subprocess.run(myCmd, shell=True)
			nb_pfcp_hb_proc = 0
			nb_sx_hb_resp = 0
			nb_sx_hb_req = 0
			with open(cwd + '/archives/' + logFileName, 'r') as logfile:
				for line in logfile:
					if nfType == 'SPGW-U':
						result = re.search('PFCP HEARTBEAT PROCEDURE', line)
						if result is not None:
							nb_pfcp_hb_proc += 1
						result = re.search('SX HEARTBEAT RESPONSE', line)
						if result is not None:
							nb_sx_hb_resp += 1
						result = re.search('SX HEARTBEAT REQUEST', line)
						if result is not None:
							nb_sx_hb_req += 1
					if nfType == 'SMF':
						result = re.search('PFCP HEARTBEAT PROCEDURE', line)
						if result is not None:
							nb_pfcp_hb_proc += 1
						result = re.search('Sending HEARTBEAT_REQUEST', line)
						if result is not None:
							nb_sx_hb_resp += 1
						result = re.search('Sending HEARTBEAT_RESPONSE', line)
						if result is not None:
							nb_sx_hb_req += 1
				logfile.close()
			if nfType == 'SMF':
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
	print('  --smf_pdu_check')

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
	elif re.match('^\-\-smf_pdu_check', myArgv, re.IGNORECASE):
		vscd.smf_pdu_check = True
	else:
		sys.exit('Invalid Parameter: ' + myArgv)

if vscd.job_name == '' or vscd.job_id == '':
	sys.exit('Missing Parameter in job description')

vscd.checkLogs()
