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
import argparse

class IpRouteCheck():
	def __init__(self):
		self.mode = ''
		self.userName = ''
		self.hostName = ''
		self.file = ''
		self.subnet = ''
		self.gatwayIp = ''
		self.interfaceName = ''

	def _parse_args(self) -> argparse.Namespace:
		"""Parse the command line args

		Returns:
			argparse.Namespace: the created parser
		"""
		parser = argparse.ArgumentParser(description='Route Add/Delete on the required Server for CI Deployment')

		# Mode
		parser.add_argument(
			'--mode',
			action='store',
			required=True,
			choices=['Add', 'Delete'],
			help='route Add/Delete',
		)
		# Server username
		parser.add_argument(
			'--userName',
			action='store',
			required=True,
			help='server username',
		)
		# Server hostname
		parser.add_argument(
			'--hostName',
			action='store',
			required=True,
			help='server hostname',
		)
		# Docker-compose file to use
		parser.add_argument(
			'--docker_compose',
			action='store',
			required=True,
			help='Docker-compose file to use',
		)
		return parser.parse_args()

	def getSubnet(self):
		cmd = f"egrep 'subnet' {self.file}"
		ret = subprocess.run(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, encoding='utf-8')
		if ret.returncode == 0:
			if ret.stdout is not None:
				result = re.search("(?:[0-9]{1,3}[.]){3}[0-9]{1,3}/[0-9]{1,2}", ret.stdout.strip())
				if result is not None:
					self.subnet = result.group(0)
					#print("found subnet:", self.subnet)
			else:
				print("subnet not found in docker compose")
				sys.exit(-1)
		else:
			print("docker-compose file not found")
			sys.exit(-1)

	def getGatwayIp(self):
		cmd = "ifconfig | grep 192.168.18"
		ret = subprocess.run(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, encoding='utf-8')
		if ret.returncode == 0:
			result = re.search("inet (?:[0-9]{1,3}[.]){3}[0-9]{1,3}", ret.stdout.strip())
			self.gatwayIp = result.group(0)
			#print("Gatway IP Address:", self.gatwayIp)
		else:
			print("No Gatway IP address starts with 196.168.18")
			sys.exit(-1)	

	def routeCheck(self):
		# Get the interface name
		cmd = "ip route | grep 192.168.18.0/24 | awk {'print $3'}"
		ret = subprocess.run(f'ssh {self.userName}@{self.hostName} {cmd} || true', shell=True, stdout=subprocess.PIPE, encoding='utf-8')
		#print('interface name:', ret.stdout.strip())
		self.interfaceName = ret.stdout.strip()
		# Check whether or not ip route exist 
		cmd = f'ip route | grep -c "{self.subnet}"'
		ret = subprocess.run(f'ssh {self.userName}@{self.hostName} {cmd} || true', shell=True, stdout=subprocess.PIPE, encoding='utf-8')
		if ret.stdout is not None:
			if ret.stdout.strip() == '1':
				#print('Route exits')
				if IPRC.mode == 'Delete':
					IPRC.routeDel()
				else:
					sys.exit(0)
			else:
				#print("Route not found")
				if IPRC.mode == 'Add':
					IPRC.routeAdd()
				else:
					sys.exit(0)

	def routeAdd(self):
		# Add the route
		cmd = f"sudo ip route add {self.subnet} via {self.gatwayIp} dev {self.interfaceName}"
		ret = subprocess.run(f'ssh {self.userName}@{self.hostName} {cmd} || true', shell=True, stdout=subprocess.PIPE, encoding='utf-8')
		print("Added ip route")

	def routeDel(self):
		# Delete the route
		cmd = f"sudo ip route del {self.subnet} via {self.gatwayIp} dev {self.interfaceName}"
		ret = subprocess.run(f'ssh {self.userName}@{self.hostName} {cmd} || true', shell=True, stdout=subprocess.PIPE, encoding='utf-8')
		print("Deleted ip route")
	
#--------------------------------------------------------------------------------------------------------
#
# Start of main
#
#--------------------------------------------------------------------------------------------------------

IPRC = IpRouteCheck()
args = IPRC._parse_args()

IPRC.mode = args.mode
IPRC.userName = args.userName
IPRC.hostName = args.hostName
IPRC.file = args.docker_compose

IPRC.getSubnet()
IPRC.getGatwayIp()
IPRC.routeCheck()