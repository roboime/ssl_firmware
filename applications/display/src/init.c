#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/logging/log.h>

#include "roboime/robo_led.h"
#include "init.h"

LOG_MODULE_REGISTER(init, LOG_LEVEL_DBG);

void init(void)
{
    // Initialize LEDS
    int ret;
    
    ret = robo_led_init();
    if (ret < 0) {
        LOG_ERR("Failed to initialize LEDs\n");
        return;
    }
 
    LOG_INF("Modules initialized successfully"); 

}
