cmake_minimum_required(VERSION 3.20)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

include("Hal/${YALS_HAL}/common_pre.cmake")
project(yals C CXX ASM)
include("Hal/${YALS_HAL}/common_post.cmake")

add_subdirectory("Hal/${YALS_HAL}")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/Hal/${YALS_HAL}/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/")

add_binary(servo_ctrl servo_ctrl.cpp)
