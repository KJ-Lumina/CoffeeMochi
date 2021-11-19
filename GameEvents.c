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
CARDEVENT E_TutBuildAHouse = { 1, BASIC_EVENT, BUILD_TYPE_EVENT, "Gorvernor, there is no houses for our citizen to stay. Build 2 houses by clicking on the grid.", B_HOUSE_INDEX, 2 , R_POPULATION_INDEX, R_GOLD_INDEX, 0, "House", NULL_CHOICE, 0 ,R_NULL_INDEX, R_NULL_INDEX, 0 ,"Ignore" };
CARDEVENT E_TutBuildAFarm = { 2, BASIC_EVENT, BUILD_TYPE_EVENT, "Gorvernor, there is currently no food source in our kingdom and our citizen are starving. Build 2 farm by clicking on the grid", B_FARM_INDEX, 2 , R_FOOD_INDEX ,R_GOLD_INDEX,  0, "Farm", NULL_CHOICE, 0, R_NULL_INDEX, R_NULL_INDEX, 0, "Ignore" };
CARDEVENT E_TutBuildAMarket = { 3 , BASIC_EVENT, BUILD_TYPE_EVENT, "Gorvernor, our economy is currently unstable. there is a need to set up markets to start our economy going. Build 2 market by clicking on the grid", B_MARKET_INDEX, 2 , R_GOLD_INDEX , R_GOLD_INDEX, 0, "Market", NULL_CHOICE, 0 , R_NULL_INDEX, R_NULL_INDEX, 0 , "Ignore" };

/*----------------------------------------------------------*/

//BASIC EVENTS - BUILD TYPE
CARDEVENT E_BuildAHouse = { 1, BASIC_EVENT, BUILD_TYPE_EVENT, "Gorvernor, our population have grown and citizen are requesting for more houses to be build", B_HOUSE_INDEX, 1 , R_POPULATION_INDEX, R_GOLD_INDEX, 5, "Proceed with the construction of a house in our vacant plot of land", NULL_CHOICE, 0 ,R_NULL_INDEX, R_MORALE_INDEX, 2 ,"There is enough Houses for everyone, those resource can be used for a better purpose." };
CARDEVENT E_BuildAMarket = { 2 , BASIC_EVENT, BUILD_TYPE_EVENT, "Gorvernor, our markets are getting too packed, constructing another market would help to spread the traffic.", B_MARKET_INDEX, 1 , R_GOLD_INDEX , R_GOLD_INDEX, 7, "Spreading the traffic would increase our income production, proceed ahead with the consturction.", NULL_CHOICE, 0, R_NULL_INDEX, R_MORALE_INDEX, 2 , "A market packed with traffic shows that our economy is good, there is not a need to construct another market." };
CARDEVENT E_ScarceFood = { 3, BASIC_EVENT, BUILD_TYPE_EVENT, "Gorvernor, there is a food shortage in our kingdom. We advice you to build a farm to solve the food shortage.", B_FARM_INDEX, 1, R_FOOD_INDEX, R_GOLD_INDEX, 3, "Get our workers to start with the construction of the farm", NULL_CHOICE, 0, R_NULL_INDEX, R_MORALE_INDEX, 2, "We have enough food supplies, there is not a need for another farm to be build" };
CARDEVENT E_MerchantSetUp = { 4, BASIC_EVENT, BUILD_TYPE_EVENT, "Gorvernor, with our kingdom reputation growth, various merchant have shown interest in settling down in our kingdom. We advice you to construct 2 markets to create market space for the new merchants", B_MARKET_INDEX, 2, R_GOLD_INDEX, R_GOLD_INDEX , 15, "Open our gates and invite the merchants in and set up 2 market spot for them.", NULL_CHOICE,0 , R_NULL_INDEX, R_NULL_INDEX, 0, "We dont have enough manpower on our hand now to construct markets right now." };
CARDEVENT E_NeedEntertainment = {5, BASIC_EVENT, BUILD_TYPE_EVENT, "Gorvernor, there is a lack of entertainment in our kingdom, our citizen are getting restless and bored. Construct a tavern to alleviate the lack of entertainment.", B_TAVERN_INDEX, 2 , R_GOLD_INDEX, R_GOLD_INDEX, 20, "Entertainment would increase our morale of our citizens, get our builders on it and proceed to construct the tavern", NULL_CHOICE, 0, R_NULL_INDEX, R_MORALE_INDEX ,5 ,"We need to focus on our necessities, entertainment can wait till those are settled." };

//BASIC EVENT - RESOURCE TYPE
CARDEVENT E_FoodMerchantArrival = { 6, BASIC_EVENT, RESOURCE_TYPE_EVENT, "Gorvernor, multiple food peddlers have arrived at your kingdom, they wish to sell us their wares", E_INCREASE_RESOURCE, 20, R_FOOD_INDEX, R_GOLD_INDEX, 10, "Inform them that we will be purchasing taking their wares and send carts over to deliver the wares to our granaries for storage.", NULL_CHOICE, 0 , R_NULL_INDEX, R_NULL_INDEX, 0, "We have a substantial amount of food in our granaries, there is not a need to purchase anymore." };
CARDEVENT E_StolenFood = { 7 , BASIC_EVENT, RESOURCE_TYPE_EVENT, "Gorvernor, there is a report of food that got stolen from our granaries. We advice to commence an investigate and apprehend the thief.", E_INCREASE_RESOURCE, 5 , R_MORALE_INDEX, R_NULL_INDEX ,0 , "Apprehend the Thief" , E_DECREASE_RESOURCE, 20, R_FOOD_INDEX, NULL_CHOICE, 0 , "Ignore the Thievery" };
CARDEVENT E_HeavyStorm = { 8 , BASIC_EVENT, RESOURCE_TYPE_EVENT, "Gorvernor, We have been hit by a heavy thunderstorm, many houses are currently damaged and citizens are requesting those to be repaired.", E_INCREASE_RESOURCE, 5, R_MORALE_INDEX, R_GOLD_INDEX, 10, "Repair damaged houses", E_DECREASE_RESOURCE, 10, R_MORALE_INDEX, R_NULL_INDEX, 0, "Ignore the damaged house" };
CARDEVENT E_GoldMineDiscovered = { 9 , BASIC_EVENT, RESOURCE_TYPE_EVENT, "Gorvernor, our miners have discovered a new untouched gold mine. they are currently waiting for instruction on how to deal with it.", E_INCREASE_RESOURCE, 25, R_GOLD_INDEX, R_NULL_INDEX, 0, "Claim all the gold to the treasury", E_INCREASE_RESOURCE, 10, R_MORALE_INDEX, R_NULL_INDEX, 0, "Distribute the gold to your citizens" };
CARDEVENT E_ContaminatedFood = { 10, BASIC_EVENT, RESOURCE_TYPE_EVENT, "Gorvernor, we have discovered that the food supplies in our granaries are contaminated. We need to clear them out as soon as possible to prevent further contaminations", E_INCREASE_RESOURCE, 5 , R_MORALE_INDEX, R_GOLD_INDEX, 10, "Replenish the Contaminated Food in our Granaries", E_DECREASE_RESOURCE, 20, R_FOOD_INDEX, R_MORALE_INDEX, 2, "Clear out the cotaminated food and leave it be" };
CARDEVENT E_VillagersKidnapped = { 11, BASIC_EVENT, RESOURCE_TYPE_EVENT, "Gorvernor, we have receive reports that some of our villagers have been kidnapped by bandits during a skirmish. We have to send out troops to rescue them as soon as possible.", NULL_CHOICE, 0, R_NULL_INDEX, R_GOLD_INDEX, 20, "Hire Mercenaries to rescue the kidnapped", NULL_CHOICE, 0 , R_NULL_INDEX, R_POPULATION_INDEX, 3, "Dispatch the Local Militia to rescue the kidnapped." };
CARDEVENT E_MagnificentHunt = { 12, BASIC_EVENT, RESOURCE_TYPE_EVENT, "Gorvernor, our hunters have return back from the wildness with a magnificent hunt. We are waiting for your instruction on what to do with the hunt.", E_INCREASE_RESOURCE, 50 , R_FOOD_INDEX, R_NULL_INDEX , 0, "Store the hunt as food for future use", E_INCREASE_RESOURCE, 20, R_MORALE_INDEX, R_GOLD_INDEX, 15, "Hold a festival to celebrate the magnificent hunt" };


//ADVANCED EVENTS - BUILD TYPE
CARDEVENT E_RefugeesHouses = { 13 , ADVANCED_EVENT, BUILD_TYPE_EVENT,"Gorvernor, multiple refugees are seeking shelter in your kingdom. What is your verdict on letting them into our kingdom.", B_HOUSE_INDEX, 2,R_POPULATION_INDEX, R_GOLD_INDEX, 10, "Houses", NULL_CHOICE,0, R_NULL_INDEX, R_NULL_INDEX, 0, "Ignore" };


//ADVANCED EVENTS - RESOURCE TYPE
CARDEVENT E_FamineStrikes = { 14, ADVANCED_EVENT, RESOURCE_TYPE_EVENT,"Gorvernor, a famine have struck your kingdom, we advice to distribute our backup food supplies to every household to prevent any further crisis from happening.", E_DECREASE_RESOURCE, 20, R_MORALE_INDEX, R_FOOD_INDEX, 30, "Distribute Food", E_DECREASE_RESOURCE, 10, R_MORALE_INDEX, R_POPULATION_INDEX, 5, "Ignore" };
CARDEVENT E_ArsonistAttack = { 15, ADVANCED_EVENT, RESOURCE_TYPE_EVENT, "Gorvernor, our farms are on fire and reports have came in that there is an arsonist on the loose.", E_INCREASE_RESOURCE, 10 , R_MORALE_INDEX, R_GOLD_INDEX, 20 , "Send Assistances to extinguish the fire", E_DECREASE_RESOURCE, 10, R_MORALE_INDEX, R_BUILDING_FARM_INDEX, 1, "Ignore and Let the fire extinguish by itself" };
CARDEVENT E_CircusTroupeVisit = { 16 , ADVANCED_EVENT, RESOURCE_TYPE_EVENT, "Gorvernor, an wandering circus troupe have arrived at our kingdom and is offering their services to brighten the mood of the kingdom.", E_INCREASE_RESOURCE, 50, R_MORALE_INDEX, R_GOLD_INDEX, 70, "Hire their services", NULL_CHOICE, 0, R_NULL_INDEX, R_NULL_INDEX, 0, "Decline their services" };
CARDEVENT E_CivilRevolt = { 17, ADVANCED_EVENT, RESOURCE_TYPE_EVENT, "Gorvernor, a revolt have broken out and citizens are demanding for more food and gold while threating to leave the kingdom if they do not receive them.", E_DECREASE_RESOURCE, 20, R_FOOD_INDEX, R_GOLD_INDEX, 10, "Please your citizen by giving into their demands.", E_DECREASE_RESOURCE, 10, R_POPULATION_INDEX, R_MORALE_INDEX, 3,  "Ignore their demands." };
CARDEVENT E_DiseaseSpread = { 18, ADVANCED_EVENT, RESOURCE_TYPE_EVENT, "Gorvernor, a disease have spread through our kingdom and we currently have no cure for it. However, there is information that a nearby allied kingdom have developed a cure for it.", E_INCREASE_RESOURCE, 5, R_MORALE_INDEX, R_GOLD_INDEX, 50, "Purchase Cure from external source", E_DECREASE_RESOURCE, 10, R_POPULATION_INDEX, R_MORALE_INDEX, 5, "Confine and seclude the diseased." };

//DESTROY TYPE EVENTS
CARDEVENT E_EarthquakeIncoming = { 19, ADVANCED_EVENT, DESTROY_TYPE_EVENT, "Gorvernor, news from nearby kingdom have reach our ears that a earthquake is coming and would striking our kingdom soon. There is an urgent need to make preparation for it when it arrives.", E_DESTROY_BUILDING, 1, R_BUILDING_HOUSE_INDEX, R_BUILDING_MARKET_INDEX, 1, "Reinforce our Farms and Taverns", E_DESTROY_BUILDING, 1, R_BUILDING_FARM_INDEX, R_BUILDING_TAVERN_INDEX, 1, "Reinforce our Houses and Market" };

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


