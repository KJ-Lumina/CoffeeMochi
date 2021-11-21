#include <stdio.h>
#include "cprocessing.h"
#include "Common_Headers.h"
#include <stdlib.h>


typedef struct
{
	int cardIndexes[20];
	int cardsInDeck;
}CARDDECK;


/*typedef struct
{
	int eventIndex; // unique ID
	int eventType; //
	char* description;
	int resourceChangeA[4];
	int resourceRewardA[2];
	char* descriptionA;
	int resourceChangeB[4];
	int resourceRewardB[2];
	char* descriptionB;
}CARDEVENT; */

CP_Image HouseCardSprite;
CP_Image FarmCardSprite;
CP_Image MarketCardSprite;
CP_Image TavernCardSprite;

#define TOTALCARDCOUNT 21
#define TOTALTUTORIALCARDSCOUNT 3
#define TOTALREWARDCARDCOUNT 5
CARDEVENT* cardList[TOTALCARDCOUNT];
CARDEVENT* tutCardList[TOTALTUTORIALCARDSCOUNT];
REWARDCARD* rewardCardList[TOTALREWARDCARDCOUNT];

CARDDECK tutorialDeck;
CARDDECK cardDeck;



CARDDECK* currentDeck;
CARDEVENT* currentEvent;
int currentCardIndex;

#pragma region Reward Cards
REWARDCARD R_NullCard = { 0, NULL_CHOICE, R_NULL_INDEX, 0, "This is a null reward." };
REWARDCARD R_HouseCard	= { 1, BUILD_TYPE_EVENT, B_HOUSE_INDEX , 1, "Click on the grid to place the house."};
REWARDCARD R_FarmCard	= { 2, BUILD_TYPE_EVENT, B_FARM_INDEX , 1, "Click on the grid to place the farm." };
REWARDCARD R_MarketCard	= { 3, BUILD_TYPE_EVENT, B_MARKET_INDEX , 1, "Click on the grid to place the market." };
REWARDCARD R_TavernCard	= { 4, BUILD_TYPE_EVENT, B_TAVERN_INDEX , 1, "Click on the grid to place the tavern." };

#pragma endregion

#pragma region Card Events
CARDEVENT emptyCard = { 0, NULL_TYPE_EVENT, "This is a null event, for errors only"
, {0,0,0,0}, {0,0}, "Why are you reading this? This is a bug. Ignore and move along."
, {0,0,0,0}, {0,0}, "You are looking at a bug right now. Please ignore." };

//TUTORIALS EVENTS [CARD INDEX STARTS WITH 0X]
CARDEVENT E_TutBuildAHouse = { 1, BUILD_TYPE_EVENT,"Gorvernor, there is no houses for our citizen to stay. Build 2 houses by clicking on the grid."
, {0,0,0,0}, {1, 2},"Get our workers to gather up materials and start working on constructing 2 houses"
, {0,0,0,0}, {1, 2},"There are some damaged house nearby, we can repair them to a usable condition. Send our workers to build materails and start on the repairs on them. " };
CARDEVENT E_TutBuildAFarm = { 2, BUILD_TYPE_EVENT, "Gorvernor, there is currently no food source in our kingdom and our citizen are starving. Build 2 farm by clicking on the grid"
, {0,0,0,0}, {2, 2}, "Scout out 2 decent place that is suited to for agriculture and start the constructing of the farms in those location."
, {0,0,0,0}, {2, 2}, "Having self-sufficient food suplies is extremely crucial for a kingdowm. Gather out workers and start constructing the 2 farms regardless of location as soon as possible."};
CARDEVENT E_TutBuildAMarket = { 3 , BUILD_TYPE_EVENT, "Gorvernor, our economy is currently unstable. there is a need to set up markets to start our economy going. Build 2 market by clicking on the grid"
, {0,0,0,0}, {3, 2}, "Start the construction of 2 markets and send out invitations to all citizens that would want to set up their business there."
, {0,0,0,0}, {3, 2}, "Send invitations out to all allied kingdom for any merchants that would want to set up trade in our kingdom. Start the constructing the markets so that the merchants have a place to sell their wares when they arrive." };

/*----------------------------------------------------------*/

//BASIC EVENTS - BUILD TYPE
CARDEVENT E_BuildAHouse = { 1, BUILD_TYPE_EVENT, "Gorvernor, our population have grown and citizen are requesting for more houses to be build"
, {5,0,0,0}, {1,1}, "Proceed with the construction of a house in our vacant plot of land"
, {0,0,0,2}, {0,0}, "There is enough Houses for everyone, those resource can be used for a better purpose." };
CARDEVENT E_BuildAMarket = { 2 , BUILD_TYPE_EVENT, "Gorvernor, our markets are getting too packed, constructing another market would help to spread the traffic."
, {7,0,0,0}, {3,1}, "Spreading the traffic would increase our income production, proceed ahead with the consturction."
, {0,0,0,2}, {0,0}, "A market packed with traffic shows that our economy is good, there is not a need to construct another market." };
CARDEVENT E_ScarceFood = { 3, BUILD_TYPE_EVENT, "Gorvernor, there is a food shortage in our kingdom. We advice you to build a farm to solve the food shortage."
, {10,0,0,0},{2,2},"Get our workers to start with the construction of the farm"
, {0,0,0,2},{0,0}, "We have enough food supplies, there is not a need for another farm to be build" };
CARDEVENT E_MerchantSetUp = { 4, BUILD_TYPE_EVENT, "Gorvernor, with our kingdom reputation growth, various merchant have shown interest in settling down in our kingdom. We advice you to construct 2 markets to create market space for the new merchants"
, {15,0,0,0}, {3,2}, "Open our gates and invite the merchants in and set up 2 market spot for them."
, {0,0,0,0}, {0,0}, "We dont have enough manpower on our hand now to construct markets right now." };
CARDEVENT E_BuidldATavern = {5, BUILD_TYPE_EVENT, "Gorvernor, there is a lack of entertainment in our kingdom, our citizen are getting restless and bored. Construct a tavern to alleviate the lack of entertainment."
, {10,0,0,0}, {4,1}, "Entertainment would increase our morale of our citizens, get our builders on it and proceed to construct the tavern"
, {0,0,0,0}, {0,0}, "We need to focus on our necessities, entertainment can wait till those are settled." };


//BASIC EVENT - RESOURCE TYPE
CARDEVENT E_FoodMerchantArrival = { 6, RESOURCE_TYPE_EVENT, "Gorvernor, multiple food peddlers have arrived at your kingdom, they wish to sell us their wares"
, {-10,20,0,0}, {0,0}, "Inform them that we will be purchasing taking their wares and send carts over to deliver the wares to our granaries for storage."
, {0,0,0,0}, {0,0}, "We have a substantial amount of food in our granaries, there is not a need to purchase anymore." };
CARDEVENT E_StolenFood = { 7 , RESOURCE_TYPE_EVENT,"Gorvernor, there is a report of food that got stolen from our granaries. We advice to commence an investigate and apprehend the culprit."
, {0,0,0,5}, {0,0}, "Our kingdom needs to be a place where the citizen can feel safe, proceed on conducting an investigation on the stolen food and apprehend the culprit"
, {0,-20,0,0}, {0,0}, "The culprit have its reason to steal food, let this incident pass but increase our security around the granaries." };
CARDEVENT E_HeavyStorm = { 8 , RESOURCE_TYPE_EVENT,"Gorvernor, We have been hit by a heavy thunderstorm, many houses are currently damaged and citizens are requesting those to be repaired."
, {-10,0,0,5}, {0,0}, "Divide our manpower on sortting out the damages from the storm but focus on repairing the damaged houses"
, {0,0,0,-10}, {0,0}, "Clean up the damages that the storm have done, the repairs can come later" };
CARDEVENT E_GoldMineDiscovered = { 9 , RESOURCE_TYPE_EVENT, "Gorvernor, our miners have discovered an hoard of gold in an untouched gold mine. They are currently waiting for instruction on how to deal with it."
, {25,0,0,0}, {0,0}, "Arrange carts to bring the gold back to the treasury, we will sell them to increase our capital."
, {0,0,0,10} , {0,0}, "Arrange multiple carts to distribute the gold to all the households. They deserve the additional wealth for their hardwork." };
CARDEVENT E_ContaminatedFood = { 10, RESOURCE_TYPE_EVENT, "Gorvernor, we have discovered that the food supplies in our granaries are contaminated. We need to clear them out as soon as possible to prevent further contaminations"
, {-10,0,0,5}, {0,0}, "Clear out the contaminated food quickly and start replenishing the contaminated food in our granaries"
, {0,-20,0,-2}, {0,0}, "Get our workers to clear out the cotaminated food as quickly as they can. There is not a need to replenish the contaminated food, however find the source of the contamination." };
CARDEVENT E_VillagersKidnapped = { 11, RESOURCE_TYPE_EVENT, "Gorvernor, we have receive reports that some of our citizens have been kidnapped by bandits during a skirmish. We have to send out troops to rescue them as soon as possible."
, {-20,0,0,0}, {0,0}, "Hire the mercenaries that have reached out to us to rescue the kidnapped citizens"
, {0,0,-3, 0}, {0,0}, "Get our local militia ready and dispatch them to rescue the kidnapped citizen as soon as possible." };
CARDEVENT E_MagnificentHunt = { 12, RESOURCE_TYPE_EVENT, "Gorvernor, our hunters have return back from the wildness with a magnificent hunt. We are waiting for your instruction on what to do with the hunt."
, {0,50,0,0}, {0,0}, "Gather the food up and store the hunt as food in our granaries for future use"
, {-15,0,0,20}, {0,0}, "This calls for a celebration. Use some of our gold in the coffers to hold a festival to celebrate the magnificent hunt." };


//ADVANCED EVENTS - BUILD TYPE
CARDEVENT E_RefugeesHouses = { 13 , BUILD_TYPE_EVENT, "Gorvernor, multiple refugees are seeking shelter in your kingdom. What is your verdict on letting them into our kingdom."
, {-10,0,0,0} , {1,2}, "Let them enter. We can house those refugees but put them to work."
, {0,0,0,0}, {0,0}, "Reject their entry to our kingdom. They might be spies from another kingdom, its too dangerous to let them enter." };
CARDEVENT E_NeedEntertainment = { 14, BUILD_TYPE_EVENT, "Gorvernor, our population have grown and the citizen desperately needs entertainment to have a place to relieve their stress level. We advice you to construct 2 taverns for the citzen to relax in."
, {-20,0,0,0}, {4,2}, "Our citizen have work hard and deserve a place to relax after their working hours. Proceed with the construction of the 2 taverns and finish it as soon as possible."
, {0,0,0,-5}, {0,0}, "There are other part of the kingdom that needs to be focus on now. The construction of the taverns will start once those are handled." };

//ADVANCED EVENTS - RESOURCE TYPE
CARDEVENT E_FamineStrikes = { 15, RESOURCE_TYPE_EVENT, "Gorvernor, a famine have struck your kingdom, we advice to distribute our backup food supplies to every household to prevent any further crisis from happening."
,{0,-30,0,10}, {0,0}, "Get all our manpower ready, we need to start distributing the food supplies to each household to overcome this famine."
, {0,0,0,-5}, {0,0}, "We must brace through this famine, Focus on helping those crucial citizens, make sure they have food to survive through this famine." };
CARDEVENT E_ArsonistAttack = { 16, RESOURCE_TYPE_EVENT, "Gorvernor, our farms are on fire and reports have came in that there is an arsonist on the loose."
, {-20,0,0,10}, {0,0}, "Send all our mens to assistances in extinguishing the fire and rebuild the farm to a working condition."
, {0,0,0,-10}, {1,-1}, "The farm that is on fire is no within vacinity of any other farms. Make sure all citizen are evacuate and let fire extinguish by itself." };
CARDEVENT E_CircusTroupeVisit = { 17 , RESOURCE_TYPE_EVENT, "Gorvernor, an wandering circus troupe have arrived at our kingdom and is offering their services to brighten the mood of the kingdom."
, {-50,0,0,30}, {0,0}, "Our citzen would enjoy the entertainment provided by the troupe. Send a representative to invite them into our kingdom and proceed to hire their service to entertain our citizens"
, {0,0,0,0}, {0,0}, "We need to converse our gold for the development of our kingdom. Send a representative down to kindly decline their services." };
CARDEVENT E_CivilRevolt = { 18, RESOURCE_TYPE_EVENT,"Gorvernor, a revolt have broken out and citizens are demanding for more food and gold while threating to leave the kingdom if they do not receive them."
, {10,-20,0,0}, {0,0}, "The kingdom would not exist without our citizens. Distribute gold and food to our citizen and keep them pleased."
, {0,0,-10,-3}, {0,0}, "We need be assertive and not submit to the citizen demands" };
CARDEVENT E_DiseaseSpread = { 19, RESOURCE_TYPE_EVENT, "Gorvernor, a disease have spread through our kingdom and we currently have no cure for it. However, there is information that a nearby allied kingdom have developed a cure for it."
, {-50,0,0,5}, {0,0}, "Quickly, isolated the diseased to prevent further spreading and send messenger over to inform them that we will be purchasing thier cure for the disease."
, {0,0,-10,-5}, {0,0}, "Confine and insolate the diseased. Start working on developing the cure. We cannot afford to show our weakness to the kingdom and have us owe them a favour." };

//DESTROY TYPE EVENTS
CARDEVENT E_EarthquakeIncoming = { 20, DESTROY_TYPE_EVENT, "Gorvernor, news from nearby kingdom have reach our ears that a earthquake is coming and would striking our kingdom soon. There is an urgent need to make preparation for it when it arrives."
,{0,0,0,0}, {5, -1}, "Sound the emergency alarm. Get all of our citizen to start reinforcing all our building but focus on the Farms and Taverns."
,{0,0,0,0}, {6, -1}, "Sound the emergency alarm. Get all of our citizen to start reinforcing all our building but focus on the Farms and Taverns." };

#pragma endregion


void InitCardList()
{
	//Init Tutorial Card List
	tutCardList[0] = &E_TutBuildAHouse;
	tutCardList[1] = &E_TutBuildAFarm;
	tutCardList[2] = &E_TutBuildAMarket;

	//Init Card List

	cardList[0] = &emptyCard;
	cardList[1] = &E_BuildAHouse;
	cardList[2] = &E_BuildAMarket;
	cardList[3] = &E_ScarceFood;
	cardList[4] = &E_MerchantSetUp;
	cardList[5] = &E_BuidldATavern;
	cardList[6] = &E_FoodMerchantArrival;
	cardList[7] = &E_StolenFood;
	cardList[8] = &E_HeavyStorm;
	cardList[9] = &E_GoldMineDiscovered;
	cardList[10] = &E_ContaminatedFood;
	cardList[11] = &E_VillagersKidnapped;
	cardList[12] = &E_MagnificentHunt;
	cardList[13] = &E_RefugeesHouses;
	cardList[14] = &E_NeedEntertainment;
	cardList[15] = &E_NeedEntertainment;
	cardList[16] = &E_ArsonistAttack;
	cardList[17] = &E_CircusTroupeVisit;
	cardList[18] = &E_CivilRevolt;
	cardList[19] = &E_DiseaseSpread;
	cardList[20] = &E_EarthquakeIncoming;

	rewardCardList[0] = &R_NullCard;
	rewardCardList[1] = &R_HouseCard;
	rewardCardList[2] = &R_FarmCard;
	rewardCardList[3] = &R_MarketCard;
	rewardCardList[4] = &R_TavernCard;

	HouseCardSprite = CP_Image_Load("./ImperoArtAssets/Impero_HouseRewardCard.png");
	FarmCardSprite = CP_Image_Load("./ImperoArtAssets/Impero_HouseRewardCard.png");
	MarketCardSprite = CP_Image_Load("./ImperoArtAssets/Impero_HouseRewardCard.png");
	TavernCardSprite = CP_Image_Load("./ImperoArtAssets/Impero_HouseRewardCard.png");
}

void InitDecks()
{	
	InitCardList();

	for (int index = 0; index < TOTALTUTORIALCARDSCOUNT; index++) {
		tutorialDeck.cardIndexes[index] = index;
		tutorialDeck.cardsInDeck++;
	}


	
	char cardDeckIndexes[] = { 5,9,6,7,16,2,8,10,3,15,4,17,12,20,13,14,18,1,11,19 };


	for (int index = 0; index < sizeof(cardDeckIndexes); index++)
	{	
		cardDeck.cardIndexes[index] = (int)cardDeckIndexes[index];
		//cardDeck.cardIndexes[index] = index; //Setting the index of card in each deck to reference in cardlist, in accordance
		cardDeck.cardsInDeck++;
	}

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

CARDEVENT* GetNextEvent(bool isTutorial)
{
	if (isTutorial == true) {
		currentEvent = tutCardList[currentDeck->cardIndexes[currentCardIndex]];
	}
	else {
		currentEvent = GetEventByIndex(currentDeck->cardIndexes[currentCardIndex]);
	}

	++currentCardIndex;

	return currentEvent;

	//switch (gamePhase) {

	//case PHASE_BUILDPHASE:

	//	/*if (currentCardIndex == currentDeck->cardsInDeck)
	//	{
	//		printf("Reset");
	//		*currentEvent = emptyCard;
	//		currentDeck = &cardDeck;
	//		currentCardIndex = 0;
	//		return currentEvent;
	//	}*/

	//	 //Adding one counter to the Card Index after Drawing it

	//	break;

	//case PHASE_GAMEPHASE:

	//	//if (currentCardIndex == currentDeck->cardsInDeck)
	//	//{
	//	//	printf("exit");
	//	//	*currentEvent = emptyCard;
	//	//	return currentEvent;
	//	//}

	//	 //Adding one counter to the Card Index after Drawing it

	//	break;
	//}
}

void SwapToMainDeck(bool isTutorial) {

	currentCardIndex = 0; //Reset Card Index Counter

	if (isTutorial == false) return; 

	currentDeck = &cardDeck;
	

	/*switch (currentGamePhase) {

	case PHASE_BUILDPHASE:
		currentDeck = &tutorialDeck;
		break;

	case PHASE_GAMEPHASE:
		currentDeck = &cardDeck;
		break;

	case PHASE_ENDPHASE:
		break;

	}*/

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

REWARDCARD* GetRewardByIndex(int index)
{
	return rewardCardList[index];
}

CP_Image* GetCardSpriteByIndex(int index)
{
	switch (index)
	{
	case 1:
		return &HouseCardSprite;
	case 2:
		return &FarmCardSprite;
	case 3:
		return &MarketCardSprite;
	case 4:
		return &TavernCardSprite;
	default:
		return &HouseCardSprite;
	}
}

