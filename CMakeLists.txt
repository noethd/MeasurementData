cmake_minimum_required(VERSION 3.25)
project(md C)

set(CMAKE_C_STANDARD 11)

add_executable(md main.c MeasurementData.c MeasurementData.h)
