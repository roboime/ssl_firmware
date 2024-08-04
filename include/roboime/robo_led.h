#ifndef ROBOIME_INCLUDE_ROBO_LED_H_
#define ROBOIME_INCLUDE_ROBO_LED_H_

#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>

/* Standard includes */
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* LED IDs */
typedef enum {
    LED_GREEN,
    LED_YELLOW,
    LED_RED,
} led_id_t;

/* LED control structure */
typedef struct {
    const struct device *dev;
    gpio_pin_t pin;
} led_control_t;

/**
 * @brief Initialize the LED driver.
 *
 * @return 0 on success, negative error code otherwise.
 */
int robo_led_init(void);

/**
 * @brief Turn on a specified LED.
 *
 * @param id LED identifier.
 * @return 0 on success, negative error code otherwise.
 */
int robo_led_on(led_id_t id);

/**
 * @brief Turn off a specified LED.
 *
 * @param id LED identifier.
 * @return 0 on success, negative error code otherwise.
 */
int robo_led_off(led_id_t id);

/**
 * @brief Toggle a specified LED.
 *
 * @param id LED identifier.
 * @return 0 on success, negative error code otherwise.
 */
int robo_led_toggle(led_id_t id);


#ifdef __cplusplus
}
#endif

#endif /* ROBOIME_INCLUDE_ROBO_LED_H_ */
