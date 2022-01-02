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

import re
import sys
import subprocess
import yaml
import os
import argparse

def main() -> None:
    """Provides the status of the test and copies the results"""
    args = _parse_args()

    if args.type == 'mini':
        path = '/RESULTS-MINI/dsTester_Summary_mini.txt'
        folder = 'RESULTS-MINI/'
    elif args.type == 'basic':
        path = '/RESULTS-BASIC/dsTester_Summary_basic.txt'
        folder = 'RESULTS-BASIC/'

    locexist = False
    cwd = os.getcwd()
    try:
        with open(cwd + path) as f:
            for line in f:
                if re.search('Result file is available here', str(line)):
                    result = re.search('(?:\/.+?\/)(.+?)(?:\/.+)', str(line))
                    if result:
                        result1 = re.search('^(.*/)([^/]*)$', str(result.group(0)))
                        filename = re.search('[^/]*$', str(result1.group(0))) 
                        subprocess.check_output(f'cp -r {result1.group(1)}* {folder}', stderr=subprocess.STDOUT, shell=True, universal_newlines=True)
                    locexist = True
    except IOError:
        sys.exit(f"File not accessible to check DSTester Summary: {path}")

    if locexist:
        try:
            with open(cwd + f'/{folder}{filename.group(0)}') as f:
                data = yaml.full_load(f)
                if data["final-result"] == 'fail':
                    sys.exit('DS Tester FrameWork final result FAILED')
        except IOError:
            sys.exit(f'File not accessible to check DSTester result: {folder}{filename.group(0)}')
    else:
        sys.exit('Result path is not Available on Console, Something wrong with DSTester')


def _parse_args() -> argparse.Namespace:
    """Parse the command line args

    Returns:
        argparse.Namespace: the created parser
    """
    parser = argparse.ArgumentParser(description='Check the status of 5GCN test with DSTester')

    # Type
    parser.add_argument(
        '--type',
        action='store',
        required=True,
        choices=['mini', 'basic'],
        help='Type of function',
    )
    return parser.parse_args()


if __name__ == '__main__':
    main()
