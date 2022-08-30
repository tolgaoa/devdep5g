cmake_minimum_required(VERSION 2.4)

project(tcpclient)

include_directories(${PROJECT_SOURCE_DIR})
set(SOURCES main.cpp ${PROJECT_SOURCE_DIR}/tcpclient.c)
add_executable(tcpclient ${SOURCES}) 
