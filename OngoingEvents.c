/*!_____________________________________________________________________________
@file       OngoingEvents.c
@author     Kelly Liaw (liaw.k@digipen.edu)
@co-authors Travess Tan (travesscheekhang.t@digipen.edu)
@course     CSD1120
@section    B
@team       CoffeeMochi
@brief      This file contains the function definitions that are used in
			the ongoing events in Impero.

			Copyright ©2021 DigiPen, All rights reserved.
*//*__________________________________________________________________________*/

#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "cprocessing.h"
#include "OngoingEvents.h"
#include "Common_Headers.h"


#define EVENTLIMIT 10
#define ISALIVE 1
#define ISDEAD 0

CP_Image sprite_rat;

ONGOING ongoingEvents[10] = { 0 };

// event_rat = { 1,CP_Vector_set(0,0), position, FARM, AreaOfEffect,0,5,"Rat has infiltrated the city , food will be compromised every turn " };
// event_one = { 2,CP_Vector_set(0,0), position, FARM, AreaOfEffect,0,5,"Rat has infiltrated the city , food will be compromised every turn " };
// event_two = { 3,CP_Vector_set(0,0), position, FARM, AreaOfEffect,0,5,"Rat has infiltrated the city , food will be compromised every turn " };
// event_three = { 4,CP_Vector_set(0,0), position, FARM, AreaOfEffect,0,5,"Rat has infiltrated the city , food will be compromised every turn " };
// event_four = { 5,CP_Vector_set(0,0), position, FARM, AreaOfEffect,0,5,"Rat has infiltrated the city , food will be compromised every turn " };
// event_five = { 6,CP_Vector_set(0,0), position, FARM, AreaOfEffect,0,5,"Rat has infiltrated the city , food will be compromised every turn " };

void InitOngoingEvents() {

	sprite_rat = CP_Image_Load("./ImperoArtAssets/Impero_Rats.png");
	//sprite_one = CP_Image_load();
	//sprite_two = CP_Image_load();
	//sprite_three = CP_Image_load();
	//sprite_four = CP_Image_load();
	//sprite_five = CP_Image_load();

	for (int i = 0; i < EVENTLIMIT; ++i)
	{
		ongoingEvents[i] = (ONGOING){ 0 };
	}
}

bool CheckCurrent(int typeofeffect, int x, int y)
{
	//int GetOccupiedIndex(int x, int y)
	//this function, if u put in the xand y position, it will return u the building index, 
	//then u check if the event is standing on the correct one
	for (int i = 0; i < EVENTLIMIT; i++) {

		if (ongoingEvents[i].alive == ISALIVE) {
			if (ongoingEvents[i].typeOfEffect == typeofeffect && ongoingEvents[i].positionx == x && ongoingEvents[i].positiony == y)
			{
				return true;
			}
		}
	}
	return false;
}


void GenerateEvents(int eventIndex, int xPos, int yPos, int turnsAlive) 
{
	//create store in array
	for (int i = 0; i < (EVENTLIMIT); ++i) {
		if (ongoingEvents[i].alive == ISDEAD) {
			switch (eventIndex)
			{
			case O_RATEVENT:
				ongoingEvents[i] = (ONGOING){ O_RATEVENT, sprite_rat, xPos, yPos, B_FARM_INDEX, turnsAlive, ISALIVE,"Rat has infiltrated the city , food will be compromised every turn "};//store into array
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
				return;
			}
		}
	}

}

void RemoveEvent(int i) 
{
	//remove current event when turn hit 0
	ongoingEvents[i].alive = ISDEAD;
}
void OnEndUpdateEvents()
{
	//turn minus one when called
	for (int i = 0; i < EVENTLIMIT; i++)
	{
		if (ongoingEvents[i].alive == ISALIVE) {

			ongoingEvents[i].turn -= 1;

			if (ongoingEvents[i].turn == 0) {

				RemoveEvent(i);
			}
		}
	}
}
void DrawOngoingEvents() 
{
	CP_Vector worldPosition;
	for (int i = 0; i < EVENTLIMIT; ++i)
	{
		if (ongoingEvents[i].alive == ISALIVE)
		{
			worldPosition.x = (float)ongoingEvents[i].positionx;
			worldPosition.y = (float)ongoingEvents[i].positiony;
			GridToWorldPosition(&worldPosition);
			CP_Image_Draw(ongoingEvents[i].sprite, worldPosition.x, worldPosition.y, 128, 160, 255);
		}
	}
}



