#include <stdio.h>
#include "cprocessing.h"
#include "Common_Headers.h"
#include <stdlib.h>

/* 
CARDEVENTS PARAMETERS
EVENT INDEX, DIFFICULTY, DESCRIPTION (MAX 100), INDEX OPTION A, COST TYPE INDEX A, COST AMOUNT A, STRING OPTION A, INDEX OPTION B, COST TYPE INDEX B, COST AMOUNT B, STRING OPTION B
*/

typedef struct
{
	int cardIndexes[20];
	int cardsInDeck;
}CARDDECK;

#define TOTALCARDCOUNT 5
CARDEVENT* cardList[TOTALCARDCOUNT];

CARDDECK tutorialDeck;

CARDDECK* currentDeck;
CARDEVENT* currentEvent;
int currentCardIndex;


#pragma region Card Events
CARDEVENT emptyCard = { 0, NULL_EVENT, "This is a null event, for errors only", NULL_CHOICE, 0,  NULL_CHOICE, 0 , "", NULL_CHOICE, 0, NULL_CHOICE, 0 , "" };

CARDEVENT b_BuildAHouse = { 1, BASIC_EVENT, "Our citizens needs a place to sleep. Click on the house below and build it on the grid.", B_HOUSE_INDEX, 1 ,R_GOLD_INDEX, 50, "House", NULL_CHOICE,0, R_NULL_INDEX, 0 ,"Ignore"};
CARDEVENT b_BuildAFarm = { 2, BASIC_EVENT, "Our city needs to eat. Click on the farm below and build it on the grid.", B_FARM_INDEX, 1 ,R_GOLD_INDEX, 25, "Farm", NULL_CHOICE, 0, R_NULL_INDEX, 0, "Ignore" };


CARDEVENT b_BuildTwoHouse = { 3 , ADVANCED_EVENT, "Refugees are seeking shelter in your kingdom. Build two house to shelter them. ", B_HOUSE_INDEX, 2, R_GOLD_INDEX, 100, "Houses", NULL_CHOICE,0, R_NULL_INDEX, 0, "Ignore" };
CARDEVENT b_BuildThreeFarm = { 4, ADVANCED_EVENT, "Famine have struck your kingdom, Build 3 farm to advert the crisis. ", B_FARM_INDEX, 3, R_GOLD_INDEX, 75, "Farm", NULL_CHOICE,0, R_NULL_INDEX, 0, "Ignore" };

#pragma endregion

void InitCardList()
{
	cardList[0] = &emptyCard;
	cardList[1] = &b_BuildAHouse;
	cardList[2] = &b_BuildAFarm;
	cardList[3] = &b_BuildTwoHouse;
	cardList[4] = &b_BuildThreeFarm;
}

void InitDecks()
{
	InitCardList();
	for (int index = 0; index < TOTALCARDCOUNT; index++) 
	{
		tutorialDeck.cardIndexes[index] = CP_Random_RangeInt(1, TOTALCARDCOUNT - 1); //Setting the index of card in each deck to reference in cardlist.
		tutorialDeck.cardsInDeck++;
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

	if (currentCardIndex == currentDeck->cardsInDeck)
	{
		printf("Card Game Over");
		GameOver();
	}
	currentEvent = cardList[currentDeck->cardIndexes[currentCardIndex]];
	++currentCardIndex; //Adding one counter to the Card Index after Drawing it
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


