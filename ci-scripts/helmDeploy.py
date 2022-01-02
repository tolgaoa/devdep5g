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
import html
import os
import re
import time
import sys
import subprocess

logging.basicConfig(
    level=logging.DEBUG,
    format="[%(asctime)s] %(name)s:%(levelname)s: %(message)s"
)

class ClusterDeploy:
	def __init__(self):
		self.OCUserName = ""
		self.OCPassword = ""
		self.OCProjectName = ""
		self.imageTags = ""
		self.mode = ""

#-----------------$
#PUBLIC Methods$
#-----------------$

	def Deploy_5gcn(self):
		ocUserName = self.OCUserName
		ocPassword = self.OCPassword
		ocProjectName = self.OCProjectName
		limageTags = self.imageTags
		if ocUserName == '' or ocPassword == '' or ocProjectName == '' or limageTags == '':
			sys.exit('Insufficient Parameter')

		logging.debug('\u001B[1m   Checking all 5GCN component IMAGES are pre existing and correct\u001B[0m')
		images = limageTags.split(',')
		for image in images:
			eachImage = image.split(':')
			imageName = eachImage[0]
			imageTag = eachImage[1]
			if imageName == 'mysql':
				continue
			# Check if image is exist on the Red Hat server, before pushing it to OC cluster
			subprocess.run(f'echo "IMAGENAME_TAG: {imageName}:{imageTag}" > archives/{imageName}_image_info.log', shell=True)
			res = subprocess.check_output("sudo podman image inspect --format='Size = {{.Size}} bytes' " + f'{imageName}:{imageTag} | tee -a archives/{imageName}_image_info.log', shell=True, universal_newlines=True)
			subprocess.run("sudo podman image inspect --format='Date = {{.Created}}' " + f'{imageName}:{imageTag} >> archives/{imageName}_image_info.log', shell=True)
			res2 = re.search('no such image', str(res.strip()))
			if res2 is not None:
				logging.error(f'\u001B[1m No such image {imageName}]\u001B[0m')
				sys.exit(-1)
			else:
				res2 = re.search('Size *= *(?P<size>[0-9\-]+) *bytes', str(res.strip()))
				if res2 is not None:
					imageSize = float(res2.group('size'))
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
		res = subprocess.check_output(f'oc login -u {ocUserName} -p {ocPassword}', shell=True, universal_newlines=True)
		res2 = re.search('Login successful.', str(res.strip()))
		if res2 is None:
			logging.error('\u001B[1m OC Cluster Login Failed\u001B[0m')
			sys.exit(-1)
		else:
			logging.debug('\u001B[1m   Login to OC Cluster Successfully\u001B[0m')
		res = subprocess.check_output(f'oc project {ocProjectName}', shell=True, universal_newlines=True)
		res2 = re.search(f'Already on project "{ocProjectName}"', str(res.strip()))
		res3 = re.search(f'Now using project "{self.OCProjectName}"', str(res.strip()))
		if res2 is None and res3 is None:
			logging.error(f'\u001B[1m Unable to access OC project {ocProjectName}\u001B[0m')
			sys.exit(-1)
		else:
			logging.debug(f'\u001B[1m   Now using project {ocProjectName}\u001B[0m')

		# Tag the image and push to the OC cluster
		res = subprocess.check_output('oc whoami -t | sudo podman login -u ' + ocUserName + ' --password-stdin https://default-route-openshift-image-registry.apps.5glab.nsa.eurecom.fr/ --tls-verify=false', shell=True, universal_newlines=True)
		res2 = re.search('Login Succeeded!', str(res.strip()))
		if res2 is None:
			logging.error('\u001B[1m Podman Login to OC Cluster Registry Failed\u001B[0m')
			sys.exit(-1)
		else:
			logging.debug('\u001B[1m Podman Login to OC Cluster Registry Successfully\u001B[0m')
		for image in images:
			eachImage = image.split(':')
			imageName = eachImage[0]
			imageTag = eachImage[1]
			if imageName == 'mysql':
				continue
			res = subprocess.check_output(f'oc create -f openshift/{imageName}-image-stream.yml 2>&1 | tee -a archives/5gcn_imagestream_summary.txt || true', stderr=subprocess.STDOUT, shell=True, universal_newlines=True)
			res2 = re.search('already exists', str(res.strip()))
			res3 = re.search('created', str(res.strip()))
			if res2 is None and res3 is None:
				logging.error(f'\u001B[1m Image Stream "{imageName}" Creation Failed on OC project {ocProjectName}\u001B[0m')
				sys.exit(-1)
			else:
				logging.debug(f'\u001B[1m   Image Stream "{imageName}" created on OC project {ocProjectName}\u001B[0m')
			subprocess.run(f'sudo podman tag {imageName}:{imageTag} default-route-openshift-image-registry.apps.5glab.nsa.eurecom.fr/{ocProjectName}/{imageName}:{imageTag}', shell=True)
			res = subprocess.check_output(f'sudo podman push default-route-openshift-image-registry.apps.5glab.nsa.eurecom.fr/{ocProjectName}/{imageName}:{imageTag} --tls-verify=false 2>&1 | tee -a archives/5gcn_imagepush_summary.txt', shell=True, universal_newlines=True)
			time.sleep(10)
			res2 = re.search('Storing signatures', str(res.strip()))
			if res2 is None:
				logging.error(f'\u001B[1m Image "{imageName}" push to OC Cluster Registry Failed\u001B[0m')
				sys.exit(-1)
			else:
				logging.debug(f'\u001B[1m Image "{imageName}" push to OC Cluster Registry Successfully\u001B[0m')

		# Using helm charts deployment
		logging.debug(f'\u001B[1m Deploying 5GCN Components on the Cluster using Helm Charts\u001B[0m')
		passPods = 0
		time.sleep(5)
		for image in images:
			eachImage = image.split(':')
			imageName = eachImage[0]
			imageTag = eachImage[1]
			nameSufix = ''
			subprocess.run(f'sed -i -e "s#PROJECT#{ocProjectName}#g" ./ci-scripts/charts/{imageName}/values.yaml', shell=True)
			subprocess.run(f'sed -i -e "s#TAG#{imageTag}#g" ./ci-scripts/charts/{imageName}/values.yaml', shell=True)
			if imageName == 'oai-nrf':
				nameSufix = 'nrf'
			if imageName == 'oai-udr':
				nameSufix = 'udr'
			if imageName == 'oai-udm':
				nameSufix = 'udm'
			if imageName == 'oai-ausf':
				nameSufix = 'ausf'
			elif imageName == 'oai-amf':
				nameSufix = 'amf'
			elif imageName == 'oai-smf':
				nameSufix = 'smf'
			elif imageName == 'oai-spgwu-tiny':
				nameSufix = 'spgwu'
			res = subprocess.check_output(f'helm install {imageName} ./ci-scripts/charts/{imageName}/ | tee -a archives/5gcn_helm_summary.txt 2>&1', shell=True, universal_newlines=True)
			res2 = re.search('STATUS: deployed', str(res.strip()))
			if res2 is None:
				subprocess.run(f'echo "{imageName}: HELM KO" >> archives/5gcn_helm_summary.txt 2>&1', shell=True)
				logging.error(f'\u001B[1m Deploying "{imageName}" Failed using helm chart on OC Cluster\u001B[0m')
				logging.error(f'\u001B[1m 	5GCN Deployment: KO \u001B[0m')
				subprocess.run(f'echo "DEPLOYMENT: KO" > archives/deployment_status.log', shell=True)
				sys.exit(-1)
			else:
				subprocess.run(f'echo "{imageName}: HELM OK" >> archives/5gcn_helm_summary.txt 2>&1', shell=True)
				logging.debug(f'\u001B[1m   Deployed "{imageName}" Successfully using helm chart\u001B[0m')
			time.sleep(20)
			res = subprocess.check_output(f'oc get pods -o wide -l app.kubernetes.io/instance={imageName} | tee -a archives/5gcn_pods_summary.txt', shell=True, universal_newlines=True)
			res2 = re.search(f'{imageName}[\S\d\w]+', str(res.strip()))
			podName = res2.group(0)
			isRunning = False
			count = 0
			while count < 12 and isRunning == False:
				time.sleep(5)
				if imageName == 'mysql':
					res = subprocess.check_output(f'oc exec {podName} -i -t -- mysqladmin -u root --password=linux ping || true', stderr=subprocess.STDOUT, shell=True, universal_newlines=True)
				else:
					res = subprocess.check_output(f'oc exec {podName} -c {nameSufix} -- ps aux || true', stderr=subprocess.STDOUT, shell=True, universal_newlines=True)
				res2 = re.search(f'oai_{nameSufix}', str(res.strip()))
				res3 = re.search(f'mysqld is alive', str(res.strip()))
				if res2 is not None or res3 is not None:
					logging.debug(f'\u001B[1m POD "{imageName}" Service Running Sucessfully\u001B[0m')
					subprocess.run(f'echo "{imageName}: POD OK" >> archives/5gcn_pods_summary.txt 2>&1', shell=True)
					isRunning = True
					passPods += 1
				count +=1	
			if isRunning == False:
				logging.error(f'\u001B[1m POD "{imageName}" Service Running FAILED \u001B[0m')
				subprocess.run(f'echo "{imageName}: POD KO" >> archives/5gcn_pods_summary.txt 2>&1', shell=True)
				sys.exit(-1)

		if passPods == 8:
			logging.debug(f'\u001B[1m   5GCN Deployment: OK \u001B[0m')
			subprocess.run(f'echo "DEPLOYMENT: OK" > archives/deployment_status.log', shell=True)
		else:
			logging.error(f'\u001B[1m 	5GCN Deployment: KO \u001B[0m')
			subprocess.run(f'echo "DEPLOYMENT: KO" > archives/deployment_status.log', shell=True)
			sys.exit(-1)
		subprocess.run('oc logout', shell=True)
		
	def UnDeploy_5gcn(self):
		logging.debug('\u001B[1m   UnDeploying the 5GCN\u001B[0m')
		# logging to OC Cluster and then switch to corresponding project
		res = subprocess.check_output(f'oc login -u {self.OCUserName} -p {self.OCPassword}|| true', stderr=subprocess.STDOUT, shell=True, universal_newlines=True)
		res2 = re.search(f'Login successful.', str(res.strip()))
		if res2 is None:
			logging.error('\u001B[1m OC Cluster Login Failed\u001B[0m')
			sys.exit(-1)
		res = subprocess.check_output(f'oc project {self.OCProjectName} || true', stderr=subprocess.STDOUT, shell=True, universal_newlines=True)
		res2 = re.search(f'Already on project "{self.OCProjectName}"', str(res.strip()))
		res3 = re.search(f'Now using project "{self.OCProjectName}"', str(res.strip()))
		if res2 is None and res3 is None:
			logging.error(f'\u001B[1m Unable to access OC project {self.OCProjectName}\u001B[0m')
			sys.exit(-1)

		# UnDeploy the 5gcn pods
		images = self.imageTags.split(',')
		for image in images:
			eachImage = image.split(':')
			imageName = eachImage[0]
			imageTag = eachImage[1]
			res = subprocess.check_output(f'helm uninstall {imageName} | tee -a archives/5gcn_helm_summary.txt 2>&1', stderr=subprocess.STDOUT, shell=True, universal_newlines=True)
			res2 = re.search(f'release "{imageName}" uninstalled', str(res.strip()))
			res3 = re.search(f'Error: uninstall: Release not loaded: {imageName}: release: not found', str(res.strip()))
			if res2 is not None:
				logging.debug(f'\u001B[1m   UnDeployed "{imageName}" Successfully on OC Cluster\u001B[0m')
			elif res3 is None:
				logging.error(f'\u001B[1m UnDeploying "{imageName}" Failed using helm chart on OC Cluster\u001B[0m')
			time.sleep(2)
			# Delete images and imagestream
			if imageName == 'mysql':
				continue
			res = subprocess.check_output(f'sudo podman rmi default-route-openshift-image-registry.apps.5glab.nsa.eurecom.fr/{self.OCProjectName}/{imageName}:{imageTag} || true', stderr=subprocess.STDOUT, shell=True, universal_newlines=True)
			res2 = re.search('in local storage: no such image', str(res.strip()))
			res = subprocess.check_output(f'oc delete is {imageName} || true', stderr=subprocess.STDOUT, shell=True, universal_newlines=True)
			res3 = re.search('Error from server (NotFound):', str(res.strip()))
			if res2 is None and res3 is None:
				logging.debug(f'\u001B[1m Deleted the "{imageName}" Image and ImageStream\u001B[0m')
		subprocess.run('oc logout', shell=True)

	def GetLogsConfigsPcaps(self):
		# copying the pod's logs, configs, pcaps
		logging.debug(f'\u001B[1m   Copying the PODs log, config & pcap\u001B[0m')
		res = subprocess.check_output(f'oc login -u {self.OCUserName} -p {self.OCPassword}|| true', stderr=subprocess.STDOUT, shell=True, universal_newlines=True)
		res2 = re.search(f'Login successful.', str(res.strip()))
		if res2 is None:
			logging.error('\u001B[1m OC Cluster Login Failed\u001B[0m')
			sys.exit(-1)
		res = subprocess.check_output(f'oc project {self.OCProjectName} || true', stderr=subprocess.STDOUT, shell=True, universal_newlines=True)
		res2 = re.search(f'Already on project "{self.OCProjectName}"', str(res.strip()))
		res3 = re.search(f'Now using project "{self.OCProjectName}"', str(res.strip()))
		if res2 is None and res3 is None:
			logging.error(f'\u001B[1m Unable to access OC project {self.OCProjectName}\u001B[0m')
			sys.exit(-1)
		subprocess.run(f'mkdir -p archives/pcap', shell=True)
		subprocess.run(f'mkdir -p archives/logs', shell=True)
		subprocess.run(f'mkdir -p archives/config', shell=True)
		images = self.imageTags.split(',')
		for image in images:
			eachImage = image.split(':')
			imageName = eachImage[0]
			imageTag = eachImage[1]
			if imageName == 'mysql':
				continue
			if imageName == 'oai-nrf':
				nameSufix = 'nrf'
			if imageName == 'oai-udr':
				nameSufix = 'udr'
			if imageName == 'oai-udm':
				nameSufix = 'udm'
			if imageName == 'oai-ausf':
				nameSufix = 'ausf'
			elif imageName == 'oai-amf':
				nameSufix = 'amf'
			elif imageName == 'oai-smf':
				nameSufix = 'smf'
			elif imageName == 'oai-spgwu-tiny':
				nameSufix = 'spgwu'
				nameSufix1 = 'spgwu-tiny'
			res = subprocess.check_output(f'oc get pods -o wide -l app.kubernetes.io/instance={imageName} || true', stderr=subprocess.STDOUT, shell=True, universal_newlines=True)
			res2 = re.search(f'{imageName}[\S\d\w]+', str(res.strip()))
			podName = res2.group(0)
			subprocess.run(f'oc logs {podName} {nameSufix} > archives/logs/{imageName}_pod.log', shell=True)
			time.sleep(1)
			if imageName == 'oai-spgwu-tiny':
				subprocess.run(f'oc cp {podName}:/openair-{nameSufix1}/etc/ archives/config/ -c {nameSufix}', shell=True)
				subprocess.run(f'oc cp {podName}:/pcap/ archives/pcap/ -c tcpdump', shell=True)
			else:
				subprocess.run(f'oc cp {podName}:/openair-{nameSufix}/etc/ archives/config/ -c {nameSufix}', shell=True)
		subprocess.run('oc logout', shell=True)

def Usage():
	print('----------------------------------------------------------------------------------------------------------------------')
	print('helmDeploy.py')
	print('   Deploy and UnDeploy the 5gcn components on the Cluster.')
	print('----------------------------------------------------------------------------------------------------------------------')
	print('Usage: python3 helmDeploy.py [options]')
	print('  --help  Show this help.')
	print('---------------------------------------------------------------------------------------------- Mandatory Options -----')
	print('  --mode=[Deploy/UnDeploy]')
	print('  --OCUserName=[Cluster UserName]')
	print('  --OCPassword=[Cluster Password]')
	print('  --OCProjectName=[Cluster Project name]')
	print('  --imageTags=[Image tags of all the 5gcn components]')
	print('------------------------------------------------------------------------------------------------- Actions Syntax -----')
	print('python3 helmDeploy.py --mode=Deploy [Mandatory Options]')
	print('python3 helmDeploy.py --mode=UnDeploy [Mandatory Options]')
	print('python3 helmDeploy.py --mode=GetLogs [Mandatory Options]')	



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
elif CN.mode == 'GetLogs':
	CN.GetLogsConfigsPcaps()
