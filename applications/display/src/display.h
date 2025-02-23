// display.h
#ifndef DISPLAY_H
#define DISPLAY_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/spi.h>

// Display dimensions
#define DISPLAY_WIDTH  240
#define DISPLAY_HEIGHT 320

// Color definitions
#define COLOR_BLACK     0x0000
#define COLOR_WHITE     0xFFFF
#define COLOR_RED       0xF800
#define COLOR_GREEN     0x07E0
#define COLOR_BLUE      0x001F
#define COLOR_YELLOW    0xFFE0
#define COLOR_CYAN      0x07FF
#define COLOR_MAGENTA   0xF81F

// Display commands
#define DISPLAY_CMD_NOP        0x00
#define DISPLAY_CMD_SWRESET    0x01
#define DISPLAY_CMD_SLPIN      0x10
#define DISPLAY_CMD_SLPOUT     0x11
#define DISPLAY_CMD_DISPOFF    0x28
#define DISPLAY_CMD_DISPON     0x29
#define DISPLAY_CMD_CASET      0x2A
#define DISPLAY_CMD_RASET      0x2B
#define DISPLAY_CMD_RAMWR      0x2C
#define DISPLAY_CMD_MADCTL     0x36
#define DISPLAY_CMD_COLMOD     0x3A

// Function declarations
int display_init(void);
void display_write_cmd(uint8_t cmd);
void display_write_data(uint8_t *data, size_t len);
void display_set_window(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void display_clear(uint16_t color);
void display_draw_pixel(uint16_t x, uint16_t y, uint16_t color);
void display_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);

#endif /* DISPLAY_H */