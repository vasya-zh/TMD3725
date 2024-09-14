# TMD3725 Arduino library

Arduino library for TMD37253 and TMD37253M RGB color sensor with proximity and ambient light functions

[TM3725 datasheet](https://look.ams-osram.com/m/6a4d0816b7d3a4bf/original/TMD3725-ALS-Color-and-Proximity-Sensor-Module.pdf)

![TMD3725 internal diagram](img/TMD3725_diagram.webp | width=100)

## Functions

See [TMD3725](src/TMD3725.h) code comments for function description

## Examples

* TMD3725_basic.ino - basic color reading in a loop example
* TCA9544_TMD3725.ino - addressable example for several color sensors with TCA9544A multiplexor (cause TMD3725 does not have address setting feature)

## Serial monitor output
```
{"timestamp":"477573","hue":"60","saturation":"1","value":"0",}
REG[35]: 07 00 1F 00 00 00 00 00 00 00 00 80 4F 80 01 00 E4 30 01 00 01 00 01 00 00 00 04 04 0C 00 00 00 50 00 00 
CLR   RED   GRN   BLU   PRX 
01 00 01 00 01 00 00 00 05 
|CLEAR   |RED     |GREEN   |BLUE    |LUX     |CCT     |
|1       |1       |1       |0       |17      |1804    |
|HUE     |SAT     |VUE      |
|60      |1       |0       |
```

## Tested on devices

* [Arduino Nano - ATmega328P](https://docs.arduino.cc/hardware/nano/)
* [Raspberry Pi Pico - RP2040](https://www.raspberrypi.com/products/raspberry-pi-pico/)