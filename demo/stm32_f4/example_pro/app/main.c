/*
 * @Author       : SuperYu 824229900@qq.com
 * @Date         : 2025-07-06 15:51
 * @LastEditors  : Please set LastEditors
 * @LastEditTime : 2025-08-17 21:58
 * @Description  : 
 */
#include "board.h"
#include <stdio.h>

#include "user_conf.h"

#include "lcd_ui.h"
#include "pic.h"


UART_HANDLER usart_handler;

int main(void)
{

	board_init();
	uart1_init(115200U);	
	tick_timer_init();
	buffer_init(&usart_handler);
	ui_init();
	//ui_show_picture(0,0,152,152,gImage_3,BLACK);
	SPI_SET_BLE(1);
	
	TFT_full_color(WHITE);
	ui_show_string(30,50,"hello world",16,0x001F);
	//LCD_ShowString(200, 150, "STM32 Display", 0xFFFF, 0x001F, 24, 1);
	//LCD_FillRect_FastStatic(30,30,50,50,0XFFE0);
#if 1
	uint8_t temp_buf[RING_BUFFER_SIZE];
	uint8_t cmp_buf[RING_BUFFER_SIZE];
#endif
	

	while(1)
	{
		//ui_sleep();
/*用于测试自定义段数据是否能够正常处理*/		
#if 1
		uint16_t num = usart_handler.get_data(temp_buf, sizeof(temp_buf));
		memset(cmp_buf,0,RING_BUFFER_SIZE);
		c_memcpy(cmp_buf,temp_buf,num);
		if(num > 0 ) {
			process_command(cmp_buf);
		}
		
		// 添加短暂延时防止CPU满载
		delay_ms(1000);
//		TFT_full_color(0XFFE0);
//		delay_ms(1000);
//		TFT_full_color(0X07FF);
//		delay_ms(1000);
//		TFT_full_color(0xF800);
//		delay_ms(1000);
//		TFT_full_color(0xF81F);
//		delay_ms(1000);
//		TFT_full_color(0x07E0);
//		delay_ms(1000);
//		TFT_full_color(0x7FFF);
		
#endif
#if 0
		
		LOG(INFO,RED,"info1\r\n");
		delay_ms(1000);
		LOG(ERR,RED,"err1\r\n");
		delay_ms(1000);
		LOG(WARN,RED,"warn1\r\n");
		delay_ms(1000);
		
		LOG(INFO,GREEN,"info2\r\n");
		delay_ms(1000);
		LOG(ERR,GREEN,"err2\r\n");
		delay_ms(1000);
		LOG(WARN,GREEN,"warn2\r\n");
		delay_ms(1000);
		
		LOG(INFO,BLUE,"info2\r\n");
		delay_ms(1000);
		LOG(ERR,BLUE,"err2\r\n");
		delay_ms(1000);
		LOG(WARN,BLUE,"warn2\r\n");
		delay_ms(1000);
#endif		
	}
}
