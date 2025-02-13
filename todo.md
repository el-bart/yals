# TODO list

## 3D model
* add end-stops (or otherwise guarantee never going out of bounds, as it stalls engine and permanently locks it, requiring manual intervention)
  - this should be doable with potentiometer and moving slow ("calibration phase"?)
* add "left" and "right" visual markers on servo edges, to signal cardinal directions

## board
* silkscreen:
  * describe each SWD pin
  * add board version
* pull-up `picorun` to Vcc
* add 3.3V in next to SWD (for ease of programming w/o external power supply)
* consider switching to hand-soldering pads for elements
* add polymer fuse for power line
* consider using JST XH2.5 for power and communication (1 plug, 4 pins?) and soldering engine wires directly

## software
* native client library to use servo with demo using serial port
* LED blinking when I/Oing (on command level)
* PID controller:
  * fine-tune PID to avoid overshoots when traveling long distance
  * make PID default for full range of travel

## bugs:
