# TODO list

## 3D model
* add end-stops (or otherwise guarantee never going out of bounds, as it stalls engine and permanently locks it, requiring manual intervention)
  - this should be doable with potentiometer and moving slow ("calibration phase"?)

## board
* silkscreen:
  * describe each SWD pin
  * add board version
* pull-up `picorun` to Vcc
* add 3.3V in next to SWD (for ease of programming w/o external power supply)
* consider switching to hand-soldering pads for elements

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
