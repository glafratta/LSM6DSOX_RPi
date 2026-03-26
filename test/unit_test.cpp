#include <gtest/gtest.h>
#include "LSM6DSOX.h"
#include <chrono>
const bool DEBUG=true;

class LSM6DSOXTest:public LSM6DSOX, public testing::Test{};

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
*@brief Tests what we are writing to int1 register
*/
TEST_F(LSM6DSOXTest, INT1Write){
    uint8_t data =LSM6DSOX_GYRO_NC | LSM6DSOX_XL_NC; 
    i2cWriteByte(LSM6DSOX_INT1_CTRL, data);
    EXPECT_EQ(i2cReadByte(LSM6DSOX_INT1_CTRL), 0x03);
}
