# TODO list

## 3D model
* add end-stops (or otherwise guarantee never going out of bounds, as it stalls engine and permanently locks it, requiring manual intervention)
  - this should be doable with potentiometer and moving slow ("calibration phase"?)

## electronics
* design PCB
  * add power measuring capability (up to 1A?)
    * sense resistor?
    * small sense resistor + OpAmp?

## software
* servo control SW
* control loop
* client library to use servo
* design I/O protocol:
  * request position
  * receive position
  * ping
  * monitor Vcc
  * monitor power consumption
