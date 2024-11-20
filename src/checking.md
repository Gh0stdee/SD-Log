# Temporal space

    #define SDA 21
    #define SCL 22

    #define csPin 15, 5
    #define SCK 14, 18
    #define MOSI 13, 23
    #define MISO 12, 19

 #include "driver/i2c.h"

int i2c_master_port = 0;
i2c_config_t conf = {

}
