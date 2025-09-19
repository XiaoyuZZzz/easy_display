/*
 * @file         : 
 * @Author       : shuyu
 * @LastEditTime : 2025-08-17 23:09
 * @Description  : 
 */

#include "lcd_ui.h"
#include "font.h"


static LCD_DRIVER ui_driver;
static PAINT paint;

static uint8_t img_bw[2888];  // EPD
//static uint16_t lcd_buffer[LCD_WIDTH * LCD_HEIGHT * sizeof(uint16_t)];   //LCD


/**
 * @funciton:   ui_clear
 * @brief:      擦除屏幕
 */
void ui_clear(uint16_t color) {
    ui_driver.clear(color);
}

/**
 * @function:       ui_buf_claer
 * @breif:          清空缓冲数组
 * @param{color}    颜色
 * @retval:         NULL
 */
void ui_buf_clear(uint8_t Color) {
  uint16_t x,y;
  uint32_t addr;
  for(y = 0;y < paint.heightByte; y++) 
  {
    for(x=0;x < paint.widthByte; x++ ) 
    {   
      addr = x + (y * paint.widthByte);         //8 pixel =  1 byte
      paint.dis_buffer[addr] = Color;
    }
  }
}

/**
 * @function:       ui_new_img
 * @brief:          创建一个画布
 * @param{image}    传入的缓冲区
 * @param{width}    创建的大小
 * @param{height}   创建的大小
 * @param{rotate}   方向
 * @param{color}    颜色
 */
static void ui_new_img(uint8_t *image,uint16_t Width,uint16_t Height,uint16_t Rotate,uint16_t Color)
{
  paint.dis_buffer = 0x00;
  paint.dis_buffer = image;
  paint.color = Color;  
  paint.widthMemory = Width;
  paint.heightMemory = Height;  
  paint.widthByte = (Width % 8 == 0)? (Width / 8 ): (Width / 8 + 1);
  paint.heightByte = Height;     
  paint.rotate = Rotate;
  if((Rotate == 0) || (Rotate == 180)) {
    paint.width=Height;
    paint.height=Width;
  }else {
    paint.width = Width;
    paint.height = Height;
  }

  ui_buf_clear(WHITE);
}   



/**
 * @funciton:       ui_set_pixel
 * @breif:          显示一个像素点
 * @param{xpoint}   x的起始地址
 * @param{ypoint}   y的起始地址
 * @param{color}    颜色
 */

static void ui_set_pixel(uint16_t xpoint,uint16_t ypoint,uint16_t color) {
    if(ui_driver.dev_type == DEV_EPD) {
      uint16_t x, y;
      uint32_t addr;
      uint8_t r_data;
          switch(paint.rotate) {
          case 0:
              x = xpoint;
              y = paint.heightMemory - ypoint - 1;    
              break;
          case 90:
              x = paint.widthMemory - ypoint - 1;
              y = paint.heightMemory - xpoint - 1;
              break;
          case 180:
              x = paint.widthMemory - xpoint - 1;
              y = ypoint;
              break;
          case 270:
              x = ypoint;
              y = xpoint;
              break;
              default:
                  return;
          }
          addr = (x / 8) + (y * paint.widthByte);
          r_data = paint.dis_buffer[addr];
          if(color == BLACK) {    
              paint.dis_buffer[addr] = r_data & ~(0x80>>(x % 8)); //将对应数据位置0
          }
          else{
              paint.dis_buffer[addr] = r_data | (0x80>>(x % 8));   //将对应数据位置1  
          }
    } else {
        if(ui_driver.set_pixel) {
              ui_driver.set_pixel(xpoint, ypoint, color);
          }
    }
    
 }


/**
 * @function:   ui_init
 * @brief:      用户使用接口ui初始化
 * @param{NULL} NULL
 */
void ui_init(void) {
#if USE_EPD
    ssd1680_driver_init_callback(&ui_driver);
#endif
#if USE_LCD
    /**Add callback funciton */
    st7789_driver_init_callback(&ui_driver);
#endif
    ui_driver.ui_init();
    if(ui_driver.dev_type == DEV_EPD) {
      ui_new_img(img_bw,LCD_WIDTH,LCD_HEIGHT,0,WHITE);
    }else {
//      for(int i = 0; i < LCD_WIDTH * LCD_HEIGHT; i++) {
//            lcd_buffer[i] = 0xFFFF; // RGB565白色
//        }
    }
    
} 


/**
 * @funciton:       ui_disp
 * @brief:          显示
 * @param{buf}:     需要显示的缓冲区
 * @retval:         NULL
 */

void ui_disp(const uint8_t* buf) {
    if(ui_driver.disp) {
      ui_driver.disp(buf);
    }
}

/**
 * @function:   ui_updata
 * @brief:      更新
 * @param{NULL} NULL
 * @retval:     NULL
 */
void ui_updata(void) {
    if(ui_driver.update) {
      ui_driver.update();
    }
    
}


/**
 * @function:   ui_show_char
 * @brief:      打印一个字符（兼容EPD和LCD）
 * @param{x}    x起始地址
 * @param{y}    y起始地址
 * @param{chr}  传入的字符
 * @param{size} 字体的大小
 * @param{color}字体的颜色（对于LCD是RGB565）
 * @retval:     NULL
 */
void ui_show_char(uint16_t x, uint16_t y, uint16_t chr, uint16_t size1, uint16_t color)
{
    uint16_t i, m, temp, size2, chr1;
    uint16_t x0, y0;
    x0 = x;
    y0 = y;
    
    // 计算字体数据大小
    if (ui_driver.dev_type == DEV_EPD) {
        if (size1 == 8) {
            size2 = 6;
        } else {
            size2 = (size1 / 8 + ((size1 % 8) ? 1 : 0)) * (size1 / 2);
        }
    } else {
        // LCD使用完整字体数据
        size2 = (size1 == 8) ? 8 : size1;
    }
    
    chr1 = chr - ' ';  // 计算偏移后的值
    
    for (i = 0; i < size2; i++) {
        // 获取字体数据
        if (size1 == 8) {
            temp = asc2_0806[chr1][i];
        } else if (size1 == 12) {
            temp = asc2_1206[chr1][i];
        } else if (size1 == 16) {
            temp = asc2_1608[chr1][i];
        } else if (size1 == 24) {
            temp = asc2_2412[chr1][i];
        } else if (size1 == 48) {
            temp = asc2_4824[chr1][i];
        } else {
            return;
        }
        
        for (m = 0; m < 8; m++) {
            if (temp & 0x01) {
                if (ui_driver.dev_type == DEV_EPD) {
                    ui_set_pixel(x, y, color);
                } else {
                    // LCD直接使用RGB565颜色
                    ui_driver.set_pixel(x, y, color);
                }
            } else {
                if (ui_driver.dev_type == DEV_EPD) {
                    ui_set_pixel(x, y, !color);
                } else {
                    // LCD使用背景色（白色）
                    ui_driver.set_pixel(x, y, 0xFFFF);
                }
            }
            temp >>= 1;
            y++;
        }
        
        x++;
        if ((size1 != 8) && ((x - x0) == size1 / 2)) {
            x = x0;
            y0 = y0 + 8;
        }
        y = y0;
    }
}



/**
 * @function:   ui_show_string
 * @brief:      显示字符串
 * @param{x}    起始x
 * @param{y}    起始y
 * @param{chr}  显示的字符串
 * @param{size} 显示的大小
 * @param{color} 显示的颜色
 */
void ui_show_string(uint16_t x,uint16_t y,uint8_t* chr,uint16_t size1,uint16_t color) {
    while(*chr!='\0') {
        ui_show_char(x,y,*chr,size1,color);
        chr++;
        x += size1 / 2;
    }
    if(ui_driver.dev_type == DEV_EPD) {
      ui_disp(img_bw);
      ui_updata();
    }
    
    
}


/**
 * @function:	ui_show_picture
 * @brief:      显示图片
 * @param{x}    起始X坐标
 * @param{y}    起始Y坐标
 * @param{sizex} X长度
 * @param{sizey} Y长度
 * @param{BMP[]} 传入的缓冲数组    
 */

void ui_show_picture(uint16_t x,uint16_t y,uint16_t sizex,uint16_t sizey,const uint8_t* BMP,uint16_t Color)
{
    uint16_t j=0,t;
    uint16_t i,temp,y0,TypefaceNum=sizex*(sizey/8+((sizey%8)?1:0));
    y0=y;
    for(i=0;i<TypefaceNum;i++)
    {
      temp=BMP[j];
      for(t=0;t<8;t++)
      {
      if(temp&0x80)
      {
        ui_set_pixel(x,y,Color);
      }
      else
      {
        ui_set_pixel(x,y,!Color);
      }
      y++;
      temp<<=1;
      }
      if((y-y0)==sizey)
      {
        y=y0;
        x++;
      }
      j++;
    }
    if(ui_driver.dev_type == DEV_EPD) {
        ui_disp(img_bw);
        ui_updata();
    }
   
}




/**
 * @function:   ui_sleep
 * @brief:      支持睡眠的低功耗屏
 * @param{NULL} NULL
 */
void ui_sleep(void) {
    if(ui_driver.sleep) {
        ui_driver.sleep();
    }
}



//  test


void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
	uint8_t temp,sizex,t,m=0;
	uint16_t i,TypefaceNum;//一个字符所占字节大小
	uint16_t x0=x;
	sizex=sizey/2;
	TypefaceNum=(sizex/8+((sizex%8)?1:0))*sizey;
	num=num-' ';    //得到偏移后的值
	st7789_set_window(x,y,x+sizex-1,y+sizey-1);  //设置光标位置 
	for(i=0;i<TypefaceNum;i++)
	{ 
		if(sizey==12)temp=asc2_1206[num][i];		       //调用6x12字体
		else if(sizey==16)temp=asc2_1608[num][i];		 //调用8x16字体
		else if(sizey==24)temp=asc2_2412[num][i];		 //调用12x24字体
		else return;
		for(t=0;t<8;t++)
		{
				if(temp&(0x01<<t)) ui_set_pixel(x,y,fc);//画一个点
				x++;
				if((x-x0)==sizex)
				{
					x=x0;
					y++;
					break;
				}
		}
	}   	 	  
}

void LCD_ShowString(uint16_t x,uint16_t y,const uint8_t *p,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{         
	while(*p!='\0')
	{       
		LCD_ShowChar(x,y,*p,fc,bc,sizey,mode);
		x+=sizey/2;
		p++;
	}  
}


 #define MAX_BUFFER_SIZE 512

 void LCD_FillRect_FastStatic(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
	
    st7789_set_window(x0, y0, x1, y1); // 设置显示范围

    uint32_t pixelCount = (x1 - x0 + 1) * (y1 - y0 + 1);
    uint8_t colorHi = color >> 8;
    uint8_t colorLo = color & 0xFF;

    // 静态缓冲区（分块传输）
    uint8_t buffer[MAX_BUFFER_SIZE * 2];  // 每个像素2字节
    uint32_t pixelsPerBlock = MAX_BUFFER_SIZE / 2;
    uint32_t remaining = pixelCount;

     SPI_SET_CS(0);
     SPI_SET_DC(1);

    while (remaining > 0) {
        uint32_t currentPixels = (remaining > pixelsPerBlock) ? pixelsPerBlock : remaining;
        
        // 填充当前块
        for (uint32_t i = 0; i < currentPixels; i++) {
            buffer[i * 2] = colorHi;
            buffer[i * 2 + 1] = colorLo;
        }

        // 发送当前块
        for(uint16_t num = 0; num < currentPixels * 2; num++) {
            spi_transbyte(buffer[num]);
        }
        remaining -= currentPixels;
    }
	
     SPI_SET_CS(1);
}
 
void TFT_full_color(unsigned int color)
{
	unsigned int ROW,column;
	
	st7789_set_window(0,0,240,284);
	for(ROW=0;ROW<284;ROW++)             //ROW loop
	{ 
		for(column=0;column<240;column++) //column loop
		{
			spi_transbyte(color>>8);
			spi_transbyte(color);
		}
	}
}


