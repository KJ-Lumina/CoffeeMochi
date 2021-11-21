#include <stdio.h>
#include "cprocessing.h"
#include "Common_Headers.h"
#include <stdlib.h>


typedef struct
{
	int cardIndexes[20];
	int cardsInDeck;
}CARDDECK;


#define TOTALCARDCOUNT 21
#define TOTALTUTORIALCARDSCOUNT 3
CARDEVENT* cardList[TOTALCARDCOUNT];
CARDEVENT* tutCardList[TOTALTUTORIALCARDSCOUNT];

CARDDECK tutorialDeck;
CARDDECK cardDeck;


CARDDECK* currentDeck;
CARDEVENT* currentEvent;
int currentCardIndex;


#pragma region Card Events
CARDEVENT emptyCard = { 0, NULL_EVENT, NULL_TYPE_EVENT, "This is a null event, for errors only", NULL_CHOICE, 0, R_NULL_INDEX, NULL_CHOICE, 0 , "Why are you reading this? This is a bug. Ignore and move along.", NULL_CHOICE, 0, R_NULL_INDEX ,NULL_CHOICE, 0 , "You are looking at a bug right now. Please ignore." };


//TUTORIALS EVENTS [CARD INDEX STARTS WITH 0X]
CARDEVENT E_TutBuildAHouse = { 1, BASIC_EVENT, BUILD_TYPE_EVENT, "Gorvernor, there is no houses for our citizen to stay. Build 2 houses by clicking on the grid.", B_HOUSE_INDEX, 2 , R_POPULATION_INDEX, R_GOLD_INDEX, 0, "Get our workers gather up materials and start working on constructing 2 houses", B_HOUSE_INDEX, 2 , R_POPULATION_INDEX, R_GOLD_INDEX, 0,"There are some damaged house nearby, we can repair them to a usable condition. Send our workers to build materails and start on the repairs on them. " };

#pragma endregion


void InitCardList()
{
	//Init Tutorial Card List
	tutCardList[0] = &E_TutBuildAHouse;

}



