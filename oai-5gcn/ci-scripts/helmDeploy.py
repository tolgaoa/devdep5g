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
#
#   Required Python Version
#     Python 3.x
#
#   Required Python Package
#     pexpect
#---------------------------------------------------------------------

#-----------------------------------------------------------
# Import
#-----------------------------------------------------------
import logging
import sshconnection as SSH
import html
import os
import re
import time
import sys

logging.basicConfig(
    level=logging.DEBUG,
    format="[%(asctime)s] %(name)s:%(levelname)s: %(message)s"
)

class ClusterDeploy:
	def __init__(self):
		self.remoteIPAdd = ""
		self.remoteUserName = ""
		self.remotePassword = ""
		self.OCUserName = ""
		self.OCPassword = ""
		self.OCProjectName = ""
		self.sourceCodePath = "/tmp/CI-CN5G-FED-RHEL8"
		self.imageTags = ""
		self.mode = ""

#-----------------$
#PUBLIC Methods$
#-----------------$

	def Deploy_5gcn(self):
		lIpAddr = self.remoteIPAdd
		lUserName = self.remoteUserName
		lPassWord = self.remotePassword
		lSourcePath = self.sourceCodePath
		ocUserName = self.OCUserName
		ocPassword = self.OCPassword
		ocProjectName = self.OCProjectName
		limageTags = self.imageTags
		if lIpAddr == '' or lUserName == '' or lPassWord == '' or lSourcePath == '' or ocUserName == '' or ocPassword == '' or ocProjectName == '' or limageTags == '':
			sys.exit('Insufficient Parameter')
		logging.debug('Running on server: ' + lIpAddr)
		mySSH = SSH.SSHConnection()
		mySSH.open(lIpAddr, lUserName, lPassWord)
		mySSH.command('cd ' + lSourcePath, '\$', 5)

		images = limageTags.split(',')
		for image in images:
			eachImage = image.split(':')
			imageName = eachImage[0]
			imageTag = eachImage[1]
			if imageName == 'mysql':
				continue
			# Check if image is exist on the Red Hat server, before pushing it to OC cluster
			mySSH.command2("sudo podman image inspect --format='Size = {{.Size}} bytes' " + imageName + ":" + imageTag + f' | tee -a {lSourcePath}/archives/{imageName}_image_info.log', 60, silent=True)
			if mySSH.cmd2Results.count('no such image') != 0:
				logging.error(f'\u001B[1m No such image {imageName}]\u001B[0m')
				mySSH.close()
				sys.exit(-1)
			else:
				result = re.search('Size *= *(?P<size>[0-9\-]+) *bytes', mySSH.cmd2Results)
				if result is not None:
					imageSize = float(result.group('size'))
					imageSize = imageSize / 1000
					if imageSize < 1000:
						logging.debug(f'\u001B[1m   {imageName} size is ' + ('%.0f' % imageSize) + ' kbytes\u001B[0m')
					else:
						imageSize = imageSize / 1000
						if imageSize < 1000:
							logging.debug(f'\u001B[1m   {imageName} size is ' + ('%.0f' % imageSize) + ' Mbytes\u001B[0m')
						else:
							imageSize = imageSize / 1000
							logging.debug(f'\u001B[1m   {imageName} is ' + ('%.3f' % imageSize) + ' Gbytes\u001B[0m')
				else:
					logging.debug(f'{imageName} size is unknown')

		# logging to OC Cluster and then switch to corresponding project
		mySSH.command2(f'oc login -u {ocUserName} -p {ocPassword}', 6, silent=True)
		if mySSH.cmd2Results.count('Login successful.') == 0:
			logging.error('\u001B[1m OC Cluster Login Failed\u001B[0m')
			mySSH.close()
			sys.exit(-1)
		else:
			logging.debug('\u001B[1m   Login to OC Cluster Successfully\u001B[0m')
		mySSH.command2(f'oc project {ocProjectName}', 6, silent=True)
		if mySSH.cmd2Results.count(f'Already on project "{ocProjectName}"') == 0 and mySSH.cmd2Results.count(f'Now using project "{self.OCProjectName}"') == 0:
			logging.error(f'\u001B[1m Unable to access OC project {ocProjectName}\u001B[0m')
			mySSH.close()
			sys.exit(-1)
		else:
			logging.debug(f'\u001B[1m   Now using project {ocProjectName}\u001B[0m')

		# Tag the image and push to the OC cluster
		mySSH.command2('oc whoami -t | sudo podman login -u ' + ocUserName + ' --password-stdin https://default-route-openshift-image-registry.apps.5glab.nsa.eurecom.fr/ --tls-verify=false', 6, silent=True)
		if mySSH.cmd2Results.count('Login Succeeded!') == 0:
			logging.error('\u001B[1m Podman Login to OC Cluster Registry Failed\u001B[0m')
			mySSH.close()
			sys.exit(-1)
		else:
			logging.debug('\u001B[1m Podman Login to OC Cluster Registry Successfully\u001B[0m')
		for image in images:
			eachImage = image.split(':')
			imageName = eachImage[0]
			imageTag = eachImage[1]
			if imageName == 'mysql':
				continue
			mySSH.command2(f'cd {lSourcePath} && oc create -f openshift/{imageName}-image-stream.yml 2>&1 | tee -a archives/5gcn_imagestream_summary.txt', 6, silent=True)
			if mySSH.cmd2Results.count('(AlreadyExists):') == 0 and mySSH.cmd2Results.count('created') == 0:
				logging.error(f'\u001B[1m Image Stream "{imageName}" Creation Failed on OC Cluster {ocProjectName}\u001B[0m')
				mySSH.close()
				sys.exit(-1)
			else:
				logging.debug(f'\u001B[1m   Image Stream "{imageName}" created on OC project {ocProjectName}\u001B[0m')
			mySSH.command2(f'sudo podman tag {imageName}:{imageTag} default-route-openshift-image-registry.apps.5glab.nsa.eurecom.fr/{self.OCProjectName}/{imageName}:{imageTag}', 6, silent=True)
			mySSH.command2(f'sudo podman push default-route-openshift-image-registry.apps.5glab.nsa.eurecom.fr/{self.OCProjectName}/{imageName}:{imageTag} --tls-verify=false 2>&1 | tee -a archives/5gcn_imagepush_summary.txt', 60, silent=True)
			time.sleep(10)
			if mySSH.cmd2Results.count('Storing signatures') == 0:
				logging.error(f'\u001B[1m Image "{imageName}" push to OC Cluster Registry Failed\u001B[0m')
				mySSH.close()
				sys.exit(-1)
			else:
				logging.debug(f'\u001B[1m Image "{imageName}" push to OC Cluster Registry Successfully\u001B[0m')

		# Using helm charts deployment
		passPods = 0
		time.sleep(5)
		for image in images:
			eachImage = image.split(':')
			imageName = eachImage[0]
			imageTag = eachImage[1]
			nameSufix = ''
			mySSH.command(f'sed -i -e "s#PROJECT#{ocProjectName}#g" ./charts/{imageName}/values.yaml', '\$', 6, silent=True)
			mySSH.command(f'sed -i -e "s#TAG#{imageTag}#g" ./charts/{imageName}/values.yaml', '\$', 6, silent=True)
			if imageName == 'oai-nrf':
				nameSufix = 'nrf'
			elif imageName == 'oai-amf':
				nameSufix = 'amf'
			elif imageName == 'oai-smf':
				nameSufix = 'smf'
			elif imageName == 'oai-spgwu-tiny':
				nameSufix = 'spgwu'
			mySSH.command2(f'cd {lSourcePath} && helm install {imageName} ./charts/{imageName}/ | tee -a archives/5gcn_helm_summary.txt 2>&1', 10, silent=True)
			if mySSH.cmd2Results.count('STATUS: deployed') == 0:
				logging.error(f'\u001B[1m Deploying "{imageName}" Failed using helm chart on OC Cluster\u001B[0m')
				self.UnDeploy_5gcn()
				self.AnalyzeLogFile_5gcn()
				sys.exit(-1)
			else:
				logging.debug(f'\u001B[1m   Deployed "{imageName}" Successfully using helm chart\u001B[0m')
			time.sleep(20)
			mySSH.command2(f'oc get pods -o wide -l app.kubernetes.io/instance={imageName} | tee -a {lSourcePath}/archives/5gcn_pods_summary.txt', 6, silent=True)
			result = re.search(f'{imageName}[\S\d\w]+', mySSH.cmd2Results)
			podName = result.group(0)
			isRunning = False
			count = 0
			while count < 3 and isRunning == False:
				time.sleep(10)
				if imageName == 'mysql':
					mySSH.command2(f'oc exec {podName} -i -t -- mysqladmin -u root --password=linux ping', 6, silent=True)
				else:
					mySSH.command2(f'oc exec {podName} -c {nameSufix} -it -- ps aux', 6, silent=True)
				if mySSH.cmd2Results.count(f'oai_{nameSufix}') != 0 or mySSH.cmd2Results.count(f'mysqld is alive') != 0 :
					logging.debug(f'\u001B[1m POD "{imageName}" Service Running Sucessfully\u001B[0m')
					isRunning = True
					passPods += 1
				count +=1	
			if isRunning == False:
				logging.error(f'\u001B[1m POD "{imageName}" Service Running FAILED \u001B[0m')
				self.UnDeploy_5gcn()
				self.AnalyzeLogFile_5gcn()
				sys.exit(-1)

		if passPods == 5:
			logging.debug(f'\u001B[1m   5gcn Deployment: OK \u001B[0m')
			mySSH.command2(f'echo "DEPLOYMENT: OK" > {lSourcePath}/archives/deployment_status.log', 5, silent=True)
		else:
			logging.error(f'\u001B[1m 	5gcn Deployment: KO \u001B[0m')
			mySSH.command2(f'echo "DEPLOYMENT: KO" > {lSourcePath}/archives/deployment_status.log', 5, silent=True)
			self.UnDeploy_5gcn()
			self.AnalyzeLogFile_5gcn()
			sys.exit(-1)
		self.AnalyzeLogFile_5gcn()
		
	def UnDeploy_5gcn(self):
		mySSH = SSH.SSHConnection()
		mySSH.open(self.remoteIPAdd, self.remoteUserName, self.remotePassword)
		mySSH.command('cd ' + self.sourceCodePath, '\$', 5)
		logging.debug('\u001B[1m   UnDeploying the 5gcn\u001B[0m')
		# logging to OC Cluster and then switch to corresponding project
		mySSH.command2(f'oc login -u {self.OCUserName} -p {self.OCPassword}', 6, silent=True)
		if mySSH.cmd2Results.count('Login successful.') == 0:
			logging.error('\u001B[1m OC Cluster Login Failed\u001B[0m')
			mySSH.close()
			sys.exit(-1)
		else:
			logging.debug('\u001B[1m   Login to OC Cluster Successfully\u001B[0m')
		mySSH.command2(f'oc project {self.OCProjectName}', 6, silent=True)
		if mySSH.cmd2Results.count(f'Already on project "{self.OCProjectName}"') == 0 and mySSH.cmd2Results.count(f'Now using project "{self.OCProjectName}"') == 0:
			logging.error(f'\u001B[1m Unable to access OC project {self.OCProjectName}\u001B[0m')
			mySSH.close()
			sys.exit(-1)
		else:
			logging.debug(f'\u001B[1m   Now using project {self.OCProjectName}\u001B[0m')

		# UnDeploy the 5gcn pods
		images = self.imageTags.split(',')
		for image in images:
			eachImage = image.split(':')
			imageName = eachImage[0]
			imageTag = eachImage[1]
			mySSH.command2(f'cd {self.sourceCodePath} && helm uninstall {imageName} | tee -a archives/5gcn_helm_summary.txt 2>&1', 6, silent=True)
			if mySSH.cmd2Results.count(f'release "{imageName}" uninstalled') == 0 and mySSH.cmd2Results.count(f'Error: uninstall: Release not loaded: {imageName}: release: not found') == 0:
				logging.error(f'\u001B[1m UnDeploying "{imageName}" Failed using helm chart on OC Cluster\u001B[0m')
			else:
				logging.debug(f'\u001B[1m   UnDeployed "{imageName}" Successfully on OC Cluster\u001B[0m')
			time.sleep(2)
			# Delete images and imagestream
			if imageName == 'mysql':
				continue
			mySSH.command2(f'sudo podman rmi default-route-openshift-image-registry.apps.5glab.nsa.eurecom.fr/{self.OCProjectName}/{imageName}:{imageTag}', 6, silent=True)
			mySSH.command2(f'oc delete is {imageName}', 6, silent=True)
			logging.debug(f'\u001B[1m Deleted the "{imageName}" Image and ImageStream\u001B[0m')
		mySSH.command2('oc logout', 6, silent=True)
		mySSH.close()
		self.AnalyzeLogFile_5gcn()


	def AnalyzeLogFile_5gcn(self):
		lIpAddr = self.remoteIPAdd
		lUserName = self.remoteUserName
		lPassWord = self.remotePassword
		lSourcePath = self.sourceCodePath
		mySSH = SSH.SSHConnection()
		mySSH.open(lIpAddr, lUserName, lPassWord)
		mySSH.command('cd ' + lSourcePath, '\$', 5)
		mySSH.copyin(lIpAddr, lUserName, lPassWord, lSourcePath + '/archives/*', '../archives/')
		mySSH.command('rm -rf ./archives', '\$', 5)
		mySSH.command('oc logout', '\$', 6)
		mySSH.close()

def Usage():
	print('----------------------------------------------------------------------------------------------------------------------')
	print('helmDeploy.py')
	print('   Deploy and UnDeploy the 5gcn components on the Cluster.')
	print('----------------------------------------------------------------------------------------------------------------------')
	print('Usage: python3 helmDeploy.py [options]')
	print('  --help  Show this help.')
	print('---------------------------------------------------------------------------------------------- Mandatory Options -----')
	print('  --mode=[Deploy/Deploy]')
	print('  --remoteIPAdd=[IP Address of remote Server]')
	print('  --remoteUserName=[UserName of remote Server]')
	print('  --remotePassword=[Password of remote Server]')
	print('  --OCUserName=[Cluster UserName]')
	print('  --OCPassword=[Cluster Password]')
	print('  --OCProjectName=[Cluster Project name]')
	print('  --imageTags=[Image tags of all the 5gcn components]')
	print('------------------------------------------------------------------------------------------------- Actions Syntax -----')
	print('python3 helmDeploy.py --mode=Deploy [Mandatory Options]')
	print('python3 helmDeploy.py --mode=UnDeploy [Mandatory Options]')



#--------------------------------------------------------------------------------------------------------
#
# Start of main
#
#--------------------------------------------------------------------------------------------------------

CN = ClusterDeploy()

argvs = sys.argv

while len(argvs) > 1:
	myArgv = argvs.pop(1)
	if re.match('^\-\-help$', myArgv, re.IGNORECASE):
		Usage()
		sys.exit(0)
	elif re.match('^\-\-mode=(.+)$', myArgv, re.IGNORECASE):
		matchReg = re.match('^\-\-mode=(.+)$', myArgv, re.IGNORECASE)
		CN.mode = matchReg.group(1)	
	elif re.match('^\-\-remoteIPAdd=(.+)$', myArgv, re.IGNORECASE):
		matchReg = re.match('^\-\-remoteIPAdd=(.+)$', myArgv, re.IGNORECASE)
		CN.remoteIPAdd = matchReg.group(1)
	elif re.match('^\-\-remoteUserName=(.+)$', myArgv, re.IGNORECASE):
		matchReg = re.match('^\-\-remoteUserName=(.+)$', myArgv, re.IGNORECASE)
		CN.remoteUserName = matchReg.group(1)
	elif re.match('^\-\-remotePassword=(.+)$', myArgv, re.IGNORECASE):
		matchReg = re.match('^\-\-remotePassword=(.+)$', myArgv, re.IGNORECASE)
		CN.remotePassword = matchReg.group(1)
	elif re.match('^\-\-OCUserName=(.+)$', myArgv, re.IGNORECASE):
		matchReg = re.match('^\-\-OCUserName=(.+)$', myArgv, re.IGNORECASE)
		CN.OCUserName = matchReg.group(1)
	elif re.match('^\-\-OCPassword=(.+)$', myArgv, re.IGNORECASE):
		matchReg = re.match('^\-\-OCPassword=(.+)$', myArgv, re.IGNORECASE)
		CN.OCPassword = matchReg.group(1)
	elif re.match('^\-\-OCProjectName=(.+)$', myArgv, re.IGNORECASE):
		matchReg = re.match('^\-\-OCProjectName=(.+)$', myArgv, re.IGNORECASE)
		CN.OCProjectName = matchReg.group(1)
	elif re.match('^\-\-imageTags=(.+)$', myArgv, re.IGNORECASE):
		matchReg = re.match('^\-\-imageTags=(.+)$', myArgv, re.IGNORECASE)
		CN.imageTags = matchReg.group(1)
	else:
		sys.exit('Invalid Parameter: ' + myArgv)

if CN.mode == 'Deploy':
	CN.Deploy_5gcn()
elif CN.mode == 'UnDeploy':
	CN.UnDeploy_5gcn()