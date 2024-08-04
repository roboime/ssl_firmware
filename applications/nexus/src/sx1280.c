#include <zephyr/kernel.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/drivers/gpio.h>
#include "sx1280.h"
#include <zephyr/logging/log.h>

#define SX1280_RESET_PIN 11  // Adjust as needed

LOG_MODULE_REGISTER(antenna, LOG_LEVEL_DBG);

static const struct device *spi_device;
static const struct gpio_dt_spec *cs_gpio;
static struct gpio_dt_spec reset_gpio = GPIO_DT_SPEC_GET_OR(DT_NODELABEL(sx1280), reset_gpios, {0});

int sx1280_init(const struct device *spi_dev, const struct gpio_dt_spec *cs)
{
    spi_device = spi_dev;
    cs_gpio = cs;

    // Configure reset GPIO
    if (!device_is_ready(reset_gpio.port)) {
        LOG_INF("Reset GPIO device not ready");
        return -1;
    }
    gpio_pin_configure_dt(&reset_gpio, GPIO_OUTPUT_INACTIVE);

    // Reset SX1280
    gpio_pin_set_dt(&reset_gpio, 1);
    k_sleep(K_MSEC(10));
    gpio_pin_set_dt(&reset_gpio, 0);
    k_sleep(K_MSEC(10));

    // Initialize SX1280 registers here
    // This would involve sending SPI commands to configure the device
    // Example (pseudo-code):
    // sx1280_write_register(REG_OPERATING_MODE, MODE_LORA);
    // sx1280_write_register(REG_FREQUENCY, FREQ_915_MHZ);
    // ... (more configuration)

    return 0;
}

int sx1280_send(const uint8_t *data, size_t len)
{
    // Implement SX1280 send functionality
    // This would involve writing to the FIFO and triggering transmission
    return 0;
}

int sx1280_receive(uint8_t *data, size_t max_len)
{
    // Implement SX1280 receive functionality
    // This would involve reading from the FIFO when data is available
    return 0;
}
/* 
// Helper functions for SPI communication
static int sx1280_write_register(uint8_t reg, uint8_t value)
{
    uint8_t tx_data[2] = {reg | 0x80, value};  // Set write bit
    const struct spi_buf tx_buf = {.buf = tx_data, .len = sizeof(tx_data)};
    const struct spi_buf_set tx = {.buffers = &tx_buf, .count = 1};

    gpio_pin_set_dt(cs_gpio, 0);  // Assert CS
    int ret = spi_write(spi_device, &tx);
    gpio_pin_set_dt(cs_gpio, 1);  // Deassert CS

    return ret;
}

static int sx1280_read_register(uint8_t reg, uint8_t *value)
{
    uint8_t tx_data[2] = {reg & 0x7F, 0};  // Clear write bit
    uint8_t rx_data[2];
    const struct spi_buf tx_buf = {.buf = tx_data, .len = sizeof(tx_data)};
    const struct spi_buf rx_buf = {.buf = rx_data, .len = sizeof(rx_data)};
    const struct spi_buf_set tx = {.buffers = &tx_buf, .count = 1};
    const struct spi_buf_set rx = {.buffers = &rx_buf, .count = 1};

    gpio_pin_set_dt(cs_gpio, 0);  // Assert CS
    int ret = spi_transceive(spi_device, &tx, &rx);
    gpio_pin_set_dt(cs_gpio, 1);  // Deassert CS

    if (ret == 0) {
        *value = rx_data[1];
    }

    return ret;
}
 */