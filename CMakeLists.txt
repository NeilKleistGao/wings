cmake_minimum_required(VERSION 3.15)
project(wings)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-D PLATFORM_LINUX)

include_directories(.)

add_executable(wings main.cc
        platforms/linux.h
        platforms/platforms.h
        socket/server_socket.h
        socket/server_socket.cc
        socket/socket.h
        socket/socket.cc socket/inet_address.h)