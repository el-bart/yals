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

#include_directories(/opt/pico/pico-sdk/src/rp2_common/hardware_clocks/include/)             
#include_directories(/opt/pico/pico-sdk/src/rp2040/hardware_structs/include/)                
#list(APPEND PICO_CONFIG_HEADER_FILES /home/el-bart/prjs/yals/board/sw/yals_board.h)     

add_definitions(-DXOSC_KHZ=8000)

# $ /opt/pico/pico-sdk/src/rp2_common/hardware_clocks/scripts/vcocalc.py --input 8 125
#Requested: 125.0 MHz
#Achieved: 125.0 MHz
#REFDIV: 1
#FBDIV: 125 (VCO = 1000.0 MHz)
#PD1: 4
#PD2: 2
add_definitions(-DSYS_CLK_KHZ=80000)
add_definitions(-DPLL_SYS_VCO_FREQ_KHZ=1600000)
add_definitions(-DPLL_SYS_POSTDIV1=5)
add_definitions(-DPLL_SYS_POSTDIV2=4)

# $ /opt/pico/pico-sdk/src/rp2_common/hardware_clocks/scripts/vcocalc.py --input 8 48
#Requested: 48.0 MHz
#Achieved: 48.0 MHz
#REFDIV: 1
#FBDIV: 180 (VCO = 1440.0 MHz)
#PD1: 6
#PD2: 5
add_definitions(-DPLL_USB_VCO_FREQ_KHZ=1440000)
add_definitions(-DPLL_USB_POSTDIV1=6)
add_definitions(-DPLL_USB_POSTDIV2=5)

pico_sdk_init()

add_executable(mtest_blink mtest/blink.cpp)
target_link_libraries(mtest_blink pico_stdlib)
# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(mtest_blink)
