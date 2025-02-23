// display.c
#include "display.h"
#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/sys/util.h>

LOG_MODULE_REGISTER(display, CONFIG_LOG_DEFAULT_LEVEL);

static const struct device *spi_dev;
static const struct device *reset_gpio;
static const struct device *dc_gpio;
static const struct device *cs_gpio;

static struct spi_config spi_cfg = {
    .frequency = 10000000,
    .operation = SPI_OP_MODE_MASTER | SPI_WORD_SET(8) | SPI_TRANSFER_MSB,
};

int display_init(void) {
    /* Get SPI device */
    spi_dev = DEVICE_DT_GET(DT_NODELABEL(spi1));
    if (!device_is_ready(spi_dev)) {
        LOG_ERR("SPI device not ready");
        return -ENODEV;
    }

    /* Get GPIO devices */
    dc_gpio = DEVICE_DT_GET(DT_GPIO_CTLR(DT_NODELABEL(lcd_display), dc_gpios));
    reset_gpio = DEVICE_DT_GET(DT_GPIO_CTLR(DT_NODELABEL(lcd_display), reset_gpios));
    cs_gpio = DEVICE_DT_GET_OR_NULL(DT_GPIO_CTLR(DT_NODELABEL(lcd_display), cs_gpios));

    if (!device_is_ready(dc_gpio) || !device_is_ready(reset_gpio)) {
        LOG_ERR("GPIO devices not ready");
        return -ENODEV;
    }
    
    /* Configure GPIOs */
    gpio_pin_configure(dc_gpio, 
        DT_GPIO_PIN(DT_NODELABEL(lcd_display), dc_gpios),
        GPIO_OUTPUT_ACTIVE | DT_GPIO_FLAGS(DT_NODELABEL(lcd_display), dc_gpios));
    
    gpio_pin_configure(reset_gpio,
        DT_GPIO_PIN(DT_NODELABEL(lcd_display), reset_gpios),
        GPIO_OUTPUT_ACTIVE | DT_GPIO_FLAGS(DT_NODELABEL(lcd_display), reset_gpios));

    if (cs_gpio != NULL) {
        gpio_pin_configure(cs_gpio,
            DT_GPIO_PIN(DT_NODELABEL(lcd_display), cs_gpios),
            GPIO_OUTPUT_ACTIVE | DT_GPIO_FLAGS(DT_NODELABEL(lcd_display), cs_gpios));
    }

    /* Reset sequence */
    gpio_pin_set(reset_gpio, 
        DT_GPIO_PIN(DT_NODELABEL(lcd_display), reset_gpios), 0);
    k_msleep(10);
    gpio_pin_set(reset_gpio,
        DT_GPIO_PIN(DT_NODELABEL(lcd_display), reset_gpios), 1);
    k_msleep(120);

    /* Initialize display */
    display_write_cmd(DISPLAY_CMD_SWRESET);
    k_msleep(120);
    
    display_write_cmd(DISPLAY_CMD_SLPOUT);
    k_msleep(120);
    
    display_write_cmd(DISPLAY_CMD_COLMOD);
    {
        uint8_t color_mode = 0x55; /* 16-bit color */
        display_write_data(&color_mode, 1);
    }
    
    display_write_cmd(DISPLAY_CMD_MADCTL);
    {
        uint8_t madctl = 0x08; /* Normal orientation */
        display_write_data(&madctl, 1);
    }
    
    display_write_cmd(DISPLAY_CMD_DISPON);

    return 0;
}

void display_write_cmd(uint8_t cmd) {
    gpio_pin_set(dc_gpio, 
        DT_GPIO_PIN(DT_NODELABEL(lcd_display), dc_gpios), 0);
    
    if (cs_gpio != NULL) {
        gpio_pin_set(cs_gpio,
            DT_GPIO_PIN(DT_NODELABEL(lcd_display), cs_gpios), 0);
    }
    
    struct spi_buf tx_buf = {
        .buf = &cmd,
        .len = 1
    };
    struct spi_buf_set tx_bufs = {
        .buffers = &tx_buf,
        .count = 1
    };
    
    spi_write(spi_dev, &spi_cfg, &tx_bufs);

    if (cs_gpio != NULL) {
        gpio_pin_set(cs_gpio,
            DT_GPIO_PIN(DT_NODELABEL(lcd_display), cs_gpios), 1);
    }
}

void display_write_data(uint8_t *data, size_t len) {
    gpio_pin_set(dc_gpio,
        DT_GPIO_PIN(DT_NODELABEL(lcd_display), dc_gpios), 1);
    
    if (cs_gpio != NULL) {
        gpio_pin_set(cs_gpio,
            DT_GPIO_PIN(DT_NODELABEL(lcd_display), cs_gpios), 0);
    }
    
    struct spi_buf tx_buf = {
        .buf = data,
        .len = len
    };
    struct spi_buf_set tx_bufs = {
        .buffers = &tx_buf,
        .count = 1
    };
    
    spi_write(spi_dev, &spi_cfg, &tx_bufs);

    if (cs_gpio != NULL) {
        gpio_pin_set(cs_gpio,
            DT_GPIO_PIN(DT_NODELABEL(lcd_display), cs_gpios), 1);
    }
}

void display_set_window(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    uint8_t data[4];
    
    display_write_cmd(DISPLAY_CMD_CASET);
    data[0] = x1 >> 8;
    data[1] = x1 & 0xFF;
    data[2] = x2 >> 8;
    data[3] = x2 & 0xFF;
    display_write_data(data, 4);
    
    display_write_cmd(DISPLAY_CMD_RASET);
    data[0] = y1 >> 8;
    data[1] = y1 & 0xFF;
    data[2] = y2 >> 8;
    data[3] = y2 & 0xFF;
    display_write_data(data, 4);
    
    display_write_cmd(DISPLAY_CMD_RAMWR);
}

void display_clear(uint16_t color) {
    uint8_t high = color >> 8;
    uint8_t low = color & 0xFF;
    
    display_set_window(0, 0, DISPLAY_WIDTH - 1, DISPLAY_HEIGHT - 1);
    
    /* Write pixels in chunks to avoid stack overflow */
    #define CHUNK_SIZE 32
    uint8_t chunk[CHUNK_SIZE * 2];
    for (int i = 0; i < CHUNK_SIZE * 2; i += 2) {
        chunk[i] = high;
        chunk[i + 1] = low;
    }
    
    int pixels_remaining = DISPLAY_WIDTH * DISPLAY_HEIGHT;
    while (pixels_remaining > 0) {
        int chunk_pixels = MIN(CHUNK_SIZE, pixels_remaining);
        display_write_data(chunk, chunk_pixels * 2);
        pixels_remaining -= chunk_pixels;
    }
}

void display_draw_pixel(uint16_t x, uint16_t y, uint16_t color) {
    if (x >= DISPLAY_WIDTH || y >= DISPLAY_HEIGHT) {
        return;
    }
    
    display_set_window(x, y, x, y);
    
    uint8_t data[2] = {color >> 8, color & 0xFF};
    display_write_data(data, 2);
}

void display_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    if (x >= DISPLAY_WIDTH || y >= DISPLAY_HEIGHT) {
        return;
    }
    
    /* Clip rectangle dimensions */
    if (x + w > DISPLAY_WIDTH) {
        w = DISPLAY_WIDTH - x;
    }
    if (y + h > DISPLAY_HEIGHT) {
        h = DISPLAY_HEIGHT - y;
    }
    
    display_set_window(x, y, x + w - 1, y + h - 1);
    
    uint8_t high = color >> 8;
    uint8_t low = color & 0xFF;
    
    /* Write pixels in chunks to avoid stack overflow */
    #define CHUNK_SIZE 32
    uint8_t chunk[CHUNK_SIZE * 2];
    for (int i = 0; i < CHUNK_SIZE * 2; i += 2) {
        chunk[i] = high;
        chunk[i + 1] = low;
    }
    
    int pixels_remaining = w * h;
    while (pixels_remaining > 0) {
        int chunk_pixels = MIN(CHUNK_SIZE, pixels_remaining);
        display_write_data(chunk, chunk_pixels * 2);
        pixels_remaining -= chunk_pixels;
    }
}
