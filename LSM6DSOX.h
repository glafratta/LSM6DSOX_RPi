#include <stdlib.h>
#include <thread>
#include <gpiod.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include "lsm6dsox_reg.h"
#include "LSM6DSOXSample.h"
#include <string>
#ifndef LSM6DSOX_H
#define LSM6DSOX_H

const unsigned int LMS6DSOX_DRDY_GPIO=22; // RPi physical pin 15

class LSM6DSOX{


    public:     
    LSM6DSOX(const std::string& _device="/dev/i2c-1", uint8_t _address=LSM6DSOX_ID):device(_device), address(_address){}

    ~LSM6DSOX();

    void start();

    void stop();

    void worker();

    struct LSM6DSOXCallback{ //abstract
        virtual void hasSample(LSM6DSOXSample sample)=0; 
    };

    private:
    std::string device; //device file
    uint8_t address=0;// chip address
    std::thread thread;

    bool running=false;

    /**
    * v1 defs for reference
    */
    struct gpiod_chip *chipDRDY = nullptr;
    // struct gpiod_line* pinDRDY=nullptr;    

    struct gpiod_chip *chip=nullptr;
    struct gpiod_line_settings *settings=nullptr;
    struct gpiod_line_config *line_cfg=nullptr;
    struct gpiod_request_config *req_cfg=nullptr;
    struct gpiod_line_request *request=nullptr;


   // int i2cOpen();
    
};
#endif