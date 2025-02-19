#include "wifi_connect.h"
#include "headers.h"
#include "ros_mqtt.h"
#include "pca9685_servo.h"
#include "serial_com.h"

#define CHANNEL_ID 0

void app_main() {
    i2c_master_init();
    pca9685_init();
    wifi_init_connect();
    mqtt_init_and_connect();
    serial_init();

}