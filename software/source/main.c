/*
 * Author : L. Moreau, A. Willème 2016
 *
 * Based on Terasic LT24 Painter's demo
 *
 */

#include "terasic_lib/terasic_includes.h"
#include "terasic_lib/touch_spi.h"
#include "graphic_lib/alt_video_display.h"
#include "graphic_lib/ILI9341.h"
#include "demo/gui.h"

int main()
{
Clr_BUFFER_FLAG();

 alt_video_display Display;
 TOUCH_HANDLE *pTouch;

 printf("Hi There !\n");

 // Write 0x3C on LED[6:0] through the dedicated custom IP
 IOWR(LED_CTRL_BASE, 0x0, 0x3C);

 // TOUCH INITIALIZATION
 pTouch = Touch_Init(LT24_TOUCH_SPI_BASE,  LT24_TOUCH_PENIRQ_N_BASE, LT24_TOUCH_PENIRQ_N_IRQ);
 if (!pTouch){
	 printf("Failed to init touch\r\n");
 }else{
	 printf("Init touch successfully\r\n");
 }

 // LCD INITIALIZATION
 LCD_Init();

 // Pattern example
 //LCD_Pattern_Horizon();
 // Sleep 3s
 //usleep(3*1000*1000);

 Set_BUFFER_FLAG();

 unsigned int X, Y;
 unsigned int posTamper =0;
 while(1){
	 if(Touch_GetXY(pTouch, &X, &Y)){
		 //printf("X: %d Y: %d\n",X,Y);
		 LCD_WR_DATA(Y);
		 LCD_WR_REG(X);
		 //we can modify the character (the image become progressively red when we touch it), we might do the same for the background
		 IOWR_16DIRECT(PIC_MEM_BASE,posTamper,0xF800);
		 posTamper++;
	 }
 }

 // Painter demo
 /*Clr_BUFFER_FLAG();
 Display.interlace = 0;
 Display.bytes_per_pixel = 2;
 Display.color_depth = 16;
 Display.height = SCREEN_HEIGHT;
 Display.width = SCREEN_WIDTH;
 GUI(&Display, pTouch);*/

 return 0;
}
