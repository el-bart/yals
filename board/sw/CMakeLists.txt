cmake_minimum_required(VERSION 3.20)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

include("Hal/${YALS_HAL}/common_pre.cmake")
project(yals C CXX ASM)
include("Hal/${YALS_HAL}/common_post.cmake")

add_compile_options(-Werror)
add_compile_options(-Wall)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/Hal/${YALS_HAL}/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/")

add_subdirectory("Hal/${YALS_HAL}")
add_subdirectory("Utils")

include("cmake/git.cmake")

add_library(servo_lib STATIC
  "${CMAKE_CURRENT_BINARY_DIR}/Utils/version.cpp"
)
# this dependency ensures that rebuild is triggered, whenever git commit info changes
add_dependencies(servo_lib version_proxy)

if ("${YALS_HAL}" STREQUAL "simulate")
  enable_testing()
  add_binary(uts main.ut.cpp
    Controller.ut.cpp
    Handler.ut.cpp
    Io/Buffer.ut.cpp
    Io/Line.ut.cpp
    Io/Proto/common.ut.cpp
    Io/checksum.ut.cpp
    Io/process.ut.cpp
    Utils/Engine_controller.ut.cpp
    sim.ut.cpp
  )
  target_link_libraries(uts hal)
  target_link_libraries(uts servo_lib)
  add_test(NAME uts COMMAND "${CMAKE_CURRENT_BINARY_DIR}/uts" --use-colour=yes)
endif()

add_binary(servo_ctrl servo_ctrl.cpp)
target_link_libraries(servo_ctrl servo_lib)
