#include "stm32f10x.h"                  // Device header

#include "lcd.h"
#include <string.h>
#include <math.h>

/*Local functions*/
void wait(void);
char setX(uint8_t x);
char setY(uint8_t y);
void LCD_cmd(uint8_t cmd);
void LCD_data(uint8_t data);
void delay(uint32_t max);

/*Local Data*/
uint16_t volatile *sr_ptr = &(SPI2->SR) ; 
uint8_t frame[LCD_SIZE];




void wait(void)
{
	while ((*sr_ptr & ((uint16_t)0x1 << 1 ) ) != 0x2 );
}
void delay(uint32_t max)
{
	uint32_t volatile counter = 0;
	while (counter++ < max);
}

void LCD_cmd(uint8_t cmd)
{
	RESET_DC;
	SPI2->DR = (cmd);
	wait();
}


void LCD_data(uint8_t data)
{
	SET_DC;
	SPI2->DR = (data);
	wait();
}


char setX(uint8_t x)
{
	if(x > 83) return 1;
	else
	{
		LCD_cmd(((uint8_t)0x1<<7) | x );
		return 0 ;
	}
}

char setY(uint8_t y)
{
	if(y > 5) return 1;
	else
	{
		LCD_cmd(((uint8_t)0x1<<6) | y );
		return 0 ;
	}
}


/*-----------------Public Interface--------------------*/

void LCD_init()
{
	RESET_DC ;
	RES;
	NRES;
	setX(0x00);
	setY(0x00);
	
	// Initial commands to init the LCD
	LCD_cmd(0x21);  // Function set : H = 1, Horizontal 
	LCD_cmd(0xb2);  // 1011 0001 - Vop register 
	LCD_cmd(0x05);  //
	LCD_cmd(0x14);  //14
	LCD_cmd(0x20);
	LCD_cmd(0x0c); // Normal mode , 0x9 turns all segments on
	LCD_cmd(0xB2);
}


void LCD_pxOn(uint8_t x , uint8_t y)
{
	if(x < 84 && y < 48)
	{
		uint8_t row = y/8 ;//row + x
		frame[row*84+x] |= (0x00 | (1 << (y%8)));
	}
	
}

void LCD_pxOff(uint8_t x , uint8_t y)
{
	if(x < 84 && y < 48)
	{
		uint8_t row = y/8 ;//row + x
		frame[row*84+x] &= ~(0x00 | (1 << (y%8)));
	}
}

void LCD_resetLCD(void)
{
	for(uint32_t i = 0 ; i < LCD_SIZE ; i++)
	{
			frame[i] = 0x00;
	}
}	

void LCD_Update_Task(void) //task
{
	setX(0x00);
	setY(0x00);
	
	delay(100);
	for(uint32_t i = 0 ; i < LCD_SIZE ; i++)
	{
			LCD_data(frame[i]);
	}
	
}
