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
include_directories(${SRC_TOP_DIR}/common)
include_directories(${SRC_TOP_DIR}/common/unicode)
include_directories(${SRC_TOP_DIR}/contexts)
include_directories(${SRC_TOP_DIR}/nas/common)
include_directories(${SRC_TOP_DIR}/utils)
include_directories(${SRC_TOP_DIR}/utils/bstr)
include_directories(${SRC_TOP_DIR}/sbi/smf_client)
include_directories(${SRC_TOP_DIR}/sbi/smf_client/api)
include_directories(${SRC_TOP_DIR}/sbi/smf_client/model)

add_library (AMF_SBI_CLIENT STATIC
  ${CMAKE_CURRENT_SOURCE_DIR}/ApiClient.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ApiConfiguration.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ApiException.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/HttpContent.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/JsonBody.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ModelBase.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/MultipartFormData.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Object.cpp
 )

