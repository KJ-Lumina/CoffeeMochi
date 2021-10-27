#include <stdio.h>
#include "cprocessing.h"
#include "game.h"
#include <stdlib.h>

/* 
CARDEVENTS PARAMETERS
EVENT INDEX, DIFFICULTY, DESCRIPTION (MAX 100), INDEX OPTION A, INDEX OPTION B, STRING OPTION A, STRING OPTION B
*/





#pragma region Card Events
CARDEVENTS b_BuildAHouse = { 1, BASIC_EVENT, "Our citizens needs a place to sleep. Click on the house below and build it on the grid.", B_HOUSE_INDEX, NULL_CHOICE, "House", "Ignore" };
CARDEVENTS b_BuildAFarm = { 2, ADVANCED_EVENT, "Our city needs to eat. Click on the farm below and build it on the grid.", B_FARM_INDEX, NULL_CHOICE, "Farm", "Ignore" };




#pragma endregion




CARDDECK basicDeck;


void InitDeck()
{
	for (int index = 0; index < 20; index++) {
		basicDeck.cards[index] = b_BuildAHouse;
	}
	
	//basicDeck[0] = b_BuildAHouse;
	//basicDeck[1] = b_BuildAFarm;

}

//Current Fix amount of Events = 20
CARDEVENTS GetNextEvent(int cardLeft,CARDEVENTS cardDeck[]) {

	int drawnFrom = 20 - cardLeft;

	return cardDeck[drawnFrom];
}

int IsBasicEvent(CARDEVENTS card) {
	if (card.difficulty == BASIC_EVENT)
		return TRUE;

	return FALSE;

}

void InitBasicDeck(void)
{

}

void InitAdvancedDeck(void)
{

}


CARDEVENTS GetBasicEvent()
{
	return basicDeck.cards[0];
}


void GetAdvancedEvent()
{
	
}