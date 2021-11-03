#include <stdio.h>
#include "cprocessing.h"
/*
Extra Notes from Erron:
Basic calculation for all resources per turn has been added but not tested until
we have a running turn-based time mechanic
Most of the default values are simplified estimations and will require multiple
testing to fine-tune
Buff/Debuff effects will be seperated to a different header file
*/

// For now Gold generated from 'Market' tiles and 'merchant' citizens 
// and tax amount is defaulted to the following values
// Subject to adjustments if Levels/Ranks are to be implemented
#define GOLD_AMT_FROM_MARKETS 20
#define TAX_AMOUNT 1

// For now Food generated from 'Farm' tiles and 'farmer' citizens 
// and consumption amount is defaulted to the following values
// Subject to adjustments if Levels/Ranks are to be implemented
#define FOOD_AMT_FROM_FARMS 20
#define FOOD_AMT_FROM_FARMERS 5
#define FOOD_CONSUMPTION_PER_PAX 2

#define PAX_PER_HOUSING 3

// Upkeep costs for every tile are defaulted to the following values
// Subject to adjustments if Levels/Ranks are to be implemented
#define MARKET_UPKEEP_COST 5
#define FARM_UPKEEP_COST 5
#define HOUSING_UPKEEP_COST 5

int curGold;
int curFood;
int curPopulation;
int initPopulation = 100;

//Gold Related Variables
int numMarkets = 0;

//Food Related Variables
int numFarms = 0;

//Population Related Variables
int numHouses = 0;



/*--------------------
GET RESOURCE FUNCTIONS
---------------------*/

int Get_current_gold()
{
	return curGold;
}

int Get_current_food()
{
	return curFood;
}

int Get_current_population()
{
	return curPopulation;
}

/*--------------------
DURING TURN FUNCTIONS
---------------------*/

int market_gold_prediction()
{
	return GOLD_AMT_FROM_MARKETS;
}

int farm_food_prediction()
{
	return FOOD_AMT_FROM_FARMS;
}

int house_pop_prediction()
{
	return PAX_PER_HOUSING;
}

/*--------------------
END OF TURN FUNCTIONS
---------------------*/

// Function to check amount of Gold resource generated per turn
void Gold_generated_per_turn()
{
	// Gold generated from market
	//int gold_generated_by_markets = (numMarkets * GOLD_AMT_FROM_MARKETS);

	// Gold generated from population tax
	//int gold_generated_by_tax = curPopulation * TAX_AMOUNT;

	// Gold deducted from facility upkeep
	//int gold_deducted_from_upkeep = (MARKET_UPKEEP_COST * numMarkets) + (FARM_UPKEEP_COST * numFarms) + (HOUSING_UPKEEP_COST * numHouses);

	// Net total Gold generated
	//curGold += gold_generated_by_markets + gold_generated_by_tax - gold_deducted_from_upkeep;

	curGold += numMarkets;
}

// Function to check amount of Food resource generated per turn
void Food_generated_per_turn()
{
	// Food generated from farms and farmer citizens
	//int food_generated_by_farms = (numFarms * FOOD_AMT_FROM_FARMS);

	// Food deducted from population consumption
	//int food_deducted_from_consumption = curPopulation * FOOD_CONSUMPTION_PER_PAX;

	// Net total Food generated
	//curFood += food_generated_by_farms - food_deducted_from_consumption;

	curFood += numFarms;
}

void Population_per_turn()
{
	//curPopulation = initPopulation + (numHouses * PAX_PER_HOUSING);
	curPopulation = numHouses * 5;
}

/*
float update_overpopulation_debuff_rate(int curPopulation, int max_population) {
	return ((float)curPopulation / (float)max_population);
}

// Function to check if kingdom is Overpopulated
bool check_for_overpopulation(int curPopulation, int max_population)
{
	// Checks curPopulation with max_population and toggles overpopulation debuff if overpopulated
	if (curPopulation > max_population)
	{
		return true;
	}
	else
	{
		return false;
	}
}
*/

//TEMPORARY FOR PROTOTYPE ONLYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY

void InitResources(int startingGold) {
	curGold = startingGold;
}

void AddMarket()
{
	numMarkets++;
}
void AddFarm()
{
	numFarms++;
}
void AddHouse()
{
	numHouses++;
}
void GenerateResourcesOnEndTurn()
{
	Gold_generated_per_turn();
	Food_generated_per_turn();
	Population_per_turn();
}

void AddNewResourceBuilding(int buildingIndex)
{
	switch (buildingIndex)
	{
	case 2:
		AddHouse();
		break;
	case 3:
		AddFarm();
		break;
	case 4:
		AddMarket();
		break;
	}
}


