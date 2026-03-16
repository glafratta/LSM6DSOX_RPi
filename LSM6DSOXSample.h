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



//optional add gyro/acc settings
#endif