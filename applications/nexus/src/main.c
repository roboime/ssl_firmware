#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>


#include "init.h"
#include "work.h"
#include "roboime/robo_led.h"

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

void main(void)
{
    LOG_INF("SSL Robot Firmware Starting...");
    
    // Initialize modules
    init();

    // Initialize work queue and threads
    work_init();

    // Main loop
    while (1) {
        LOG_INF("Alive");
        robo_led_toggle(LED_YELLOW);
        robo_led_on(LED_GREEN);
        k_sleep(K_MSEC(1000));
    }
}