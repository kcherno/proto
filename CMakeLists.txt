cmake_minimum_required(VERSION 3.27.0)

project(proto LANGUAGES CXX)

enable_testing()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD          20)

add_subdirectory(test)
