#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(led_control, LOG_LEVEL_INF);

/* Define the LED nodes based on your device tree */
#define LED0_NODE DT_NODELABEL(green_led)
#define LED1_NODE DT_NODELABEL(yellow_led)
#define LED2_NODE DT_NODELABEL(red_led)

/* Initialize GPIO specs for each LED */
static const struct gpio_dt_spec led_green = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec led_yellow = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
static const struct gpio_dt_spec led_red = GPIO_DT_SPEC_GET(LED2_NODE, gpios);

/* Define an array of the LED specs */
static const struct gpio_dt_spec *leds[] = { &led_green, &led_yellow, &led_red };

/* Function to initialize LEDs */
int robo_led_init(void) {
    int ret;

    for (int id = 0; id < 3; id++) {
        if (!gpio_is_ready_dt(leds[id])) {
            LOG_ERR("LED %d device not ready", id);
            return -ENODEV;
        }

        ret = gpio_pin_configure_dt(leds[id], GPIO_OUTPUT_INACTIVE);
        if (ret < 0) {
            LOG_ERR("Failed to configure LED %d: %d", id, ret);
            return ret;
        }
    }

    return 0;
}

/* Function to turn on an LED by its ID */
int robo_led_on(int id) {
    if (id < 0 || id >= 3) {
        LOG_ERR("Invalid LED ID %d", id);
        return -EINVAL;
    }

    int ret = gpio_pin_set_dt(leds[id], 1);
    if (ret < 0) {
        LOG_ERR("Failed to turn on LED %d: %d", id, ret);
        return ret;
    }

    return 0;
}

/* Function to turn off an LED by its ID */
int robo_led_off(int id) {
    if (id < 0 || id >= 3) {
        LOG_ERR("Invalid LED ID %d", id);
        return -EINVAL;
    }

    int ret = gpio_pin_set_dt(leds[id], 0);
    if (ret < 0) {
        LOG_ERR("Failed to turn off LED %d: %d", id, ret);
        return ret;
    }

    return 0;
}

/* Function to toggle an LED by its ID */
int robo_led_toggle(int id) {
    if (id < 0 || id >= 3) {
        LOG_ERR("Invalid LED ID %d", id);
        return -EINVAL;
    }

    int ret = gpio_pin_toggle_dt(leds[id]);
    if (ret < 0) {
        LOG_ERR("Failed to toggle LED %d: %d", id, ret);
        return ret;
    }

    return 0;
}
