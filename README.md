# LSM6DSOX for Raspberry Pi 5
C++ implementation of real-time reading from a LSM6DSOX IMU.

## Requirements

Raspberry Pi with Debian Trixie OS, gpiod library version 2

`sudo apt-get install libgpiod cmake libgtest-dev`

## Connect Pi 5 to LSM6DSOX
* GPIO17 (physical pin 11 on Pi) ->I1 (data ready pin 1)
* 3V (physical pin 1 on Pi) -> VDD
* GND -> GND
* SDA (physical pin 3) -> SDA
* SCL (physical pin 5) -> SCL

## Compile
`cmake .`

`make`

## Run
`ctest` : run unit tests
`./test/main` : runs simple example program printing values on screen

<!-- ### Find device address
`sudo apt install i2c-tools`
`i2cdetect -y 1`
Address will be `0x+output` -->
