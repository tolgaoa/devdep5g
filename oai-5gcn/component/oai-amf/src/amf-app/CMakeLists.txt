################################################################################
# Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.
# The OpenAirInterface Software Alliance licenses this file to You under
# the OAI Public License, Version 1.1  (the "License"); you may not use this file
# except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.openairinterface.org/?page_id=698
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#-------------------------------------------------------------------------------
# For more information about the OpenAirInterface (OAI) Software Alliance:
#      contact@openairinterface.org
################################################################################

include_directories(${SRC_TOP_DIR}/../build/ext/spdlog/include)
include_directories(${SRC_TOP_DIR}/amf-app)
include_directories(${SRC_TOP_DIR}/itti)
include_directories(${SRC_TOP_DIR}/itti/msgs)
include_directories(${SRC_TOP_DIR}/common)
include_directories(${SRC_TOP_DIR}/common/unicode)
include_directories(${SRC_TOP_DIR}/contexts)
include_directories(${SRC_TOP_DIR}/nas/ies)
include_directories(${SRC_TOP_DIR}/nas/common)
include_directories(${SRC_TOP_DIR}/nas/msgs)
include_directories(${SRC_TOP_DIR}/nas/utils)
include_directories(${SRC_TOP_DIR}/ngap/libngap)
include_directories(${SRC_TOP_DIR}/ngap/ngap_app)
include_directories(${SRC_TOP_DIR}/ngap/ngapIEs)
include_directories(${SRC_TOP_DIR}/ngap/ngapMsgs)
include_directories(${SRC_TOP_DIR}/sbi/amf_server)
include_directories(${SRC_TOP_DIR}/sbi/amf_server/api)
include_directories(${SRC_TOP_DIR}/sbi/amf_server/impl)
include_directories(${SRC_TOP_DIR}/sbi/amf_server/model)
include_directories(${SRC_TOP_DIR}/sbi/smf_client)
include_directories(${SRC_TOP_DIR}/sbi/smf_client/api)
include_directories(${SRC_TOP_DIR}/sbi/smf_client/inputs)
include_directories(${SRC_TOP_DIR}/sbi/smf_client/model)
include_directories(${SRC_TOP_DIR}/sctp)
include_directories(${SRC_TOP_DIR}/secu_algorithms/5gaka)
include_directories(${SRC_TOP_DIR}/secu_algorithms/nas_enc_int)
include_directories(${SRC_TOP_DIR}/utils)
include_directories(${SRC_TOP_DIR}/utils/bstr)

file(GLOB AMF_src_files
  ${CMAKE_CURRENT_SOURCE_DIR}/amf_app.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/amf_config.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/amf_module_from_config.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/amf_n1.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/amf_n2.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/amf_n11.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/amf_profile.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/amf_statistics.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/mysql_db.cpp
  ${SRC_TOP_DIR}/nas/msgs/*.cpp
  ${SRC_TOP_DIR}/nas/ies/*.cpp
  ${SRC_TOP_DIR}/nas/utils/*.c
  ${SRC_TOP_DIR}/utils/*.cpp
  ${SRC_TOP_DIR}/utils/*.c
  ${SRC_TOP_DIR}/utils/bstr/*.c
  ${SRC_TOP_DIR}/ngap/libngap/*.c
  ${SRC_TOP_DIR}/ngap/ngap_app/*.cpp
  ${SRC_TOP_DIR}/ngap/ngapMsgs/*.cpp
  ${SRC_TOP_DIR}/ngap/ngapIEs/*.cpp   
)
 
add_library (AMF STATIC
${AMF_src_files}
)

