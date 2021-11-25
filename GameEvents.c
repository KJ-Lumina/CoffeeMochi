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

CP_Image buildCardSprite;
CP_Image destroyCardSprite;

#define TOTALCARDCOUNT 26
#define TOTALTUTORIALCARDSCOUNT 3
#define TOTALREWARDCARDCOUNT 11
CARDEVENT* cardList[TOTALCARDCOUNT];
CARDEVENT* tutCardList[TOTALTUTORIALCARDSCOUNT];
REWARDCARD* rewardCardList[TOTALREWARDCARDCOUNT];

CARDDECK tutorialDeck;
CARDDECK prototypeDeck = { 23, 1,2,3,8,12,9,10,19,5,11,13,6,18,7,20,15,23,16,17,21,4,14,22, 0 };
CARDDECK debugDeck = { 6, 1, 2, 24, 23, 23, 23 };



CARDDECK* currentDeck;
CARDEVENT* currentEvent;
int currentCardIndex;

#pragma region Reward Cards
REWARDCARD R_NullCard				= { 0, NULL_CHOICE, R_NULL_INDEX, "This is a null reward.", };
REWARDCARD R_HouseCard				= { 1, BUILD_TYPE_EVENT, B_HOUSE_INDEX , "Click on the grid to construct the house."};
REWARDCARD R_FarmCard				= { 2, BUILD_TYPE_EVENT, B_FARM_INDEX , "Click on the grid to construct the farm." };
REWARDCARD R_MarketCard				= { 3, BUILD_TYPE_EVENT, B_MARKET_INDEX , "Click on the grid to construct the market." };
REWARDCARD R_TavernCard				= { 4, BUILD_TYPE_EVENT, B_TAVERN_INDEX , "Click on the grid to construct the tavern." };
REWARDCARD R_DemolishHouseCard		= { 5, DESTROY_TYPE_EVENT, B_HOUSE_INDEX , "" };
REWARDCARD R_DemolishFarmCard		= { 6, DESTROY_TYPE_EVENT, B_FARM_INDEX , "" };
REWARDCARD R_DemolishMarketCard		= { 7, DESTROY_TYPE_EVENT, B_MARKET_INDEX , "" };
REWARDCARD R_DemolishTavernCard		= { 8, DESTROY_TYPE_EVENT, B_TAVERN_INDEX , "" };
REWARDCARD R_RatCard				= { O_RATEVENT, ONGOING_TYPE_EVENT, B_FARM_INDEX , "" };
REWARDCARD R_RainCard				= { 10, ONGOING_TYPE_EVENT, R_NULL_INDEX , "" };

#pragma endregion

#pragma region Card Events
CARDEVENT emptyCard = { 0, NULL_TYPE_EVENT,"", "This is a null event, for errors only"
, {0,0,0,0}, {0,0}, "Why are you reading this? This is a bug. Ignore and move along."
, {0,0,0,0}, {0,0}, "You are looking at a bug right now. Please ignore." };

//TUTORIALS EVENTS [CARD INDEX STARTS WITH 0X]
CARDEVENT E_TutBuildAHouse = { 1, BUILD_TYPE_EVENT,"Build Houses","Gorvernor, we need to build houses for our citizens. Click on either red or the blue card."
, {-20,0,0,5}, { {1, 2}, {0,0} } ,"Gather our workers! Let us construct two houses!"
, {-10,0,0,-20}, { {1, 2}, {0,0} } ,"Find the cheapest materials you can! They don't need such luxury." };
CARDEVENT E_TutBuildAFarm = { 2, BUILD_TYPE_EVENT,"Build Farms", "Gorvernor, we need to feed our citizens. Building farms can grow us some food."
, {-20,0,0,5}, { {2, 2}, {0,0} }, "The kingdom needs self-sufficient food to survive. Gather the workers and start building as soon as possible!"
, {-20,5,0,-5}, { {2, 2}, {0,0} }, "I should have my own personal farm."};
CARDEVENT E_TutBuildAMarket = { 3 , BUILD_TYPE_EVENT,"Build Markets", "Gorvernor, our economy is unstable. We need income! Set up two markets."
, {-20,0,0,10}, { {3, 2}, {0,0} }, "We are open for business! Start the build of markets so visiting merchants can set up trade."
, {10,0,0,-20}, { {3, 2}, {0,0} }, "Let the citizens set up their own vendors." };

/*----------------------------------------------------------*/

//BASIC EVENTS - BUILD TYPE
CARDEVENT E_BuildAHouse = { 4, BUILD_TYPE_EVENT,"Lodging Request", "Gorvernor, our population are growing and citizens are requesting for more lodging."
, {-10,0,0,5}, { {1,1}, {0,0} }, "Send in workers, build more houses for our citizens!"
, {5,0,0,-15}, { {0,0}, {0,0} }, "There are enough houses for everyone, the citizens are over exaggerating." };
CARDEVENT E_BuildAMarket = { 5 , BUILD_TYPE_EVENT,"Expand Markets", "Gorvernor, our markets are crowding up! Constructing another market will reduce the crowd."
, {5,0,0,-10}, { {3,1}, {0,0} }, "Spreading the crowd would help our income ,proceed with the construction."
, {10,0,0,10}, { {0,0}, {0,0} }, "A crowded market means good business, there is not a need to construct another market." };
CARDEVENT E_ScarceFood = { 6, BUILD_TYPE_EVENT,"Expand Farmland", "Gorvernor, there is food shortage in our kingdom. We advice you to allow farms to be built."
, {-20,0,0,5},{ {2,2}, {0,0} },"Get our workers to start with the construction of the farm. We have enough blueprints for two."
, {10,10,0,-15},{ {0,0},{0,0} }, "The citizens could do with some exercise." };
CARDEVENT E_MerchantSetUp = { 7, BUILD_TYPE_EVENT,"Welcome Neighbours", "Gorvernor, various merchants have shown interest in settling down. Allow the merchants in by constructing two markets."
, {-20,0,0,0},{ {3,2},{0,0} }, "Open our gates and invite the merchants in and set up two market spot for them."
, {10,0,0,-10}, { {0,0},{0,0} }, "We don't have enough manpower to construct markets." };
CARDEVENT E_BuidldATavern = {8, BUILD_TYPE_EVENT,"Entertainment Request", "Gorvernor, there is a lack of entertainment in our kingdom, our citizen are getting restless. Construct a tavern to give them entertainment."
, {-10,0,0,10}, { {4,1},{0,0} }, "Entertainment would boost our citizens morale, get our builders to construct the taverns."
, {5,0,0,-15}, { {0,0},{0,0} }, "We need to focus on our necessities, entertainment can wait." };


//BASIC EVENT - RESOURCE TYPE
CARDEVENT E_FoodMerchantArrival = { 9, RESOURCE_TYPE_EVENT,"Merchants Trade", "Gorvernor, food peddlers have arrived at your kingdom, they wish to trade"
, {-30,15,0,0},{ {0,0},{0,0} }, "Inform them that we will be accepting their trades, send carts over to carry the goods to our granaries."
, {0,0,0,-15},{ {0,0},{0,0} }, "We have a substantial amount of food in our granaries, there is not a need to trade." };
CARDEVENT E_StolenFood = { 10 , RESOURCE_TYPE_EVENT,"Granary Thievery","Gorvernor, there is a report of food stolen from our granaries. We advice an investigation to apprehend the culprit."
, {-15,5,0,-15}, { {0,0},{0,0} }, "We need to give our citizens a sense of security, proceed with the investigation! We want the culprit aprehended. "
, {0,-25,0,5}, { {0,0},{0,0} }, "He must really needed the food, let it pass and increase our security around the granaries." };
CARDEVENT E_HeavyStorm = { 11 , RESOURCE_TYPE_EVENT,"Storm Bypassing","Gorvernor, we have been hit by a heavy thunderstorm, many houses are currently damaged and citizens are requesting for repair."
, {-25,-25,0,15}, { {0,0},{0,0} }, "Divide our manpower to sort out the damages and to repair the damaged houses."
, {10,10,0,-25}, { {0,0},{0,0} }, "Our food and gold must be protected, the repairs can wait." };
CARDEVENT E_GoldMineDiscovered = { 12 , RESOURCE_TYPE_EVENT,"Gold Mine Discovery", "Gorvernor, our miners have discovered a gold mine.They are currently waiting on instruction to deal with it."
, {25,0,0,5}, { {0,0},{0,0} }, "Arrange carts to bring the gold back to the treasury, we will sell them to increase our capital."
, {5,0,0,10} ,{ {0,0},{0,0} }, "Arrange multiple carts to distribute the gold to all the households. They deserve a reward for their hardwork." };
CARDEVENT E_ContaminatedFood = { 13, RESOURCE_TYPE_EVENT,"Granary Contamination", "Gorvernor, we have discovered that the food in our granaries are contaminated. We need to dispose of them to prevent further damage."
, {0,-25,0,10}, { {0,0},{0,0} }, "Dispose the contaminated food quickly."
, {0,15,0,-20},{ {0,0},{0,0} }, "Perhaps the citizens won't notice if I distribute some." };
CARDEVENT E_VillagersKidnapped = { 14, RESOURCE_TYPE_EVENT,"Kidnap and Extortion", "Gorvernor, we received reports that some of our citizens have been kidnapped by bandits in a skirmish! We have to send out troops to rescue them."
, {0,-20,0,10}, { {0,0},{0,0} }, "Hire the mercenaries that have reached out to us to rescue the kidnapped citizens"
, {-30,0,0,10}, { {0,0},{0,0} }, "Bribe them, we can't risk hurting our citizens" };
CARDEVENT E_MagnificentHunt = { 15, RESOURCE_TYPE_EVENT,"Great Food Hunt", "Gorvernor, our hunters have return back from the wildness with a magnificent hunt. We are waiting for your instruction on what to do with the hunt."
, {0,20,0,0}, { {0,0},{0,0} }, "Gather the food and store the hunt in our granaries for future use."
, {-15,0,0,10}, { {0,0},{0,0} }, "This calls for a celebration. Use some of our gold in the coffers to hold a festival to celebrate the magnificent hunt." };


//ADVANCED EVENTS - BUILD TYPE
CARDEVENT E_RefugeesHouses = { 16 , BUILD_TYPE_EVENT,"Host Refugees", "Gorvernor, multiple refugees are seeking shelter in your kingdom. What is your verdict?"
, {0,-20,10,10} ,{ {1,2}, {0,0} }, "Let them enter. We can house those refugees and put them to work."
, {0,10,0,-30}, { {0,0},{0,0} }, "Reject their entry. They might be spies from another kingdom, its too dangerous to let them enter." };
CARDEVENT E_NeedEntertainment = { 17, BUILD_TYPE_EVENT,"More Entertainment", "Gorvernor, our population have grown, our citizen desperately needs entertainment to relieve stress. We advice the construction of two taverns."
, {-20,-5,0,-5},{ {4,2}, {0,0} }, "Our citizen have worked hard and deserve a place to relax. Proceed with the construction of the two taverns and finish it as soon as possible."
, {10,0,0,-10}, { {0,0},{0,0} }, "There are other parts of the kingdom that needs to be focused on. The construction of the taverns can start later." };

//ADVANCED EVENTS - RESOURCE TYPE
CARDEVENT E_FamineStrikes = { 18, RESOURCE_TYPE_EVENT,"Famine Strike", "Gorvernor, a famine have struck your kingdom, we advice to distribute the backup food storage to every household to prevent any further crisis from happening."
,{0,-30,0,10}, { {0,0},{0,0} }, "Get all our manpower ready, we need to start distributing the food supplies to each household to overcome this famine."
, {0,-15,0,-25}, { {0,0},{0,0} }, "Prepare the citizens for rationing." };
CARDEVENT E_ArsonistAttack = { 19, RESOURCE_TYPE_EVENT,"Arsonist Invasion", "Gorvernor, our farms are on fire and it is reported that there is an arsonist on the loose."
, {-20,0,0,0}, { {0,0},{0,0} }, "Send all our men to assist in extinguishing the fire and rebuild the farm to a working condition."
, {0,-15,0,-20}, { {2,-1}, {0,0} }, "The farm that is on fire is not within vacinity of any other farms. Make sure all citizen evacuate the area and let fire extinguish by itself." };
CARDEVENT E_CircusTroupeVisit = { 20 , RESOURCE_TYPE_EVENT,"Travelling Circus", "Gorvernor, a famous circus troupe have arrived at our kingdom and is offering their services to brighten the mood of the kingdom."
, {-30,0,0,30}, { {0,0},{0,0} }, "Our citzens would enjoy the entertainment provided by the troupe. Send a representative to invite them into our kingdom."
, {15,0,0,-20}, { {0,0},{0,0} }, "We need to conserve our gold for the development of our kingdom. Send a representative down to kindly decline their services." };
CARDEVENT E_CivilRevolt = { 21, RESOURCE_TYPE_EVENT,"Citizens Rebellion","Gorvernor, a revolt have broken out and citizens are demanding for more food and gold while threating to leave the kingdom if they do not receive them."
, {-20,-20,0,10}, { {0,0},{0,0} }, "The kingdom would not exist without our citizens. Distribute gold and food to our citizen and keep them pleased."
, {10,10,0,-30}, { {0,0},{0,0} }, "We must stand our ground and not submit to our citizens demands!" };
CARDEVENT E_DiseaseSpread = { 22, RESOURCE_TYPE_EVENT,"Global Epidemic", "Gorvernor, a disease have spread through our kingdom and we currently have no cure for it. However, the neighbouring kingdom has already developed a cure."
, {-50,0,0,10}, { {0,0},{0,0} }, "Quickly! Isolate the diseased to prevent further spreading and send messenger over to inform them that we will be purchasing thier cure for the disease."
, {0,-10,0,-20}, { {0,0},{0,0} }, "Confine and isolate the diseased. Start working on developing the cure. We cannot afford to show our weakness to the other kingdom and have us owe them a favour." };

//ADVANCED EVENTS - ONGOING TYPE
CARDEVENT E_RatInfestation = { 24, ONGOING_TYPE_EVENT,"Rat Infestation", "Gorvernor, there is an outbreak of rats in our agricultural locations!"
, {-10, 0, 0, 0}, { {O_RATEVENT,1},{0,0} }, "Prepare some cages. We will capture and release them peacefully. This will take a while."
, {0,-10,0, -10}, { {O_RATEVENT,1},{0,0} }, "Burn the fields down.", {1, 5, 7, 9, 13, 17, 19, 21, 25, 0} };

CARDEVENT E_HeavyRain = { 25, ONGOING_TYPE_EVENT,"Raining Season", "Looks like a storm is approaching."
, {-30, 0, 0, 0}, { {0,0},{0,0} }, "Upgrade our crop drainage trenches."
, {-30, 0,0, 0}, { {0,0},{0,0} }, "Reinforce our shelters" };

//DESTROY TYPE EVENTS
//CARDEVENT E_EarthquakeIncoming = { 23, DESTROY_TYPE_EVENT,"Disaster Strike", "Gorvernor, news from nearby kingdom have reach our ears that a earthquake is coming and would reach our kingdom soon. There is an urgent need to prepare for it when it arrives."
//,{0,-20,0,-30},{ {2, -1}, {4, -1} }, "Sound the emergency alarm. Get all of our citizen to start reinforcing all our building but focus on the House and Markets."
//,{-20,0,0,-15},{ {1, -1}, {3, -1} }, "Sound the emergency alarm. Get all of our citizen to start reinforcing all our building but focus on the Farms and Taverns." };

CARDEVENT E_EarthquakeIncoming = { 23, DESTROY_TYPE_EVENT,"Disaster Strike", "Gorvernor, news from nearby kingdom have reach our ears that a earthquake is coming and would reach our kingdom soon. There is an urgent need to prepare for it when it arrives."
,{0,-20,0,-30},{ {6, 1}, {8, 1} }, "Sound the emergency alarm. Get all of our citizen to start reinforcing all our building but focus on the House and Markets."
,{-20,0,0,-15},{ {5, 1}, {7, 1} }, "Sound the emergency alarm. Get all of our citizen to start reinforcing all our building but focus on the Farms and Taverns.", {26, 0} };
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
	cardList[24] = &E_RatInfestation;
	cardList[25] = &E_HeavyRain;

	rewardCardList[0] = &R_NullCard;
	rewardCardList[1] = &R_HouseCard;
	rewardCardList[2] = &R_FarmCard;
	rewardCardList[3] = &R_MarketCard;
	rewardCardList[4] = &R_TavernCard;
	rewardCardList[5] = &R_DemolishHouseCard;
	rewardCardList[6] = &R_DemolishFarmCard;
	rewardCardList[7] = &R_DemolishMarketCard;
	rewardCardList[8] = &R_DemolishTavernCard;
	rewardCardList[9] = &R_RatCard;
	rewardCardList[10] = &R_RainCard;

	buildCardSprite = CP_Image_Load("./ImperoArtAssets/Impero_CardBuild.png");
	destroyCardSprite = CP_Image_Load("./ImperoArtAssets/Impero_CardDestroy.png");
}

void InitDecks()
{	
	InitCardList();
	currentCardIndex = 0;
	//currentDeck = &prototypeDeck;
	currentDeck = &debugDeck;
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

CP_Image* GetCardSpriteByType(int type)
{
	switch (type)
	{
	case 1:
		return &buildCardSprite;
	case 3:
		return &destroyCardSprite;
	default:
		return &buildCardSprite;
	}
}

