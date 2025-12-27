/**
 * @file: sc_conf.h
 * @brief: 对外进行宏定义开启关闭
 * @info: v1.0  2025-7-6    @sy
 * @changelog: 1.添加RTT调试部分
 */

#ifndef SC_CONF_H__
#define SC_CONF_H__

#include "user_conf.h"

#include <stdarg.h>
#include <string.h>
#include <stdint.h>

/******************************LOG*********************************/
/**
 * 开启调试
 * 支持DEBUG_RTT、DEBUG_UART、DEBUG_FLASH
 * TODO:目前只支持前两个调试方法、FLASH使用异常
 */

#ifdef _WIN32
#define ALIGN_4 __declspec(align(4))
#else
#define ALIGN_4 __attribute__((aligned(4)))
#endif

#define DEBUG_ENABLE (1)
/*
 * |DEBUG_ENABLE
 * |->DEBUG
 * |-->DEBUG_RTT
 * |-->DEBUG_UART
 * |-->DEBUG_FLASH
 * |-->warning message
 */
#define LOG_PATH_ENABLE (1) // 是否开启文件路径及函数调用

/**日志类型 */
enum { INFO, ERR, WARN };

// TODO:日志部分调试完成之后需要修改成NOT_SUPPORT

#if DEBUG_ENABLE
#define NOT_SUPPORT (1 << 0)
#define DEBUG_RTT_BIT (1 << 1)
#define DEBUG_UART_BIT (1 << 2)
#define DEBUG_FLASH_BIT (1 << 3)
#define DEBG_ALL_BIT (DEBUG_UART_BIT | DEBUG_RTT_BIT | DEBUG_FLASH_BIT)
#define DEBUG_TARGETS (DEBUG_RTT_BIT)

#if (DEBUG_TARGETS & DEBUG_RTT_BIT)
#define ERR_TERMINAL_ID (0)
#define ERR_CHANNEL_INDEX (0)
#define INFO_TERMINAL_ID (1)
#define INFO_CHANNEL_INDEX (0)
#define RAW_TERMINAL_ID (2)
#define RAW_CHANNEL_INDEX (0)
#define WARN_TERMINAL_ID (3)
#define WARN_CHANNEL_INDEX (0)

#define RTT_DEBUG_ENABLE
#endif
#if (DEBUG_TARGETS & DEBUG_UART_BIT)
#define UART_DEBUG_ENABLE

/**用户API接口定义*/
#define SEND_CHAR send_char     // 注册用户底层的发送字节函数
#define SUPPORT_DEBUG_LEN (128) // 默认支持128字节的长度调试

#endif
#if (DEBUG_TARGETS & DEBUG_FLASH_BIT)

#define FLASH_DEBUG_ENABLE

#define SUPPORT_MAX_WRITE_SIZE (16) // 支持最大写入的数据大小
#define WRITE_FLAG (0xAA)           // 写入标志位

#define FLASH_USER_START_ADDR (0x08000000ul) // 默认FLASH的起始地址为0x08000000
#define FLASH_APP_SIZE (1024 * 100)          // 默认FLASH中APP区大小为100K字节

/*如果使用ST芯片，API为擦除扇区，最好是扇区的起始地址，起始地址用来存放一个表格结构体，作为信息管理 */
/*默认采用的是STM32F4的最后一个扇区作为开发测试 */
#define FLASH_LOG_START_ADDR (0x08040000)
#define FLASH_START_ADDR (FLASH_LOG_START_ADDR)   // 默认FLASH日志起始地址
#define FLAHS_SUPPOR_SIZE (1024 * 6)              // 默认FLASH日志支持6K字节的存储
#define FLASH_SECTOR_SIZE (FLAHS_SUPPOR_SIZE / 3) // 默认每个模块大小为2K字节

/* 用户注册函数 */
#define FLASH_WRITE_FUNC (mcu_flash_wirte)
#define FLASH_READ_FUNC (mcu_flash_read)
#define FLASH_ERASE_FUNC (mcu_flash_erase)

/**错误信息*/
enum {
    LOG_OK,
    LOG_ERROR,
    LOG_INFO_SIZE_FULL,
    LOG_ERR_SIZE_FULL,
    LOG_RAWN_SIZE_FULL,
    LOG_INFO_FAIL,
    LOG_ERR_FAIL,
    LOG_WARN_FAIL,
    LOG_INFO_INDX_LARGER,
    LOG_ERR_INDX_LARGER,
    LOG_WARN_INDX_LARGER,
};

#endif
#if (DEBUG_TARGETS & NOT_SUPPORT)
#warning "Debug functionality is disabled"
#endif

#endif
/***********************RING_BUFFER******************************/
#define RING_BUFFER_ENABLE (1)
#if RING_BUFFER_ENABLE

/**设置环形缓冲区的大小*/
#define RING_BUFFER_SIZE (256)
/**环形缓冲区设置写入方式，默认一字节写入*/
#define PUSH_CHAR (0)
#define PUSH_STRING (1)

#endif
/******************************DRIVER*********************************/

// 默认开启驱动，且开启时是同时支持IIC和SPI
#define AGREEMENT_ENABLE (1)

#define USE_EPD (0)
#define USE_LCD (1)

#if AGREEMENT_ENABLE

#define NOT_AGREEMENT (1 << 0)
#define AGREEMENT_SPI (1 << 1)
#define AGREEMENT_IIC (1 << 2)
#define AGREEMENT_ALL (AGREEMENT_IIC | AGREEMENT_SPI)
#define AGREEMENT_TARGETS (AGREEMENT_ALL)

#if (AGREEMENT_TARGETS & AGREEMENT_SPI)

#define DELAY_MS(tick) \
    do { \
        uint32_t start = get_system_tick(); \
        while ((uint32_t)(get_system_tick() - start) < (tick)); \
    } while (0);

// 0/1表示竖屏     2/3表示横屏
#define SET_SCREEN_ORIENTATION (0)

#if (SET_SCREEN_ORIENTATION == 0) || (SET_SCREEN_ORIENTATION == 1)
#define LCD_W LCD_WIDTH
#define LCD_H LCD_HEIGHT
#else
#define LCD_W LCD_HEIGHT
#define LCD_H LCD_WIDTH
#endif

// 用户的屏幕大小

// 注册用户的屏幕大小
#if USE_EPD
#define LCD_WIDTH (152)
#define LCD_HEIGHT (152)
#else
#define LCD_WIDTH (240)
#define LCD_HEIGHT (284)
#endif

#define AGREEMENT_SPI_ENABLE

// 输入用户的SPI初始化函数
#define SPI_INIT spi_gpio_init
// 输入用户的SPI发送函数
#define SPI_SWAP_BYTE spi_transbyte
// 输入用户设置CS函数
#define SPI_SET_CS spi_set_cs
// 输入用户设置DC函数
#define SPI_SET_DC spi_set_dc

#define NEED_JUD_BUSY (1)
#define NEED_SIPPORT_RESET (1)
#define NEED_SPIPORT_BL (1)

// 如果需要BUSY线，那么需要开启宏定义
#if NEED_JUD_BUSY
#define SPI_READ_BUSY spi_read_busy
#endif
// 如果需要支持RESET引脚，那么需要开启宏定义
#if NEED_SIPPORT_RESET
#define SPI_SET_RESET spi_set_reset
#endif

// 如果需要支持BL背光引脚，那么需要开启宏定义
#if NEED_SPIPORT_BL
#define SPI_SET_BLE spi_set_bl
#endif

#endif

#if (AGREEMENT_TARGETS & AGREEMENT_IIC)
#define AGREEMENT_IIC_ENABLE

#define SSD1306_ENABLE (0)

#endif

#endif

#endif
