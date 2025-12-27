/*
 * @file         : 
 * @Author       : shuyu
 * @LastEditTime : 2025-08-17 20:09
 * @Description  : 
 */
#ifndef __MCU_SPI_H__
#define __MCU_SPI_H__

#include "stm32f4xx.h"

#define SPI_SCL_GPIO_PORT GPIOE
#define SPI_SCL_GPIO_PIN GPIO_Pin_7
#define SPI_SCL_GPIO_CLK RCC_AHB1Periph_GPIOE

#define SPI_SDA_GPIO_PORT GPIOE
#define SPI_SDA_GPIO_PIN GPIO_Pin_8
#define SPI_SDA_GPIO_CLK RCC_AHB1Periph_GPIOE

#define SPI_RES_GPIO_PORT GPIOE
#define SPI_RES_GPIO_PIN GPIO_Pin_9
#define SPI_RES_GPIO_CLK RCC_AHB1Periph_GPIOE

#define SPI_DC_GPIO_PORT GPIOE
#define SPI_DC_GPIO_PIN GPIO_Pin_10
#define SPI_DC_GPIO_CLK RCC_AHB1Periph_GPIOE

#define SPI_CS_GPIO_PORT GPIOE
#define SPI_CS_GPIO_PIN GPIO_Pin_11
#define SPI_CS_GPIO_CLK RCC_AHB1Periph_GPIOE

#define SPI_BUSY_GPIO_PORT GPIOE
#define SPI_BUSY_GPIO_PIN GPIO_Pin_12
#define SPI_BUSY_GPIO_CLK RCC_AHB1Periph_GPIOE

#define SPI_BL_GPIO_PORT GPIOE
#define SPI_BL_GPIO_PIN GPIO_Pin_13
#define SPI_BL_GPIO_CLK RCC_AHB1Periph_GPIOE

#define SPI_SCL_Clr() GPIO_ResetBits(SPI_SCL_GPIO_PORT, SPI_SCL_GPIO_PIN)
#define SPI_SCL_Set() GPIO_SetBits(SPI_SCL_GPIO_PORT, SPI_SCL_GPIO_PIN)

#define SPI_SDA_Clr() GPIO_ResetBits(SPI_SDA_GPIO_PORT, SPI_SDA_GPIO_PIN)
#define SPI_SDA_Set() GPIO_SetBits(SPI_SDA_GPIO_PORT, SPI_SDA_GPIO_PIN)

#define SPI_RES_Clr() GPIO_ResetBits(SPI_RES_GPIO_PORT, SPI_RES_GPIO_PIN)
#define SPI_RES_Set() GPIO_SetBits(SPI_RES_GPIO_PORT, SPI_RES_GPIO_PIN)

#define SPI_DC_Clr() GPIO_ResetBits(SPI_DC_GPIO_PORT, SPI_DC_GPIO_PIN)
#define SPI_DC_Set() GPIO_SetBits(SPI_DC_GPIO_PORT, SPI_DC_GPIO_PIN)

#define SPI_CS_Clr() GPIO_ResetBits(SPI_CS_GPIO_PORT, SPI_CS_GPIO_PIN)
#define SPI_CS_Set() GPIO_SetBits(SPI_CS_GPIO_PORT, SPI_CS_GPIO_PIN)

#define SPI_ReadBusy GPIO_ReadInputDataBit(SPI_BUSY_GPIO_PORT, SPI_BUSY_GPIO_PIN)

#define SPI_BL_Clr() GPIO_ResetBits(SPI_BL_GPIO_PORT, SPI_BL_GPIO_PIN)
#define SPI_BL_Set() GPIO_SetBits(SPI_BL_GPIO_PORT, SPI_BL_GPIO_PIN)

void spi_gpio_init(void);
uint8_t spi_transbyte(uint8_t dat); /**屏幕没有数据回应*/
void spi_set_cs(uint8_t mode);
void spi_set_dc(uint8_t mode);
uint8_t spi_read_busy(void);
void spi_set_reset(uint8_t mode);
void spi_set_bl(uint8_t mode);

/*
void SPI_WR_REG(u8 reg);	
void SPI_WR_DATA8(u8 dat);
*/

#endif
