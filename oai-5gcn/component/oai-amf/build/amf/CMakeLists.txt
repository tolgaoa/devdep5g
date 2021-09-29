cmake_minimum_required (VERSION 3.0.2)
project(OpenAirInterface)
# Override options for AMF
set (  PACKAGE_NAME                    "AMF" )
set (  STATIC_LINKING                   False )
#############################################
# Base directories, compatible with legacy OAI building
################################################
set (OPENAIRCN_DIR   $ENV{OPENAIRCN_DIR})
set (BUILD_TOP_DIR   ${OPENAIRCN_DIR}/build)
set (SRC_TOP_DIR     $ENV{OPENAIRCN_DIR}/src)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../src/oai-amf/CMakeLists.txt)
