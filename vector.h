#ifndef _VECTOR_H_
#define _VECTOR_H_


#include "stm32f10x.h"                  // Device header
#include "types.h"

void VECTOR_normalizeAndMultiply (t_VECTOR* a, uint32_t maxSpeed);

/*--------------------------------------------------------------*/
/*---------NORMALIZES A, STORES THE RESULT IN B-----------------*/
/*--------------------------------------------------------------*/
void VECTOR_normalize (t_VECTOR* a,t_VECTOR*b);

/*--------------------------------------------------------------*/
/*---------SUBTRACTS B FROM A, STORES THE RESULT IN C-----------*/
/*--------------------------------------------------------------*/
void VECTOR_sub (t_VECTOR* a,t_VECTOR* b,t_VECTOR* c);
/*--------------------------------------------------------------*/
/*------------ADDS A AND B, STORES THE RESULT IN C--------------*/
/*--------------------------------------------------------------*/
void VECTOR_add (t_VECTOR* a,t_VECTOR* b,t_VECTOR* c);
void VECTOR_accumulate (t_VECTOR* a,t_VECTOR* b);

/*--------------------------------------------------------------*/
/*----------MULTIPLIES A * B, STORES THE RESULT IN C------------*/
/*--------------------------------------------------------------*/
void VECTOR_mult (t_VECTOR* a,int b,t_VECTOR* c);
void VECTOR_div (t_VECTOR* a,int b);
double VECTOR_mag(t_VECTOR*);
double VECTOR_dot(t_VECTOR* a,t_VECTOR* b);




#endif
