
#include "display.h"
#include "lcd.h"
#include "AA.h"

void DISP_displayAgents(void)
{
	uint8_t x;
	uint8_t y;
	uint32_t numOfAgents;
	t_VECTOR** positions;
	
	positions   = AA_getPosArr();
	numOfAgents = AA_getNumOfAgents();
	
	LCD_resetLCD();
	
	for(uint32_t i = 0 ; i < numOfAgents ; i++)
	{
		x = positions[i]->x;
		y = positions[i]->y;	
		LCD_pxOn(x,y);
		LCD_pxOn(x+1,y);
		LCD_pxOn(x,y+1);
		LCD_pxOn(x+1,y+1);
	}
	
}


