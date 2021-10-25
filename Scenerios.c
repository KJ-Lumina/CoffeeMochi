#include <stdio.h>
#include "cprocessing.h"
#include "game.h"
#include <stdlib.h>

int eventIndex;
int difficulty;
char description[100];
int indexOptionA;
int indexOptionB;
char* stringOptionA;
char* stringOptionB;

/* 
CARDEVENTS PARAMETERS
EVENT INDEX, DIFFICULTY, DESCRIPTION (MAX 100), INDEX OPTION A, INDEX OPTION B, STRING OPTION A, STRING OPTION B
*/


#pragma region Card Events
CARDEVENTS b_BuildAHouse = { 1, 1, "Our citizens needs a place to sleep. Click on the house below and build it on the grid.", 2, 0, "house", "grass" };
CARDEVENTS b_BuildAFarm = { 2, 1, "Our city needs to eat. Click on the farm below and build it on the grid.", 3, 0, "farm", "grass" };


#pragma endregion




CARDEVENTS basicDeck[20];


void InitDeck()
{
	
	
	basicDeck[0] = b_BuildAHouse;
	basicDeck[1] = b_BuildAFarm;

}

CARDEVENTS GetBasicEvent()
{
	return basicDeck[0];
}


void GetAdvancedEvent()
{
	
}