# protocol definition

communication protocol definition, used to communicate with the YALS device.
it consists of 1-line commands and responses and ends with checksum.
line ends with a new-line character (`\n` or `\r`).

consecutive new-line characters are ignored.

despite of being ASCII-printable, for ease of parsing, each command is fixed-width.
this means that values might need to be `0`-padded.


## checksum

checksum is calculated as a bit XOR of all bytes of the source frame.
checksum is hex-encoded.

example checksum of 3 bytes of data (here as hex): `0xFF 0x42 0x10`  is equal to:
`0xFF ^ 0x42 ^ 0x10`.
note that new-line characters are not part of checksum.

checksum is optional.
a literal string `XX` can be used to indicate it's not set and should not be checked.
this is mostly for debugging purposes - it should never be used in production!


## protocol

### ping
#### request
```
~XX\n
```
where:
* `XX` is hex-encoded checksum (or literal `XX` - meaning unset)
#### reply
* OK: `+msgXX\n` where:
  * `msg` is a string containing basic device info (name, version, etc.). 30B max.
  * `XX` is hex-encoded checksum
* error: `-msgXX\n` where:
  * `msg` is an error description string. 30B max.
  * `XX` is hex-encoded checksum

### set servo position
#### request
```
@nnnXX\n
```
where:
* `nnn` is servo position in 000..999 range
* `XX` is hex-encoded checksum (or literal `XX` - meaning unset)
#### reply
* OK: `+XX\n`
  * `XX` is hex-encoded checksum
* error: `-msgXX\n` where:
  * `msg` is a string containing basic device info (name, version, etc.). 30B max.
  * `XX` is hex-encoded checksum

### get servo position
#### request
```
!XX\n
```
where:
* `XX` is hex-encoded checksum (or literal `XX` - meaning unset)
#### reply
* OK: `+nnnXX\n` where:
  * `nnn` is a current 000..999 servo position
  * `XX` is hex-encoded checksum
* error: `-msgXX\n` where:
  * `msg` is a string containing basic device info (name, version, etc.). 30B max.
  * `XX` is hex-encoded checksum

### set min servo position
#### request
```
<nnnXX\n
```
where:
* `nnn` is 000...999 position
* `XX` is hex-encoded checksum (or literal `XX` - meaning unset)
#### reply
* OK: `+XX\n`
  * `XX` is hex-encoded checksum
* error: `-msgXX\n` where:
  * `msg` is a string containing basic device info (name, version, etc.). 30B max.
  * `XX` is hex-encoded checksum

### set max servo position
#### request
```
>nnnXX\n
```
where:
* `nnn` is 000...999 position
* `XX` is hex-encoded checksum (or literal `XX` - meaning unset)
#### reply
* OK: `+XX\n`
  * `XX` is hex-encoded checksum
* error: `-msgXX\n` where:
  * `msg` is a string containing basic device info (name, version, etc.). 30B max.
  * `XX` is hex-encoded checksum

### set LED brightness
#### request
```
*nnXX\n
```
where:
* `nn` is 00...99 LED brightness
* `XX` is hex-encoded checksum (or literal `XX` - meaning unset)
#### reply
* OK: `+XX\n`
  * `XX` is hex-encoded checksum
* error: `-msgXX\n` where:
  * `msg` is a string containing basic device info (name, version, etc.). 30B max.
  * `XX` is hex-encoded checksum

### get telemetry
#### request
```
#XX\n
```
where:
* `XX` is hex-encoded checksum (or literal `XX` - meaning unset)
#### reply
* OK: `+IiiiiUuuuuuXX\n` where:
  * `iiii` is current engine current in `mA` (0000..9999); e.g. `1234` is 1.234 A.
  * `uuuuu` is current main voltage in `mV` (00000..99999); e.g. `12345` is 12.345 V.
  * `XX` is hex-encoded checksum
* error: `-msgXX\n` where:
  * `msg` is a string containing basic device info (name, version, etc.). 30B max.
  * `XX` is hex-encoded checksum

### get persistent configuration
#### request
```
?XX\n
```
where:
* `XX` is hex-encoded checksum (or literal `XX` - meaning unset)
#### reply
* OK: `+<mmm>MMM*bbXX\n` where:
  * `mmm` is 000..999 servo min position
  * `MMM` is 000..999 servo max position
  * `bb` is 00..99 LED brightness
  * `XX` is hex-encoded checksum
* error: `-msgXX\n` where:
  * `msg` is a string containing basic device info (name, version, etc.). 30B max.
  * `XX` is hex-encoded checksum


### cheatsheet

this section is a brief summary of the whole section.
it's based on examples with 1-liner explanation.

all `XX` characters should be replaced with checksum, or can be taken literally (i.e. no checksum).
replies also ommit checksums.

* ping / get version info:
  * request: `~XX\n`
  * reply: `YALS v1.2.3-42-abcedfXX\n`
* set servo position to `98`
  * request: `@098XX\n`
  * reply: `+XX\n`
* return servo position (set to `098`)
  * request: `!XX\n`
  * reply: `+098XX\n`
* set servo minimum position to `200`:
  * request: `<200XX\n`
  * reply: `+XX\n`
* set servo maximum position to `800`:
  * request: `>800XX\n`
  * reply: `+XX\n`
* set LED birghtness to `42`:
  * request: `*42XX\n`
  * reply: `+XX\n`
* get telemetry:
  * request: `#XX\n`
  * reply: `+I01234U12345XX\n` (i.e. 0.1234 A and 12.345 V
* get persisted configuration:
  * request: `?XX\n`
  * reply: `+<200>800*42XX\n` (i.e. pos limits are 200..800, LED brightness is 42)
