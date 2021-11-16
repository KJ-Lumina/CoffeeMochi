#include <stdio.h>
#include "cprocessing.h"
#include "Common_Headers.h"
#include <stdlib.h>

/* 
CARDEVENTS PARAMETERS
EVENT INDEX, DIFFICULTY, DESCRIPTION (MAX 100), INDEX OPTION A, COST TYPE INDEX A, COST AMOUNT A, STRING OPTION A, INDEX OPTION B, COST TYPE INDEX B, COST AMOUNT B, STRING OPTION B
*/
/*
typedef struct
{
	int eventIndex;
	int eventDifficultyType;
	int eventType;
	char description[100];

	int indexOptionA;
	int optionAmountA;
	int optionTypeA;
	int costTypeA;
	int costAmountA;
	char* descriptionA;

	int indexOptionB;
	int optionAmountB;
	int optionTypeB;
	int costTypeB;
	int costAmountB;
	char* descriptionB;


}CARDEVENT;
*/

typedef struct
{
	int cardIndexes[20];
	int cardsInDeck;
}CARDDECK;


#define TOTALCARDCOUNT 20
#define TOTALTUTORIALCARDSCOUNT 3
CARDEVENT* cardList[TOTALCARDCOUNT];
CARDEVENT* tutCardList[TOTALTUTORIALCARDSCOUNT];

CARDDECK tutorialDeck;
CARDDECK cardDeck;


CARDDECK* currentDeck;
CARDEVENT* currentEvent;
int currentCardIndex;


#pragma region Card Events
CARDEVENT emptyCard = { 0, NULL_EVENT, NULL_TYPE_EVENT, "This is a null event, for errors only", NULL_CHOICE, 0, R_NULL_INDEX, NULL_CHOICE, 0 , "", NULL_CHOICE, 0, R_NULL_INDEX ,NULL_CHOICE, 0 , "" };


//TUTORIALS EVENTS [CARD INDEX STARTS WITH 0X]
CARDEVENT E_TutBuildAHouse = { 1, BASIC_EVENT, BUILD_TYPE_EVENT, "Our citizens needs a place to sleep. Click on the grid and build 2 houses.", B_HOUSE_INDEX, 2 , R_POPULATION_INDEX, R_GOLD_INDEX, 0, "House", NULL_CHOICE, 0 ,R_NULL_INDEX, R_NULL_INDEX, 0 ,"Ignore" };
CARDEVENT E_TutBuildAFarm = { 2, BASIC_EVENT, BUILD_TYPE_EVENT, "Our kingdom needs to food to survive. Click on the grid and build 2 farms.", B_FARM_INDEX, 2 , R_FOOD_INDEX ,R_GOLD_INDEX,  0, "Farm", NULL_CHOICE, 0, R_NULL_INDEX, R_NULL_INDEX, 0, "Ignore" };
CARDEVENT E_TutBuildAMarket = { 3 , BASIC_EVENT, BUILD_TYPE_EVENT, "Our kingdom's economy is unstable. Click on the grid and build 2 market.", B_MARKET_INDEX, 2 , R_GOLD_INDEX , R_GOLD_INDEX, 0, "Market", NULL_CHOICE, 0 , R_NULL_INDEX, R_NULL_INDEX, 0 , "Ignore" };

/*----------------------------------------------------------*/

//BASIC EVENTS - BUILD TYPE
CARDEVENT E_BuildAHouse = { 1, BASIC_EVENT, BUILD_TYPE_EVENT, "Our population have grown and citizen are requesting for more house to be build", B_HOUSE_INDEX, 1 , R_POPULATION_INDEX, R_GOLD_INDEX, 5, "House", E_DECREASE_RESOURCE, 2 ,R_MORALE_INDEX, R_NULL_INDEX, 0 ,"There is enough Houses, Ignore your citizen's request" };
CARDEVENT E_BuildAMarket = { 2 , BASIC_EVENT, BUILD_TYPE_EVENT, "Our citizen have requested for more market to be build", B_MARKET_INDEX, 1 , R_GOLD_INDEX , R_GOLD_INDEX, 7, "Market", E_DECREASE_RESOURCE, 2 , R_MORALE_INDEX, R_NULL_INDEX, 0 , "Refuse the request to build a market." };
CARDEVENT E_ScarceFood = { 3, BASIC_EVENT, BUILD_TYPE_EVENT, "Our Food Supplies are scarce", B_FARM_INDEX, 1, R_FOOD_INDEX, R_GOLD_INDEX, 3, "Build a Farm", E_DECREASE_RESOURCE, 2, R_MORALE_INDEX, R_NULL_INDEX, 0, "Refuse the request to build a farm" };
CARDEVENT E_MerchantSetUp = { 4, BASIC_EVENT, BUILD_TYPE_EVENT, "Various Merchant have requested to set up in your kingdom", B_MARKET_INDEX, 2, R_GOLD_INDEX, R_GOLD_INDEX , 15, "Invite the merchants in and set up 2 market spot for them.", NULL_CHOICE,0 , R_NULL_INDEX, R_NULL_INDEX, 0, "Reject the Merchants from entering." };
CARDEVENT E_NeedEntertainment = {5, BASIC_EVENT, BUILD_TYPE_EVENT, "Our Citizen are bored and getting depress with the lack of entertainment", B_TAVERN_INDEX, 2 , R_GOLD_INDEX, R_GOLD_INDEX, 20, "Build 2 Taverns for our citizens to enjoy", E_DECREASE_RESOURCE, 5, R_MORALE_INDEX, R_NULL_INDEX ,0 ,"Ignore the plead from your citizen for entertainment" };

//BASIC EVENT - RESOURCE TYPE
CARDEVENT E_FoodMerchantArrival = { 6, BASIC_EVENT, RESOURCE_TYPE_EVENT, "Food Peddlers have arrived at your kingdom, they wish to trade 20 Food for 10 Gold", E_INCREASE_RESOURCE, 20, R_FOOD_INDEX, R_GOLD_INDEX, 10, "Buy Food", NULL_CHOICE, 0 , R_NULL_INDEX, R_NULL_INDEX, 0, "Ignore" };
CARDEVENT E_StolenFood = { 7 , BASIC_EVENT, RESOURCE_TYPE_EVENT, "It have come your notice that Food from the storages haven been stolen.", E_INCREASE_RESOURCE, 5 , R_MORALE_INDEX, R_NULL_INDEX ,0 , "Apprehend the Thief" , E_DECREASE_RESOURCE, 20, R_FOOD_INDEX, NULL_CHOICE, 0 , "Ignore the Thievery" };
CARDEVENT E_HeavyStorm = { 8 , BASIC_EVENT, RESOURCE_TYPE_EVENT, "A heavy storm has passed and many houses are currently damaged", E_INCREASE_RESOURCE, 5, R_MORALE_INDEX, R_GOLD_INDEX, 10, "Repair damaged houses", E_DECREASE_RESOURCE, 10, R_MORALE_INDEX, R_NULL_INDEX, 0, "Ignore the damaged house" };
CARDEVENT E_GoldMineDiscovered = { 9 , BASIC_EVENT, RESOURCE_TYPE_EVENT, "Our miners have discovered a fresh new gold mine", E_INCREASE_RESOURCE, 25, R_GOLD_INDEX, R_NULL_INDEX, 0, "Claim all the gold to the treasury", E_INCREASE_RESOURCE, 10, R_MORALE_INDEX, R_NULL_INDEX, 0, "Distribute the gold to your citizens" };
CARDEVENT E_ContaminatedFood = { 10, BASIC_EVENT, RESOURCE_TYPE_EVENT, "The supplies in our granaries are contaminated.", E_INCREASE_RESOURCE, 5 , R_MORALE_INDEX, R_GOLD_INDEX, 10, "Replenish the Contaminated Food in our Granaries", E_DECREASE_RESOURCE, 20, R_FOOD_INDEX, R_MORALE_INDEX, 2, "Clear out the cotaminated food and leave it be" };
CARDEVENT E_VillagersKidnapped = { 11, BASIC_EVENT, RESOURCE_TYPE_EVENT, "Our Villagers have been kidnapped by bandits.", NULL_CHOICE, 0, R_NULL_INDEX, R_GOLD_INDEX, 20, "Hire Mercenaries to rescue the kidnapped", NULL_CHOICE, 0 , R_NULL_INDEX, R_POPULATION_INDEX, 3, "Dispatch the Local Militia to rescue the kidnapped." };
CARDEVENT E_MagnificentHunt = { 12, BASIC_EVENT, RESOURCE_TYPE_EVENT, "Our Hunters have return back with a magnificent hunt", E_INCREASE_RESOURCE, 50 , R_FOOD_INDEX, R_NULL_INDEX , 0, "Store the hunt as food for future use", E_INCREASE_RESOURCE, 20, R_MORALE_INDEX, R_GOLD_INDEX, 15, "Hold a festival to celebrate the magnificent hunt" };


//ADVANCED EVENTS - BUILD TYPE
CARDEVENT E_RefugeesHouses = { 13 , ADVANCED_EVENT, BUILD_TYPE_EVENT,"Refugees are seeking shelter in your kingdom. Build two house to shelter them. ", B_HOUSE_INDEX, 2,R_POPULATION_INDEX, R_GOLD_INDEX, 10, "Houses", NULL_CHOICE,0, R_NULL_INDEX, R_NULL_INDEX, 0, "Ignore" };


//ADVANCED EVENTS - RESOURCE TYPE
CARDEVENT E_FamineStrikes = { 14, ADVANCED_EVENT, RESOURCE_TYPE_EVENT,"Famine have struck your kingdom, Distribute Food to advert the crisis. ", E_DECREASE_RESOURCE, 20, R_MORALE_INDEX, R_FOOD_INDEX, 30, "Distribute Food", E_DECREASE_RESOURCE, 10, R_MORALE_INDEX, R_POPULATION_INDEX, 5, "Ignore" };
CARDEVENT E_ArsonistAttack = { 15, ADVANCED_EVENT, RESOURCE_TYPE_EVENT, "An arsonist have set farms on fire.", E_INCREASE_RESOURCE, 10 , R_MORALE_INDEX, R_GOLD_INDEX, 20 , "Send Assistances to extinguish the fire", E_DECREASE_RESOURCE, 10, R_MORALE_INDEX, R_BUILDING_FARM_INDEX, 1, "Ignore and Let the fire extinguish by itself" };
CARDEVENT E_CircusTroupeVisit = { 16 , ADVANCED_EVENT, RESOURCE_TYPE_EVENT, "An Wandering Circus Troupe have arrived and is offering their services.", E_INCREASE_RESOURCE, 50, R_MORALE_INDEX, R_GOLD_INDEX, 70, "Hire their services", NULL_CHOICE, 0, R_NULL_INDEX, R_NULL_INDEX, 0, "Decline their services" };
CARDEVENT E_CivilRevolt = { 17, ADVANCED_EVENT, RESOURCE_TYPE_EVENT, "A Revolt have broke out demanding for more food and gold, threating to leave the kingdom.", E_DECREASE_RESOURCE, 20, R_FOOD_INDEX, R_GOLD_INDEX, 10, "Please your citizen by giving into their demands.", E_DECREASE_RESOURCE, 10, R_POPULATION_INDEX, R_MORALE_INDEX, 3,  "Ignore their demands." };
CARDEVENT E_DiseaseSpread = { 18, ADVANCED_EVENT, RESOURCE_TYPE_EVENT, "A Disease have spread through our Kingdom and we have no cure for it.", E_INCREASE_RESOURCE, 5, R_MORALE_INDEX, R_GOLD_INDEX, 50, "Purchase Cure from external source", E_DECREASE_RESOURCE, 10, R_POPULATION_INDEX, R_MORALE_INDEX, 5, "Confine and seclude the diseased." };

//DESTROY TYPE EVENTS
CARDEVENT E_EarthquakeIncoming = { 19, ADVANCED_EVENT, DESTROY_TYPE_EVENT, "It is fore told that a Earthquake would striking our kingdom", E_DESTROY_BUILDING, 1, R_BUILDING_HOUSE_INDEX, R_BUILDING_MARKET_INDEX, 1, "Reinforce our Farms and Taverns", E_DESTROY_BUILDING, 1, R_BUILDING_FARM_INDEX, R_BUILDING_TAVERN_INDEX, 1, "Reinforce our Houses and Market" };

#pragma endregion


void InitCardList()
{
	//Init Tutorial Card List
	tutCardList[0] = &E_TutBuildAHouse;
	tutCardList[1] = &E_TutBuildAFarm;
	tutCardList[2] = &E_TutBuildAMarket;

	//Init Card List
	cardList[0] = &E_NeedEntertainment;
	cardList[1] = &E_GoldMineDiscovered;
	cardList[2] = &E_FoodMerchantArrival;
	cardList[3] = &E_StolenFood;
	cardList[4] = &E_ArsonistAttack;
	cardList[5] = &E_BuildAMarket;
	cardList[6] = &E_HeavyStorm;
	cardList[7] = &E_ContaminatedFood;
	cardList[8] = &E_ScarceFood;
	cardList[9] = &E_FamineStrikes;
	cardList[10] = &E_FoodMerchantArrival;
	cardList[11] = &E_MerchantSetUp;
	cardList[12] = &E_CircusTroupeVisit;
	cardList[13] = &E_MagnificentHunt;
	cardList[14] = &E_EarthquakeIncoming;
	cardList[15] = &E_CivilRevolt;
	cardList[16] = &E_RefugeesHouses;
	cardList[17] = &E_BuildAHouse;
	cardList[18] = &E_VillagersKidnapped;
	cardList[19] = &E_DiseaseSpread;
}

void InitDecks()
{	
	InitCardList();

	for (int index = 0; index < TOTALTUTORIALCARDSCOUNT; index++) {
		tutorialDeck.cardIndexes[index] = index;
		tutorialDeck.cardsInDeck++;
	}


	for (int index = 0; index < TOTALCARDCOUNT; index++)
	{
		cardDeck.cardIndexes[index] = index; //Setting the index of card in each deck to reference in cardlist, in accordance
		cardDeck.cardsInDeck++;
	}
	//for (int index = 0; index < TOTALCARDCOUNT; index++) 
	//{
	//	cardDeck.cardIndexes[index] = CP_Random_RangeInt(0, TOTALCARDCOUNT - 1); //Setting the index of card in each deck to reference in cardlist.
	//	cardDeck.cardsInDeck++;
	//}

	currentCardIndex = 0;
	currentDeck = &tutorialDeck;
}

CARDEVENT* GetCurrentEvent()
{
	return currentEvent;
}

int GetCardsLeft()
{
	return currentDeck->cardsInDeck - currentCardIndex;
}

CARDEVENT* GetNextEvent(GAMEPHASE gamePhase)
{

	switch (gamePhase) {

	case PHASE_BUILDPHASE:

		/*if (currentCardIndex == currentDeck->cardsInDeck)
		{
			printf("Reset");
			*currentEvent = emptyCard;
			currentDeck = &cardDeck;
			currentCardIndex = 0;
			return currentEvent;
		}*/

		printf("Card Left:%d",GetCardsLeft());
		printf("Card Index:%d", currentCardIndex);
		currentEvent = tutCardList[currentDeck->cardIndexes[currentCardIndex]];
		++currentCardIndex; //Adding one counter to the Card Index after Drawing it

		break;

	case PHASE_GAMEPHASE:

		//if (currentCardIndex == currentDeck->cardsInDeck)
		//{
		//	printf("exit");
		//	*currentEvent = emptyCard;
		//	return currentEvent;
		//}

		printf("Get Game Phase Event");
		currentEvent = cardList[currentDeck->cardIndexes[currentCardIndex]];
		++currentCardIndex; //Adding one counter to the Card Index after Drawing it

		break;
	}

	return currentEvent;

}

void ChangeDeckByPhase(GAMEPHASE currentGamePhase) {

	currentCardIndex = 0;

	switch (currentGamePhase) {

	case PHASE_BUILDPHASE:
		currentDeck = &tutorialDeck;
		break;

	case PHASE_GAMEPHASE:
		printf("Changed Phase");
		currentDeck = &cardDeck;
		break;

	case PHASE_ENDPHASE:
		break;

	}

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


