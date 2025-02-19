#include "headers.h"
#include "pca9685_servo.h"

#define SERVO_CHANNEL 0
#define SERVO_MIN 150
#define SERVO_MAX 600

void serial_task(void *arg) {
    char data[32];

    while (1) {
        int len = fread(data, 1, sizeof(data) - 1, stdin);  // Read from USB serial (UART0)
        if (len > 0) {
            data[len] = '\0';  // Null-terminate string
            printf("Received: %s\n", data);  // Debugging print to serial monitor

            if (strncmp(data, "OPEN", 4) == 0) {
                ESP_LOGI("esplog", "Servo open");
                move_servo(SERVO_CHANNEL, SERVO_MIN, SERVO_MAX);
            } else if (strncmp(data, "CLOSE", 5) == 0) {
                ESP_LOGI("esplog", "Servo close");
                move_servo(SERVO_CHANNEL, SERVO_MAX, SERVO_MIN);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(100));  // Yield CPU for 100ms
    }
}

void serial_init() {
    xTaskCreate(serial_task, "serial_task", 4096, NULL, 5, NULL);
}
