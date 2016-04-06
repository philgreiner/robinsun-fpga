/*
 * ILI9341.h
 *
 *  Created on: 2014-2-13
 *      Author: Administrator
 */

#ifndef ILI9341_H_
#define ILI9341_H_

#include "../terasic_lib/terasic_includes.h"

#define  LCD_WR_REG(value)  IOWR(LT24_CTRL_BASE,0x00,value)
#define  LCD_WR_DATA(value)  IOWR(LT24_CTRL_BASE,0x01,value)


#define  Set_LCD_RST  IOWR_ALTERA_AVALON_PIO_DATA(LT24_LCD_RSTN_BASE,0x01)
#define  Clr_LCD_RST  IOWR_ALTERA_AVALON_PIO_DATA(LT24_LCD_RSTN_BASE,0x00)

#define RED 	0xf800
#define GREEN 	0x07e0
#define BLUE 	0x001f
#define BLACK  	0x0000
#define WHITE 	0xffff

void LCD_Init();
void LCD_Clear(alt_u16 Color);
void LCD_DrawPoint(alt_u16 x,alt_u16 y,alt_u16 color );
void LCD_Pattern_Horizon(void);
void LCD_Pattern_Vertical(void);
void Set_BUFFER_FLAG();
void Clr_BUFFER_FLAG();

#endif /* ILI9341_H_ */
