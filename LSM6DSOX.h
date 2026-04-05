#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <gpiod.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
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

    struct LSM6DSOXCallback{ //abstract
        public:
        virtual void hasSample(const LSM6DSOXSample &_sample)=0; 
    };
    
    /**
    * @brief Opens gpio chip, sets gpio settings, line config, request config and requests, starts thread running the busy loop
    */
    void start();

    /**
    @brief Terminates thread, releases lines
    */
    void stop();

    /**
    * @brief Assigns custom implementation of virtual callback
    */
    void registerCallback(LSM6DSOXCallback * _cb);

    void setXLScale(XLSettings::XL_SCALE);

    void setGyroScale(GyroSettings::GYRO_SCALE);

    void setXLSamplingRate(XLSettings::XL_ODR);

    void setGyroSamplingRate(GyroSettings::GYRO_ODR);

    bool isxlFSHigh();

    protected:
    std::string device; //device file
    uint8_t address=0;// chip address
    std::thread thread;
    GyroSettings gyroSettings{};
    XLSettings xlSettings{};
    friend class LSM6DSOXTest; //for unit testing
    uint8_t running=false;
    LSM6DSOXSample sample;
    LSM6DSOXCallback * callback=nullptr;
    float xlRes; //accelerometer resolution
    float gRes; //gyroscope resolution

    struct gpiod_chip *chipDRDY = nullptr; //chip where the data ready pin is
    struct gpiod_line_settings *settings=nullptr;
    struct gpiod_line_config *line_cfg=nullptr;
    struct gpiod_request_config *req_cfg=nullptr;
    struct gpiod_line_request *request=nullptr;

    /**
    * @brief Busy loop, waits for interrupt on data ready pin
    */
    void worker();

    /**
    * @brief Reads from accelerometer/gyroscope/temperature sensor registers
    */
    void getData();

    RawData readGyroscope();

    RawData readAccelerometer();

    /**
    * @brief reads bytes contiguously from the given address
    * @param address starting address
    * @param container array to which values will be written
    * @param nBytes number of bytes to read
    */
    void contiguousReadBytes(uint8_t address, uint8_t * container, uint8_t nBytes);


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
    * @brief Enable gyroscope, set sampling rate and data scale from gyroSettings
    */
    void initGyro();

    /**
    * @brief Enable accelerometer, set sampling rate and data scale from xlSettings
    */
    void initAccelerometer();

    /**
    * @brief throws a different exception based on what's in the status register
    */
    const char* throwStatus(uint8_t statusByte);

    /**
    * @brief Bit of a hack, but uses status register readings to flush any existing data
    * and allowing interrupt pin to go low for the next reading
    */
    uint8_t flushData(uint8_t statusByte);

    /**
    *@brief Gyroscope resolution (16 bit)
    */
    float getGRes();

    /**
    * @brief Accelerometer resolution (16 bit)
    */
    float getXlRes();

    /**
    * @brief Get bits to write to gyro register CTRL2_G to set scale. Shifts bits automatically
    * so they can directly be written to register
    */
    uint8_t gyroScaleBits(); 

    /**
    * @brief Get bits to write to accelerometer register CTRL1_XL to set scale. Shifts bits automatically
    * so they can directly be written to register
    */
    uint8_t xlScaleBits();

};
#endif