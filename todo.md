# TODO list

## 3D model
* update model from coupler to universal joint
* add end-stops (or otherwise guarantee never going out of bounds, as it stalls engine and permanently locks it, requiring manual intervention)
* redesign to use MT44 engine instead

## electronics
* choose µC
* choose I/O protocol:
  * request position
  * receive position
  * ping
  * monitor Vcc
  * monitor power consumption
* design PCB
* design I/O [rptpcp;

## software
* servo control SW
* control loop
* client library to use servo
