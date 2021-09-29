#!/bin/bash
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

function usage {
    echo "Openair-CN components synchronization"
    echo "   Original Author: Raphael Defosseux"
    echo ""
    echo "   Requirement: git shall be installed"
    echo ""
    echo "   By default (no options) all components will be synchronized to"
    echo "     the 'develop' branch."
    echo "   Each component can be synchronized a dedicated branch."
    echo ""
    echo "Usage:"
    echo "------"
    echo "    syncComponents.sh [OPTIONS]"
    echo ""
    echo "Options:"
    echo "--------"
    echo "    --nrf-branch ####"
    echo "    Specify the source branch for the OAI-NRF component"
    echo ""
    echo "    --amf-branch ####"
    echo "    Specify the source branch for the OAI-AMF component"
    echo ""
    echo "    --smf-branch ####"
    echo "    Specify the source branch for the OAI-SMF component"
    echo ""
    echo "    --spgwu-tiny-branch ####"
    echo "    Specify the source branch for the OAI-SPGW-U-TINY component"
    echo ""
    echo "    --help OR -h"
    echo "    Print this help message."
    echo ""
}

NRF_BRANCH='develop'
AMF_BRANCH='develop'
SMF_BRANCH='develop'
SPGWU_BRANCH='master'

doDefault=1

while [[ $# -gt 0 ]]
do
key="$1"

case $key in
    -h|--help)
    shift
    usage
    exit 0
    ;;
    --nrf-branch)
    NRF_BRANCH="$2"
    doDefault=0
    shift
    shift
    ;;
    --amf-branch)
    AMF_BRANCH="$2"
    doDefault=0
    shift
    shift
    ;;
    --smf-branch)
    SMF_BRANCH="$2"
    doDefault=0
    shift
    shift
    ;;
    --spgwu-tiny-branch)
    SPGWU_BRANCH="$2"
    doDefault=0
    shift
    shift
    ;;
    *)
    echo "Syntax Error: unknown option: $key"
    echo ""
    usage
    exit 1
esac

done

echo "---------------------------------------------------------"
echo "OAI-NRF    component branch : ${NRF_BRANCH}"
echo "OAI-AMF    component branch : ${AMF_BRANCH}"
echo "OAI-SMF    component branch : ${SMF_BRANCH}"
echo "OAI-SPGW-U component branch : ${SPGWU_BRANCH}"
echo "---------------------------------------------------------"

# First do a clean-up
echo "git submodule deinit --all --force"
git submodule deinit --all --force > /dev/null 2>&1

echo "git submodule init"
git submodule init > /dev/null 2>&1
echo "git submodule update"
git submodule update  > /dev/null 2>&1

if [ $doDefault -eq 1 ]
then
    git submodule foreach 'git fetch --prune && git checkout develop && git pull origin develop'  > /dev/null 2>&1
else
    pushd component/oai-nrf
    git fetch --prune > /dev/null 2>&1
    git checkout $NRF_BRANCH > /dev/null 2>&1
    git pull origin $NRF_BRANCH > /dev/null 2>&1
    popd
    pushd component/oai-amf
    git fetch --prune > /dev/null 2>&1
    git checkout $AMF_BRANCH > /dev/null 2>&1
    git pull origin $AMF_BRANCH > /dev/null 2>&1
    popd
    pushd component/oai-smf
    git fetch --prune > /dev/null 2>&1
    git checkout $SMF_BRANCH > /dev/null 2>&1
    git pull origin $SMF_BRANCH > /dev/null 2>&1
    popd
    pushd component/oai-upf-equivalent
    git fetch --prune > /dev/null 2>&1
    git checkout $SPGWU_BRANCH > /dev/null 2>&1
    git pull origin $SPGWU_BRANCH > /dev/null 2>&1
    popd
fi
