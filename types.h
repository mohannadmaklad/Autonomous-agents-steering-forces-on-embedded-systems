#ifndef _TYPES_H_
#define _TYPES_H_

#include "stm32f10x.h"                  // Device header

typedef struct vector
{
	int32_t x;
	int32_t y;
}t_VECTOR;

typedef struct agent t_AGENT;

typedef struct agent
{
	//Data
	t_VECTOR initialPos;
	t_VECTOR initialV;
	
	t_VECTOR* pos;
	t_VECTOR* v;
	t_VECTOR* heading;
	t_VECTOR* normal;
	
	
	uint32_t mass;
	uint32_t maxSpeed;
	
	//flags
	uint8_t seeking;
	uint8_t persuiting;
	uint8_t fleeing;
	uint8_t wandering;
	
	//targets, if exist
	t_AGENT* seekingTarget;
	t_AGENT* persuitingTarget;
	t_AGENT* fleeingTarget;
	
}t_AGENT;

#endif
