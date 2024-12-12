# release notes

## dev

## v0.4
* added screw holes to mount servo to the external element
* persisting values in EEPROM is now operational
* added full sanitization of the EEPROM-read values
* EEPROM writes simplified

## v0.3
* saves current when servo is at the destination
* software implementation:
  * PID-based control loop
  * compact, ASCII-based communication protocol
    * getting and setting sevo position
  * Watchdog support
  * Serial I/O (UART) with buffering
  * 250Hz control loop (I/O is handled real time)
  * hardware-based limits for travel movement, to avoid hitting hard stops
  * feedback for:
    * supply voltage (Vcc)
    * current consumption from engine
  * LED dimming
* docker-based SDK for:
  * 3D body (OpenSCAD)
  * electronics (KiCad)
  * software (C++)

## v0.2
* new body designed around `MT44` engine
* electronics design
* first PCB version sent for manufacturing
* designed so that it's not back-drivable

## v0.1
* 3D model for body (designed for `MT83` engine)
