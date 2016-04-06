#ifndef TERASIC_TOUCHPANEL_H_
#define TERASIC_TOUCHPANEL_H_
#include "alt_types.h"
typedef void *TOUCH_HANDLE;

#define TRUE 1
#define FALSE 0



TOUCH_HANDLE Touch_Init(const alt_u32 spi_base, const alt_u32 penirq_base, const alt_u32 penirq_irq);
void Touch_UnInit(TOUCH_HANDLE pHandle);
int Touch_GetXY(TOUCH_HANDLE p, int *x, int *y);
void Touch_EmptyFifo(TOUCH_HANDLE pHandle);


#endif /*TERASIC_TOUCHPANEL_H_*/
