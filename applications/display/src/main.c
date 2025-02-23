#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>
#include "init.h"
#include "work.h"
#include "roboime/robo_led.h"
#include "display.h"

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

// Display update function prototypes
static void update_display_status(void);
static void draw_robot_info(void);

void main(void)
{
    int ret;
    LOG_INF("SSL Robot Firmware Starting...");

    /* Initialize modules */
    init();

    /* Initialize work queue and threads */
    work_init();

    /* Initialize the display */
    ret = display_init();
    if (ret != 0) {
        LOG_ERR("Display initialization failed: %d", ret);
    } else {
        LOG_INF("Display initialized successfully");
        
        /* Initial display setup */
        display_clear(0x0000);  // Clear to black
        draw_robot_info();      // Draw initial robot status
    }

    uint32_t update_counter = 0;

    /* Main loop */
    while (1) {
        LOG_INF("Alive");
        LOG_DBG("Debug message test");
        LOG_ERR("Error message test");
        
        robo_led_toggle(LED_YELLOW);
        robo_led_on(LED_GREEN);

        // Update display every few iterations
        if (update_counter++ % 5 == 0) {
            update_display_status();
        }

        k_sleep(K_MSEC(1000));
    }
}

static void update_display_status(void)
{
    static uint32_t runtime_seconds = 0;
    runtime_seconds++;

    // Update display with current status
    display_set_window(10, 10, 230, 310);  // Set display window for status area
    
    // Here you can add code to display:
    // - Robot status
    // - Battery level
    // - Connection status
    // - Sensor readings
    // - Error conditions
    // - Runtime information
    
    // For now, we'll just update a simple counter
    char buf[32];
    snprintf(buf, sizeof(buf), "Runtime: %lu s", runtime_seconds);
    // Note: You'll need to implement text rendering functions
    // in your display driver to actually show text
}

static void draw_robot_info(void)
{
    // Draw initial static information
    // You can add code here to draw:
    // - Robot ID
    // - Firmware version
    // - Hardware version
    // - Team information
    // - Static UI elements
    
    // For now, we'll just draw a border
    // Top border
    display_set_window(0, 0, DISPLAY_WIDTH-1, 2);
    display_clear(0x07E0);  // Green
    
    // Bottom border
    display_set_window(0, DISPLAY_HEIGHT-3, DISPLAY_WIDTH-1, DISPLAY_HEIGHT-1);
    display_clear(0x07E0);  // Green
    
    // Left border
    display_set_window(0, 0, 2, DISPLAY_HEIGHT-1);
    display_clear(0x07E0);  // Green
    
    // Right border
    display_set_window(DISPLAY_WIDTH-3, 0, DISPLAY_WIDTH-1, DISPLAY_HEIGHT-1);
    display_clear(0x07E0);  // Green
}