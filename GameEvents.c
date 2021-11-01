#include <stdio.h>
#include "cprocessing.h"
#include "Common_Headers.h"
#include <stdlib.h>

/* 
CARDEVENTS PARAMETERS
EVENT INDEX, DIFFICULTY, DESCRIPTION (MAX 100), INDEX OPTION A, INDEX OPTION B, STRING OPTION A, STRING OPTION B
*/

typedef struct
{
	int cardIndexes[20];
	int cardsInDeck;
}CARDDECK;

#define TOTALCARDCOUNT 3
CARDEVENT* cardList[TOTALCARDCOUNT];

CARDDECK tutorialDeck;

CARDDECK* currentDeck;
CARDEVENT* currentEvent;
int currentCardIndex;


#pragma region Card Events
CARDEVENT emptyCard = { 0, NULL_EVENT, "This is a null event, for errors only", NULL_CHOICE, NULL_CHOICE, "", "" };
CARDEVENT b_BuildAHouse = { 1, BASIC_EVENT, "Our citizens needs a place to sleep. Click on the house below and build it on the grid.", B_HOUSE_INDEX, NULL_CHOICE, "House", "Ignore" };
CARDEVENT b_BuildAFarm = { 2, ADVANCED_EVENT, "Our city needs to eat. Click on the farm below and build it on the grid.", B_FARM_INDEX, NULL_CHOICE, "Farm", "Ignore" };




#pragma endregion

void InitCardList()
{
	cardList[0] = &emptyCard;
	cardList[1] = &b_BuildAHouse;
	cardList[2] = &b_BuildAFarm;
}

void InitDecks()
{
	InitCardList();
	for (int index = 0; index < 20; index++) 
	{
		tutorialDeck.cardIndexes[index] = CP_Random_RangeInt(1, TOTALCARDCOUNT - 1);
	}

	currentCardIndex = 0;
	currentDeck = &tutorialDeck;
}

CARDEVENT* GetCurrentEvent()
{
	return currentEvent;
}

CARDEVENT* GetNextEvent() 
{
	++currentCardIndex;
	if (currentCardIndex == currentDeck->cardsInDeck)
	{
		GameOver();
	}
	currentEvent = cardList[currentDeck->cardIndexes[currentCardIndex]];
	return currentEvent;
}

int IsBasicEvent(CARDEVENT card) {
	if (card.eventType == BASIC_EVENT)
		return TRUE;

	return FALSE;
}

CARDEVENT* GetEventByIndex(int index)
{
	return cardList[index];
}


