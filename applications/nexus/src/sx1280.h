#ifndef SX1280_H
#define SX1280_H

#include <zephyr/drivers/spi.h>
#include <zephyr/drivers/gpio.h>

int sx1280_init(const struct device *spi_dev, const struct gpio_dt_spec *cs_gpio);
int sx1280_send(const uint8_t *data, size_t len);
int sx1280_receive(uint8_t *data, size_t max_len);

#endif /* SX1280_H */