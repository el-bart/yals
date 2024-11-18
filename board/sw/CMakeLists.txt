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

function (mtest name)
  set(mtname "mtest_${name}")
  add_executable("${mtname}" "mtest/${name}.cpp")
  target_link_libraries("${mtname}" ${HAL_LIBS} ${ARGN})
  # create map/bin/hex/uf2 file etc.
  pico_add_extra_outputs("${mtname}")
endfunction()

mtest(blink)
mtest(serial)
mtest(engine)
mtest(adc_vcc)
mtest(adc_eng_current)
mtest(adc_pos_feedback)
