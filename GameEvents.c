/*!_____________________________________________________________________________
@file       GameEvents.c
@author     Seow Kai Jun (s.kaijun@digipen.edu)
@co-authors Travess Tan (travesscheekhang.t@digipen.edu)
			Kelly Liaw (liaw.k@digipen.edu)
@course     CSD1120
@section    B
@team       CoffeeMochi
@brief      This file contains the Events related variables & function definitions
			that are used in Impero.

			Copyright ©2021 DigiPen, All rights reserved.
*//*__________________________________________________________________________*/

#include <stdio.h>
#include "cprocessing.h"
#include "Common_Headers.h"
#include <stdlib.h>

typedef struct
{
	int cardsInDeck;
	int cardIndexes[30];
}CARDDECK;

CP_Image buildCardSprite;
CP_Image destroyCardSprite;
CP_Image blankCardSprite;

#define TOTALCARDCOUNT 50
#define TOTALTUTORIALCARDSCOUNT 3
#define TOTALREWARDCARDCOUNT 30
CARDEVENT* cardList[TOTALCARDCOUNT];
CARDEVENT* tutCardList[TOTALTUTORIALCARDSCOUNT];
REWARDCARD* rewardCardList[TOTALREWARDCARDCOUNT];

CARDDECK tutorialDeck;
CARDDECK prototypeDeck = { 23, 1,2,3,8,12,9,10,19,5,11,13,6,18,7,20,15,23,16,17,21,4,14,22, 0 };
CARDDECK debugDeck = { 11, 1, 2, 1, 2, 1, 2, 9,9,9,9 };
CARDDECK randomDeck;


CARDDECK* currentDeck;
CARDEVENT* currentEvent;
int currentCardIndex;
CARDEVENT* forcedCards[5];
int forcedCardCount;

#pragma region Reward Cards
REWARDCARD R_NullCard				= { 0, NULL_CHOICE, R_NULL_INDEX, "This is a null reward.", "This is a null reward." };
REWARDCARD R_HouseCard				= { 1, BUILD_TYPE_EVENT, B_HOUSE_INDEX , "Click on the card below to collect your house", "Click on the grid to construct the house."};
REWARDCARD R_FarmCard				= { 2, BUILD_TYPE_EVENT, B_FARM_INDEX , "Click on the card below to collect your farm", "Click on the grid to construct the farm." };
REWARDCARD R_MarketCard				= { 3, BUILD_TYPE_EVENT, B_MARKET_INDEX , "Click on the card below to collect your market", "Click on the grid to construct the market." };
REWARDCARD R_TavernCard				= { 4, BUILD_TYPE_EVENT, B_TAVERN_INDEX , "Click on the card below to collect your tavern", "Click on the grid to construct the tavern." };
REWARDCARD R_DemolishHouseCard		= { 5, DESTROY_TYPE_EVENT, B_HOUSE_INDEX , "", "" };
REWARDCARD R_DemolishFarmCard		= { 6, DESTROY_TYPE_EVENT, B_FARM_INDEX , "", "" };
REWARDCARD R_DemolishMarketCard		= { 7, DESTROY_TYPE_EVENT, B_MARKET_INDEX , "", "" };
REWARDCARD R_DemolishTavernCard		= { 8, DESTROY_TYPE_EVENT, B_TAVERN_INDEX, "", "" };
REWARDCARD R_RatCard				= { O_RATEVENT, ONGOING_TYPE_EVENT, B_FARM_INDEX , "", ""};
REWARDCARD R_RainCard				= { 10, ONGOING_TYPE_EVENT, R_NULL_INDEX , "", "" };
REWARDCARD R_ForcedCard				 = { 29, EVENT_TYPE_REWARD , 40 , "", "" };

#pragma endregion

#pragma region Card Events
CARDEVENT emptyCard = { 0, NULL_TYPE_EVENT,"", "This is a null event, for errors only"
, {0,0,0,0}, {0,0}, "Why are you reading this? This is a bug. Ignore and move along.", "Please wait"
, {0,0,0,0}, {0,0}, "You are looking at a bug right now. Please ignore.", "Wait for it..." };

//TUTORIALS EVENTS
CARDEVENT E_TutBuildAHouse = { 1, BUILD_TYPE_EVENT,"Build Houses","Gorvernor, we need to build houses for our citizens. Click on either red or the blue card."
, {-20,0,0,5}, { {1, 2}, {0,0} } ,"Lay a strong foundation. Who knows what might happen in the future.","Citizens are happy with new shelters."
, {-5,0,0,-10}, { {1, 2}, {0,0} } ,"Lay the foundation with enough materials to stand. We can't afford more." ,"At least we have roofs..."};
CARDEVENT E_TutBuildAFarm = { 2, BUILD_TYPE_EVENT,"Build Farms", "Gorvernor, we need to feed our citizens. Building farms can grow us some food."
, {-20,0,0,5}, { {2, 2}, {0,0} }, "The kingdom needs self-sufficient food to survive. Lets start with 2 farms.", "We wait for prosperous growth"
, {-30,0,0,-5}, { {2, 3}, {0,0} }, "We should have an extra farm for myself.", "These extra food can be useful in the future" };
CARDEVENT E_TutBuildAMarket = { 3 , BUILD_TYPE_EVENT,"Build Markets", "Gorvernor, our economy is unstable. We need income! Markets can generate us some gold."
, {-20,0,0,10}, { {3, 2}, {0,0} }, "Traders can now set up in our markets. Start building the infrastructure for 2.", " We are open for business!"
, {10,0,0,-20}, { {3, 3}, {0,0} }, "We need more than 2 markets. Gold is the foundation of our kingdom", "This is a step towards our expansion!" };

/*----------------------------------------------------------*/
//, ""
//BASIC EVENTS - BUILD TYPE
CARDEVENT E_BuildAHouse = { 4, BUILD_TYPE_EVENT,"Lodging Request", "Gorvernor, our population are growing and citizens are requesting for more lodging."
, {-10,0,0,5}, { {1,1}, {0,0} }, "Send in workers, build more houses for our citizens!", "Our kingdom is expanding!"
, {0,0,0,-10}, { {0,0}, {0,0} }, "There are enough houses for everyone, that room could fit an extra person or two.", "A citizen left the kingdom with unsastisfactory living conditions." };
CARDEVENT E_BuildAMarket = { 5 , BUILD_TYPE_EVENT,"Expand Markets", "Gorvernor, our markets are crowding up!"
, {15,0,2,0}, { {3,1}, {0,0} }, "Having another market can appeal to more buisness.", "Our city grows!"
, {0,0,0,10}, { {0,0}, {0,0} }, "A crowded market means good business, there is not a need to construct another market." , "Our gold seems sustainable for now...right?" };
CARDEVENT E_ScarceFood = { 6, BUILD_TYPE_EVENT,"Expand Farmland", "Gorvernor, we need more food if we want a bigger kingdom. Should we create more farms."
, {-20,0,0,5},{ {2,2}, {0,0} },"Get our workers to start with the construction of the farm. We have enough blueprints for two.", "Starvation will not be a concern in my kingdom."
, {0,10,0,-15},{ {0,0},{0,0} }, "The citizens could do with some exercise." , "We have to adapt to be sustainable." };
CARDEVENT E_MerchantSetUp = { 7, BUILD_TYPE_EVENT,"Welcome Neighbours", "Gorvernor, merchants bringing luxurious goods have arrived. They will need 2 markets to settle down."
, {-20,0,2,0},{ {3,2},{0,0} }, "Welcome them in. Prepare 2 quality markets for them", "This will bring us more income"
, {0,0,0,-10}, { {0,0},{0,0} }, "Luxury products brings no practical use. We don't need them.", "Luxuries can wait." };
CARDEVENT E_BuidldATavern = {8, BUILD_TYPE_EVENT,"Entertainment Request", "There is a lack of entertainment in our kingdom, our citizens are getting restless. Construct a tavern to raise their morale."
, {-10,0,0,10}, { {4,1},{0,0} }, "Entertainment would boost our citizens morale, get our builders to construct the taverns.", "Laughter and merriment fill the previously silent nights."
, {0,0,0,-15}, { {0,0},{0,0} }, "I will not risk the laziness disease. Entertainments are a distraction to our expansion." , "Citizens are definitely not happy with that decision..." };


//BASIC EVENT - RESOURCE TYPE
CARDEVENT E_FoodMerchantArrival = { 9, RESOURCE_TYPE_EVENT,"Food peddlers", "A group of sleazy-looking food peddlers have arrived at your kingdom, they wish to trade."
, {-30,15,0,0},{ {0,0},{0,0} }, "We shall accept their trades, send them sufficient gold.", "We have more to eat!"
, {0,20,0,-15},{ {0,0},{0,0} }, "Acquire those food one way or another. They will not be touching my gold!", "They left and never came back." };
CARDEVENT E_StolenFood = { 10 , RESOURCE_TYPE_EVENT,"Granary Thievery","Gorvernor, there is a report of a child stealing food from our granaries."
, {0,-25,0,10}, { {0,0},{0,0} }, "Let it pass. The child might really need it.", "The citizens applaud your magnanimity."
, {-15,5,0,-15}, { {0,0},{0,0} }, "We want the culprit apprehended. Let this be known it is a crime in my kingdom", "Fear strikes the children of the kingdom." };
CARDEVENT E_HeavyStorm = { 11 , RESOURCE_TYPE_EVENT,"Storm Bypassing","Gorvernor, we have been hit by a heavy thunderstorm, many houses are currently damaged and citizens are requesting for repair."
, {-25,-25,0,15}, { {0,0},{0,0} }, "Divide our manpower to sort out the damages and to repair the damaged houses.", "At least the citizens are safe."
, {0,0,0,-25}, { {0,0},{0,0} }, "Our food and gold must be protected, the repairs can wait.", "At least our essentials are safe." };
CARDEVENT E_GoldMineDiscovered = { 12 , RESOURCE_TYPE_EVENT,"Gold Mine Discovery", "Gorvernor, We have discovered a gold mine"
, {0,0,0,10} ,{ {0,0},{0,0} }, "Distribute gold to everyone", "Citizens are happy to receive gold." 
, {15,0,0,-10}, { {0,0},{0,0} }, "The gold belongs to me.", "We...I am rich." };
CARDEVENT E_ContaminatedFood = { 13, RESOURCE_TYPE_EVENT,"Granary Contamination", "Gorvernor, we've discovered mold in one of our granaries. We need to dispose of them to prevent further damage."
, {0,-25,0,10}, { {0,0},{0,0} }, "Dispose the contaminated grain.", "Our people's health is safe."
, {0,15,0,-20},{ {29,40},{0,0} }, "A little bit of mold won't hurt.", "We have more food to serve."};

CARDEVENT E_VillagersKidnapped = { 14, RESOURCE_TYPE_EVENT,"Kidnap and Extortion", "Gorvernor, some of our citizens have been kidnapped by bandits!"
, {-35,0,0,30}, { {0,0},{0,0} }, "Give in to their demands, we can't risk hurting our citizens", "Citizens grew faith in their leader."
, {0,0,2,-20}, { {0,0},{0,0} }, "Kidnap more citizens from other kingdoms", "Our population...grows...?" };

CARDEVENT E_MagnificentHunt = { 15, RESOURCE_TYPE_EVENT,"Great Food Hunt", "Gorvernor, our hunters have return with a magnificent hunt."
, {-10,0,0,20}, { {0,0},{0,0} }, "Hold a festival for our kingdom.", "Drinks are toasted in your honour."
, {-10,0,4,5}, { {0,0},{0,0} }, "Hold a festival, invite neighbouring kingdoms to join us!", "People enjoyed themselves here and have decided to stay." };


//ADVANCED EVENTS - BUILD TYPE
CARDEVENT E_RefugeesHouses = { 16 , BUILD_TYPE_EVENT,"Refugees", "Gorvernor, refugees are seeking shelter in your kingdom."
, {0,-20,4,10} ,{ {0,0}, {0,0} }, "Allow their entry. We can always use more hands.", "You have not heard a single refugee demanding an apology."
, {0,10,0,-30}, { {0,0},{0,0} }, "Reject their entry. They might be spies; it's too dangerous to let them enter." , "Whispers of your cruelty spreads across the land." };
CARDEVENT E_NeedEntertainment = { 17, BUILD_TYPE_EVENT,"Entertainment", "Gorvernor, our citizens have put in a request for a tavern for entertainment."
, {-20,-5,0,-5},{ {4,2}, {0,0} }, "Build two taverns, it is what our citizens derserve.", "The citizens no longer party rock in the house."
, {10,0,0,-10}, { {0,0},{0,0} }, "Delay the construction, we need to shore up our coffers first.", "The citizens party rock in front of your house to protest." };

//ADVANCED EVENTS - RESOURCE TYPE
CARDEVENT E_FamineStrikes = { 18, RESOURCE_TYPE_EVENT,"Famine", "Gorvernor, a famine have struck your kingdom, we advice to distribute the backup food storage to every household to prevent any further crisis from happening."
,{0,-30,0,10}, { {0,0},{0,0} }, "Distribute food supplies to each household to overcome this famine.", "The citizens gains faith in their leader."
, {0,-15,0,-25}, { {0,0},{0,0} }, "Prepare the citizens for rationing." , "The citizens questions your priorities." };
CARDEVENT E_ArsonistAttack = { 19, RESOURCE_TYPE_EVENT,"Arsonist Invasion", "Gorvernor, our farms are on fire and it is reported that there is an arsonist on the loose."
, {-20,-20,2,20}, { {0,0},{0,0} }, "Get our citizens to safety, deal with the arsonist accordingly", "People found security in your kingdom. Population grows."
, {0,0,0,-20}, { {0,0}, {0,0} }, "Protect our farms.", "Several citizens have suffered minor burns." };
CARDEVENT E_CircusTroupeVisit = { 20 , RESOURCE_TYPE_EVENT,"Travelling Circus", "Gorvernor, a famous circus troupe have arrived at our kingdom and is offering their services to brighten the mood of the kingdom."
, {-30,0,2,30}, { {0,0},{0,0} }, "Our citzens would enjoy the entertainment provided by the troupe. Invite them into our kingdom.", "The troupe decided to stay."
, {0,0,0,-20}, { {0,0},{0,0} }, "We need to conserve our gold for the development of our kingdom. Kindly decline their services.", "Our people returned to work in gloom." };
CARDEVENT E_CivilRevolt = { 21, RESOURCE_TYPE_EVENT,"Citizens Rebellion","Gorvernor, a revolt have broken out and citizens are demanding for more food and gold while threating to leave the kingdom if they do not receive them."
, {-20,-20,0,10}, { {0,0},{0,0} }, "The kingdom would not exist without our citizens. Distribute gold and food to our citizen and keep them pleased.", ""
, {10,10,0,-30}, { {0,0},{0,0} }, "We must stand our ground and not submit to our citizens demands!", "" };
CARDEVENT E_DiseaseSpread = { 22, RESOURCE_TYPE_EVENT,"The Purple Plague", "Gorvernor, a disease have spread through our kingdom and we currently have no cure for it. However, the neighbouring kingdom has already developed a cure."
, {-50,0,0,10}, { {0,0},{0,0} }, "Quickly! Isolate the diseased to prevent further spreading and send messenger over to inform them that we will be purchasing their cure for the disease.", ""
, {0,-30,0,-20}, { {0,0},{0,0} }, "Confine and isolate the diseased. Start working on developing the cure. We cannot afford to show our weakness to the other kingdom and have us owe them a favour.", "" };
CARDEVENT E_TheGreatOstrichWar = { 25 , BUILD_TYPE_EVENT, "The Great Ostrich War", "There has been increased sightings of flocks of large birds."
, {0,0,0,10}, { {0,0}, {0,0} }, "Leave them be, they pose no threat.", "Those birds are really fast..."
, {-25,0,0,-20}, { {0,0}, {0,0} }, "Prepare weapons, hunt them for food", "The birds were too fast! Our spears couldn't catch any! Why does this sound familiar?" };


//ADVANCED EVENTS - ONGOING TYPE
CARDEVENT E_RatInfestation = { 24, ONGOING_TYPE_EVENT,"Rat Infestation", "Gorvernor, there is an outbreak of rats in our agricultural locations!"
, {-20, 0, 0, 0}, { {O_RATEVENT,1},{1,0} }, "Hire an expert to deal with it.", "Our harvest have suffered a little."
, {0,0,0, -10}, { {O_RATEVENT,1},{3,0} }, "Have my men deal with it, 'tis but some rats!", "Our men reported to require 2 more turns.", {26} };

CARDEVENT E_HeavyRain = { 25, ONGOING_TYPE_EVENT,"Raining Season", "Looks like a storm is approaching."
, {-30, 0, 0, 0}, { {0,0},{0,0} }, "Upgrade our crop drainage trenches.", ""
, {-30, 0,0, 0}, { {0,0},{0,0} }, "Reinforce our shelters", "" };

//DESTROY TYPE EVENTS

CARDEVENT E_Raid_Of_Shadows = { 23, DESTROY_TYPE_EVENT,"Incoming Raid", "Gorvernor, the neightbouring kingdoms have been victims of the legendary shadow bandits. They may strike us soon."
,{0,-20,0,-15},{ {6, 1}, {8, 1} }, "Reinforce our House and Markets! We need to protect our people!", "The raiders made off with a week's food supply."
,{-30,0,0,-30},{ {5, 1}, {7, 1} }, "Reinforce our Farms and Taverns! We cannot let them seize our means of production!", "The raid of shadow legends left the treasury with whatever they could carry.", {26, 0} };

//GOLD CARDS
CARDEVENT E_GoldenOne = { 45 , RESOURCE_TYPE_EVENT, "Mysterious Man", "A mysterious man arrived in your kingdom. He seems to know the ins-and-outs of your kingdom."
, {30,0,0,0}, { {0,0}, {0,0} }, "Welcome him in peacefully.", "The town is blessed by a shower of gold not long after his arrival."
, {0,0,4,30}, { {0,0}, {0,0} }, "Turn him away, this man is suspicious.", "The population surges after 9 months. You suspect he may have found a way in..." };

CARDEVENT E_GoldenTwo = { 46 , RESOURCE_TYPE_EVENT, "Unknown Expert", "Perfectly crafted blueprints were given to you by an unknown recipient."
, {-10,0,0,0}, { {1,1}, {3,1} }, "Follow the instructions.", "The materials were effectively used."
, {-10,0,0,0}, { {2,1}, {4,1} }, "Follow the instructions.", "The blueprints were very efficient with materials usage."};

// FOLLOW UP EVENTS
CARDEVENT E_FoodPoisoning = { 40 , RESOURCE_TYPE_EVENT, "Food Poisoning", "There is an outbreak of food poisoning. What could have been the cause?"
, {-25,-10,0,15}, { {0,0}, {0,0} }, "Deploy men to assist the doctors, we need tend to the sick.", "The outbreak ends as quickly as it arrived."
, {0,0,-2,-20}, { {0,0}, {0,0} }, "Do nothing. It is merely food poisioning. Their bodies can handle it.", "The citizens look exhausted." };


/*CARDEVENT E_GoldenOne = {00 , BUILD_TYPE_EVENT, "", ""
, {0,0,0,0}, { {0,0}, {0,0} }, ""
, {0,0,0,0}, { {0,0}, {0,0} }, "" }*/

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
	cardList[18] = &E_FamineStrikes;
	cardList[19] = &E_ArsonistAttack;
	cardList[20] = &E_CircusTroupeVisit;
	cardList[21] = &E_CivilRevolt;
	cardList[22] = &E_DiseaseSpread;
	cardList[23] = &E_Raid_Of_Shadows;
	cardList[24] = &E_RatInfestation;

	cardList[25] = &E_TheGreatOstrichWar;

	cardList[40] = &E_FoodPoisoning;
	cardList[45] = &E_GoldenOne;
	cardList[46] = &E_GoldenTwo;

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


	rewardCardList[29] = &R_ForcedCard;

	buildCardSprite = CP_Image_Load("./ImperoArtAssets/Impero_CardBuild.png");
	destroyCardSprite = CP_Image_Load("./ImperoArtAssets/Impero_CardDestroy.png");
	blankCardSprite = CP_Image_Load("./ImperoArtAssets/Impero_CardBlank.png");
}

void RandomizeDeck()
{
	for (int i = 0; i < 30; ++i)
	{
		randomDeck.cardIndexes[i] = 0;
	}
	randomDeck.cardIndexes[0] = 1;
	randomDeck.cardIndexes[1] = 2;
	randomDeck.cardIndexes[2] = 3;
	randomDeck.cardIndexes[3] = 8;
	
	for (int i = 4; i < 8; ++i)
	{
		randomDeck.cardIndexes[i] = CP_Random_RangeInt(9, 15);
	}
	randomDeck.cardIndexes[8] = CP_Random_RangeInt(4, 8);
	for (int i = 9; i < 13; ++i)
	{
		randomDeck.cardIndexes[i] = CP_Random_RangeInt(13, 20);
	}
	randomDeck.cardIndexes[13] = CP_Random_RangeInt(4, 8);
	for (int i = 14; i < 17; ++i)
	{
		randomDeck.cardIndexes[i] = CP_Random_RangeInt(14, 20);
	}
	randomDeck.cardIndexes[17] = CP_Random_RangeInt(4, 8);
	for (int i = 18; i < 26; ++i)
	{
		randomDeck.cardIndexes[i] = CP_Random_RangeInt(18, 24);
	}
	randomDeck.cardsInDeck = 25;
}

void InitDecks()
{	
	InitCardList();
	currentCardIndex = 0;
	//currentDeck = &prototypeDeck;
	//currentDeck = &debugDeck;
	RandomizeDeck();
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
	if (forcedCardCount > 0)
	{
		currentEvent = forcedCards[forcedCardCount];
		--forcedCardCount;
		return forcedCards[forcedCardCount + 1];
	}
	else
	{
		currentEvent = GetEventByIndex(currentDeck->cardIndexes[currentCardIndex]);
		++currentCardIndex;
		return currentEvent;
	}
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
	case 6:
		return &blankCardSprite;
	default:
		return &buildCardSprite;
	}
}

void EventAddForcedEvent(int index)
{
	++forcedCardCount;
	forcedCards[forcedCardCount] = GetEventByIndex(index);
}

void EventSetGoldenCard()
{
	int randGold = 0;
	randGold = CP_Random_RangeInt(45, 46);
	EventAddForcedEvent(randGold);
}

void RollForcedCard(int cardIndex, int chance)
{
	int roll = CP_Random_RangeInt(0, 100);
	if (roll <= chance)
	{
		EventAddForcedEvent(cardIndex);
	}
}

