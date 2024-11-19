cmake_minimum_required(VERSION 3.20)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

include("Hal/${YALS_HAL}/common_pre.cmake")
project(yals C CXX ASM)
include("Hal/${YALS_HAL}/common_post.cmake")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/Hal/${YALS_HAL}/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/")

add_subdirectory("Hal/${YALS_HAL}")

add_library(servo_lib STATIC
  placeholder.cpp
)

if ("${YALS_HAL}" STREQUAL "simulate")
  enable_testing()
  add_binary(uts main.ut.cpp
    sim.ut.cpp
    Io/Frame.ut.cpp
  )
  target_link_libraries(uts hal)
  target_link_libraries(uts servo_lib)
  add_test(NAME uts COMMAND "${CMAKE_CURRENT_BINARY_DIR}/uts")
endif()

add_binary(servo_ctrl servo_ctrl.cpp)
target_link_libraries(servo_ctrl servo_lib)
