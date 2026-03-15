#include "LSM6DSOX.h"

void LSM6DSOX::start(){
    chipDRDY=gpiod_chip_open(device.c_str());
    pinDRDY=gpiod_chip_get_line(chipDRDY, LMS6DSOX_DRDY_GPIO);
    int ret=gpiod_line_request_rising_edge_events(pinDRDY, "Consumer"); //request rising edge events not. on a single line
    if (ret<0)
}

void LSM6DSOX::worker(){
    running=true;
    while (running){
        const struct timespec ts={1,0};
        int r=gpiod_line_event_wait(pinDRDY, &ts);
        if (r==1){
            struct gpiod_line_event event;
            gpiod_line_event_read(pinDRDY, &event);
            //callback goes here
        }
        else{
            running=false;
        }
    }

}

// int LSM6DSOX::i2cOpen(){
//     // const int fd_i2c=open(device, O_RDWR); //open i2c device
//     // if (fd_i2c<0){
//     //     return fd_i2c;
//     // }
// }