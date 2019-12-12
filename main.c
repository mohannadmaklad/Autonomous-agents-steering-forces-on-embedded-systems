#include "stm32f10x.h"                  // Device header
#include <stdlib.h>
#include "HWInit.h"
#include "lcd.h"
#include "GPIO.h"

#include "types.h"
#include "AA.h"
#include "display.h"

int main(void)
 { 
	CLK_init();
	RCC_init ();
	GPIO_init();
	SPI_init ();	
	LCD_init ();
	
	
	t_VECTOR F_pos = {1,1};
	t_VECTOR F_v = {5,5};
	
	t_VECTOR A_pos = {1,40};
	t_VECTOR A_v = {0,0};
	
	t_VECTOR A_pos2 = {80,40};
	t_VECTOR A_v2 = {0,0};
	
	t_AGENT* myAgent  			   =   AA_createAgent(&A_pos,&A_v,1,7);
	t_AGENT* myAgent2  			   =   AA_createAgent(&A_pos2,&A_v2,1,10);
	t_AGENT* fleeingTarget  	 =   AA_createAgent(&F_pos,&F_v,1,4);
	//t_AGENT* persuitinTarget   =   AA_createAgent(&P_pos,&P_v,1,5);
	
	//AA_persuit(myAgent,persuitinTarget);
	
	AA_persuit(myAgent, fleeingTarget);
	AA_persuit(myAgent2, fleeingTarget);

	
	
	while(1)
	{
		AA_updateAgentsTask();
		DISP_displayAgents();
		LCD_Update_Task();
		for(uint32_t volatile i = 0; i < 4000000 ; i++);
	}
	 
	
}

