#include <stdio.h>
#include <driver/i2c.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "pca9685.h"


#define I2C_MASTER_SCL_IO   4    /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO   5    /*!< gpio number for I2C master data  */
#define I2C_EXAMPLE_MASTER_FREQ_HZ  100000     /*!< I2C master clock frequency */
#define I2C_EXAMPLE_MASTER_NUM      I2C_NUM_0   /*!< I2C port number for master dev */
#define I2C_EXAMPLE_MASTER_TX_BUF_DISABLE   0   /*!< I2C master do not need buffer */
#define I2C_EXAMPLE_MASTER_RX_BUF_DISABLE   0   /*!< I2C master do not need buffer */


void i2c_configure_init()
{
    //step 1 - Configure I2C driver
    i2c_config_t i2c_drive_handle;
    i2c_drive_handle.mode = I2C_MODE_MASTER;
    i2c_drive_handle.sda_io_num = I2C_MASTER_SDA_IO;
    i2c_drive_handle.scl_io_num = I2C_MASTER_SCL_IO;
    i2c_drive_handle.sda_pullup_en = GPIO_PULLUP_ENABLE;
    i2c_drive_handle.scl_pullup_en = GPIO_PULLUP_ENABLE;
    i2c_drive_handle.master.clk_speed = I2C_EXAMPLE_MASTER_FREQ_HZ;


    i2c_param_config(I2C_NUM_0, &i2c_drive_handle);

    //Initialize/Install i2c driver.
    i2c_driver_install(I2C_NUM_0,i2c_drive_handle.mode, 0, 0, 0);

}


void app_main(void)
{
    i2c_configure_init();
 
    i2c_dev_t pca9685_dev = {0};
    ESP_ERROR_CHECK(pca9685_init_desc(&pca9685_dev, 0x40, I2C_NUM_0, I2C_MASTER_SDA_IO, I2C_MASTER_SCL_IO));
    ESP_ERROR_CHECK(pca9685_init(&pca9685_dev)); 
 

    // pca9685_init_desc();
}
