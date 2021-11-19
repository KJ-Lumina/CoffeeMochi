#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "cprocessing.h"
#include "OngoingEvents.h"
#include "Common_Headers.h"


#define EVENTLIMIT 10
#define ISALIVE 1
#define ISDEAD 0

	//int eventIndex;
	//CP_Image sprite;
	//CP_Vector direction;//where the sprite is going next
	//CP_Vector currentPosition;// where the events are at 
	//int typeOfEffect;//index of which building it will affect
	//CP_Vector AreaofEffect;//where it will affect , pull in location of building 
	//int eventsize;//number of event spawn when initialized
	//int turn;//number of turn it will stay 
	//int live;

CP_Image sprite_rat;

ONGOING currentEvent[10] = { 0 };

// event_rat = { 1,CP_Vector_set(0,0), position, FARM, AreaOfEffect,0,5,"Rat has infiltrated the city , food will be compromised every turn " };
// event_one = { 2,CP_Vector_set(0,0), position, FARM, AreaOfEffect,0,5,"Rat has infiltrated the city , food will be compromised every turn " };
// event_two = { 3,CP_Vector_set(0,0), position, FARM, AreaOfEffect,0,5,"Rat has infiltrated the city , food will be compromised every turn " };
// event_three = { 4,CP_Vector_set(0,0), position, FARM, AreaOfEffect,0,5,"Rat has infiltrated the city , food will be compromised every turn " };
// event_four = { 5,CP_Vector_set(0,0), position, FARM, AreaOfEffect,0,5,"Rat has infiltrated the city , food will be compromised every turn " };
// event_five = { 6,CP_Vector_set(0,0), position, FARM, AreaOfEffect,0,5,"Rat has infiltrated the city , food will be compromised every turn " };

void InitEvents() {

	sprite_rat = CP_Image_load();
	//sprite_one = CP_Image_load();
	//sprite_two = CP_Image_load();
	//sprite_three = CP_Image_load();
	//sprite_four = CP_Image_load();
	//sprite_five = CP_Image_load();

}

void GenerateEvents(int eventIndex, CP_Vector currentPosition) {
	//create store in array

	for (int i = 0; i < (EVENTLIMIT); ++i) {
		if (currentEvent[i].alive == ISDEAD) {
			switch (eventIndex)
			{
			case 1:
				currentEvent[i] = (ONGOING){1, sprite_rat, CP_Vector_set(0,0), CP_Vector_set(currentPosition.x,currentPosition.y), R_BUILDING_FARM_INDEX, 5, ISALIVE,"Rat has infiltrated the city , food will be compromised every turn "};//store into array
				return;
			case 2:
				//currentEvent[i] = { 2,sprite_one, CP_Vector_set(0,0), position, FARM, AreaOfEffect,0,5,"Rat has infiltrated the city , food will be compromised every turn ", ISALIVE };
				return;
			case 3:
				//currentEvent[i] = { 3,sprite_two, CP_Vector_set(0,0), position, FARM, AreaOfEffect,0,5,"Rat has infiltrated the city , food will be compromised every turn " , ISALIVE };
				return;

			case 4:
				//currentEvent[i] = { 4,sprite_three, CP_Vector_set(0,0), position, FARM, AreaOfEffect,0,5,"Rat has infiltrated the city , food will be compromised every turn " , ISALIVE };
				return;
			case 5:
				//currentEvent[i] = { 5,sprite_four, CP_Vector_set(0,0), position, FARM, AreaOfEffect,0,5,"Rat has infiltrated the city , food will be compromised every turn " , ISALIVE };
				return;
			case 6:
				//currentEvent[i] = { 6,sprite_five, CP_Vector_set(0,0), position, FARM, AreaOfEffect,0,5,"Rat has infiltrated the city , food will be compromised every turn " , ISALIVE };
				return;
			default:
				return 0;
			}
		}
	}

}

void RemoveEvent(int i) 
{
	//remove current event when turn hit 0
	currentEvent[i].alive = ISDEAD;
}
void EndTurn()
{
	//turn minus one when called
	for (int i = 0; i < EVENTLIMIT; i++)
	{
		if (currentEvent[i].alive != ISDEAD) {

			currentEvent[i].turn -= 1;

			if (currentEvent[i].turn == 0) {

				removeEvent(i);
			}
		}
	}
}
void DrawEvent() 
{
	for (int i = 0; i < EVENTLIMIT; ++i)
	{
		if (currentEvent[i].alive == ISALIVE)
		{
			CP_Image_Draw(currentEvent[i].sprite, currentEvent[i].currentPosition.x, currentEvent[i].currentPosition.y, 100, 100, 255);
		}
	}
}



