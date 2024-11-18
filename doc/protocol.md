# protocol definition

communication protocol definition, used to communicate with the YALS device.
it consists of multiple layers, namely:
- frame (sends data as bytes, in a pre-defined packages)
- payload (actual data)


## frame layer

each frame has a header, payload and checksum.
if checksum does not match, payload is dropped.

each byte of binary frame (header, payload, checksum) is encoded in ASCII-hex.

frame starts with a single `!` character.
frame ends with a single `\n` character.


### header

header is 1 byte long.
it's constructed as:
```
1vvrssss
```
where:
* `1` is a fixed bit set to `1` (indicating 1st byte of a frame)
* `vv` is 2 bit field representing version (`00` atm)
* `r` is 1 bit reserved field
* `ssss`is 4 bit payload size in user data bytes +1 (i.e. `0000` == 1B, `1111` == 16B)


### frame payload

each byte of payload transmitted in hex-encoded (i.e. 2 ASCII bytes), lower-case.


### checksum

checksum is calculated as a bit XOR of all bytes of the source frame.
checksum is hex-encoded


### example
for example to encode 3 bytes of user data: `0xFF 0x42 0x10`.
thus payload size is defined as `2` (i.e. 3 bytes).
binary value of header therefor is:
```
1000 0010
```
this makes for frame look like:
```
!82ff42102f\n
```
where `\n` is EOL character.

example program that generates such a sequence:
```
#include <cstdio>
#include <cstdint>
int main()
{
  uint8_t h = 0x82;
  uint8_t p[] = { 0xff, 0x42, 0x10 };
  uint8_t c = h ^ p[0] ^ p[1] ^ p[2];
  printf("!%02x%02x%02x%02x%02x\n", h, p[0], p[1], p[2], c);
}
```


## payload layer

this section defines higher-level protocol for communicating with YALS, in a form of logical frames.
i.e. here only payload of the frame is covered, by it's now split into fields.

first byte of each payload is 4-bit protocol version and 4-bit request/reply ID.
protocol version is binary `0000` here.

### set servo position
#### request
* ID: `0`.
* 1 byte servo position, where:
  * `0x00` is next to the engine (min left)
  * `0xff` is far away (max right)
#### reply
* ID: `0`.
* 1 byte confirming position set (i.e. the same as request)

### read servo position
#### request
* ID: `1`.
#### reply
* ID: `1`.
* 1 byte representing current servo position

### read status information
#### request
* ID: `2`.
#### reply
* ID: `2`.
* 2B of Vcc in mA
* 2B engine current in mA
* 1B of current servo position


### set LED brightness
#### request
* ID: `3`.
* 1B brightness setting (0..255 mapped to 0..100%)
#### reply
* ID: `3`.
* 1B brightness setting (0..255 mapped to 0..100%)
