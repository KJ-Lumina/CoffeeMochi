#include <stdio.h>
#include "cprocessing.h"

typedef struct {
	int eventIndex;
	CP_Image sprite;
	int positionx;
	int positiony;
	int typeOfEffect;//index of which building it will affect
	int turn;//number of turn it will stay
	int alive;
	char description[100];
}ONGOING;



void OnEndUpdateEvents();//end turn, turn reduce by 1 
void RemoveEvent(int index);//check if  turn left is 0, if 0, remove event from array 