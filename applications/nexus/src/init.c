#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/logging/log.h>

#include "roboime/robo_led.h"
#include "init.h"
#include "sx1280.h"

LOG_MODULE_REGISTER(init, LOG_LEVEL_DBG);
/* 
#define SX1280_TX_NODE DT_NODELABEL(sx1280_tx)
#define SX1280_RX_NODE DT_NODELABEL(sx1280_rx)

const struct device *spi_dev_tx = DEVICE_DT_GET(DT_BUS(SX1280_TX_NODE));
const struct device *spi_dev_rx = DEVICE_DT_GET(DT_BUS(SX1280_RX_NODE));

// CS GPIO specifications
static const struct gpio_dt_spec cs_gpio_tx = GPIO_DT_SPEC_GET(SX1280_TX_NODE, cs_gpios);
static const struct gpio_dt_spec cs_gpio_rx = GPIO_DT_SPEC_GET(SX1280_RX_NODE, cs_gpios); */

void init(void)
{
    // Initialize LEDS
    int ret;
    
    ret = robo_led_init();
    if (ret < 0) {
        LOG_ERR("Failed to initialize LEDs\n");
        return;
    }
  /*   // Initialize SPI for SX1280
    if (!device_is_ready(spi_dev_tx) || !device_is_ready(spi_dev_rx)) {
        LOG_INF("SPI device not ready");
        return;
    }

    // Initialize CS GPIO
    if (!device_is_ready(cs_gpio_tx.port) || !device_is_ready(cs_gpio_rx.port)) {
        LOG_INF("CS GPIO device not ready");
        return;
    }
    gpio_pin_configure_dt(&cs_gpio_tx, GPIO_OUTPUT_INACTIVE);
    gpio_pin_configure_dt(&cs_gpio_rx, GPIO_OUTPUT_INACTIVE);

    // Initialize SX1280
    if (sx1280_init(spi_dev_tx, &cs_gpio_tx) != 0 || sx1280_init(spi_dev_rx, &cs_gpio_rx) != 0) {
        LOG_INF("Failed to initialize SX1280");
        return;
    }
*/
    LOG_INF("Modules initialized successfully"); 

}
