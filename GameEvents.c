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


#define TOTALCARDCOUNT 11
CARDEVENT* cardList[TOTALCARDCOUNT];

CARDDECK tutorialDeck;


CARDDECK* currentDeck;
CARDEVENT* currentEvent;
int currentCardIndex;


#pragma region Card Events
CARDEVENT emptyCard = { 0, NULL_EVENT, NULL_TYPE_EVENT, "This is a null event, for errors only", NULL_CHOICE, 0, R_NULL_INDEX, NULL_CHOICE, 0 , "", NULL_CHOICE, 0, R_NULL_INDEX ,NULL_CHOICE, 0 , "" };

//BASIC EVENTS - BUILD TYPE
CARDEVENT E_BuildAHouse = { 1, BASIC_EVENT, BUILD_TYPE_EVENT, "Our citizens needs a place to sleep. Click on the house below and build it on the grid.", B_HOUSE_INDEX, 1 , R_POPULATION_INDEX, R_GOLD_INDEX, 5, "House", NULL_CHOICE, 0 ,R_NULL_INDEX, R_NULL_INDEX, 0 ,"Ignore" };
CARDEVENT E_BuildAFarm = { 2, BASIC_EVENT, BUILD_TYPE_EVENT, "Our city needs to eat. Click on the farm below and build it on the grid.", B_FARM_INDEX, 1 , R_FOOD_INDEX ,R_GOLD_INDEX,  3, "Farm", NULL_CHOICE, 0, R_NULL_INDEX, R_NULL_INDEX, 0, "Ignore" };
CARDEVENT E_BuildAMarket = { 3 , BASIC_EVENT, BUILD_TYPE_EVENT, "Our economy is unstable. Click on the market below and build it on the grid.", B_MARKET_INDEX, 1 , R_GOLD_INDEX , R_GOLD_INDEX, 7, "Market", NULL_CHOICE, 0 , R_NULL_INDEX, R_NULL_INDEX, 0 , "Ignore" };

//BASIC EVENT - RESOURCE TYPE
CARDEVENT E_FoodMerchantArrival = { 6, BASIC_EVENT, RESOURCE_TYPE_EVENT, "Food Peddlers have arrived at your kingdom, they wish to trade 200 Food for 100 Gold", E_INCREASE_RESOURCE, 20, R_FOOD_INDEX, R_GOLD_INDEX, 10, "Buy Food", NULL_CHOICE, 0 , R_NULL_INDEX, R_NULL_INDEX, 0, "Ignore" };
CARDEVENT E_StolenFood = { 7 , BASIC_EVENT, RESOURCE_TYPE_EVENT, "It have come your notice that Food from the storages haven been stolen.", E_INCREASE_RESOURCE, 5 , R_MORALE_INDEX, R_NULL_INDEX ,0 , "Apprehend the Thief" , E_DECREASE_RESOURCE, 20, R_FOOD_INDEX, NULL_CHOICE, 0 , "Ignore the Thievery" };
CARDEVENT E_CircusTroupeVisit = { 8 , ADVANCED_EVENT, RESOURCE_TYPE_EVENT, "An Wandering Circus Troupe have arrived and is offering their services.", E_INCREASE_RESOURCE, 50, R_MORALE_INDEX, R_GOLD_INDEX, 70, "Hire their services", NULL_CHOICE, 0, R_NULL_INDEX, R_NULL_INDEX, 0, "Decline their services" };
CARDEVENT E_HeavyStorm = { 9 , BASIC_EVENT, RESOURCE_TYPE_EVENT, "A heavy storm has passed and many houses are currently damaged", E_INCREASE_RESOURCE, 5, R_MORALE_INDEX, R_GOLD_INDEX, 10, "Repair damaged houses", E_DECREASE_RESOURCE, 10, R_MORALE_INDEX, R_NULL_INDEX, 0, "Ignore the damaged house" };
CARDEVENT E_GoldMineDiscovered = { 10 , BASIC_EVENT, RESOURCE_TYPE_EVENT, "Our miners have discovered a fresh new gold mine", E_INCREASE_RESOURCE, 25, R_GOLD_INDEX, R_NULL_INDEX, 0, "Claim all the gold to the treasury", E_INCREASE_RESOURCE, 10, R_MORALE_INDEX, R_NULL_INDEX, 0, "Distribute the gold to your citizens" };
//ADVANCED EVENTS - BUILD TYPE
CARDEVENT E_RefugeesHouses = { 4 , ADVANCED_EVENT, BUILD_TYPE_EVENT,"Refugees are seeking shelter in your kingdom. Build two house to shelter them. ", B_HOUSE_INDEX, 2,R_POPULATION_INDEX, R_GOLD_INDEX, 10, "Houses", NULL_CHOICE,0, R_NULL_INDEX, R_NULL_INDEX, 0, "Ignore" };
CARDEVENT E_FamineStrikes = { 5, ADVANCED_EVENT, BUILD_TYPE_EVENT,"Famine have struck your kingdom, Distribute Food to advert the crisis. ", E_DECREASE_RESOURCE, 20, R_MORALE_INDEX, R_FOOD_INDEX, 30, "Distribute Food", E_DECREASE_RESOURCE, 10, R_MORALE_INDEX, R_POPULATION_INDEX, 5, "Ignore" };


//ADVANCED EVENTS - RESOURCE TYPE



#pragma endregion


void InitCardList()
{
	cardList[0] = &emptyCard;
	cardList[1] = &E_BuildAHouse;
	cardList[2] = &E_BuildAFarm;
	cardList[3] = &E_BuildAMarket;
	cardList[4] = &E_RefugeesHouses;
	cardList[5] = &E_FamineStrikes;
	cardList[6] = &E_FoodMerchantArrival;
	cardList[7] = &E_StolenFood;
	cardList[8] = &E_CircusTroupeVisit;
	cardList[9] = &E_HeavyStorm;
	cardList[10] = &E_GoldMineDiscovered;
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


