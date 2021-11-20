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


#define TOTALCARDCOUNT 21
#define TOTALTUTORIALCARDSCOUNT 3
CARDEVENT* cardList[TOTALCARDCOUNT];
CARDEVENT* tutCardList[TOTALTUTORIALCARDSCOUNT];

CARDDECK tutorialDeck;
CARDDECK cardDeck;


CARDDECK* currentDeck;
CARDEVENT* currentEvent;
int currentCardIndex;
#pragma region Reward Cards
REWARDCARD R_House		= { 1, BUILD_TYPE_EVENT, B_HOUSE_INDEX , 1, "Click on the grid to place the house."};
REWARDCARD R_Farm		= { 2, BUILD_TYPE_EVENT, B_FARM_INDEX , 1, "Click on the grid to place the farm." };
REWARDCARD R_Market		= { 3, BUILD_TYPE_EVENT, B_MARKET_INDEX , 1, "Click on the grid to place the market." };
REWARDCARD R_Tavern		= { 4, BUILD_TYPE_EVENT, B_TAVERN_INDEX , 1, "Click on the grid to place the tavern." };

#pragma endregion

#pragma region Card Events
CARDEVENT emptyCard = { 0, NULL_TYPE_EVENT, "This is a null event, for errors only"
, {0,0,0,0}, {0,0}, "Why are you reading this? This is a bug. Ignore and move along."
, {0,0,0,0}, {0,0}, "You are looking at a bug right now. Please ignore." };

//TUTORIALS EVENTS [CARD INDEX STARTS WITH 0X]
CARDEVENT E_TutBuildAHouse = { 1, BUILD_TYPE_EVENT,"Gorvernor, there is no houses for our citizen to stay. Build 2 houses by clicking on the grid."
, {0,0,0,0}, {1, 2},"Get our workers gather up materials and start working on constructing 2 houses"
, {0,0,0,0}, {1, 2},"There are some damaged house nearby, we can repair them to a usable condition. Send our workers to build materails and start on the repairs on them. " };
CARDEVENT E_TutBuildAFarm = { 2, BUILD_TYPE_EVENT, "Gorvernor, there is currently no food source in our kingdom and our citizen are starving. Build 2 farm by clicking on the grid"
, {0,0,0,0}, {2, 2}, "Scout out 2 decent place that is suited to for agriculture and start the constructing of the farms in those location."
, {0,0,0,0}, {2, 2}, "Having self-sufficient food suplies is extremely crucial for a kingdowm. Gather out workers and start constructing the 2 farms regardless of location as soon as possible."};
CARDEVENT E_TutBuildAMarket = { 3 , BUILD_TYPE_EVENT, "Gorvernor, our economy is currently unstable. there is a need to set up markets to start our economy going. Build 2 market by clicking on the grid"
, {0,0,0,0}, {3, 2}, "Start the construction of 2 markets and send out invitations to all citizens that would want to set up their business there."
, {0,0,0,0}, {3, 2}, "Send invitations out to all allied kingdom for any merchants that would want to set up trade in our kingdom. Start the constructing the markets so that the merchants have a place to sell their wares when they arrive." };

/*----------------------------------------------------------*/

//BASIC EVENTS - BUILD TYPE
CARDEVENT E_BuildAHouse = { 1, BUILD_TYPE_EVENT, B_HOUSE_INDEX, 1 , R_POPULATION_INDEX, R_GOLD_INDEX, 5, NULL_CHOICE, 0 ,R_NULL_INDEX, R_MORALE_INDEX, 2
, "Gorvernor, our population have grown and citizen are requesting for more houses to be build"
, "Proceed with the construction of a house in our vacant plot of land"
, "There is enough Houses for everyone, those resource can be used for a better purpose." };
CARDEVENT E_BuildAMarket = { 2 , BUILD_TYPE_EVENT, B_MARKET_INDEX, 1 , R_GOLD_INDEX , R_GOLD_INDEX, 7, NULL_CHOICE, 0, R_NULL_INDEX, R_MORALE_INDEX, 2
, "Gorvernor, our markets are getting too packed, constructing another market would help to spread the traffic."
, "Spreading the traffic would increase our income production, proceed ahead with the consturction." 
, "A market packed with traffic shows that our economy is good, there is not a need to construct another market." };
CARDEVENT E_ScarceFood = { 3, BUILD_TYPE_EVENT, B_FARM_INDEX, 1, R_FOOD_INDEX, R_GOLD_INDEX, 3, NULL_CHOICE, 0, R_NULL_INDEX, R_MORALE_INDEX, 2
, "Gorvernor, there is a food shortage in our kingdom. We advice you to build a farm to solve the food shortage."
, "Get our workers to start with the construction of the farm"
, "We have enough food supplies, there is not a need for another farm to be build" };
CARDEVENT E_MerchantSetUp = { 4, BUILD_TYPE_EVENT, B_MARKET_INDEX, 2, R_GOLD_INDEX, R_GOLD_INDEX , 15, NULL_CHOICE, 0 , R_NULL_INDEX, R_NULL_INDEX, 0
, "Gorvernor, with our kingdom reputation growth, various merchant have shown interest in settling down in our kingdom. We advice you to construct 2 markets to create market space for the new merchants"
, "Open our gates and invite the merchants in and set up 2 market spot for them."
, "We dont have enough manpower on our hand now to construct markets right now." };
CARDEVENT E_BuidldATavern = {5, BUILD_TYPE_EVENT, B_TAVERN_INDEX, 1 , R_GOLD_INDEX, R_GOLD_INDEX, 10, NULL_CHOICE, 0, R_NULL_INDEX, R_NULL_INDEX ,0
, "Gorvernor, there is a lack of entertainment in our kingdom, our citizen are getting restless and bored. Construct a tavern to alleviate the lack of entertainment."
, "Entertainment would increase our morale of our citizens, get our builders on it and proceed to construct the tavern"
, "We need to focus on our necessities, entertainment can wait till those are settled." };


//BASIC EVENT - RESOURCE TYPE
CARDEVENT E_FoodMerchantArrival = { 6, RESOURCE_TYPE_EVENT, E_INCREASE_RESOURCE, 20, R_FOOD_INDEX, R_GOLD_INDEX, 10, NULL_CHOICE, 0 , R_NULL_INDEX, R_NULL_INDEX, 0
, "Gorvernor, multiple food peddlers have arrived at your kingdom, they wish to sell us their wares"
, "Inform them that we will be purchasing taking their wares and send carts over to deliver the wares to our granaries for storage."
, "We have a substantial amount of food in our granaries, there is not a need to purchase anymore." };
CARDEVENT E_StolenFood = { 7 , RESOURCE_TYPE_EVENT, E_INCREASE_RESOURCE, 5 , R_MORALE_INDEX, R_NULL_INDEX ,0, E_DECREASE_RESOURCE, 20, R_FOOD_INDEX, R_MORALE_INDEX, 2
, "Gorvernor, there is a report of food that got stolen from our granaries. We advice to commence an investigate and apprehend the culprit."
, "Our kingdom needs to be a place where the citizen can feel safe, proceed on conducting an investigation on the stolen food and apprehend the culprit" 
, "The culprit have its reason to steal food, let this incident pass but increase our security around the granaries." };
CARDEVENT E_HeavyStorm = { 8 , RESOURCE_TYPE_EVENT, E_INCREASE_RESOURCE, 5, R_MORALE_INDEX, R_GOLD_INDEX, 10, E_DECREASE_RESOURCE, 10, R_MORALE_INDEX, R_NULL_INDEX, 0
, "Gorvernor, We have been hit by a heavy thunderstorm, many houses are currently damaged and citizens are requesting those to be repaired."
, "Divide our manpower on sortting out the damages from the storm but focus on repairing the damaged houses"
, "Clean up the damages that the storm have done, the repairs can come later" };
CARDEVENT E_GoldMineDiscovered = { 9 , RESOURCE_TYPE_EVENT, E_INCREASE_RESOURCE, 25, R_GOLD_INDEX, R_NULL_INDEX, 0, E_INCREASE_RESOURCE, 10, R_MORALE_INDEX, R_NULL_INDEX, 0
, "Gorvernor, our miners have discovered an hoard of gold in an untouched gold mine. They are currently waiting for instruction on how to deal with it."
, "Arrange carts to bring the gold back to the treasury, we will sell them to increase our capital."
, "Arrange multiple carts to distribute the gold to all the households. They deserve the additional wealth for their hardwork." };
CARDEVENT E_ContaminatedFood = { 10, RESOURCE_TYPE_EVENT, E_INCREASE_RESOURCE, 5 , R_MORALE_INDEX, R_GOLD_INDEX, 10, E_DECREASE_RESOURCE, 20, R_FOOD_INDEX, R_MORALE_INDEX, 2
, "Gorvernor, we have discovered that the food supplies in our granaries are contaminated. We need to clear them out as soon as possible to prevent further contaminations"
, "Clear out the contaminated food quickly and start replenishing the contaminated food in our granaries"
, "Get our workers to clear out the cotaminated food as quickly as they can. There is not a need to replenish the contaminated food, however find the source of the contamination." };
CARDEVENT E_VillagersKidnapped = { 11, RESOURCE_TYPE_EVENT, NULL_CHOICE, 0, R_NULL_INDEX, R_GOLD_INDEX, 20, NULL_CHOICE, 0 , R_NULL_INDEX, R_POPULATION_INDEX, 3
, "Gorvernor, we have receive reports that some of our citizens have been kidnapped by bandits during a skirmish. We have to send out troops to rescue them as soon as possible."
, "Hire the mercenaries that have reached out to us to rescue the kidnapped citizens"
, "Get our local militia ready and dispatch them to rescue the kidnapped citizen as soon as possible." };
CARDEVENT E_MagnificentHunt = { 12, RESOURCE_TYPE_EVENT, E_INCREASE_RESOURCE, 50 , R_FOOD_INDEX, R_NULL_INDEX , 0, E_INCREASE_RESOURCE, 20, R_MORALE_INDEX, R_GOLD_INDEX, 15
, "Gorvernor, our hunters have return back from the wildness with a magnificent hunt. We are waiting for your instruction on what to do with the hunt."
, "Gather the food up and store the hunt as food in our granaries for future use"
, "This calls for a celebration. Use some of our gold in the coffers to hold a festival to celebrate the magnificent hunt." };


//ADVANCED EVENTS - BUILD TYPE
CARDEVENT E_RefugeesHouses = { 13 , BUILD_TYPE_EVENT, B_HOUSE_INDEX, 2,R_POPULATION_INDEX, R_GOLD_INDEX, 10, NULL_CHOICE,0, R_NULL_INDEX, R_NULL_INDEX, 0
,"Gorvernor, multiple refugees are seeking shelter in your kingdom. What is your verdict on letting them into our kingdom."
, "Let them enter. We can house those refugees but put them to work."
, "Reject their entry to our kingdom. They might be spies from another kingdom, its too dangerous to let them enter." };
CARDEVENT E_NeedEntertainment = { 14, BUILD_TYPE_EVENT, B_TAVERN_INDEX, 2, R_MORALE_INDEX,R_GOLD_INDEX, 20, NULL_CHOICE, 0 , R_NULL_INDEX, R_MORALE_INDEX, 5
, "Gorvernor, our population have grown and the citizen desperately needs entertainment to have a place to relieve their stress level. We advice you to construct 2 taverns for the citzen to relax in."
, "Our citizen have work hard and deserve a place to relax after their working hours. Proceed with the construction of the 2 taverns and finish it as soon as possible."
, "There are other part of the kingdom that needs to be focus on now. The construction of the taverns will start once those are handled." };

//ADVANCED EVENTS - RESOURCE TYPE
CARDEVENT E_FamineStrikes = { 15, RESOURCE_TYPE_EVENT, E_DECREASE_RESOURCE, 20, R_MORALE_INDEX, R_FOOD_INDEX, 30, E_DECREASE_RESOURCE, 10, R_MORALE_INDEX, R_POPULATION_INDEX, 5
,"Gorvernor, a famine have struck your kingdom, we advice to distribute our backup food supplies to every household to prevent any further crisis from happening."
, "Get all our manpower ready, we need to start distributing the food supplies to each household to overcome this famine."
, "We must brace through this famine, Focus on helping those crucial citizens, make sure they have food to survive through this famine." };
CARDEVENT E_ArsonistAttack = { 16, RESOURCE_TYPE_EVENT, E_INCREASE_RESOURCE, 10 , R_MORALE_INDEX, R_GOLD_INDEX, 20, E_DECREASE_RESOURCE, 10, R_MORALE_INDEX, R_BUILDING_FARM_INDEX, 1
, "Gorvernor, our farms are on fire and reports have came in that there is an arsonist on the loose."
, "Send all our mens to assistances in extinguishing the fire and rebuild the farm to a working condition."
, "The farm that is on fire is no within vacinity of any other farms. Make sure all citizen are evacuate and let fire extinguish by itself." };
CARDEVENT E_CircusTroupeVisit = { 17 , RESOURCE_TYPE_EVENT, E_INCREASE_RESOURCE, 50, R_MORALE_INDEX, R_GOLD_INDEX, 70, NULL_CHOICE, 0, R_NULL_INDEX, R_NULL_INDEX, 0
, "Gorvernor, an wandering circus troupe have arrived at our kingdom and is offering their services to brighten the mood of the kingdom."
, "Our citzen would enjoy the entertainment provided by the troupe. Send a representative to invite them into our kingdom and proceed to hire their service to entertain our citizens"
, "We need to converse our gold for the development of our kingdom. Send a representative down to kindly decline their services." };
CARDEVENT E_CivilRevolt = { 18, RESOURCE_TYPE_EVENT, E_DECREASE_RESOURCE, 20, R_FOOD_INDEX, R_GOLD_INDEX, 10, E_DECREASE_RESOURCE, 10, R_POPULATION_INDEX, R_MORALE_INDEX, 3
, "Gorvernor, a revolt have broken out and citizens are demanding for more food and gold while threating to leave the kingdom if they do not receive them."
, "The kingdom would not exist without our citizens. Distribute gold and food to our citizen and keep them pleased."
,  "We need be assertive and not submit to the citizen demands" };
CARDEVENT E_DiseaseSpread = { 19, RESOURCE_TYPE_EVENT, E_INCREASE_RESOURCE, 5, R_MORALE_INDEX, R_GOLD_INDEX, 50, E_DECREASE_RESOURCE, 10, R_POPULATION_INDEX, R_MORALE_INDEX, 5
, "Gorvernor, a disease have spread through our kingdom and we currently have no cure for it. However, there is information that a nearby allied kingdom have developed a cure for it."
, "Quickly, isolated the diseased to prevent further spreading and send messenger over to inform them that we will be purchasing thier cure for the disease."
, "Confine and insolate the diseased. Start working on developing the cure. We cannot afford to show our weakness to the kingdom and have us owe them a favour." };

//DESTROY TYPE EVENTS
CARDEVENT E_EarthquakeIncoming = { 20, DESTROY_TYPE_EVENT, E_DESTROY_BUILDING, 1, R_BUILDING_HOUSE_INDEX, R_BUILDING_MARKET_INDEX, 1, E_DESTROY_BUILDING, 1, R_BUILDING_FARM_INDEX, R_BUILDING_TAVERN_INDEX, 1
, "Gorvernor, news from nearby kingdom have reach our ears that a earthquake is coming and would striking our kingdom soon. There is an urgent need to make preparation for it when it arrives."
, "Sound the emergency alarm. Get all of our citizen to start reinforcing all our building but focus on the Farms and Taverns."
, "Sound the emergency alarm. Get all of our citizen to start reinforcing all our building but focus on the Farms and Taverns." };

#pragma endregion


void InitCardList()
{
	//Init Tutorial Card List
	tutCardList[0] = &E_TutBuildAHouse;
	tutCardList[1] = &E_TutBuildAFarm;
	tutCardList[2] = &E_TutBuildAMarket;

	//Init Card List
	cardList[0] = &E_BuidldATavern;
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
	cardList[15] = &E_RefugeesHouses;
	cardList[16] = &E_NeedEntertainment;
	cardList[17] = &E_CivilRevolt;
	cardList[18] = &E_BuildAHouse;
	cardList[19] = &E_VillagersKidnapped;
	cardList[20] = &E_DiseaseSpread;
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


