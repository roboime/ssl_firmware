#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <stm32h7xx_hal.h>
#include <zephyr/drivers/gpio.h>

#include "init.h"
#include "work.h"
#include "roboime/robo_led.h"

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

#define ZEPHYR_USER_NODE DT_PATH(zephyr_user)

const struct gpio_dt_spec signal = GPIO_DT_SPEC_GET(ZEPHYR_USER_NODE, signal_gpios);
const struct gpio_dt_spec signal1 = GPIO_DT_SPEC_GET(ZEPHYR_USER_NODE, signal1_gpios);

void main(void)
{
    LOG_INF("SSL Robot Firmware Starting...");
    
    // Initialize modules
    init();

    // Initialize work queue and threads
    work_init();

    // Simple how to deal with gpio
    gpio_pin_configure_dt(&signal, GPIO_OUTPUT_INACTIVE);
    gpio_pin_set_dt(&signal, 1);
    gpio_pin_configure_dt(&signal1, GPIO_OUTPUT_INACTIVE);
    gpio_pin_set_dt(&signal1, 1);

    // Main loop
    while (1) {
        LOG_INF("Alive");
        LOG_ERR("Alive");
        LOG_DBG("Alive");
        robo_led_toggle(LED_YELLOW);
        robo_led_on(LED_GREEN);
        k_sleep(K_MSEC(1000));
    }
}