#pragma once
/*
$ /opt/pico/pico-sdk/src/rp2_common/hardware_clocks/scripts/vcocalc.py --input 8 125
Requested: 125.0 MHz
Achieved: 125.0 MHz
REFDIV: 1
FBDIV: 125 (VCO = 1000.0 MHz)
PD1: 4
PD2: 2
*/

#define XOSC_MHZ 8u

/*
#define PLL_COMMON_REFDIV 1

#define PLL_SYS_VCO_FREQ_KHZ (100u*1000u)
#define PLL_SYS_POSTDIV1 4
#define PLL_SYS_POSTDIV2 2

#define PLL_USB_VCO_FREQ_KHZ (48u*1000u)
#define PLL_USB_POSTDIV1 6
#define PLL_USB_POSTDIV2 5
*/

//#include "hardware/clocks.h"
//#include "/opt/pico/pico-sdk/src/rp2_common/hardware_clocks/include/hardware/clocks.h"
