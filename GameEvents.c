#include <stdio.h>
#include "cprocessing.h"
#include "Common_Headers.h"
#include <stdlib.h>


typedef struct
{
	int cardsInDeck;
	int cardIndexes[30];
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

#define TOTALCARDCOUNT 24
#define TOTALTUTORIALCARDSCOUNT 3
#define TOTALREWARDCARDCOUNT 5
CARDEVENT* cardList[TOTALCARDCOUNT];
CARDEVENT* tutCardList[TOTALTUTORIALCARDSCOUNT];
REWARDCARD* rewardCardList[TOTALREWARDCARDCOUNT];

CARDDECK tutorialDeck;
CARDDECK prototypeDeck = { 23, 1,2,3,8,12,9,10,19,5,11,13,6,18,7,20,15,23,16,17,21,4,14,22, 0 };



CARDDECK* currentDeck;
CARDEVENT* currentEvent;
int currentCardIndex;

#pragma region Reward Cards
REWARDCARD R_NullCard = { 0, NULL_CHOICE, R_NULL_INDEX, 0, "This is a null reward." };
REWARDCARD R_HouseCard	= { 1, BUILD_TYPE_EVENT, B_HOUSE_INDEX , 1, "Click on the grid to construct the house."};
REWARDCARD R_FarmCard	= { 2, BUILD_TYPE_EVENT, B_FARM_INDEX , 1, "Click on the grid to construct the farm." };
REWARDCARD R_MarketCard	= { 3, BUILD_TYPE_EVENT, B_MARKET_INDEX , 1, "Click on the grid to construct the market." };
REWARDCARD R_TavernCard	= { 4, BUILD_TYPE_EVENT, B_TAVERN_INDEX , 1, "Click on the grid to construct the tavern." };

#pragma endregion

#pragma region Card Events
CARDEVENT emptyCard = { 0, NULL_TYPE_EVENT, "This is a null event, for errors only"
, {0,0,0,0}, {0,0}, "Why are you reading this? This is a bug. Ignore and move along."
, {0,0,0,0}, {0,0}, "You are looking at a bug right now. Please ignore." };

//TUTORIALS EVENTS [CARD INDEX STARTS WITH 0X]
CARDEVENT E_TutBuildAHouse = { 1, BUILD_TYPE_EVENT,"Gorvernor, we need to build houses for our citizens. Click on either red or the blue card."
, {-20,0,0,5}, { {1, 2}, {0,0} } ,"Gather our workers! Let us construct two houses!"
, {-10,0,0,-20}, { {1, 2}, {0,0} } ,"Find the cheapest materials you can! They don't need such luxury." };
CARDEVENT E_TutBuildAFarm = { 2, BUILD_TYPE_EVENT, "Gorvernor, we need to feed our citizens. Building a farm can grow us some food."
, {-20,0,0,5}, { {2, 2}, {0,0} }, "Decide on two empty land to place the farms where it is suited for crops to grow."
, {0,0,0,0}, { {2, 2}, {0,0} }, "The kingdom needs self-sufficient food to survive. Gather the workers and start building ASAP!"};
CARDEVENT E_TutBuildAMarket = { 3 , BUILD_TYPE_EVENT, "Gorvernor, our economy is unstable. We need income! Set up two markets."
, {-20,0,0,5}, { {3, 2}, {0,0} }, "Let known to citizens that they can start setting up shops! Time to make some money!!"
, {0,0,0,0}, { {3, 2}, {0,0} }, "We are open for business! Start the build of markets so visiting merchants can set up trade." };

/*----------------------------------------------------------*/

//BASIC EVENTS - BUILD TYPE
CARDEVENT E_BuildAHouse = { 4, BUILD_TYPE_EVENT, "Gorvernor, our population are growing and citizens are requesting for more lodging."
, {-10,0,0,5}, { {1,1}, {0,0} }, "Send in workers, build more houses for our citizens!"
, {0,0,0,-10}, { {0,0}, {0,0} }, "There are enough Houses for everyone, the citizens are over exaggerating." };
CARDEVENT E_BuildAMarket = { 5 , BUILD_TYPE_EVENT, "Gorvernor, our markets are crowding up! Constructing another market will reduce the crowd."
, {7,0,0,0}, { {3,1}, {0,0} }, "Spreading the crowd would help our income ,proceed with the construction."
, {0,0,0,2}, { {0,0}, {0,0} }, "A crowded market means good business, there is not a need to construct another market." };
CARDEVENT E_ScarceFood = { 6, BUILD_TYPE_EVENT, "Gorvernor, there is food shortage in our kingdom. We advice you to allow farms to be built."
, {-20,0,0,5},{ {2,2}, {0,0} },"Get our workers to start with the construction of the farm. We have enough blueprints for 2."
, {10,0,0,-10},{ {0,0},{0,0} }, "The citizens can eat less." };
CARDEVENT E_MerchantSetUp = { 7, BUILD_TYPE_EVENT, "Gorvernor, various merchants have shown interest in settling down in our kingdom. Allow the merchants in by constructing two markets."
, {15,0,0,0},{ {3,2},{0,0} }, "Open our gates and invite the merchants in and set up two market spot for them."
, {0,0,0,0}, { {0,0},{0,0} }, "We dont have enough manpower to construct markets." };
CARDEVENT E_BuidldATavern = {8, BUILD_TYPE_EVENT, "Gorvernor, there is a lack of entertainment in our kingdom, our citizen are getting restless. Construct a tavern to give them entertainment."
, {10,0,0,0}, { {4,1},{0,0} }, "Entertainment would boost our citizens morale, get our builders to construct the taverns."
, {0,0,0,0}, { {0,0},{0,0} }, "We need to focus on our necessities, entertainment can wait." };


//BASIC EVENT - RESOURCE TYPE
CARDEVENT E_FoodMerchantArrival = { 9, RESOURCE_TYPE_EVENT, "Gorvernor, food peddlers have arrived at your kingdom, they wish to trade"
, {-10,20,0,0},{ {0,0},{0,0} }, "Inform them that we will be accepting their trades, send carts over to carry the goods to our granaries."
, {0,0,0,0},{ {0,0},{0,0} }, "We have a substantial amount of food in our granaries, there is not a need to trade." };
CARDEVENT E_StolenFood = { 10 , RESOURCE_TYPE_EVENT,"Gorvernor, there is a report of food stolen from our granaries. We advice an investigation to apprehend the culprit."
, {0,0,0,5}, { {0,0},{0,0} }, "We need to give our citizens a sense of security, proceed with the investigation! I want the culprit aprehended. "
, {0,-20,0,0}, { {0,0},{0,0} }, "This is just a one time thing, let it pass and increase our security around the granaries." };
CARDEVENT E_HeavyStorm = { 11 , RESOURCE_TYPE_EVENT,"Gorvernor, We have been hit by a heavy thunderstorm, many houses are currently damaged and citizens are requesting for repair."
, {-10,0,0,5}, { {0,0},{0,0} }, "Divide our manpower to sort out the damages and to repair the damaged houses."
, {0,0,0,-10}, { {0,0},{0,0} }, "Clean up the damages that the storm have done, the repairs can wait." };
CARDEVENT E_GoldMineDiscovered = { 12 , RESOURCE_TYPE_EVENT, "Gorvernor, our miners have discovered a gold mine. They are currently waiting on instruction to deal with it."
, {25,0,0,0}, { {0,0},{0,0} }, "Arrange carts to bring the gold back to the treasury, we will sell them to increase our capital."
, {0,0,0,10} ,{ {0,0},{0,0} }, "Arrange multiple carts to distribute the gold to all the households. They deserve a reward for their hardwork." };
CARDEVENT E_ContaminatedFood = { 13, RESOURCE_TYPE_EVENT, "Gorvernor, we have discovered that the food in our granaries are contaminated. We need to dispose of them as soon as possible to prevent further contamination."
, {-10,0,0,5}, { {0,0},{0,0} }, "Dispose the contaminated food quickly and start replenishing new food in our granaries"
, {0,-20,0,-2},{ {0,0},{0,0} }, "Get our workers to dispose the cotaminated food as quickly as possible. There is not a need to replenish the contaminated food, however find the source of the contamination." };
CARDEVENT E_VillagersKidnapped = { 14, RESOURCE_TYPE_EVENT, "Gorvernor, we received reports that some of our citizens have been kidnapped by bandits in a skirmish! We have to send out troops to rescue them."
, {-20,0,0,0}, { {0,0},{0,0} }, "Hire the mercenaries that have reached out to us to rescue the kidnapped citizens"
, {0,0,-3, 0}, { {0,0},{0,0} }, "Get our local militia ready and dispatch them to rescue the kidnapped citizen as soon as possible." };
CARDEVENT E_MagnificentHunt = { 15, RESOURCE_TYPE_EVENT, "Gorvernor, our hunters have return back from the wildness with a magnificent hunt. We are waiting for your instruction on what to do with the hunt."
, {0,50,0,0}, { {0,0},{0,0} }, "Gather the food and store the hunt in our granaries for future use."
, {-15,0,0,20}, { {0,0},{0,0} }, "This calls for a celebration. Use some of our gold in the coffers to hold a festival to celebrate the magnificent hunt." };


//ADVANCED EVENTS - BUILD TYPE
CARDEVENT E_RefugeesHouses = { 16 , BUILD_TYPE_EVENT, "Gorvernor, multiple refugees are seeking shelter in your kingdom. What is your verdict?"
, {-10,0,0,0} ,{ {1,2}, {0,0} }, "Let them enter. We can house those refugees and put them to work."
, {0,0,0,0}, { {0,0},{0,0} }, "Reject their entry. They might be spies from another kingdom, its too dangerous to let them enter." };
CARDEVENT E_NeedEntertainment = { 17, BUILD_TYPE_EVENT, "Gorvernor, our population have grown, our citizen desperately needs entertainment to relieve stress. We advice the construction of two taverns."
, {-20,0,0,0},{ {4,2}, {0,0} }, "Our citizen have worked hard and deserve a place to relax. Proceed with the construction of the two taverns and finish it as soon as possible."
, {0,0,0,-5}, { {0,0},{0,0} }, "There are other parts of the kingdom that needs to be focused on. The construction of the taverns can start later." };

//ADVANCED EVENTS - RESOURCE TYPE
CARDEVENT E_FamineStrikes = { 18, RESOURCE_TYPE_EVENT, "Gorvernor, a famine have struck your kingdom, we advice to distribute the backup food storage to every household to prevent any further crisis from happening."
,{0,-30,0,10}, { {0,0},{0,0} }, "Get all our manpower ready, we need to start distributing the food supplies to each household to overcome this famine."
, {0,0,0,-5}, { {0,0},{0,0} }, "We must brace through this famine. Focus on helping those crucial citizens, make sure they have food to survive." };
CARDEVENT E_ArsonistAttack = { 19, RESOURCE_TYPE_EVENT, "Gorvernor, our farms are on fire and it is reported that there is an arsonist on the loose."
, {-20,0,0,10}, { {0,0},{0,0} }, "Send all our men to assist in extinguishing the fire and rebuild the farm to a working condition."
, {0,0,0,-10}, { {1,-1}, {0,0} }, "The farm that is on fire is not within vacinity of any other farms. Make sure all citizen evacuate the area and let fire extinguish by itself." };
CARDEVENT E_CircusTroupeVisit = { 20 , RESOURCE_TYPE_EVENT, "Gorvernor, a wandering circus troupe have arrived at our kingdom and is offering their services to brighten the mood of the kingdom."
, {-50,0,0,30}, { {0,0},{0,0} }, "Our citzens would enjoy the entertainment provided by the troupe. Send a representative to invite them into our kingdom."
, {0,0,0,0}, { {0,0},{0,0} }, "We need to conserve our gold for the development of our kingdom. Send a representative down to kindly decline their services." };
CARDEVENT E_CivilRevolt = { 21, RESOURCE_TYPE_EVENT,"Gorvernor, a revolt have broken out and citizens are demanding for more food and gold while threating to leave the kingdom if they do not receive them."
, {10,-20,0,0}, { {0,0},{0,0} }, "The kingdom would not exist without our citizens. Distribute gold and food to our citizen and keep them pleased."
, {0,0,-10,-3}, { {0,0},{0,0} }, "We must stand our ground and not submit to our citizens demands!" };
CARDEVENT E_DiseaseSpread = { 22, RESOURCE_TYPE_EVENT, "Gorvernor, a disease have spread through our kingdom and we currently have no cure for it. However, there is information that a nearby allied kingdom have developed a cure for it."
, {-50,0,0,5}, { {0,0},{0,0} }, "Quickly! isolate the diseased to prevent further spreading and send messenger over to inform them that we will be purchasing thier cure for the disease."
, {0,0,-10,-5}, { {0,0},{0,0} }, "Confine and isolate the diseased. Start working on developing the cure. We cannot afford to show our weakness to the other kingdom and have us owe them a favour." };

//DESTROY TYPE EVENTS
CARDEVENT E_EarthquakeIncoming = { 23, DESTROY_TYPE_EVENT, "Gorvernor, news from nearby kingdom have reach our ears that a earthquake is coming and would reach our kingdom soon. There is an urgent need to prepare for it when it arrives."
,{0,0,0,0},{ {2, -1}, {4, -1} }, "Sound the emergency alarm. Get all of our citizen to start reinforcing all our building but focus on the House and Markets."
,{0,0,0,0},{ {1, -1}, {3, -1} }, "Sound the emergency alarm. Get all of our citizen to start reinforcing all our building but focus on the Farms and Taverns." };

#pragma endregion


void InitCardList()
{
	//Init Tutorial Card List
	tutCardList[0] = &E_TutBuildAHouse;
	tutCardList[1] = &E_TutBuildAFarm;
	tutCardList[2] = &E_TutBuildAMarket;

	//Init Card List
	cardList[0] = &emptyCard;
	cardList[1] = &E_TutBuildAHouse;
	cardList[2] = &E_TutBuildAFarm;
	cardList[3] = &E_TutBuildAMarket;
	cardList[4] = &E_BuildAHouse;
	cardList[5] = &E_BuildAMarket;
	cardList[6] = &E_ScarceFood;
	cardList[7] = &E_MerchantSetUp;
	cardList[8] = &E_BuidldATavern;
	cardList[9] = &E_FoodMerchantArrival;
	cardList[10] = &E_StolenFood;
	cardList[11] = &E_HeavyStorm;
	cardList[12] = &E_GoldMineDiscovered;
	cardList[13] = &E_ContaminatedFood;
	cardList[14] = &E_VillagersKidnapped;
	cardList[15] = &E_MagnificentHunt;
	cardList[16] = &E_RefugeesHouses;
	cardList[17] = &E_NeedEntertainment;
	cardList[18] = &E_NeedEntertainment;
	cardList[19] = &E_ArsonistAttack;
	cardList[20] = &E_CircusTroupeVisit;
	cardList[21] = &E_CivilRevolt;
	cardList[22] = &E_DiseaseSpread;
	cardList[23] = &E_EarthquakeIncoming;

	rewardCardList[0] = &R_NullCard;
	rewardCardList[1] = &R_HouseCard;
	rewardCardList[2] = &R_FarmCard;
	rewardCardList[3] = &R_MarketCard;
	rewardCardList[4] = &R_TavernCard;

	HouseCardSprite = CP_Image_Load("./ImperoArtAssets/Impero_CardBuild.png");
	FarmCardSprite = CP_Image_Load("./ImperoArtAssets/Impero_CardBuild.png");
	MarketCardSprite = CP_Image_Load("./ImperoArtAssets/Impero_CardBuild.png");
	TavernCardSprite = CP_Image_Load("./ImperoArtAssets/Impero_CardBuild.png");
}

void InitDecks()
{	
	InitCardList();
	currentCardIndex = 0;
	currentDeck = &prototypeDeck;
}

CARDEVENT* GetCurrentEvent()
{
	return currentEvent;
}

int GetCardsLeft()
{
	return currentDeck->cardsInDeck - currentCardIndex;
}

CARDEVENT* GetNextEvent()
{
	currentEvent = GetEventByIndex(currentDeck->cardIndexes[currentCardIndex]);
	++currentCardIndex;

	return currentEvent;
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

