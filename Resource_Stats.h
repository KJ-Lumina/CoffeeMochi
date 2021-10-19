#include <stdio.h>
#include "cprocessing.h"
/*
Extra Notes from Erron:
Basic calculation for all resources per turn has been added but not tested until
we have a running turn-based time mechanic
Most of the default values are simplified estimations and will require multiple
testing to fine-tune
Once tested and okay'd, will further implement more calculation to factor
different debuff values to affect resource generation to add more depth to calculation
*/


// Max Morale is capped at 100 and is reduced by every debuff present
// computed with the Morale_Decrease_Modifier and the unhappiness factor
#define MAX_MORALE 100
#define MORALE_DECREASE_MODIFIER 40

// For now Gold generated from 'Market' tiles and 'merchant' citizens 
// and tax amount is defaulted to the following values
// Subject to adjustments if Levels/Ranks are to be implemented
#define GOLD_AMT_FROM_MARKETS 20
#define GOLD_AMT_FROM_MERCHANTS 5
#define TAX_AMOUNT 3

// For now Food generated from 'Farm' tiles and 'farmer' citizens 
// and consumption amount is defaulted to the following values
// Subject to adjustments if Levels/Ranks are to be implemented
#define FOOD_AMT_FROM_FARMS 20
#define FOOD_AMT_FROM_FARMERS 5
#define FOOD_CONSUMPTION_PER_PAX 2

// Upkeep costs for every tile are defaulted to the following values
// Subject to adjustments if Levels/Ranks are to be implemented
#define MARKET_UPKEEP_COST 5
#define FARM_UPKEEP_COST 5
#define HOUSING_UPKEEP_COST 5

#define TRUE 1
#define FALSE 0

int current_gold;
int current_food;
int current_population;
int max_population = 100;
int current_morale;

int num_of_markets = 0;
int num_of_merchant_citizen = 0;
bool is_bankrupt = FALSE;
float bankrupt_debuff = 0.75f;

int num_of_farms = 0;
int num_of_farmer_citizen = 0;
bool is_starving = FALSE;
float starving_debuff = 0.75f;

int num_of_housing = 0;
bool is_overpopulated = FALSE;
float overpopulation_debuff_rate = 0.0f;
// Note for unhappiness factor, it is the value generated from
// tile placement (e.g. housing tiles too close to farm tiles
// generate unhappiness factor per pax living in the affected
// housing tile)
// Function for unhappiness_factor not implemented yet
int unhappiness_factor = 0;

// Function to check amount of Gold resource generated per turn
void gold_generated_per_turn()
{
	// Gold generated from market and merchant citizens
	int gold_generated_by_markets = (num_of_markets * GOLD_AMT_FROM_MARKETS) + (num_of_merchant_citizen * GOLD_AMT_FROM_MERCHANTS);

	// Gold generated from population tax
	int gold_generated_by_tax = current_population * TAX_AMOUNT;

	// Gold deducted from facility upkeep
	int gold_deducted_from_upkeep = (MARKET_UPKEEP_COST * num_of_markets) + (FARM_UPKEEP_COST * num_of_farms) + (HOUSING_UPKEEP_COST * num_of_housing);

	// Net total Gold generated
	current_gold += gold_generated_by_markets + gold_generated_by_tax - gold_deducted_from_upkeep;

	// Check if net total is below 0 or not and toggles is_bankrupt debuff accordingly
	if (current_gold < 0)
	{
		is_bankrupt = TRUE;
		current_gold = 0;
	}
	else
	{
		is_bankrupt = FALSE;
	}
}

// Function to check amount of Food resource generated per turn
void food_generated_per_turn()
{
	// Food generated from farms and farmer citizens
	int food_generated_by_farms = (num_of_farms * FOOD_AMT_FROM_FARMS) + (num_of_farmer_citizen * FOOD_AMT_FROM_FARMERS);

	// Food deducted from population consumption
	int food_deducted_from_consumption = current_population * FOOD_CONSUMPTION_PER_PAX;

	// Net total Food generated
	current_food += food_generated_by_farms - food_deducted_from_consumption;

	// Check if net total is below 0 or not and toggles is_starving debuff accordingly
	if (current_food < 0)
	{
		is_starving = TRUE;
		current_food = 0;
	}
	else
	{
		is_starving = FALSE;
	}
}

// Function to check if kingdom is Overpopulated
void check_for_overpopulation()
{
	// Checks current_population with max_population and toggles overpopulation debuff if overpopulated
	if (current_population > max_population)
	{
		is_overpopulated = TRUE;
		overpopulation_debuff_rate = ((float)current_population / (float)max_population);
	}
	else
	{
		is_overpopulated = FALSE;
	}
}

// Function to update Morale of population based on current state of the kingdom
void update_morale()
{
	// Check if kingdom has any of the respective debuffs and adjust current_morale accordingly
	// Tentatively both bankrupt and starving debuff will reduce morale by 30 each if TRUE
	// while overpopulation debuff will scale with the magnitude of overpopulation
	// such that it is possible to hit 0 current_morale
	// Changing MORALE_DECREASE_MODIFIER will change the amount of morale penalty from each debuff
	if (is_bankrupt == TRUE && is_starving == TRUE && is_overpopulated == TRUE)
	{
		current_morale = MAX_MORALE - (
			(bankrupt_debuff * MORALE_DECREASE_MODIFIER) +
			(starving_debuff * MORALE_DECREASE_MODIFIER) +
			(overpopulation_debuff_rate * MORALE_DECREASE_MODIFIER) +
			unhappiness_factor);
	}

	else if (is_bankrupt == TRUE && is_starving == TRUE)
	{
		current_morale = MAX_MORALE - (
			(bankrupt_debuff * MORALE_DECREASE_MODIFIER) +
			(starving_debuff * MORALE_DECREASE_MODIFIER) +
			unhappiness_factor);
	}

	else if (is_bankrupt == TRUE && is_overpopulated == TRUE)
	{
		current_morale = MAX_MORALE - (
			(bankrupt_debuff * MORALE_DECREASE_MODIFIER) +
			(overpopulation_debuff_rate * MORALE_DECREASE_MODIFIER) +
			unhappiness_factor);
	}

	else if (is_starving == TRUE && is_overpopulated == TRUE)
	{
		current_morale = MAX_MORALE - (
			(starving_debuff * MORALE_DECREASE_MODIFIER) +
			(overpopulation_debuff_rate * MORALE_DECREASE_MODIFIER) +
			unhappiness_factor);
	}

	else
	{
		//Note that unhappiness_factor calculation is not implemented yet
		current_morale = MAX_MORALE - unhappiness_factor;
	}
}
