/*
 * @file         : 
 * @Author       : shuyu
 * @LastEditTime : 2025-08-17 20:46
 * @Description  : 
 */

#ifndef ST7789_H__
#define ST7789_H__

#include "lcd_driver.h"

void st7789_set_window(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void st7789_driver_init_callback(LCD_DRIVER *lcd_driver);

#endif
