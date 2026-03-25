#include <gtest/gtest.h>
#include "LSM6DSOX.h"
#include <chrono>
const bool DEBUG=true;

class LSM6DSOXTest:public LSM6DSOX, public testing::Test{};

// TEST_F(LSM6DSOXTest, StartStop){
//     start();
//     std::this_thread::sleep_for(std::chrono::seconds(nSeconds));
//     stop();
// }

TEST_F(LSM6DSOXTest, whoAmI){
   try{
        uint8_t b=i2cReadByte(LSM6DSOX_WHO_AM_I_REG);
        EXPECT_EQ(b, (LSM6DSOX_WHO_AM_I_ADDRESS));
   } 
   catch(std::exception &e){
        std::cout<<e.what()<<std::endl;
   }
}

TEST_F(LSM6DSOXTest, i2cOpen){
    int fd=i2cOpen(LSM6DSOX_INT1_CTRL);
    EXPECT_GE(fd, 0);
    close(fd);
}