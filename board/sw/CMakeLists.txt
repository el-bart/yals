cmake_minimum_required(VERSION 3.20)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

if (DEFINED ENV{PICO_SDK_PATH})
  set(PICO_SDK_PATH "$ENV{PICO_SDK_PATH}")
else()
  set(PICO_SDK_PATH "/opt/pico/pico-sdk")
endif()

set(PICO_BOARD pico_w CACHE STRING "Board type")

include("${PICO_SDK_PATH}/external/pico_sdk_import.cmake")

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.5.1")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.5.1 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

project(yals C CXX ASM)

include(clocks.cmake)
pico_sdk_init()

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

set(HAL_LIBS ${HAL_LIBS} pico_stdlib)
set(HAL_LIBS ${HAL_LIBS} hardware_adc)
set(HAL_LIBS ${HAL_LIBS} hardware_pwm)


function (add_binary name)
  add_executable("${name}" ${ARGN})
  target_link_libraries("${name}" ${HAL_LIBS})
  # create map/bin/hex/uf2 file etc.
  pico_add_extra_outputs("${name}")
endfunction()

add_binary(servo_ctrl servo_ctrl.cpp)


function (add_mtest name)
  add_binary("mtest_${name}" "mtest/${name}.cpp")
endfunction()

add_mtest(blink)
add_mtest(serial)
add_mtest(engine)
add_mtest(adc_vcc)
add_mtest(adc_eng_current)
add_mtest(adc_pos_feedback)
