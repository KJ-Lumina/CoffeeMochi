#include <stdio.h>
#include "cprocessing.h"

typedef struct {
	int eventIndex;
	CP_Image sprite;
	CP_Vector direction;//where the sprite is going next
	CP_Vector currentPosition;// where the events are at 
	int typeOfEffect;//index of which building it will affect
	int turn;//number of turn it will stay
	int alive;
	char description[100];
	//CP_Vector AreaofEffect;//where it will affect , pull in location of building 
	//int eventsize;//number of event spawn when initialized
}ONGOING;

void InitEvents();
void GenerateEvents(int eventIndex, CP_Vector currentPosition);// generate the event of rat|| any ongoing events 
void DrawEvent();// draws current events
void OnEndUpdateEvents();//end turn, turn reduce by 1 
void RemoveEvent(int index);//check if  turn left is 0, if 0, remove event from array 