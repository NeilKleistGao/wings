cmake_minimum_required(VERSION 3.15)
project(wings)

set(CMAKE_CXX_STANDARD 17)

add_executable(wings main.cpp wing_tcp_server.h wing_tcp_client.h)