#/*
# * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
# * contributor license agreements.  See the NOTICE file distributed with
# * this work for additional information regarding copyright ownership.
# * The OpenAirInterface Software Alliance licenses this file to You under
# * the OAI Public License, Version 1.1  (the "License"); you may not use this file
# * except in compliance with the License.
# * You may obtain a copy of the License at
# *
# *   http://www.openairinterface.org/?page_id=698
# *
# * Unless required by applicable law or agreed to in writing, software
# * distributed under the License is distributed on an "AS IS" BASIS,
# * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# * See the License for the specific language governing permissions and
# * limitations under the License.
# *-------------------------------------------------------------------------------
# * For more information about the OpenAirInterface (OAI) Software Alliance:
# *   contact@openairinterface.org
# */
#---------------------------------------------------------------------

import os
import re
import sys
import subprocess
import yaml

class HtmlReport():
	def __init__(self):
		self.job_name = ''
		self.job_id = ''
		self.job_url = ''
		self.job_start_time = 'TEMPLATE_TIME'

	def generate(self):
		cwd = os.getcwd()
		self.file = open(cwd + '/test_results_oai_cn5g_oc.html', 'w')
		self.generateHeader()
		self.deploymentSummaryHeader()
		finalStatus = self.testSummaryHeader()
		self.testSummaryDetails()
		self.testSummaryFooter()

		self.generateFooter()
		self.file.close()

		if finalStatus:
			sys.exit(0)
		else:
			print("DS-TESTER testing FAILED")

	def generateHeader(self):
		# HTML Header
		self.file.write('<!DOCTYPE html>\n')
		self.file.write('<html class="no-js" lang="en-US">\n')
		self.file.write('<head>\n')
		self.file.write('  <meta name="viewport" content="width=device-width, initial-scale=1">\n')
		self.file.write('  <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css">\n')
		self.file.write('  <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>\n')
		self.file.write('  <script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js"></script>\n')
		self.file.write('  <title>OAI 5G Core Network Test Results for ' + self.job_name + ' job build #' + self.job_id + '</title>\n')
		self.file.write('</head>\n')
		self.file.write('<body><div class="container">\n')
		self.file.write('  <table width = "100%" style="border-collapse: collapse; border: none;">\n')
		self.file.write('   <tr style="border-collapse: collapse; border: none;">\n')
		self.file.write('   <td style="border-collapse: collapse; border: none;">\n')
		self.file.write('    <a href="http://www.openairinterface.org/">\n')
		self.file.write('      <img src="http://www.openairinterface.org/wp-content/uploads/2016/03/cropped-oai_final_logo2.png" alt="" border="none" height=50 width=150>\n')
		self.file.write('      </img>\n')
		self.file.write('    </a>\n')
		self.file.write('   </td>\n')
		self.file.write('   <td style="border-collapse: collapse; border: none; vertical-align: center;">\n')
		self.file.write('     <b><font size = "6">Job Summary -- Job: ' + self.job_name + ' -- Build-ID: <a href="' + self.job_url + '">' + self.job_id + '</a></font></b>\n')
		self.file.write('   </td>\n')
		self.file.write('   </tr>\n')
		self.file.write('  </table>\n')
		self.file.write('  <br>\n')

	def generateFooter(self):
		self.file.write('  <div class="well well-lg">End of Test Report -- Copyright <span class="glyphicon glyphicon-copyright-mark"></span> 2020 <a href="http://www.openairinterface.org/">OpenAirInterface</a>. All Rights Reserved.</div>\n')
		self.file.write('</div></body>\n')
		self.file.write('</html>\n')

	def deploymentSummaryHeader(self):
		self.file.write('  <h2>Deployment Summary</h2>\n')
		cwd = os.getcwd()
		if os.path.isfile(cwd + '/archives/deployment_status.log'):
			cmd = 'egrep -c "DEPLOYMENT: OK" archives/deployment_status.log || true'
			status = False
			ret = subprocess.run(cmd, shell=True, stdout=subprocess.PIPE, encoding='utf-8')
			if ret.stdout is not None:
				if ret.stdout.strip() == '1':
					status = True
			if status:
				self.file.write('  <div class="alert alert-success">\n')
				self.file.write('    <strong>Successful Deployment! <span class="glyphicon glyphicon-warning-sign"></span></strong>\n')
				self.file.write('  </div>\n')
			else:
				self.file.write('  <div class="alert alert-danger">\n')
				self.file.write('    <strong>Failed Deployment! <span class="glyphicon glyphicon-warning-sign"></span></strong>\n')
				self.file.write('  </div>\n')
		else:
			self.file.write('  <div class="alert alert-warning">\n')
			self.file.write('    <strong>LogFile not available! <span class="glyphicon glyphicon-warning-sign"></span></strong>\n')
			self.file.write('  </div>\n')
		self.file.write('  <br>\n')
		self.file.write('  <button data-toggle="collapse" data-target="#deployment-details">More details on Deployment</button>\n')
		self.file.write('  <br>\n')
		self.file.write('  <div id="deployment-details" class="collapse">\n')
		self.file.write('  <br>\n')
		self.file.write('  <table class="table-bordered" width = "80%" align = "center" border = 1>\n')
		self.file.write('     <tr bgcolor = "#33CCFF" >\n')
		self.file.write('       <th>Container Name</th>\n')
		self.file.write('       <th>Used Image Tag</th>\n')
		self.file.write('       <th>Image Creation Date</th>\n')
		self.file.write('       <th>Used Image Size</th>\n')
		self.file.write('       <th>Helm Status</th>\n')
		self.file.write('       <th>Pod Status</th>\n')
		self.file.write('     </tr>\n')
		self.addImageRow('mysql')
		self.addImageRow('oai-nrf')
		self.addImageRow('oai-udr')
		self.addImageRow('oai-udm')
		self.addImageRow('oai-ausf')
		self.addImageRow('oai-amf')
		self.addImageRow('oai-smf')
		self.addImageRow('oai-spgwu-tiny')
		self.file.write('  </table>\n')
		self.file.write('  </div>\n')

	def addImageRow(self, imageName):
		cwd = os.getcwd()
		if os.path.isfile(cwd + '/archives/' + imageName + '_image_info.log'):
			usedTag = ''
			createDate = ''
			size = ''
			with open(cwd + '/archives/' + imageName + '_image_info.log') as imageLog:
				for line in imageLog:
					line = line.strip()
					result = re.search('IMAGENAME_TAG: (?P<tag>[a-zA-Z0-9\-\_:]+)', line)
					if result is not None:
						usedTag = result.group('tag')
					result = re.search('Date = (?P<date>[a-zA-Z0-9\-\_:]+)', line)
					if result is not None:
						createDate = result.group('date')
					result = re.search('Size = (?P<size>[0-9]+) bytes', line)
					if result is not None:
						sizeInt = int(result.group('size'))
						if sizeInt < 1000000:
							sizeInt = int(sizeInt / 1000)
							size = str(sizeInt) + ' kB'
						else:
							sizeInt = int(sizeInt / 1000000)
							size = str(sizeInt) + ' MB'
			imageLog.close()
			helmState = 'UNKNOW'
			podState = 'UNKNOW'
			cmd = f'egrep -c "{imageName}: HELM OK" archives/5gcn_helm_summary.txt || true'
			ret = subprocess.run(cmd, shell=True, stdout=subprocess.PIPE, encoding='utf-8')
			if ret.stdout is not None:
				if ret.stdout.strip() == '1':
					helmState = 'OK'
			cmd = f'egrep -c "{imageName}: HELM KO" archives/5gcn_helm_summary.txt || true'
			ret = subprocess.run(cmd, shell=True, stdout=subprocess.PIPE, encoding='utf-8')
			if ret.stdout is not None:
				if ret.stdout.strip() == '1':
					helmState = 'KO'
			cmd = f'egrep -c "{imageName}: POD OK" archives/5gcn_pods_summary.txt || true'
			ret = subprocess.run(cmd, shell=True, stdout=subprocess.PIPE, encoding='utf-8')
			if ret.stdout is not None:
				if ret.stdout.strip() == '1':
					podState = 'OK'
			cmd = f'egrep -c "{imageName}: POD KO" archives/5gcn_pods_summary.txt || true'
			ret = subprocess.run(cmd, shell=True, stdout=subprocess.PIPE, encoding='utf-8')
			if ret.stdout is not None:
				if ret.stdout.strip() == '1':
					podState = 'KO'    
			self.file.write('     <tr>\n')
			self.file.write('       <td>' + imageName + '</td>\n')
			self.file.write('       <td>' + usedTag + '</td>\n')
			self.file.write('       <td>' + createDate + '</td>\n')
			self.file.write('       <td>' + size + '</td>\n')
			if helmState == 'OK':
				self.file.write('       <td bgcolor = "DarkGreen"><b><font color="white">' + helmState + '</font></b></td>\n')
			elif helmState == 'KO':
				self.file.write('       <td bgcolor = "Red"><b><font color="white">' + helmState + '</font></b></td>\n')
			else:
				self.file.write('       <td bgcolor = "DarkOrange"><b><font color="white">' + helmState + '</font></b></td>\n')
			if podState == 'OK':
				self.file.write('       <td bgcolor = "DarkGreen"><b><font color="white">' + podState + '</font></b></td>\n')
			elif podState == 'KO':
				self.file.write('       <td bgcolor = "Red"><b><font color="white">' + podState + '</font></b></td>\n')
			else:
				self.file.write('       <td bgcolor = "DarkOrange"><b><font color="white">' + podState + '</font></b></td>\n')
			self.file.write('     </tr>\n')
		else:
			if imageName == 'mysql':
				self.file.write('     <tr>\n')
				self.file.write('       <td>mysql</td>\n')
				self.file.write('       <td>mysql:5.7.30</td>\n')
				self.file.write('       <td>N/A</td>\n')
				self.file.write('       <td>449MB</td>\n')
				helmState = 'UNKNOW'
				podState = 'UNKNOW'
				cmd = f'egrep -c "{imageName}: HELM OK" archives/5gcn_helm_summary.txt || true'
				ret = subprocess.run(cmd, shell=True, stdout=subprocess.PIPE, encoding='utf-8')
				if ret.stdout is not None:
					if ret.stdout.strip() == '1':
						helmState = 'OK'
				cmd = f'egrep -c "{imageName}: HELM KO" archives/5gcn_helm_summary.txt || true'
				ret = subprocess.run(cmd, shell=True, stdout=subprocess.PIPE, encoding='utf-8')
				if ret.stdout is not None:
					if ret.stdout.strip() == '1':
						helmState = 'KO'
				cmd = f'egrep -c "{imageName}: POD OK" archives/5gcn_pods_summary.txt || true'
				ret = subprocess.run(cmd, shell=True, stdout=subprocess.PIPE, encoding='utf-8')
				if ret.stdout is not None:
					if ret.stdout.strip() == '1':
						podState = 'OK'
				cmd = f'egrep -c "{imageName}: POD KO" archives/5gcn_pods_summary.txt || true'
				ret = subprocess.run(cmd, shell=True, stdout=subprocess.PIPE, encoding='utf-8')
				if ret.stdout is not None:
					if ret.stdout.strip() == '1':
						podState = 'KO'    
				if helmState == 'OK':
					self.file.write('       <td bgcolor = "DarkGreen"><b><font color="white">' + helmState + '</font></b></td>\n')
				elif helmState == 'KO':
					self.file.write('       <td bgcolor = "Red"><b><font color="white">' + helmState + '</font></b></td>\n')
				else:
					self.file.write('       <td bgcolor = "DarkOrange"><b><font color="white">' + helmState + '</font></b></td>\n')
				if podState == 'OK':
					self.file.write('       <td bgcolor = "DarkGreen"><b><font color="white">' + podState + '</font></b></td>\n')
				elif podState == 'KO':
					self.file.write('       <td bgcolor = "Red"><b><font color="white">' + podState + '</font></b></td>\n')
				else:
					self.file.write('       <td bgcolor = "DarkOrange"><b><font color="white">' + podState + '</font></b></td>\n')
				self.file.write('     </tr>\n')
			else:
				self.file.write('     <tr>\n')
				self.file.write('       <td>' + imageName + '</td>\n')
				self.file.write('       <td>UNKNOWN</td>\n')
				self.file.write('       <td>N/A</td>\n')
				self.file.write('       <td>N/A</td>\n')
				self.file.write('       <td bgcolor = "DarkOrange"><b><font color="white">UNKNOW</font></b></td>\n')
				self.file.write('       <td bgcolor = "DarkOrange"><b><font color="white">UNKNOW</font></b></td>\n')
				self.file.write('     </tr>\n')

	def testSummaryHeader(self):
		self.file.write('  <h2>DS Tester Summary</h2>\n')
		cwd = os.getcwd()
		finalStatusOK = False
		if os.path.isfile(cwd + '/DS-TEST-RESULTS/hcintegration.yaml'):
			cmd = f'egrep -c "final-result: pass" DS-TEST-RESULTS/hcintegration.yaml || true'
			ret = subprocess.run(cmd, shell=True, stdout=subprocess.PIPE, encoding='utf-8')
			if ret.stdout is not None:
				if ret.stdout.strip() == '1':
					finalStatusOK = True
			if finalStatusOK:
				self.file.write('  <div class="alert alert-success">\n')
				self.file.write('   <strong>Successful DsTester suite! <span class="glyphicon glyphicon-warning-sign"></span></strong>\n')
				self.file.write('  </div>\n')
			else:
				self.file.write('  <div class="alert alert-danger">\n')
				self.file.write('   <strong>Failed DsTester suite! <span class="glyphicon glyphicon-warning-sign"></span></strong>\n')
				self.file.write('  </div>\n')
		else:
			self.file.write('  <div class="alert alert-warning">\n')
			self.file.write('   <strong>LogFile not available! <span class="glyphicon glyphicon-warning-sign"></span></strong>\n')
			self.file.write('  </div>\n')
		return finalStatusOK

	def testSummaryFooter(self):
		self.file.write('  <br>\n')

	def testSummaryDetails(self):
		self.file.write('  <br>\n')
		self.file.write('  <button data-toggle="collapse" data-target="#ds-tester-details">More details on DsTester results</button>\n')
		self.file.write('  <div id="ds-tester-details" class="collapse">\n')
		self.file.write('  <table class="table-bordered" width = "60%" align = "center" border = 1>\n')
		self.file.write('    <tr bgcolor = "#33CCFF" >\n')
		self.file.write('      <th>Test Name</th>\n')
		self.file.write('      <th>Test Status</th>\n')
		self.file.write('      <th>Test Details</th>\n')
		self.file.write('    </tr>\n')
		cwd = os.getcwd()
		if os.path.isfile(cwd + '/DS-TEST-RESULTS/hcintegration.yaml'):
			with open(cwd + '/DS-TEST-RESULTS/hcintegration.yaml') as f:
				data = yaml.load(f)
				nScenarios = len(data['scenarios'])
				for scenario in range(nScenarios):
					self.file.write('     <tr>\n')
					self.file.write('      <td>' + data['scenarios'][scenario]['name'] + '</td>\n')
					if data['scenarios'][scenario]['result'] == 'fail':
						self.file.write('      <td bgcolor = "Red"><b><font color="white">KO</font></b></td>\n')
					elif data['scenarios'][scenario]['result'] == 'pass':
						self.file.write('      <td bgcolor = "DarkGreen"><b><font color="white">OK</font></b></td>\n')
					else:
						self.file.write('      <td bgcolor = "DarkOrange"><b><font color="white">UNKNOW</font></b></td>\n')
					testDetails = ''
					for x,y in data['scenarios'][scenario]['conditions'].items():
						testDetails += str(x) + ': ' + str(y) + '\n'
						#details += '\n'
					self.file.write('     <td><pre>' + testDetails + '</pre></td>\n')
					self.file.write('    </tr>\n')
		else:
			print ('no details???')
		self.file.write('  </table>\n')
		self.file.write('  </div>\n')

def Usage():
	print('----------------------------------------------------------------------------------------------------------------------')
	print('dsTestGenerateHTMLReport.py')
	print('   Generate an HTML report for the Jenkins pipeline on oai-cn5g-fed.')
	print('----------------------------------------------------------------------------------------------------------------------')
	print('Usage: python3 generateHtmlReport.py [options]')
	print('  --help  Show this help.')
	print('---------------------------------------------------------------------------------------------- Mandatory Options -----')
	print('  --job_name=[Jenkins Job name]')
	print('  --job_id=[Jenkins Job Build ID]')
	print('  --job_url=[Jenkins Job Build URL]')

#--------------------------------------------------------------------------------------------------------
#
# Start of main
#
#--------------------------------------------------------------------------------------------------------

argvs = sys.argv
argc = len(argvs)

HTML = HtmlReport()

while len(argvs) > 1:
	myArgv = argvs.pop(1)
	if re.match('^\-\-help$', myArgv, re.IGNORECASE):
		Usage()
		sys.exit(0)
	elif re.match('^\-\-job_name=(.+)$', myArgv, re.IGNORECASE):
		matchReg = re.match('^\-\-job_name=(.+)$', myArgv, re.IGNORECASE)
		HTML.job_name = matchReg.group(1)
	elif re.match('^\-\-job_id=(.+)$', myArgv, re.IGNORECASE):
		matchReg = re.match('^\-\-job_id=(.+)$', myArgv, re.IGNORECASE)
		HTML.job_id = matchReg.group(1)
	elif re.match('^\-\-job_url=(.+)$', myArgv, re.IGNORECASE):
		matchReg = re.match('^\-\-job_url=(.+)$', myArgv, re.IGNORECASE)
		HTML.job_url = matchReg.group(1)
	else:
		sys.exit('Invalid Parameter: ' + myArgv)

if HTML.job_name == '' or HTML.job_id == '' or HTML.job_url == '':
	sys.exit('Missing Parameter in job description')

HTML.generate()
