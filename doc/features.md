# features

## startup
* moving servo to within min..max range on init
* setting LED brightness

## protocol
* simple, ASCII-based serial protocol for I/O
* getting and setting:
  * servo position
  * min/max positions range for servo (preserved across restarts)
* readout:
  * SW information
  * telemetry (Vcc, engine current)

## misc
* hard min/max limits, compensating for potentiometer inaccuracies
