// #include <stdio.h>
// #include "driver/i2c.h"
// #include "esp_log.h"
// #include "esp_system.h"
// #include "esp_wifi.h"
// #include "esp_event.h"
// #include "esp_log.h"
// #include "nvs_flash.h"
// #include "mqtt_client.h"
#include "wifi_connect.h"
#include "headers.h"
#include "ros_mqtt.h"
#include "pca9685_servo.h"

// PCA9685 I2C address and parameters
// #define I2C_MASTER_NUM I2C_NUM_0
// #define I2C_MASTER_SDA_IO 21
// #define I2C_MASTER_SCL_IO 22
// #define I2C_MASTER_FREQ_HZ 100000
// #define PCA9685_ADDR 0x40
// #define PCA9685_MODE1 0x00
// #define PCA9685_PRESCALE 0xFE
// #define SERVO_MIN 150
// #define SERVO_MAX 600
#define CHANNEL_ID 0

// #define Desk_SSID "university"
// #define Desk_PASS "bokuniversityofpeshawar"

// #define WIFI_TAG "WIFI_STATION"
// // #define MQTT_TAG "MQTT_CLIENT"
// #define MQTT_BROKER_URI "mqtt://mqtt.eclipseprojects.io"  // Replace with your MQTT broker URI
// #define MQTT_TOPIC "test/topic/servo"                    // Replace with your topic

// static const char *TAG = "PCA9685_SERVO";

// void i2c_master_init() {
//     i2c_config_t conf = {
//         .mode = I2C_MODE_MASTER,
//         .sda_io_num = I2C_MASTER_SDA_IO,
//         .sda_pullup_en = GPIO_PULLUP_ENABLE,
//         .scl_io_num = I2C_MASTER_SCL_IO,
//         .scl_pullup_en = GPIO_PULLUP_ENABLE,
//         .master.clk_speed = I2C_MASTER_FREQ_HZ,
//     };
//     i2c_param_config(I2C_MASTER_NUM, &conf);
//     i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);
// }

// // Event handler for Wi-Fi events
// static void wifi_event_handler(void *arg, esp_event_base_t event_base,
//                                int32_t event_id, void *event_data) {
//     if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
//         esp_wifi_connect();
//     } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
//         ESP_LOGI(WIFI_TAG, "Disconnected. Retrying...");
//         esp_wifi_connect();
//     } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
//         ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
//         ESP_LOGI(WIFI_TAG, "Got IP: " IPSTR, IP2STR(&event->ip_info.ip));
//     }
// }

// void wifi_init_connect()
// {
//     nvs_flash_init();
//     esp_netif_init();
//     ESP_ERROR_CHECK(esp_event_loop_create_default());
//     esp_netif_create_default_wifi_sta();

//     wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
//     esp_wifi_init(&wifi_init_config);

//     // Register event handlers
//     ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID,
//                                                         &wifi_event_handler, NULL, NULL));
//     ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP,
//                                                         &wifi_event_handler, NULL, NULL));

//     wifi_config_t wifi_config = {
//         .sta = {
//             .ssid = Desk_SSID,
//             .password = Desk_PASS
//         },
//     };
//     ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
//     ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
//     ESP_ERROR_CHECK(esp_wifi_start() );
//     ESP_LOGI(TAG, "wifi_init_sta finished.");
// }

// void pca9685_write_byte(uint8_t reg, uint8_t data) {
//     uint8_t write_buf[2] = {reg, data};
//     i2c_master_write_to_device(I2C_MASTER_NUM, PCA9685_ADDR, write_buf, 2, 1000 / portTICK_PERIOD_MS);
// }

// void pca9685_set_pwm(uint8_t channel, uint16_t on, uint16_t off) {
//     uint8_t write_buf[5] = {
//         0x06 + 4 * channel,  // LEDn_ON_L register address
//         on & 0xFF,           // Low byte of ON value
//         on >> 8,             // High byte of ON value
//         off & 0xFF,          // Low byte of OFF value
//         off >> 8             // High byte of OFF value
//     };
//     i2c_master_write_to_device(I2C_MASTER_NUM, PCA9685_ADDR, write_buf, 5, 1000 / portTICK_PERIOD_MS);
// }

// void pca9685_init() {
//     pca9685_write_byte(PCA9685_MODE1, 0x10);  // Put PCA9685 into sleep mode
//     uint8_t prescale = (uint8_t)(25000000 / (4096 * 50) - 1);  // Set PWM frequency to 50 Hz
//     pca9685_write_byte(PCA9685_PRESCALE, prescale);
//     pca9685_write_byte(PCA9685_MODE1, 0xA0);  // Restart and set to normal mode
// }

// void move_servo(uint16_t channel_id,uint16_t start, uint16_t end) {
//     if (start < end) {
//         for (int pulse = start; pulse <= end; pulse++) {
//             pca9685_set_pwm(channel_id, 0, pulse);
//             vTaskDelay(10 / portTICK_PERIOD_MS);
//         }
//     } else {
//         for (int pulse = start; pulse >= end; pulse--) {
//             pca9685_set_pwm(channel_id, 0, pulse);
//             vTaskDelay(10 / portTICK_PERIOD_MS);
//         }
//     }
// }


// static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
// {
//     uint16_t channel_id = 0;
//     ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%" PRIi32 "", base, event_id);
//     esp_mqtt_event_handle_t event = event_data;
//     esp_mqtt_client_handle_t client = event->client;
//     int msg_id;
//     switch ((esp_mqtt_event_id_t)event_id) {
//     case MQTT_EVENT_CONNECTED:
//         ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
//         // msg_id = esp_mqtt_client_publish(client, "/topic/qos1", "data_3", 0, 1, 0);
//         // ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);

//         msg_id = esp_mqtt_client_subscribe(client, "servo/serv0", 1);
//         msg_id = esp_mqtt_client_subscribe(client, "servo/serv1", 1);
//         msg_id = esp_mqtt_client_subscribe(client, "servo/serv2", 1);

//         // msg_id = esp_mqtt_client_unsubscribe(client, "/topic/qos1");
//         // ESP_LOGI(TAG, "sent unsubscribe successful, msg_id=%d", msg_id);
//         break;
//     case MQTT_EVENT_DISCONNECTED:
//         ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
//         break;

//     case MQTT_EVENT_SUBSCRIBED:
//         ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
//         break;
//     case MQTT_EVENT_UNSUBSCRIBED:
//         ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
//         break;
//     case MQTT_EVENT_PUBLISHED:
//         ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
//         break;
//     case MQTT_EVENT_DATA:
//         ESP_LOGI(TAG, "MQTT_EVENT_DATA");
//         printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
//         printf("DATA=%.*s\r\n", event->data_len, event->data);
//         if(strncmp(event->topic, "servo/serv0", event->topic_len) == 0)
//         {
//             ESP_LOGW("PCA_Ctrl", "Servo 0 selected");
//             channel_id = 0;
            
//         }
//         else if(strncmp(event->topic, "servo/serv1", event->topic_len) == 0)
//         {
//             ESP_LOGW("PCA_Ctrl", "Servo 1 selected");
//             channel_id = 1;

//         } 
//         else if(strncmp(event->topic, "servo/serv2", event->topic_len) == 0)
//         {
//             ESP_LOGW("PCA_Ctrl", "Servo 2 selected");
//             channel_id = 2;

//         }        
//         else
//         {
//             ESP_LOGE("PCA_Ctrl", "All Servo selected");
//             channel_id = 3;

//         }
//         if (strncmp(event->data, "0", event->data_len) == 0) {
//             move_servo(channel_id,SERVO_MIN, SERVO_MAX);
//             ESP_LOGE(TAG, "Moving servo");
//         } else if (strncmp(event->data, "1", event->data_len) == 0) {
//             move_servo(channel_id, SERVO_MAX, SERVO_MIN);
//             ESP_LOGE(TAG, "Moving servo");
//         }
//         break;
//     case MQTT_EVENT_ERROR:
//         ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
//         break;
//     default:
//         ESP_LOGI(TAG, "Other event id:%d", event->event_id);
//         break;
//     }
// }

// // MQTT initialization and connection function
// void mqtt_init_and_connect() {
//     esp_mqtt_client_config_t mqtt_cfg = {
//         .broker.address.uri = "mqtt://mqtt.eclipseprojects.io",  // MQTT broker URI
//     };

//     esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
//     esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
//     esp_mqtt_client_start(client);
//     ESP_LOGI(MQTT_TAG, "MQTT client started");
// }

void app_main() {
    // ESP_LOGI(TAG, "Initializing I2C...");
    i2c_master_init();
    
    // ESP_LOGI(TAG, "Initializing PCA9685...");
    pca9685_init();
    wifi_init_connect();
    mqtt_init_and_connect();

}