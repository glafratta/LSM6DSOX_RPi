#include "LSM6DSOX.h"

void LSM6DSOX::start(){
    chipDRDY=gpiod_chip_open(device.c_str());
    settings=gpiod_line_settings_new();
    gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_INPUT); //defined in gpiod.h
    gpiod_line_settings_set_edge_detection(settings, GPIOD_LINE_EDGE_RISING); //defined in gpiod.h
    line_cfg = gpiod_line_config_new();
    gpiod_line_config_add_line_settings(line_cfg, &LMS6DSOX_DRDY_GPIO, 1, settings); //offset is the gpio pin (22)
    req_cfg = gpiod_request_config_new();
    gpiod_request_config_set_consumer(req_cfg, "imu");
    request = gpiod_chip_request_lines(chip, req_cfg, line_cfg);
    //pinDRDY=gpiod_chip_get_line_info(chipDRDY, LMS6DSOX_DRDY_GPIO);
    //v1, need to rewrite in v2
   // int ret=gpiod_line_request_rising_edge_events(pinDRDY, "Consumer"); //request rising edge events not. on a single line
    if (!request){
        gpiod_line_request_release(request);
        throw "Could not request event";
    }
    thread=std::thread(&LSM6DSOX::worker, this);
}

void LSM6DSOX::worker(){
    running=true;
    while (running){
        int r=gpiod_line_request_wait_edge_events(request, -1);
        if (r){
            // struct gpiod_line_event event;
            // gpiod_line_event_read(pinDRDY, &event);
            struct gpiod_edge_event_buffer *buffer;
            buffer = gpiod_edge_event_buffer_new(1);
            int n = gpiod_line_request_read_edge_events(request, buffer, 1);
            if (n){
                //callback goes here
                printf("Interrupt happened!");
            }
            gpiod_edge_event_buffer_free(buffer);
        }
        else{
            running=false;
        }
    }

}

void LSM6DSOX::stop(){
    running = false;
    if (thread.joinable()) {
	    thread.join();
    }
    if (NULL != request) {
	    //gpiod_line_release(pinDRDY);
        gpiod_line_request_release(request);
    }
    if (NULL != chipDRDY) {
	    gpiod_chip_close(chipDRDY);
    }
    // pinDRDY = NULL;
    // chipDRDY = NULL;
}

// int LSM6DSOX::i2cOpen(){
//     // const int fd_i2c=open(device, O_RDWR); //open i2c device
//     // if (fd_i2c<0){
//     //     return fd_i2c;
//     // }
// }