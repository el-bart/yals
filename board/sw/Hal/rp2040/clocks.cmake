# onboard's quartz
add_definitions(-DXOSC_KHZ=8000)

#$ /opt/pico/pico-sdk/src/rp2_common/hardware_clocks/scripts/vcocalc.py --input 8 80
#Requested: 80.0 MHz
#Achieved: 80.0 MHz
#REFDIV: 1
#FBDIV: 200 (VCO = 1600.0 MHz)
#PD1: 5
#PD2: 4
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
