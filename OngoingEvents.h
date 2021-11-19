#include <stdio.h>
#include "cprocessing.h"

typedef struct {
	int eventIndex;
	CP_Image sprite;
	CP_Vector direction;//where the sprite is going next
	CP_Vector currentPosition;// where the events are at 
	int typeOfEffect;//index of which building it will affect
	CP_Vector AreaofEffect;//where it will affect , pull in location of building 
	int eventsize;//number of event spawn when initialized
	int turn;//number of turn it will stay 
	char effect[];//name of resources affected
	int live;
}ONGOING;

void InitEvents();
void UpdateTurn(eventCount);
void generateEvents()// generate the event of rat|| any ongoing events 
void drawEvent();// draws current events
void endTurn(int turns);//end turn, turn reduce by 1 
void removeEvent();//check if  turn left is 0, if 0, remove event from array 