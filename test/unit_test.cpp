#include <gtest/gtest.h>
#include "LSM6DSOX.h"
#include <chrono>
const bool DEBUG=true;

class LSM6DSOXTest:public LSM6DSOX, public testing::Test{

    int healthCheck(){
        
    }
};

/**
* @brief Checks if device is LSM6DSOX 
*/
TEST_F(LSM6DSOXTest, whoAmI){
     uint8_t b=i2cReadByte(LSM6DSOX_WHO_AM_I_REG);
     EXPECT_EQ(b, (LSM6DSOX_WHO_AM_I_VALUE));
}

/**
* @brief Tests if data interrupt pin 1 opens and closes correctly
*/
TEST_F(LSM6DSOXTest, INT1Open){
    int fd=i2cOpen(LSM6DSOX_INT1_CTRL);
    EXPECT_GE(fd, 0);
    close(fd);
}

/**
*@brief Tests what we (me and the people living in the walls) are writing to int1 register
*/
TEST_F(LSM6DSOXTest, INT1Write){
    uint8_t data =LSM6DSOX_GYRO_NC | LSM6DSOX_XL_NC; 
    i2cWriteByte(LSM6DSOX_INT1_CTRL, data);
    EXPECT_EQ(i2cReadByte(LSM6DSOX_INT1_CTRL), 0x03);
}

/**
* @brief Test that the correct bits have been written to accelerometer register 
*/
TEST_F(LSM6DSOXTest, initAccelerometer){
    initAccelerometer();
    EXPECT_EQ(i2cReadByte(LSM6DSOX_CTRL1_XL), 0x30); //[0][1][0][0][0][0][0][0] =64
}

TEST_F(LSM6DSOXTest, readAccelerometer){
    AccelerometerData ad;
    ad=readAccelerometer();
    EXPECT_NE(ad.x, 0);
    EXPECT_NE(ad.y,0);
    EXPECT_NE(ad.z, 0);
    EXPECT_NE(ad.x, 999);
    EXPECT_NE(ad.y, 999);
    EXPECT_NE(ad.z,999);
    EXPECT_NE(i2cReadByte(LSM6DSOX_STATUS_REG), 0X01);
}