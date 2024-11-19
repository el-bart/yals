# protocol definition

communication protocol definition, used to communicate with the YALS device.
it consists of 1-line commands and responses and ends with checksum.
line ends with a new-line character (`\n` or `\r`).

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
* OK: `+msg\n` where:
  * `msg` is a string containing basic device info (name, version, etc.). 32B max.
* error: `-msg\n` where:
  * `msg` is an error description string. 32B max.

### set servo position
#### request
```
@nnnXX\n
```
where:
* `nnn` is servo position in 000..999 range
* `XX` is hex-encoded checksum (or literal `XX` - meaning unset)
#### reply
* OK: `+\n`
* error: `-msg\n` where:
  * `msg` is a string containing basic device info (name, version, etc.). 32B max.

### get servo position
#### request
```
?@XX\n
```
where:
* `XX` is hex-encoded checksum (or literal `XX` - meaning unset)
#### reply
* OK: `+nnn\n` where:
  * `nnn` is a current 000..999 servo position
* error: `-msg\n` where:
  * `msg` is a string containing basic device info (name, version, etc.). 32B max.

### set min servo position
#### request
```
<nnnXX\n
```
where:
* `nnn` is 000...999 position
* `XX` is hex-encoded checksum (or literal `XX` - meaning unset)
#### reply
* OK: `+\n`
* error: `-msg\n` where:
  * `msg` is a string containing basic device info (name, version, etc.). 32B max.

### set max servo position
#### request
```
>nnnXX\n
```
where:
* `nnn` is 000...999 position
* `XX` is hex-encoded checksum (or literal `XX` - meaning unset)
#### reply
* OK: `+\n`
* error: `-msg\n` where:
  * `msg` is a string containing basic device info (name, version, etc.). 32B max.

### set LED brightness
#### request
```
*nnXX\n
```
where:
* `nn` is 00...99 LED brightness
* `XX` is hex-encoded checksum (or literal `XX` - meaning unset)
#### reply
* OK: `+\n`
* error: `-msg\n` where:
  * `msg` is a string containing basic device info (name, version, etc.). 32B max.

### get telemetry
#### request
```
?tXX\n
```
where:
* `XX` is hex-encoded checksum (or literal `XX` - meaning unset)
#### reply
* OK: `+IiiiiUuuuuu\n` where:
  * `iiii` is current engine current in `mA` (0000..9999); e.g. `1234` is 1.234 A.
  * `uuuuu` is current main voltage in `mV` (00000..99999); e.g. `12345` is 12.345 V.
* error: `-msg\n` where:
  * `msg` is a string containing basic device info (name, version, etc.). 32B max.

### get persistent configuration
#### request
```
?cXX\n
```
where:
* `XX` is hex-encoded checksum (or literal `XX` - meaning unset)
#### reply
* OK: `+<mmm>MMM*bb\n` where:
  * `mmm` is 000..999 servo min position
  * `MMM` is 000..999 servo max position
  * `bb` is 00..99 LED brightness
* error: `-msg\n` where:
  * `msg` is a string containing basic device info (name, version, etc.). 32B max.
