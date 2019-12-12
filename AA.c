#include <stdlib.h>
#include "AA.h"
#include "vector.h"

/*----------------DEFINITIONS--------------*/
#define maxNumOfAgents		8
/*----------------PRIVATE DATA--------------*/
uint32_t numOfAgents = 0;
t_AGENT*  agents[maxNumOfAgents];
t_VECTOR* positions[maxNumOfAgents] ;


t_AGENT*  currentAgent 	          = NULL;
t_AGENT*  currentSeekingTarget	  = NULL;
t_AGENT*  currentFleeingTarget	  = NULL;
t_AGENT*  currentPersuitingTarget = NULL;

//STEERING FORCE GLOBAL, IT WILL BE UPDATED WHENEVER ANY OF (SEEK,FLEE,PERSUIT...ETC) IS CALLED 
t_VECTOR    sf;
t_VECTOR*	steeringForce = &sf ;

/*----------------------------------------------------------------------------*/
/*------------------------------PRIVATE FUNCTIONS-----------------------------*/
/*----------------------------------------------------------------------------*/
void seek			(t_VECTOR* pos);
void flee			(t_VECTOR* pos);
void persuit	    (void);
void updatePosition(void);

/*----------------------------------------------------------------------------*/
/*--UPDATES SF WTIH A FORCE VECTOR THAT PUSHES THE AGENT TOWARDS THE TARGET---*/
/*----------------------------------------------------------------------------*/
void seek(t_VECTOR* pos)
{
	t_VECTOR tmpSteeringForce;
	t_VECTOR desiredVelocity;
	VECTOR_sub( pos , currentAgent->pos, &desiredVelocity);
	VECTOR_normalizeAndMultiply(&desiredVelocity, currentAgent->maxSpeed) ;
	
	VECTOR_sub(&desiredVelocity, currentAgent->v, &tmpSteeringForce); 
	VECTOR_accumulate(steeringForce, &tmpSteeringForce);
	
}

/*----------------------------------------------------------------------------*/
/*--UPDATES SF WITH A FORCE VECTOR THAT PUSHES THE AGENT AWAY FROM THE TARGET-*/
/*----------------------------------------------------------------------------*/
void flee(t_VECTOR* pos)
{
	t_VECTOR distance;
	VECTOR_sub(pos,currentAgent->pos,&distance);
	int distanceValue = (int) VECTOR_mag(&distance);
	
	if(distanceValue > 5)
	{
		if(currentAgent->seeking != 1 && currentAgent->persuiting !=1 && currentAgent->wandering != 1) 
		{
			currentAgent -> v -> x = currentAgent -> initialV.x;
			currentAgent -> v -> y = currentAgent -> initialV.y;
		}
		return;
	}
	else
	{
		t_VECTOR desiredVelocity;
		t_VECTOR tmpSteeringForce;
		VECTOR_sub( currentAgent->pos, pos,&desiredVelocity );
		VECTOR_normalizeAndMultiply(&desiredVelocity, currentAgent->maxSpeed) ;
		
		VECTOR_sub(&desiredVelocity, currentAgent->v, &tmpSteeringForce); 
		VECTOR_accumulate(steeringForce, &tmpSteeringForce);
		
	}
}

/*--------------------------------------------------------------------------------------------*/
/*--UPDATES SF WITH A FORCE VECTOR THAT PUSHES THE AGENT TOWARDS THE TARGET'S FUTURE POSITION-*/
/*--------------------------------------------------------------------------------------------*/
void persuit(void)
{
	t_VECTOR	futurePos;
	t_VECTOR  toEvader;

	VECTOR_sub(currentPersuitingTarget->pos,currentAgent->pos, &toEvader);
	//BASE CASE, IF IT'S TOO CLOSE, JUST SEEK FOR IT'S POSITION DIRECTLY
	double relativeHeading = VECTOR_dot(currentAgent->heading,currentPersuitingTarget->heading);
	if(relativeHeading < -0.95) seek(currentPersuitingTarget->pos);
	
	int lookAheadTime = (int) VECTOR_mag(&toEvader) / (currentAgent->maxSpeed + 1) ;
	VECTOR_mult(currentPersuitingTarget->v, lookAheadTime, &futurePos);
	VECTOR_add(currentPersuitingTarget->pos, &futurePos, &futurePos);

	seek(&futurePos);

}

/*------------------------------------------------------------------------------*/
/*-INTERNAL TASK THAT LOOPS OVER THE AGENTS, ACCUMULATES THEIR STEERING FORCES--*/
/*------------------------THEN UPDATES THEIR NEW POSITION-----------------------*/
/*------------------------------------------------------------------------------*/
void updatePosition(void)
{
	t_VECTOR* currentPosition;
	t_VECTOR* currentVelocity;
	t_VECTOR newPosition;
	t_VECTOR newVelocity;
	
	currentPosition = currentAgent -> pos;
	currentVelocity = currentAgent -> v;
	
	/*IN CASE OF THE PRESEDENCE OF STEERING BEHAVIORS*/
	if(steeringForce != NULL)
	{
		VECTOR_div(steeringForce, currentAgent->mass); //acceleration
		//get the new data
		VECTOR_add(currentVelocity , steeringForce, &newVelocity);
		VECTOR_add(currentPosition , &newVelocity, &newPosition); 
		
		currentAgent->v->x = newVelocity.x;
		currentAgent->v->y = newVelocity.y;
	}
	else //NO STEERING BEHAVRIORS, JUST UPDATE THE POSITION
	{
		VECTOR_add(currentAgent->pos,currentAgent->v,&newPosition);
	}
	
	if(newPosition.x < 84 && newPosition.y <48 )
		{
			currentAgent->pos->x = newPosition.x;
			currentAgent->pos->y = newPosition.y;
		}
		else
		{
			if(newPosition.x > 84 && newPosition.y <48 ) currentAgent->pos->y = newPosition.y;
			
			else if(newPosition.x < 84 && newPosition.y > 48 ) currentAgent->pos->x = newPosition.x;
			
			else //BOTH AXIS ARE OUT OF BOUNDARY
			{
				currentAgent->pos->x = 0;
				currentAgent->pos->y = 0;
			}
		}	
}

/*----------------------------------------------------------------------------*/
/*------------------------------INTERFACE FUNCTIONS---------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*------------- CREATES AN AGENT AND RETURNS A POINTER TO IT------------------*/
/*----------------------------------------------------------------------------*/
/* pos : INITIAL POSITION
* velocity : INITIAL VELOCITY
* mass,maxSpeed : OBVIOUSLY...
*/
/*----------------------------------------------------------------------------*/
t_AGENT*			AA_createAgent(t_VECTOR* pos, t_VECTOR* velocity, uint32_t mass, uint32_t maxSpeed)
{
	if(numOfAgents >= maxNumOfAgents) return NULL;
	
	t_AGENT* rtnAgent  = malloc(sizeof(t_AGENT));
	if(rtnAgent == NULL) return NULL;
	
	rtnAgent -> mass = mass ;
	rtnAgent -> maxSpeed = maxSpeed;
	rtnAgent -> pos = pos;
	rtnAgent -> initialPos.x = pos->x, rtnAgent -> initialPos.y = pos->y;
	rtnAgent -> v = velocity;
	rtnAgent -> initialV.x = velocity->x, rtnAgent -> initialV.y = velocity->y;
	
	rtnAgent -> seeking    = 0;
	rtnAgent -> persuiting = 0;
	rtnAgent -> fleeing    = 0;
	rtnAgent -> wandering  = 0;
	
	rtnAgent -> seekingTarget    = NULL;
	rtnAgent -> fleeingTarget    = NULL;
	rtnAgent -> persuitingTarget = NULL;
	
	agents[numOfAgents]    = rtnAgent;
	positions[numOfAgents] = rtnAgent->pos;
	numOfAgents++;
	
	//INITIALIZE HEADING AND NORMAL VECOTRS
	VECTOR_normalize(rtnAgent->v, rtnAgent->heading);
	//FUCK THE NORMAL VECTOR, I DON'T NEED IT FOR NOW
	return rtnAgent;
}

void					AA_seek(t_AGENT* moving, t_AGENT* target)
{
	moving -> seeking = 1;
	moving -> seekingTarget  = target;
}	


void 					AA_persuit(t_AGENT* moving, t_AGENT* target)
{
	moving -> persuiting = 1;
	moving -> persuitingTarget  = target;
}

void					AA_flee(t_AGENT* moving, t_AGENT* target)
{
	moving -> fleeing = 1;
	moving -> fleeingTarget  = target;
}

uint32_t 			AA_getNumOfAgents(void)
{
	return numOfAgents;
}

t_VECTOR** 			AA_getPosArr(void)
{
	return positions;
}


void					AA_updateAgentsTask(void)
{
	uint32_t i ;
	for(i = 0 ; i < numOfAgents; i++)
	{
		//RESET THE STEERING FORCE
		steeringForce->x = 0; steeringForce->y = 0;
		
		currentAgent = agents[i];
		currentSeekingTarget = currentAgent->seekingTarget;
		currentFleeingTarget = currentAgent->fleeingTarget;
		currentPersuitingTarget = currentAgent->persuitingTarget;
		
		//UPDATE HEADING AND NORMAL VECTORS
		if(VECTOR_mag(currentAgent->v) > 0.0001)
		{
			VECTOR_normalize(currentAgent->v,currentAgent->heading);
			//FUCK THE NORMAL VECTOR, I DON'T NEED IT FOR NOW
		}
		
		if(currentAgent -> seeking == 1)
		{
			seek(currentSeekingTarget->pos);	//UPDATE THE STEERING FORCE GLOBAL VARIABLE
		}
		else if (currentAgent -> persuiting == 1)
		{
			persuit();
		}
		else if (currentAgent -> fleeing == 1)
		{
			//apply fleeing algorithm
			flee(currentFleeingTarget->pos);
		}
		else if (currentAgent -> wandering)
		{
			
		}
		updatePosition();
		positions[i] = currentAgent->pos;
	}
}


