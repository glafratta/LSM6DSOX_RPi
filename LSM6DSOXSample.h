#ifndef LSM6DSOX_SAMPLE
#define LSM6DSOX_SAMPLE
#include <stdlib.h>

struct GyroscopeData{
    float x=0;
    float y=0;
    float z=0;
};

struct AccelerometerData{
    float x=0;
    float y=0;
    float z=0;
};


struct LSM6DSOXSample{
    /**
    * Acceleration on x axis (m^2/s)
    */
    float ax=0;
    /**
    * Acceleration on y axis (m^2/s)
    */
    float ay=0;
    /**
    * Acceleration on z axis (m^2/s)
    */
    float az=0;

    /**
    * Rotation on x axis (m^2/s)
    */

    float gx=0;

    /**
    * Rotation on y axis (m^2/s)
    */

    float gy=0;

    /**
    * Rotation on z axis (m^2/s)
    */
    float gz=0;

    /**
    * Temperature
    */
    float temperature=0;

};

struct XLSettings{
    /**
    * @brief Linear acceleration sensitivity
    */
    enum XL_SCALE{
        XL_2G=2, XL_4G=4, XL_8G=8, XL_16G=16
    };

    /**
    * @brief Linear acceleration output data rate
    */
    enum XL_ODR:uint8_t{       //LOW-POWER MODE       HIGH-PERFORMANCE MODE (set via XL_HM_MODE)
        XL_ODR_1_6=0x0B, //1.6 Hz                     12.5 Hz
        XL_ODR_12_5=0x01, //12.5                   the rest of the sampling rates are the same
        XL_ODR_26=0x02, //26 Hz
        XL_ODR_52=0x03, //52 Hz
        XL_ODR_104=0x04, //104 Hz
        XL_ODR_208=0x05, //208 Hz
        XL_ODR_416=0x06, //416 Hz
        XL_ODR_833=0x07, //833 Hz
        XL_ODR_1666=0x08, //1666 Hz
        XL_ODR_3332=0x09, //3332 Hz
        XL_ODR_6664=0x0A //6664 Hz
    };

    enum XL_POWER_MODES{
        XL_POWER_DOWN, XL_ULTRALOW, XL_LOW, XL_NORMAL, XL_HIGH_PERF 
    };

    XL_ODR samplingRate=XL_ODR_104;
    XL_SCALE scale=XL_2G;

};

struct GyroSettings{
    /**
    *@brief Angular rate sensitivity
    */
    enum G_So{
        GYRO_125_DPS=125, GYRO_250_DPS=250, GYRO_500_DPS=500, GYRO_1000_DPS=1000, GYRO_2000_DPS=2000 
    };

        /**
    * @brief Angular rate output data rate
    */
    enum GYRO_ODR{
        GYRO_ODR_12_5=12, //12.5
        GYRO_ODR_26=26, //26 Hz
        GYRO_ODR_52=52, //52 Hz
        GYRO_ODR_104=104, //104 Hz
        GYRO_ODR_208=208, //208 Hz
        GYRO_ODR_416=416, //416 Hz
        GYRO_ODR_833=833, //833 Hz
        GYRO_ODR_1666=1666, //1666 Hz
        GYRO_ODR_3332=3332, //3332 Hz
        GYRO_ODR_6664=6664 //6664 Hz
    };

    GYRO_ODR samplingRate=GYRO_ODR_104;

};


#endif