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

class HtmlReport():
	def __init__(self):
		self.job_name = ''
		self.job_id = ''
		self.job_url = ''
		self.job_start_time = 'TEMPLATE_TIME'
		self.amfConfigStatus = True
		self.smfConfigStatus = True
		self.upfConfigStatus = True

	def generate(self):
		cwd = os.getcwd()
		self.file = open(cwd + '/deploy_results_oai_cn5g.html', 'w')
		self.generateHeader()

		finalStatus = self.deploySummaryHeader()
		self.deploySummaryDetails()

		self.generateFooter()
		self.file.close()

		finalStatus = True
		if finalStatus:
			sys.exit(0)
		else:
			sys.exit(-1)

	def generateHeader(self):
		# HTML Header
		self.file.write('<!DOCTYPE html>\n')
		self.file.write('<html class="no-js" lang="en-US">\n')
		self.file.write('<head>\n')
		self.file.write('  <meta name="viewport" content="width=device-width, initial-scale=1">\n')
		self.file.write('  <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css">\n')
		self.file.write('  <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>\n')
		self.file.write('  <script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js"></script>\n')
		self.file.write('  <title>OAI Core Network Test Results for ' + self.job_name + ' job build #' + self.job_id + '</title>\n')
		self.file.write('</head>\n')
		self.file.write('<body><div class="container">\n')
		self.file.write('  <table width = "100%" style="border-collapse: collapse; border: none;">\n')
		self.file.write('   <tr style="border-collapse: collapse; border: none;">\n')
		self.file.write('	<td style="border-collapse: collapse; border: none;">\n')
		self.file.write('	  <a href="http://www.openairinterface.org/">\n')
		self.file.write('		 <img src="http://www.openairinterface.org/wp-content/uploads/2016/03/cropped-oai_final_logo2.png" alt="" border="none" height=50 width=150>\n')
		self.file.write('		 </img>\n')
		self.file.write('	  </a>\n')
		self.file.write('	</td>\n')
		self.file.write('	<td style="border-collapse: collapse; border: none; vertical-align: center;">\n')
		self.file.write('	  <b><font size = "6">Job Summary -- Job: ' + self.job_name + ' -- Build-ID: <a href="' + self.job_url + '">' + self.job_id + '</a></font></b>\n')
		self.file.write('	</td>\n')
		self.file.write('   </tr>\n')
		self.file.write('  </table>\n')
		self.file.write('  <br>\n')

	def generateFooter(self):
		self.file.write('  <div class="well well-lg">End of Test Report -- Copyright <span class="glyphicon glyphicon-copyright-mark"></span> 2020 <a href="http://www.openairinterface.org/">OpenAirInterface</a>. All Rights Reserved.</div>\n')
		self.file.write('</div></body>\n')
		self.file.write('</html>\n')

	def deploySummaryHeader(self):
		self.file.write('  <h2>Deployment Summary</h2>\n')

		finalStatusOK = True
		cwd = os.getcwd()
		# Checking AMF config log
		if os.path.isfile(cwd + '/archives/amf_config.log'):
			amfStatus = False
			with open(cwd + '/archives/amf_config.log','r') as amfLog:
				for line in amfLog:
					result = re.search('AMF Configuration Successful', line)
					if result is not None:
						amfStatus = True
			amfLog.close()
			if not amfStatus:
				finalStatusOK = False
		else:
			amfStatus = False
			finalStatusOK = False
		self.amfConfigStatus = amfStatus

		if os.path.isfile(cwd + '/archives/smf_config.log'):
			smfStatus = False
			with open(cwd + '/archives/smf_config.log','r') as smfLog:
				for line in smfLog:
					result = re.search('SMF Configuration Successful', line)
					if result is not None:
						smfStatus = True
			smfLog.close()
			if not smfStatus:
				finalStatusOK = False
		else:
			smfStatus = False
			finalStatusOK = False
		self.smfConfigStatus = smfStatus

		self.upfConfigStatus = True

		if finalStatusOK:
				self.file.write('  <div class="alert alert-success">\n')
				self.file.write('	<strong>Successful Deployment! <span class="glyphicon glyphicon-warning-sign"></span></strong>\n')
				self.file.write('  </div>\n')
		else:
				self.file.write('  <div class="alert alert-danger">\n')
				self.file.write('	<strong>Failed Deployment! <span class="glyphicon glyphicon-warning-sign"></span></strong>\n')
				self.file.write('  </div>\n')

		return finalStatusOK

	def deploySummaryDetails(self):
		self.file.write('  <br>\n')
		self.file.write('  <button data-toggle="collapse" data-target="#deploy-details">More details on deployment results</button>\n')
		self.file.write('  <br>\n')
		self.file.write('  <div id="deploy-details" class="collapse">\n')
		self.file.write('  <table class="table-bordered" width = "60%" align = "center" border = 1>\n')
		self.file.write('	 <tr bgcolor = "#33CCFF" >\n')
		if self.amfConfigStatus:
			self.file.write('	   <th>AMF Deployment OK</th>\n')
		else:
			self.file.write('	   <th>AMF Deployment KO</th>\n')
		if self.smfConfigStatus:
			self.file.write('	   <th>SMF Deployment OK</th>\n')
		else:
			self.file.write('	   <th>SMF Deployment KO</th>\n')
		if self.upfConfigStatus:
			self.file.write('	   <th>UPF Deployment OK</th>\n')
		else:
			self.file.write('	   <th>UPF Deployment KO</th>\n')
		self.file.write('	 </tr>\n')
		self.file.write('  </table>\n')
		self.file.write('  </div>\n')
		self.file.write('  <br>\n')


def Usage():
	print('----------------------------------------------------------------------------------------------------------------------')
	print('generateHtmlReportDeployment.py')
	print('   Generate an HTML report for the Jenkins pipeline on oai-cn5g-fed.')
	print('----------------------------------------------------------------------------------------------------------------------')
	print('Usage: python3 generateHtmlReportDeployment.py [options]')
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
