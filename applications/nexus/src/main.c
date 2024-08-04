#include <zephyr/kernel.h>
#include "init.h"
#include "work.h"
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

void main(void)
{
    printk("SSL Robot Firmware Starting...\n");
    
    // Initialize modules
    init();

    // Initialize work queue and threads
    work_init();

    // Main loop
    while (1) {
        LOG_INF("Alive");
        k_sleep(K_MSEC(1000));
    }
}