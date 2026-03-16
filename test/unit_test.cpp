#include <gtest/gtest.h>
#include "LSM6DSOX.h"
const bool DEBUG=true;

class LSM6DSOXTest:public LSM6DSOX, public testing::Test{};

TEST_F(LSM6DSOXTest, StartStop){
    start();
    std::this_thread::sleep_for(std::chrono::seconds(5));
    stop();
}