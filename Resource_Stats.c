#include <stdio.h>
#include "cprocessing.h"
#include "MainSystem.h"
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
int gold_generated_per_turn(int curGold, int curPopulation, int numMarkets, int numFarms, int numHouses)
{
	// Gold generated from market
	int gold_generated_by_markets = (numMarkets * GOLD_AMT_FROM_MARKETS);

	// Gold generated from population tax
	int gold_generated_by_tax = curPopulation * TAX_AMOUNT;

	// Gold deducted from facility upkeep
	int gold_deducted_from_upkeep = (MARKET_UPKEEP_COST * numMarkets) + (FARM_UPKEEP_COST * numFarms) + (HOUSING_UPKEEP_COST * numHouses);

	// Net total Gold generated
	curGold += gold_generated_by_markets + gold_generated_by_tax - gold_deducted_from_upkeep;

	return curGold;

}

// Function to check amount of Food resource generated per turn
int food_generated_per_turn(int curFood, int curPopulation, int numFarms)
{
	// Food generated from farms and farmer citizens
	int food_generated_by_farms = (numFarms * FOOD_AMT_FROM_FARMS);

	// Food deducted from population consumption
	int food_deducted_from_consumption = curPopulation * FOOD_CONSUMPTION_PER_PAX;

	// Net total Food generated
	curFood += food_generated_by_farms - food_deducted_from_consumption;

	return curFood;
}

int population_per_turn(int numHouses, int initPopulation, int curPopulation)
{
	curPopulation = initPopulation + (numHouses * PAX_PER_HOUSING);
	return curPopulation;
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


