#include <stdlib.h>
#include <thread>
#include <gpiod.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <lsm6dsox_reg.h>
#ifndef LSM6DSOX_H
#define LSM6DSOX_H

const int LMS6DSOX_DRDY_GPIO=22; // RPi physical pin 15

class LSM6DSOX{


    public:     
    LSM6DSOX(const std:string& _device="/dev/i2c-1", uint8_t _address=LSM6DSOX_ID):device(_device), address(_address){
        
    }

    ~LSM6DSOX();

    void start();

    void stop();

    void worker();

    private:
    std::string device;
    uint8_t address=0x0;//null
    std::thread thread;

    bool running=false;

    struct gpiod_chip *chipDRDY = nullptr;
    struct gpiod_line* pinDRDY=nullptr;    

   // int i2cOpen();
    
};
#endif