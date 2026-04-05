#include <gtest/gtest.h>
#include "LSM6DSOX.h"
#include <chrono>
const bool DEBUG=true;

class LSM6DSOXTest:public LSM6DSOX, public testing::Test, public testing::WithParamInterface<uint8_t>{};

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

/**
* @brief Test that the correct bits have been written to gyroscope register 
*/
TEST_F(LSM6DSOXTest, initGyro){
    initGyro();
    EXPECT_EQ(i2cReadByte(LSM6DSOX_CTRL2_G)>>5, 1); //[0][1][0][0][0][0][0][0] =64
}

/**
* @brief having an issue with not detecting rising edges in int1 pin so this reads xl data available
*/
TEST_F(LSM6DSOXTest, readAccelerometer){
    RawData ad;
    ad=readAccelerometer();
    EXPECT_NE(ad.x, 0);
    EXPECT_NE(ad.y,0);
    EXPECT_NE(ad.z, 0);
    EXPECT_NE(ad.x, 999);
    EXPECT_NE(ad.y, 999);
    EXPECT_NE(ad.z,999);
    EXPECT_NE(i2cReadByte(LSM6DSOX_STATUS_REG), 0X01);
}

/**
* @brief having an issue with not detecting rising edges in int1 pin so this reads xl data available
*/
TEST_F(LSM6DSOXTest, readGyroscope){
    RawData gd;
    gd=readGyroscope();
    EXPECT_NE(gd.x, 0);
    EXPECT_NE(gd.y,0);
    EXPECT_NE(gd.z, 0);
    EXPECT_NE(gd.x, 999);
    EXPECT_NE(gd.y, 999);
    EXPECT_NE(gd.z,999);
    EXPECT_NE(i2cReadByte(LSM6DSOX_STATUS_REG), 0X02);
}

TEST_P(LSM6DSOXTest, flushData){
    EXPECT_EQ(flushData(GetParam()), GetParam());
}

INSTANTIATE_TEST_CASE_P(Codes, LSM6DSOXTest, ::testing::Values(uint8_t(0), uint8_t(1), uint8_t(2), uint8_t(3)));
                                   
//TESTING XL SCALE
                                                                                                                //fs     //scale  //expected
class LSM6DSOXXLScaleTest:public LSM6DSOX, public testing::Test, public testing::WithParamInterface<std::tuple<uint8_t, XLSettings::XL_SCALE, uint8_t>>{};


TEST_P(LSM6DSOXXLScaleTest, xlScaleBits){
    uint8_t fs=std::get<0>(GetParam());
    i2cWriteByte(LSM6DSOX_CTRL8_XL, fs<<1); //set fs
    EXPECT_EQ(isxlFSHigh(), bool(fs)); //sanity check
    setXLScale(std::get<1>(GetParam()));
    initAccelerometer();
    uint8_t ctrl1_xl=i2cReadByte(LSM6DSOX_CTRL1_XL);
    uint8_t masked=(ctrl1_xl&0x04)>>2;//get bit 1 and 2 and shift by 2
    EXPECT_EQ(masked, std::get<2>(GetParam()));

}

INSTANTIATE_TEST_CASE_P(XLScales, LSM6DSOXXLScaleTest, ::testing::Values(
                        std::tuple<uint8_t, XLSettings::XL_SCALE, uint8_t>(0x00, XLSettings::XL_2G, 0x00),
                        std::tuple<uint8_t, XLSettings::XL_SCALE, uint8_t>(0x01, XLSettings::XL_2G, 0x01),
                        std::tuple<uint8_t, XLSettings::XL_SCALE, uint8_t>(0x00, XLSettings::XL_4G, 0x02),
                        std::tuple<uint8_t, XLSettings::XL_SCALE, uint8_t>(0x01, XLSettings::XL_4G, 0x02),
                        std::tuple<uint8_t, XLSettings::XL_SCALE, uint8_t>(0x00, XLSettings::XL_8G, 0x03),
                        std::tuple<uint8_t, XLSettings::XL_SCALE, uint8_t>(0x01, XLSettings::XL_8G, 0x03),
                        std::tuple<uint8_t, XLSettings::XL_SCALE, uint8_t>(0x00, XLSettings::XL_16G, 0x01),
                        std::tuple<uint8_t, XLSettings::XL_SCALE, uint8_t>(0x01, XLSettings::XL_16G, 0x01)
                    )
);

// TESTING GYRO SCALE


                                                                                                                                      //scale  //expected
class LSM6DSOXGyroScaleTest:public LSM6DSOX, public testing::Test, public testing::WithParamInterface<std::tuple<GyroSettings::GYRO_SCALE, uint8_t>>{};


TEST_P(LSM6DSOXGyroScaleTest, gyroScaleBits){
    setGyroScale(std::get<0>(GetParam()));
    initGyro();
    uint8_t ctrl2_g=i2cReadByte(LSM6DSOX_CTRL2_G);
    if (std::get<0>(GetParam())>GyroSettings::GYRO_125_DPS){
        uint8_t masked=(ctrl2_g&0x04)>>1;//get bit 1 and 2 and shift by 1
    }
    else{
        uint8_t masked=(ctrl2_g&0x01);//get bit 1 and 2 and shift by 2
    }
    EXPECT_EQ(masked, std::get<1>(GetParam()));


}

INSTANTIATE_TEST_CASE_P(GyroScales, LSM6DSOXGyroScaleTest, ::testing::Values(
                        std::tuple<GyroSettings::GYRO_SCALE, uint8_t>( GyroSettings::GYRO_125_DPS, 0x01),
                        std::tuple<GyroSettings::GYRO_SCALE, uint8_t>( GyroSettings::GYRO_250_DPS, 0x00),
                        std::tuple<GyroSettings::GYRO_SCALE, uint8_t>( GyroSettings::GYRO_500_DPS, 0x01),
                        std::tuple<GyroSettings::GYRO_SCALE, uint8_t>( GyroSettings::GYRO_1000_DPS, 0x02),
                        std::tuple<GyroSettings::GYRO_SCALE, uint8_t>( GyroSettings::GYRO_2000_DPS, 0x03)
                    )
);

