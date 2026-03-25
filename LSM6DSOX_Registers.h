#ifndef LSM6DSOX_REGISTERS_H
#define LSM6DSOX_REGISTERS_H

#include <stdlib.h>


#define LSM6DSOX_ADDRESS            0x6A

#define LSM6DSOX_WHO_AM_I_VALUE     0x6C
#define LSM6DSOX_WHO_AM_I_REG       0X0F

#define LSM6DSOX_GYRO_NC            0x01
#define LSM6DSOX_XL_NC              0x02

#define LSM6DSOX_CTRL1_XL           0X10
#define LSM6DSOX_CTRL2_G            0X11

#define LSM6DSOX_STATUS_REG         0X1E

#define LSM6DSOX_CTRL6_C            0X15
#define LSM6DSOX_CTRL7_G            0X16
#define LSM6DSOX_CTRL8_XL           0X17

#define LSM6DSOX_OUT_TEMP_L         0X20
#define LSM6DSOX_OUT_TEMP_H         0X21

#define LSM6DSOX_OUTX_L_G           0X22
#define LSM6DSOX_OUTX_H_G           0X23
#define LSM6DSOX_OUTY_L_G           0X24
#define LSM6DSOX_OUTY_H_G           0X25
#define LSM6DSOX_OUTZ_L_G           0X26
#define LSM6DSOX_OUTZ_H_G           0X27

#define LSM6DSOX_OUTX_L_XL          0X28
#define LSM6DSOX_OUTX_H_XL          0X29
#define LSM6DSOX_OUTY_L_XL          0X2A
#define LSM6DSOX_OUTY_H_XL          0X2B
#define LSM6DSOX_OUTZ_L_XL          0X2C
#define LSM6DSOX_OUTZ_H_XL          0X2D

#define LSM6DSOX_INT1_CTRL          0X0D //interrupt pin 1
#define LSM6DSOX_INT2_CTRL          0X0E //interrupt pin 2

#endif