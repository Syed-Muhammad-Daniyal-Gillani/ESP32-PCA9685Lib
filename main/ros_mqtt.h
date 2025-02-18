#pragma once
#include "headers.h"
#include "pca9685_servo.h"
#define SERVO_MIN 150
#define SERVO_MAX 900
#define MQTT_TAG "MQTT_CLIENT"

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    uint16_t channel_id = 0;
    ESP_LOGD(MQTT_TAG, "Event dispatched from event loop base=%s, event_id=%" PRIi32 "", base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id = 0;
    switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(MQTT_TAG, "MQTT_EVENT_CONNECTED");
        // msg_id = esp_mqtt_client_publish(client, "/topic/qos1", "data_3", 0, 1, 0);
        // ESP_LOGI(MQTT_TAG, "sent publish successful, msg_id=%d", msg_id);

        msg_id = esp_mqtt_client_subscribe(client, "servo/serv0", 1);
        msg_id = esp_mqtt_client_subscribe(client, "servo/serv1", 1);
        msg_id = esp_mqtt_client_subscribe(client, "servo/serv2", 1);

        // msg_id = esp_mqtt_client_unsubscribe(client, "/topic/qos1");
        // ESP_LOGI(MQTT_TAG, "sent unsubscribe successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(MQTT_TAG, "MQTT_EVENT_DISCONNECTED");
        break;

    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(MQTT_TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(MQTT_TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(MQTT_TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(MQTT_TAG, "MQTT_EVENT_DATA");
        printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        printf("DATA=%.*s\r\n", event->data_len, event->data);
        if(strncmp(event->topic, "servo/serv0", event->topic_len) == 0)
        {
            ESP_LOGW("PCA_Ctrl", "Servo 0 selected");
            channel_id = 0;
            
        }
        else if(strncmp(event->topic, "servo/serv1", event->topic_len) == 0)
        {
            ESP_LOGW("PCA_Ctrl", "Servo 1 selected");
            channel_id = 1;

        } 
        else if(strncmp(event->topic, "servo/serv2", event->topic_len) == 0)
        {
            ESP_LOGW("PCA_Ctrl", "Servo 2 selected");
            channel_id = 2;

        }        
        else
        {
            ESP_LOGE("PCA_Ctrl", "All Servo selected");
            channel_id = 3;
        }
        if (strncmp(event->data, "0", event->data_len) == 0) {
            move_servo(channel_id,SERVO_MIN, SERVO_MAX);
            ESP_LOGE(MQTT_TAG, "Moving servo");
        } else if (strncmp(event->data, "1", event->data_len) == 0) {
            move_servo(channel_id, SERVO_MAX, SERVO_MIN);
            ESP_LOGE(MQTT_TAG, "Moving servo");
        }
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(MQTT_TAG, "MQTT_EVENT_ERROR");
        break;
    default:
        ESP_LOGI(MQTT_TAG, "Other event id:%d", event->event_id);
        break;
    }
}

// MQTT initialization and connection function
void mqtt_init_and_connect() {
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = "mqtt://mqtt.eclipseprojects.io",  // MQTT broker URI
    };

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
    ESP_LOGI(MQTT_TAG, "MQTT client started");
}