#include "vector.h"
#include <math.h>
#include <stdlib.h>

//MANY FLOAT OPERATIONS :((
void VECTOR_normalizeAndMultiply (t_VECTOR* a, uint32_t maxSpeed)
{
	double rms;
	double x = a->x ;
	double y = a->y ;
	
	double xSquared = x*x ;
	double ySquared = y*y ;
	
	rms = sqrt(xSquared+ySquared);
	
	a->x = (int) round(maxSpeed * x/ rms);
	a->y = (int) round(maxSpeed * y/ rms);
	
}


void VECTOR_normalize (t_VECTOR* a,t_VECTOR*b)
{
	t_VECTOR* rtnVector = b;
	if(b != NULL)
	{
		double rms;
		double x = a->x ;
		double y = a->y ;
		
		double xSquared = x*x ;
		double ySquared = y*y ;
		
		rms = sqrt(xSquared+ySquared);
		
		rtnVector->x = (int) round(x/ rms);
		rtnVector->y = (int) round(y/ rms);
	}
}

void VECTOR_sub (t_VECTOR* a,t_VECTOR* b,t_VECTOR* c)
{
	t_VECTOR* rtnVector = c;
	if(rtnVector)
	{
		rtnVector ->x = (a->x) - (b->x) ;
		rtnVector ->y = (a->y) - (b->y) ;
	}
}

void VECTOR_add (t_VECTOR* a,t_VECTOR* b,t_VECTOR* c)
{
	t_VECTOR* rtnVector = c;
	if(rtnVector)
	{
		rtnVector ->x = (a->x) + (b->x) ;
		rtnVector ->y = (a->y) + (b->y) ;
	}
}

void VECTOR_accumulate (t_VECTOR* a,t_VECTOR* b)
{
		a->x = (a->x) + (b->x) ;
		a->y = (a->y) + (b->y) ;
}



void VECTOR_mult (t_VECTOR* a, int b, t_VECTOR* c)
{
	t_VECTOR* rtnVector = c;
	if(rtnVector)
	{
		rtnVector ->x = (a->x)*b;
		rtnVector ->y = (a->y)*b ;
	}
}

void VECTOR_div (t_VECTOR* a,int b)
{
	a->x = (a->x) / b ;
	a->y = (a->y) / b ;
}

double VECTOR_mag(t_VECTOR* a)
{
	double rms;
	double x = a->x ;
	double y = a->y ;
	
	double xSquared = x*x ;
	double ySquared = y*y ;
	
	rms = sqrt(xSquared+ySquared);	
	return rms;
}

double VECTOR_dot(t_VECTOR* a,t_VECTOR* b )
{
	double x1 = a->x, x2 = b->x ;
	double y1 = a->y, y2 = b->y ;
	
	double newX = x1*x2 ;
	double newY = y1*y2 ;
	
	return (newX + newY);
}
