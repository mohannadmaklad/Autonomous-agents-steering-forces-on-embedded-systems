#ifndef _AA_H_
#define _AA_H_

#include "stm32f10x.h"                  // Device header
#include "types.h"

t_AGENT*				AA_createAgent(t_VECTOR* pos, t_VECTOR* velocity, uint32_t mass, uint32_t maxSpeed);
void						AA_seek(t_AGENT* moving, t_AGENT* target);
void						AA_flee(t_AGENT* moving, t_AGENT* target);
void 						AA_persuit(t_AGENT* moving, t_AGENT* target);
uint32_t 				AA_getNumOfAgents(void);
t_VECTOR** 			AA_getPosArr(void);

void						AA_updateAgentsTask(void); //task
	
#endif


