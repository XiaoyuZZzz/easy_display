/*
 * @file         : lcd_driver.c
 * @Author       : shuyu
 * @LastEditTime : 2025-08-17 00:05
 * @Description  : lcd_driver的驱动文件
 * @note:        : FIX:   @sy   2025-08-16      不同的屏幕大小不同，如果有的屏幕大于256，那么需要传输两个字节，因此添加声明
 */
#include "lcd_driver.h"

// TODO:目前无法支持多个SPI—LCD设备
static SPI_DEV spi_dev = {
    .spi_init = SPI_INIT,
    .spi_transbyte = SPI_SWAP_BYTE,
    .set_cs = SPI_SET_CS,
    .set_dc = SPI_SET_DC
};

/**
 * @fuunciton:      spi_write_reg
 * @brief:          写命令函数
 * @param{data}:    写入的命令数据
 * @retval:         NULL
 */
static void spi_write_cmd(uint8_t cmd) {
    spi_dev.set_dc(0);
    spi_dev.set_cs(0);
    spi_dev.spi_transbyte(cmd);
    spi_dev.set_cs(1);
    spi_dev.set_dc(1);
}

/**
 * @fuunciton:      spi_write_data
 * @brief:          写数据函数
 * @param{data}:    写入的具体的数据
 * @retval:         NULL
 */
static void spi_write_data(uint8_t data) {
    spi_dev.set_dc(1);
    spi_dev.set_cs(0);
    spi_dev.spi_transbyte(data);
    spi_dev.set_cs(1);
    spi_dev.set_dc(1);
}


/**
 * @funciton:       spi_write_double_data
 * @breif:          写入两个字节的数据
 * @param{data}:    256以上
 * @retval:         NULL
 */
static void spi_write_double_data(uint16_t data) {
    spi_write_data(data>>8);
    spi_write_data(data);
}


// TODO:目前无法支持多个SPI—LCD设备
void lcd_fn_register(BASE_SPI* lcd_dev) {
    lcd_dev->dev = &spi_dev;
    lcd_dev->write_cmd = spi_write_cmd;
    lcd_dev->write_data = spi_write_data;
    lcd_dev->read_busy = SPI_READ_BUSY;
    lcd_dev->set_reset = SPI_SET_RESET;
}



