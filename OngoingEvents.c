#include <stdio.h>
#include "cprocessing.h"
//#include "Common_Headers.h"
#include <math.h>
#include "OngoingEvents.h"
#include <stdbool.h>
//#include "Buildings.c"


#define EVENTLIMIT 10

#define ISALIVE 1
#define DEAD 0



	//int eventIndex;
	//CP_Image sprite;
	//CP_Vector direction;//where the sprite is going next
	//CP_Vector currentPosition;// where the events are at 
	//int typeOfEffect;//index of which building it will affect
	//CP_Vector AreaofEffect;//where it will affect , pull in location of building 
	//int eventsize;//number of event spawn when initialized
	//int turn;//number of turn it will stay 
	//char effect[];//name of resources affected
	//int live;

ONGOING currentEvent[10] = { 0 };

ONGOING event_rat = { 1,CP_Vector_set(0,0), position, FARM, AreaOfEffect,0,5,"Rat has infiltrated the city , food will be compromised every turn " };
ONGOING event_one = { 2,CP_Vector_set(0,0), position, FARM, AreaOfEffect,0,5,"Rat has infiltrated the city , food will be compromised every turn " };
ONGOING event_two = { 3,CP_Vector_set(0,0), position, FARM, AreaOfEffect,0,5,"Rat has infiltrated the city , food will be compromised every turn " };
ONGOING event_three = { 4,CP_Vector_set(0,0), position, FARM, AreaOfEffect,0,5,"Rat has infiltrated the city , food will be compromised every turn " };
ONGOING event_four = { 5,CP_Vector_set(0,0), position, FARM, AreaOfEffect,0,5,"Rat has infiltrated the city , food will be compromised every turn " };
ONGOING event_five = { 6,CP_Vector_set(0,0), position, FARM, AreaOfEffect,0,5,"Rat has infiltrated the city , food will be compromised every turn " };

void InitEvents() {

	sprite_rat = CP_Image_load();
	sprite_one = CP_Image_load();
	sprite_two = CP_Image_load();
	sprite_three = CP_Image_load();
	sprite_four = CP_Image_load();//initialize image of rat
	sprite_five = CP_Image_load();

}

void GenerateEvents(int eventIndex, CP_Vector currentPosition) {
	//create store in array

	for (int i = 0; i < (EVENTLIMIT); ++i) {
		if (currentEvent[i].live == DEAD) {
			switch (eventIndex)
			{
			case 1:
				currentevent[i] = { 1,sprite_rat, CP_Vector_set(0,0), currentPosition, FARM, AreaOfEffect,0,5,"Rat has infiltrated the city , food will be compromised every turn " , ISALIVE };//store into array

				return;

			case 2:
				currentevent[i] = { 2,sprite_one, CP_Vector_set(0,0), position, FARM, AreaOfEffect,0,5,"Rat has infiltrated the city , food will be compromised every turn ", ISALIVE };

				return;

			case 3:
				currentevent[i] = { 3,sprite_two, CP_Vector_set(0,0), position, FARM, AreaOfEffect,0,5,"Rat has infiltrated the city , food will be compromised every turn " , ISALIVE };
				return;

			case 4:
				currentevent[i] = { 4,sprite_three, CP_Vector_set(0,0), position, FARM, AreaOfEffect,0,5,"Rat has infiltrated the city , food will be compromised every turn " , ISALIVE };
				return;

			case 5:
				currentevent[i] = { 5,sprite_four, CP_Vector_set(0,0), position, FARM, AreaOfEffect,0,5,"Rat has infiltrated the city , food will be compromised every turn " , ISALIVE };
				return;

			case 6:
				currentevent[i] = { 6,sprite_five, CP_Vector_set(0,0), position, FARM, AreaOfEffect,0,5,"Rat has infiltrated the city , food will be compromised every turn " , ISALIVE };
				return;

			default:
				return 0;
			}
		}
	}

}

void removeEvent(int i) {
	//remove current event when turn hit 0

	currentEvent[i].live = DEAD;
}
void EndTurn() {
	//turn minus one when called
	for (int i = 0; i < EVENTLIMIT; i++;) {

		if (currentEvent[i].live != DEAD) {

			currentEvent[i].turn -= 1;

			if (currentEvent[i].turn == 0) {

				removeEvent(i);
			}
		}

	}

	void drawEvent() {
		for (int i = 0; i < EVENTLIMIT; ++i)
		{
			if (currentEvent[i].live == ISALIVE)
			{

				CP_Image_Draw(currentEvent[i].sprite, currentEvent[i].currentPosition.x, currentEvent[i].currentPosition.y, 100, 100, 255);

			}
		}
		//draw sprite in respective location??
	}



