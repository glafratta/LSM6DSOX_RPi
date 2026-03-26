#include "LSM6DSOX.h"

void LSM6DSOX::start(){
    //setup to detect rising edge on the data ready pin
    chipDRDY=gpiod_chip_open(DRDY_CHIP); //open chip with the gpio pins
    settings=gpiod_line_settings_new();
    gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_INPUT); //input line
    gpiod_line_settings_set_edge_detection(settings, GPIOD_LINE_EDGE_RISING); //detect rising edge
    line_cfg = gpiod_line_config_new();
    if(gpiod_line_config_add_line_settings(line_cfg, &LMS6DSOX_DRDY_GPIO, 1, settings)<0 ){ //offset is the gpio pin (22)
        throw "Add line settings failed";
    }
    req_cfg = gpiod_request_config_new();
    gpiod_request_config_set_consumer(req_cfg, "LSM6DSOX");
    request = gpiod_chip_request_lines(chipDRDY, req_cfg, line_cfg);
    if (request==nullptr){
        gpiod_line_request_release(request);
        throw "Could not request event";
    }

    //need to set drdy pin to go high for accelerometer and gyroscope
    uint8_t data =LSM6DSOX_GYRO_NC | LSM6DSOX_XL_NC; 
    i2cWriteByte(LSM6DSOX_INT1_CTRL,data);
    //start thread with busy loop
    thread=std::thread(&LSM6DSOX::worker, this);
    if (DEBUG){
        std::cout<<"Started!"<<std::endl;
    }
}

void LSM6DSOX::worker(){
    running=true;
    while (running){
        int r=gpiod_line_request_wait_edge_events(request, wait_line);
        if (DEBUG){
            std::cout<<"Edge event code:"<<r<<std::endl;
        }
        if (r>0){
            struct gpiod_edge_event_buffer *buffer;
            buffer = gpiod_edge_event_buffer_new(1);
            int n = gpiod_line_request_read_edge_events(request, buffer, 1);
            if (DEBUG){
                std::cout<<"Edge event read:"<<n<<std::endl;
            }
            if (n>0){
                getData();
                //callback goes here
                printf("Interrupt happened!");
            }
            gpiod_edge_event_buffer_free(buffer);
        }
        else{
            running=false; //need to delete this once you start getting data
        }
    }

}

void LSM6DSOX::stop(){
    if (DEBUG){
        std::cout<<"Stopping..."<<std::endl;
    }
    running = false;
    if (thread.joinable()) {
	    thread.join();
    }
    if (NULL != request) {
        gpiod_line_request_release(request);
    }
    if (NULL != chipDRDY) {
	    gpiod_chip_close(chipDRDY);
    }
    request = NULL;
    chipDRDY = NULL;
}


int LSM6DSOX::getData(){
    return -1;
}

GyroscopeData LSM6DSOX::readGyro(){
    GyroscopeData gd;
    uint8_t tmp[32]; //test if data is 8 bit
    try{
        contiguousReadBytes(LSM6DSOX_OUTX_L_G, tmp, 6);//read 6 bytes from outx_l_g
    }
    catch (int fError){
        gd.x=gd.y=gd.z=9999;
    }
    return gd;
}

void LSM6DSOX::contiguousReadBytes(uint8_t address, uint8_t * container, uint8_t nBytes){

}

int LSM6DSOX::i2cOpen(int address){
    const int fd=open(device.c_str(), O_RDWR);
    if (fd<0){
        throw "Can't open I2C device";
    }
    //I2C_SLAVE comes from linux kernel driver, no need to define
    //assign address as i2c peripheral (slave)
    const int r=ioctl(fd, I2C_SLAVE, address);
    if (r<0){
        throw "Cannot assign I2C peripheral address";
    }
    return fd;
}

void LSM6DSOX::i2cWriteByte(uint8_t address, uint8_t data){
    int fd=i2cOpen(LSM6DSOX_ADDRESS); //open device chip first
    uint8_t buffer[3];
    buffer[0]=address;
    buffer[1]=data;
    ssize_t w=write(fd, buffer, 2);
    close(fd);
}


uint8_t LSM6DSOX::i2cReadByte(uint8_t address){
    int fd=i2cOpen(LSM6DSOX_ADDRESS); //open device chip first
    uint8_t buffer[1];
    buffer[0]=address;
    ssize_t w=write(fd, buffer, 1); //test write (have to do it or it can't read, don't know why)
    ssize_t r= read(fd, buffer, 1); //read one byte and write to tmp buffer
    if (r<0){
        throw "Can't read from i2c";
    }
    close(fd);
    return buffer[0];
}

void LSM6DSOX::initGyro(){
    //from datasheet: gyroscope is activated writing ODR_G[3:0] (7th bit) to CTRL2_G
    i2cWriteByte(LSM6DSOX_CTRL2_G, 0X00);
}

void LSM6DSOX::initAccelerometer(){
    //from datasheet: accelerometer is activated writing ODR_G[3:0] (7th bit) to CTRL1_XL
    //                      CTRL1_XL description
    // [ODR_XL3] [ODR_XL2] [ODR_XL1] [ODR_XL0] [FS1_XL] [FS0_XL] [LPF2_XL_EN] [0]
    i2cWriteByte(LSM6DSOX_CTRL1_XL, 0X00);
}