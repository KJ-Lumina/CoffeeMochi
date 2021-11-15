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
#define TOTALTUTORIALCARDSCOUNT 3
CARDEVENT* cardList[TOTALCARDCOUNT];
CARDEVENT* tutCardList[TOTALTUTORIALCARDSCOUNT];

CARDDECK tutorialDeck;


CARDDECK* currentDeck;
CARDEVENT* currentEvent;
int currentCardIndex;


#pragma region Card Events
CARDEVENT emptyCard = { 0, NULL_EVENT, NULL_TYPE_EVENT, "This is a null event, for errors only", NULL_CHOICE, 0, R_NULL_INDEX, NULL_CHOICE, 0 , "", NULL_CHOICE, 0, R_NULL_INDEX ,NULL_CHOICE, 0 , "" };


//TUTORIALS EVENTS [CARD INDEX STARTS WITH 0X]
CARDEVENT E_TutBuildAHouse = { 01, BASIC_EVENT, BUILD_TYPE_EVENT, "Our citizens needs a place to sleep. Click on the house below and build it on the grid.", B_HOUSE_INDEX, 2 , R_POPULATION_INDEX, R_GOLD_INDEX, 0, "House", NULL_CHOICE, 0 ,R_NULL_INDEX, R_NULL_INDEX, 0 ,"Ignore" };
CARDEVENT E_TutBuildAFarm = { 02, BASIC_EVENT, BUILD_TYPE_EVENT, "Our kingdom needs to food to survive. Click on the farm below and build it on the grid.", B_FARM_INDEX, 2 , R_FOOD_INDEX ,R_GOLD_INDEX,  0, "Farm", NULL_CHOICE, 0, R_NULL_INDEX, R_NULL_INDEX, 0, "Ignore" };
CARDEVENT E_TutBuildAMarket = { 03 , BASIC_EVENT, BUILD_TYPE_EVENT, "Our kingdom's economy is unstable. Click on the market below and build it on the grid.", B_MARKET_INDEX, 2 , R_GOLD_INDEX , R_GOLD_INDEX, 0, "Market", NULL_CHOICE, 0 , R_NULL_INDEX, R_NULL_INDEX, 0 , "Ignore" };

/*----------------------------------------------------------*/

//BASIC EVENTS - BUILD TYPE
CARDEVENT E_BuildAHouse = { 1, BASIC_EVENT, BUILD_TYPE_EVENT, "Our citizens needs a place to sleep. Click on the house below and build it on the grid.", B_HOUSE_INDEX, 1 , R_POPULATION_INDEX, R_GOLD_INDEX, 5, "House", NULL_CHOICE, 0 ,R_NULL_INDEX, R_NULL_INDEX, 0 ,"Ignore" };
CARDEVENT E_BuildAFarm = { 2,BASIC_EVENT, BUILD_TYPE_EVENT, "Our city needs to eat. Click on the farm below and build it on the grid.", B_FARM_INDEX, 1 , R_FOOD_INDEX ,R_GOLD_INDEX,  3, "Farm", NULL_CHOICE, 0, R_NULL_INDEX, R_NULL_INDEX, 0, "Ignore" };
CARDEVENT E_BuildAMarket = { 3 , BASIC_EVENT, BUILD_TYPE_EVENT, "Our economy is unstable. Click on the market below and build it on the grid.", B_MARKET_INDEX, 1 , R_GOLD_INDEX , R_GOLD_INDEX, 7, "Market", NULL_CHOICE, 0 , R_NULL_INDEX, R_NULL_INDEX, 0 , "Ignore" };

//BASIC EVENT - RESOURCE TYPE
CARDEVENT E_FoodMerchantArrival = { 4, BASIC_EVENT, RESOURCE_TYPE_EVENT, "Food Peddlers have arrived at your kingdom, they wish to trade 200 Food for 100 Gold", E_INCREASE_RESOURCE, 20, R_FOOD_INDEX, R_GOLD_INDEX, 10, "Buy Food", NULL_CHOICE, 0 , R_NULL_INDEX, R_NULL_INDEX, 0, "Ignore" };
CARDEVENT E_StolenFood = { 5 , BASIC_EVENT, RESOURCE_TYPE_EVENT, "It have come your notice that Food from the storages haven been stolen.", E_INCREASE_RESOURCE, 5 , R_MORALE_INDEX, R_NULL_INDEX ,0 , "Apprehend the Thief" , E_DECREASE_RESOURCE, 20, R_FOOD_INDEX, NULL_CHOICE, 0 , "Ignore the Thievery" };
CARDEVENT E_CircusTroupeVisit = { 6 , ADVANCED_EVENT, RESOURCE_TYPE_EVENT, "An Wandering Circus Troupe have arrived and is offering their services.", E_INCREASE_RESOURCE, 50, R_MORALE_INDEX, R_GOLD_INDEX, 70, "Hire their services", NULL_CHOICE, 0, R_NULL_INDEX, R_NULL_INDEX, 0, "Decline their services" };
CARDEVENT E_HeavyStorm = { 7 , BASIC_EVENT, RESOURCE_TYPE_EVENT, "A heavy storm has passed and many houses are currently damaged", E_INCREASE_RESOURCE, 5, R_MORALE_INDEX, R_GOLD_INDEX, 10, "Repair damaged houses", E_DECREASE_RESOURCE, 10, R_MORALE_INDEX, R_NULL_INDEX, 0, "Ignore the damaged house" };
CARDEVENT E_GoldMineDiscovered = { 10 , BASIC_EVENT, RESOURCE_TYPE_EVENT, "Our miners have discovered a fresh new gold mine", E_INCREASE_RESOURCE, 25, R_GOLD_INDEX, R_NULL_INDEX, 0, "Claim all the gold to the treasury", E_INCREASE_RESOURCE, 10, R_MORALE_INDEX, R_NULL_INDEX, 0, "Distribute the gold to your citizens" };
//ADVANCED EVENTS - BUILD TYPE
CARDEVENT E_RefugeesHouses = { 8 , ADVANCED_EVENT, BUILD_TYPE_EVENT,"Refugees are seeking shelter in your kingdom. Build two house to shelter them. ", B_HOUSE_INDEX, 2,R_POPULATION_INDEX, R_GOLD_INDEX, 10, "Houses", NULL_CHOICE,0, R_NULL_INDEX, R_NULL_INDEX, 0, "Ignore" };

CARDEVENT E_ContaminatedFood = { 12, BASIC_EVENT, RESOURCE_TYPE_EVENT, "The supplies in our granaries are contaminated.", E_INCREASE_RESOURCE, 5 , R_MORALE_INDEX, R_GOLD_INDEX, 10, "Replenish the Contaminated Food in our Granaries", E_DECREASE_RESOURCE, 20, R_FOOD_INDEX, R_MORALE_INDEX, 2, "Clear out the cotaminated food and leave it be" };
CARDEVENT E_DiseaseSpread = { 13, ADVANCED_EVENT, RESOURCE_TYPE_EVENT, "A Disease have spread through our Kingdom and we have no cure for it.", E_INCREASE_RESOURCE, 5, R_MORALE_INDEX, R_GOLD_INDEX, 100, "Purchase Cure from external source", E_DECREASE_RESOURCE, 10, R_POPULATION_INDEX, R_MORALE_INDEX, 5, "Confine and seclude the diseased." };
CARDEVENT E_VillagersKidnapped = { 14, BASIC_EVENT, RESOURCE_TYPE_EVENT, "Our Villagers have been kidnapped by bandits.", NULL_CHOICE, 0, R_NULL_INDEX, R_GOLD_INDEX, 20, "Hire Mercenaries to rescue the kidnapped", NULL_CHOICE, 0 , R_NULL_INDEX, R_POPULATION_INDEX, 3, "Dispatch the Local Militia to rescue the kidnapped." };
CARDEVENT E_MagnificentHunt = { 15, BASIC_EVENT, RESOURCE_TYPE_EVENT, "Our Hunters have return back with a magnificent hunt", E_INCREASE_RESOURCE, 50 , R_FOOD_INDEX, R_NULL_INDEX , 0, "Store the hunt as food for Future use", E_INCREASE_RESOURCE, 20, R_MORALE_INDEX, R_GOLD_INDEX, 15, "Hold a festival to celebrate the magnificent hunt" };
CARDEVENT E_CivilRevolt = { 16, ADVANCED_EVENT, RESOURCE_TYPE_EVENT, "A Revolt have broke out demanding for more food and gold, threating to leave the kingdom.", E_DECREASE_RESOURCE, 20, R_FOOD_INDEX, R_GOLD_INDEX, 10, "Please your citizen by giving into their demands.", E_DECREASE_RESOURCE, 10, R_POPULATION_INDEX, R_MORALE_INDEX, 3,  "Ignore their demands." };
CARDEVENT E_ScarceFood = { 17, BASIC_EVENT, BUILD_TYPE_EVENT, "Our Food Supplies are scarce", B_FARM_INDEX, 1, R_FOOD_INDEX, R_GOLD_INDEX, 3, "Build a Farm", E_DECREASE_RESOURCE, 2, R_MORALE_INDEX, R_NULL_INDEX, 0, "Refuse the request to build a farm" };
CARDEVENT E_MerchantSetUp = { 18, BASIC_EVENT, BUILD_TYPE_EVENT, "Various Merchant have requested to set up in your kingdom", B_MARKET_INDEX, 2, R_GOLD_INDEX, R_GOLD_INDEX , 15, "Invite the merchants in and set up 2 market spot for them.", NULL_CHOICE,0 , R_NULL_INDEX, R_NULL_INDEX, 0, "Reject the Merchants from entering." };
CARDEVENT E_NeedEntertainment = { 19, BASIC_EVENT, BUILD_TYPE_EVENT, "Our Citizen are bored and getting depress with the lack of entertainment", B_TAVERN_INDEX, 2 , R_GOLD_INDEX, R_GOLD_INDEX, 20, "Build 2 Taverns for our citizens to enjoy", E_DECREASE_RESOURCE, 5, R_MORALE_INDEX, R_NULL_INDEX ,0 ,"Ignore the plead from your citizen for entertainment" };
CARDEVENT E_EarthquakeIncoming = { 20, ADVANCED_EVENT, DESTROY_TYPE_EVENT, "It is fore told that a Earthquake would striking our kingdom", E_DESTROY_BUILDING, 1, R_BUILDING_HOUSE_INDEX, R_BUILDING_MARKET_INDEX, 1, "Reinforce our Farms and Taverns", E_DESTROY_BUILDING, 1, R_BUILDING_FARM_INDEX, R_BUILDING_TAVERN_INDEX, 1, "Reinforce our Houses and Market" };
//ADVANCED EVENTS - RESOURCE TYPE
CARDEVENT E_FamineStrikes = { 9, ADVANCED_EVENT, RESOURCE_TYPE_EVENT,"Famine have struck your kingdom, Distribute Food to advert the crisis. ", E_DECREASE_RESOURCE, 20, R_MORALE_INDEX, R_FOOD_INDEX, 30, "Distribute Food", E_DECREASE_RESOURCE, 10, R_MORALE_INDEX, R_POPULATION_INDEX, 5, "Ignore" };
CARDEVENT E_ArsonistAttack = { 11, ADVANCED_EVENT, RESOURCE_TYPE_EVENT, "An arsonist have set farms on fire.", E_INCREASE_RESOURCE, 10 , R_MORALE_INDEX, R_GOLD_INDEX, 20 , "Send Assistances to extinguish the fire", E_DECREASE_RESOURCE, 10, R_MORALE_INDEX, R_BUILDING_FARM_INDEX, 1, "Ignore and Let the fire extinguish by itself" };




#pragma endregion


void InitCardList()
{
	//Init Tutorial Card List
	tutCardList[0] = &emptyCard;
	tutCardList[1] = &E_TutBuildAHouse;
	tutCardList[2] = &E_TutBuildAFarm;
	tutCardList[3] = &E_TutBuildAMarket;

	//Init Card List
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

	for (int index = 0; index < TOTALTUTORIALCARDSCOUNT; index++) {
		tutorialDeck.cardIndexes[index] = index+1;
	}

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


CARDEVENT* GetNextEvent(GAMEPHASE gamePhase)
{

	switch (gamePhase) {

	case PHASE_BUILDPHASE:



		break;

	case PHASE_GAMEPHASE:

		if (currentCardIndex == currentDeck->cardsInDeck)
		{
			*currentEvent = emptyCard;
			return currentEvent;
		}
		currentEvent = cardList[currentDeck->cardIndexes[currentCardIndex]];
		++currentCardIndex; //Adding one counter to the Card Index after Drawing it
		return currentEvent;

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


