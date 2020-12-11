cmake_minimum_required(VERSION 3.15)
project(wings)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-D PLATFORM_LINUX)

include(socket/CMakeLists.txt)
include(platforms/CMakeLists.txt)
include(exceptions/CMakeLists.txt)

include_directories(.)

add_executable(wings main.cc
        ${PLATFORM_HEADER}
        ${PLATFORM_SRC}
        ${SOCKET_HEADER}
        ${SOCKET_SRC}
        ${EXCEPTIONS_HEADER}
        ${EXCEPTIONS_SRC}
        wings.h)