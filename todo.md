# TODO list

## 3D model
* add end-stops (or otherwise guarantee never going out of bounds, as it stalls engine and permanently locks it, requiring manual intervention)
  - this should be doable with potentiometer and moving slow ("calibration phase"?)

## electronics
* design I/O protocol
* choose I/O protocol:
  * request position
  * receive position
  * ping
  * monitor Vcc
  * monitor power consumption
* design PCB
  * add power measuring capability (up to 1A?)
    * sense resistor?
    * small sense resistor + OpAmp?
  * add voltage measuring capability (12V)
  * DRV8220 - design package

## software
* servo control SW
* control loop
* client library to use servo
