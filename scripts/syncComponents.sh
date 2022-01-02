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
    echo "     the 'master' branch."
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
    echo "    --ausf-branch ####"
    echo "    Specify the source branch for the OAI-AUSF component"
    echo ""
    echo "    --udm-branch ####"
    echo "    Specify the source branch for the OAI-UDM component"
    echo ""
    echo "    --udr-branch ####"
    echo "    Specify the source branch for the OAI-UDR component"
    echo ""
    echo "    --upf-vpp-branch ####"
    echo "    Specify the source branch for the OAI-UPF-VPP component"
    echo ""
    echo "    --help OR -h"
    echo "    Print this help message."
    echo ""
}

NRF_BRANCH='master'
AMF_BRANCH='master'
SMF_BRANCH='master'
SPGWU_BRANCH='master'
AUSF_BRANCH='master'
UDM_BRANCH='master'
UDR_BRANCH='master'
UPF_VPP_BRANCH='master'

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
    --ausf-branch)
    AUSF_BRANCH="$2"
    doDefault=0
    shift
    shift
    ;;
    --udm-branch)
    UDM_BRANCH="$2"
    doDefault=0
    shift
    shift
    ;;
    --udr-branch)
    UDR_BRANCH="$2"
    doDefault=0
    shift
    shift
    ;;
    --upf-vpp-branch)
    UPF_VPP_BRANCH="$2"
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
echo "OAI-NRF     component branch : ${NRF_BRANCH}"
echo "OAI-AMF     component branch : ${AMF_BRANCH}"
echo "OAI-SMF     component branch : ${SMF_BRANCH}"
echo "OAI-SPGW-U  component branch : ${SPGWU_BRANCH}"
echo "OAI-AUSF    component branch : ${AUSF_BRANCH}"
echo "OAI-UDM     component branch : ${UDM_BRANCH}"
echo "OAI-UDR     component branch : ${UDR_BRANCH}"
echo "OAI-UPF-VPP component branch : ${UPF_VPP_BRANCH}"
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
    git submodule foreach 'git fetch --prune && git branch -D master&& git checkout -b master origin/master' > /dev/null 2>&1
else
    pushd component/oai-nrf
    git fetch --prune > /dev/null 2>&1
    git branch -D $NRF_BRANCH > /dev/null 2>&1
    if [[ $? -ne 0 ]]; then
        git checkout $NRF_BRANCH > /dev/null 2>&1
    else
        git checkout -b $NRF_BRANCH origin/$NRF_BRANCH > /dev/null 2>&1
    fi
    popd
    pushd component/oai-amf
    git fetch --prune > /dev/null 2>&1
    git branch -D $AMF_BRANCH > /dev/null 2>&1
    if [[ $? -ne 0 ]]; then
        git checkout $AMF_BRANCH > /dev/null 2>&1
    else
        git checkout -b $AMF_BRANCH origin/$AMF_BRANCH > /dev/null 2>&1
    fi
    popd
    pushd component/oai-smf
    git fetch --prune > /dev/null 2>&1
    git branch -D $SMF_BRANCH > /dev/null 2>&1
    if [[ $? -ne 0 ]]; then
        git checkout $SMF_BRANCH > /dev/null 2>&1
    else
        git checkout -b $SMF_BRANCH origin/$SMF_BRANCH > /dev/null 2>&1
    fi
    popd
    pushd component/oai-upf-equivalent
    git fetch --prune > /dev/null 2>&1
    git branch -D $SPGWU_BRANCH > /dev/null 2>&1
    if [[ $? -ne 0 ]]; then
        git checkout  $SPGWU_BRANCH > /dev/null 2>&1
    else
        git checkout -b $SPGWU_BRANCH origin/$SPGWU_BRANCH > /dev/null 2>&1
    fi
    popd
    pushd component/oai-ausf
    git fetch --prune > /dev/null 2>&1
    git branch -D $AUSF_BRANCH > /dev/null 2>&1
    if [[ $? -ne 0 ]]; then
        git checkout $AUSF_BRANCH > /dev/null 2>&1
    else
        git checkout -b $AUSF_BRANCH origin/$AUSF_BRANCH > /dev/null 2>&1
    fi
    popd
    pushd component/oai-udm
    git fetch --prune > /dev/null 2>&1
    git branch -D $UDM_BRANCH > /dev/null 2>&1
    if [[ $? -ne 0 ]]; then
        git checkout $UDM_BRANCH > /dev/null 2>&1
    else
        git checkout -b $UDM_BRANCH origin/$UDM_BRANCH > /dev/null 2>&1
    fi
    popd
    pushd component/oai-udr
    git fetch --prune > /dev/null 2>&1
    git branch -D $UDR_BRANCH > /dev/null 2>&1
    if [[ $? -ne 0 ]]; then
        git checkout $UDR_BRANCH > /dev/null 2>&1
    else
        git checkout -b $UDR_BRANCH origin/$UDR_BRANCH > /dev/null 2>&1
    fi
    popd
    pushd component/oai-upf-vpp
    git fetch --prune > /dev/null 2>&1
    git branch -D $UPF_VPP_BRANCH > /dev/null 2>&1
    if [[ $? -ne 0 ]]; then
        git checkout $UPF_VPP_BRANCH > /dev/null 2>&1
    else
        git checkout -b $UPF_VPP_BRANCH origin/$UPF_VPP_BRANCH > /dev/null 2>&1
    fi
    popd
fi
