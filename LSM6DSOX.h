#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <gpiod.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
//#include <i2c/smbus.h> //system management bus
#include <fcntl.h>
// #include "lsm6dsox_reg.h"
#include "LSM6DSOXSample.h"
#include "LSM6DSOX_Registers.h"
#include <string>
#ifndef LSM6DSOX_H
#define LSM6DSOX_H

const unsigned int LMS6DSOX_DRDY_GPIO=17; // RPi physical pin 15, connected to data ready pin on LSM
const char * DRDY_CHIP="/dev/gpiochip0";
extern const bool DEBUG;
class LSM6DSOX{

    public:     
    LSM6DSOX(const std::string& _device="/dev/i2c-1", uint8_t _address=LSM6DSOX_ADDRESS):device(_device), address(_address){}

    ~LSM6DSOX(){}

    /**
    * @brief Opens gpio chip, sets gpio settings, line config, request config and requests, starts thread running the busy loop
    */
    void start();

    /**
    @brief Terminates thread, releases lines
    */
    void stop();

    /**
    * @brief Busy loop, waits for interrupt on data ready pin
    */
    void worker();

    struct LSM6DSOXCallback{ //abstract
        virtual void hasSample(LSM6DSOXSample sample)=0; 
    };

    protected:
    std::string device; //device file
    uint8_t address=0;// chip address
    std::thread thread;
    GyroSettings gyroSettings{};
    XLSettings xlSettings{};

    uint8_t running=false;

    /**
    * v1 defs for reference
    */
    struct gpiod_chip *chipDRDY = nullptr;
    // struct gpiod_line* pinDRDY=nullptr;    

    // maybe move all this into start as not all are retained
    struct gpiod_line_settings *settings=nullptr;
    struct gpiod_line_config *line_cfg=nullptr;
    struct gpiod_request_config *req_cfg=nullptr;
    struct gpiod_line_request *request=nullptr;

    /**
    * @brief Reads from accelerometer/gyroscope/temperature sensor registers
    */
    int getData();

    GyroscopeData readGyro();

    /**
    * @brief reads bytes contiguously from the given address
    * @param address starting address
    * @param container array to which values will be written
    * @param nBytes number of bytes to read
    */
    void contiguousReadBytes(uint8_t address, uint8_t * container, uint8_t nBytes);

    friend class LSM6DSOXTest; //for unit testing

    /**
    * @brief Code for "gpiod_line_request_wait_edge_events" 
    * -1: wait indefinitely
    * 0: return immediately
    */
    int wait_line=-1; 

    /**
    * @brief Opens i2c device at a certain @param address. Throws exceptions
    * Address is the chip address!!! 
    */
    int i2cOpen(int address);

    /**
    @brief Writes one byte of @param data from register at @param address
    */
    void i2cWriteByte(uint8_t address, uint8_t data);

    /**
    * @brief Reads one byte from register at @param address
    */
    uint8_t i2cReadByte(uint8_t address);

    /**
    * @brief Enable gyroscope
    */
    void initGyro();

    /**
    * @brief Enable accelerometer
    */
    void initAccelerometer();

    /**
    * @brief throws a different exception based on what's in the status register
    */
    const char* throwStatus(uint8_t statusByte);

};
#endif