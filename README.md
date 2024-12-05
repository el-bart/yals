# YALS: Yet Another Linear Servo

![YALS 1st prototype](doc/yals.jpg)

project to build a linear servo for RC models.


## key project assumptions

* long move range
* digital position control
* digital feedback on current servo position
* 12V power supply (directly from battery)
* does not consume power when not moving, while still keeping position
* possible magnetic coupling with element moved


## building

TL;DR just run `make` from project's root directory.

toolchain has been dockerized.
to make usage more convenient, there's a top-level `Makefile` to run all the elements.
it automatically keeps dependsncies in place.

generated stuff:
* 3D models for printing:
  * all in: `body/build/voron_24_revo/petg/strength/*.gcode`
  * note that there can be >1 carriage to choose from
* electronics (HW):
  * gerber files for PCB: `board/hw/build/gerber.zip`
  * PCB assembly: `board/hw/build/graphics/`
  * BOM: `board/hw/build/bom.csv`
* software (SW):
  * to flash use `./rp2020_sdk rp2040_flash <name_of_binary.elf>` from `board/sw` directory
  * binaries:
    * controller to flash (production): `board/sw/build/rp2040/servo_ctrl.elf`
    * manual tests (for validation of HW assembly):
      * `board/sw/build/rp2040/Hal/rp2040/Hal_*.mtest.elf`
      * `board/sw/build/rp2040/Utils/*.mtest.elf`


## assembly

### BOM
* 1x PCB assembly (see `baord/hw` - build it to get `board/hw/build/bom.csv`)
* 1x MT44 DC motor
* 2x 3x7x3mm ball bearing
* 1x 3mm-3mm universal joing (aka: Cardan joint)
* 1x 3mm threaded rod (~12cm)
* 3x 3mm threaded inset (1x for carriage and 2x for engine)
* 2x M3x14 screws (engine mount)
* 2x M3x10 screws (potentiometer mount)

### tools
* soldering iron
* 3D printer for YALS' body
* hex-key set
* pico-probe or raspberry pi SBC to flash `rp2040` µC
