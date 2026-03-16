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