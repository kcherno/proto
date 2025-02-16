cmake_minimum_required(VERSION 3.27.0)

project(proto LANGUAGES CXX)

enable_testing()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD          20)

add_subdirectory(test)

add_library(proto STATIC
    ${CMAKE_SOURCE_DIR}/src/net/icmp_echo.cpp
    ${CMAKE_SOURCE_DIR}/src/net/netdevice.cpp
    ${CMAKE_SOURCE_DIR}/src/net/icmp.cpp
    ${CMAKE_SOURCE_DIR}/src/net/ipv4.cpp)

target_include_directories(proto PUBLIC
    ${CMAKE_SOURCE_DIR}/include)

set_target_properties(proto
    PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
