# LSM6DSOX for Raspberry Pi 5
C++ implementation of real-time reading from a LSM6DSOX IMU.

### Find device address
`sudo apt install i2c-tools`
`i2cdetect -y 1`
Address will be `0x+output`
