cmake_minimum_required(VERSION 3.23)
project(isec-iia-optimization-methods C)

set(CMAKE_C_STANDARD 99)

add_executable(isec-iia-optimization-methods algorithms/hill-climbing.c utils/utils.c)
