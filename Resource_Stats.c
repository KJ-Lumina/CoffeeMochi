/*!_____________________________________________________________________________
@file       Resource_Stats.c
@copyright  Copyright: 2021, Digipen Institute of Technology, Singapore
@author     Erron Quek (e.quek@digipen.edu)
@co-authors Seow Kai Jun (s.kaijun@digipen.edu)
            Travess Tan (travesscheekhang.t@digipen.edu)
@course     CSD1120
@section    B
@team       CoffeeMochi
@brief      This file contains the function definitions that are used in
            resource calculation, generation and distribution in the core game
            loop of Impero.

            Copyright ©2021 DigiPen, All rights reserved.
*//*__________________________________________________________________________*/

#include <stdio.h>
#include "cprocessing.h"
#include <stdbool.h>
#include "Common_Headers.h"

#define GOLD_AMT_FROM_MARKETS 2
#define TAX_AMOUNT 1

#define FOOD_AMT_FROM_FARMS 7
#define FOOD_CONSUMPTION_PER_PAX 5

#define PAX_PER_HOUSING 1
#define INCREMENT_PERCENTAGE_PER_PAX 1
#define PAX_LOW_MODIFIER 0.1
#define PAX_MEDIUM_MODIFIER 0.25

#define TAVERN_UPKEEP_COST 5
#define MORALE_AMT_FROM_TAVERN 2

#define HIGH_MORALE 150
#define MEDIUM_MORALE 100
#define LOW_MORALE 50

#define MAX_DELAYRES 50

int curGold = 100;
int curFood = 40;
int curPopulation = 0;
int initPopulation = 100;
int curMorale = 50;
int additionalMorale = 0;
int curBlessing = 0;

//Gold Related Variables
int numMarkets = 0;
int isPoor = 0;

//Food Related Variables
int numFarms = 0;
int isStarved = 0;

//Population Related Variables
int numHouses = 0;

//Morale Related Variables
int numTaverns = 0;
int currentMoraleStatus = HIGH_MORALE;
int tavernModifier = 4;

/*--------------------
SET RESOURCE FUNCTIONS
---------------------*/
/*!_____________________________________________________________________________
@brief      This set of functions are used to set the values of the individual
            resources Gold, Food, Population and Morale to their respective
            global variables. All functions take an input int and return no
            output.
*//*__________________________________________________________________________*/


void Set_current_gold(int gold)
{
	curGold = gold;
}

void Set_current_food(int food)
{
	curFood = food;
}

void Set_current_population(int population)
{
	curPopulation = population;
}

void Set_additional_morale(int addMorale) 
{
	additionalMorale = addMorale;
}

/*--------------------
GET RESOURCE FUNCTIONS
---------------------*/
/*!_____________________________________________________________________________
@brief      This set of functions are used retrieve the values of the resources
            from their respective global variables. All functions take in 
            no input, and return an int corresponding to the global value.
*//*__________________________________________________________________________*/
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

int Get_current_morale()
{
	return curMorale;
}

int Get_additional_morale() 
{
	return additionalMorale;
}

int Get_current_blessing()
{
    return curBlessing;
}

/*--------------------
DURING TURN FUNCTIONS
---------------------*/
/*!_____________________________________________________________________________
@brief      This set of functions are used retrieve the expected resource gain
            from the current total number of building tiles the player has set.
            All functions take in no input and return an int corresponding to 
            the global variable of the expected resource gain.
*//*__________________________________________________________________________*/

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

int tavern_morale_preduction()
{
    return MORALE_AMT_FROM_TAVERN;
}

/*--------------------
END OF TURN FUNCTIONS
---------------------*/
/*!_____________________________________________________________________________
@brief      This set of functions are used to calculate the amount of resource
            gain or loss at the end of a turn, after the player has chosen a 
            decision and finished setting any building tiles, if applicable.
*//*__________________________________________________________________________*/


int delayedGold = 0;
int delayedFood = 0;
int delayedPop = 0;
int delayedMorale = 0;
int delayedBlessing = 0;
float lerpGold = 0;
float lerpFood = 0;
float lerpPop = 0;
float lerpMorale = 0;
float lerpBlessing = 0;

typedef struct
{
    int resourceType;
    int resourceAmt;
    float timer;
}DELAYEDRESOURCE;

DELAYEDRESOURCE delayedList[MAX_DELAYRES];

void CheckMoraleStatus()
{
    if (curMorale >= HIGH_MORALE)
    {
        currentMoraleStatus = HIGH_MORALE;
    }
    else if (curMorale >= MEDIUM_MORALE)
    {
        currentMoraleStatus = MEDIUM_MORALE;
    }
    else
    {
        currentMoraleStatus = LOW_MORALE;
    }
}

/*!_____________________________________________________________________________
@brief      This function calculates the amount of Population at the end of a
            turn. It takes no input and gives no output.
*//*__________________________________________________________________________*/
void GenerateResourcesOnEndTurn()
{
    CheckMoraleStatus();
    switch (currentMoraleStatus)
    {
    case HIGH_MORALE:
        curPopulation += 2;
        SpawnNpc(CP_Vector_Set(810, 430), 2);
        break;
    case MEDIUM_MORALE:
        curPopulation += 1;
        SpawnNpc(CP_Vector_Set(810, 430), 1);
        break;

    case LOW_MORALE:
        break;
    }
}

/*!_____________________________________________________________________________
@brief      This function initialises the resources for a new round of game.
            It takes in 4 seperate int values and returns no output.
*//*__________________________________________________________________________*/
void InitResources(int startingGold, int startingFood, int startingPopulation, int startingMorale) {
	curGold = startingGold;
    curFood = startingFood;
    curPopulation = startingPopulation;
    curMorale = startingMorale;
    curBlessing = 0;
    additionalMorale = 0;

    numHouses = 0;
    numFarms = 0;
    numTaverns = 0;
    numMarkets = 0;

    for (int i = 0; i < MAX_DELAYRES; ++i)
    {
        delayedList[i].timer = 0;
    }
}

/*!_____________________________________________________________________________
@brief      This function updates the value of the resources shown on the UI 
            with lerp() function whenever there is a change in resource value.
            It takes no input and returns no output.
*//*__________________________________________________________________________*/
void UpdateResources()
{
    float deltaRes = CP_System_GetDt();
    if (lerpGold < 1)
    {
        lerpGold += deltaRes;
    }
    if (lerpFood < 1)
    {
        lerpFood += deltaRes;
    }
    if (lerpPop < 1)
    {
        lerpPop += deltaRes;
    }
    if (lerpMorale < 1)
    {
        lerpMorale += deltaRes;
    }
    if (lerpBlessing > 1)
    {
        lerpBlessing += deltaRes;
    }

    float resourceDelta = CP_System_GetDt();
    for (int i = 0; i < 20; ++i)
    {
        if (delayedList[i].timer > 0)
        {
            delayedList[i].timer -= resourceDelta;
            if (delayedList[i].timer < 0)
            {
                switch (delayedList[i].resourceType)
                {
                case 1:
                    IncreaseGold(delayedList[i].resourceAmt);
                    break;
                case 2:
                    IncreaseFood(delayedList[i].resourceAmt);
                    break;
                case 3:
                    IncreasePop(delayedList[i].resourceAmt);
                    break;
                case 4:
                    IncreaseMorale(delayedList[i].resourceAmt);
                    break;
                case 5:
                    IncreaseBlessing(delayedList[i].resourceAmt);
                    break;
                }
            }
        }
    }
}

/*!_____________________________________________________________________________
@brief      This function stores the 'delayed' resource value for calculation
            in the lerp() function in UpdateResource() function. It takes 2 int
            values corresponding to the resource type and the amount, and
            a float for the delay timer, and returns no output;
*//*__________________________________________________________________________*/
void AddDelayedResource(int type, int amt, float delay)
{
    for (int i = 0; i < MAX_DELAYRES; ++i)
    {
        if (delayedList[i].timer <= 0)
        {
            delayedList[i].resourceType = type;
            delayedList[i].resourceAmt = amt;
            delayedList[i].timer = delay;
            return;
        }
    }
}

/*!_____________________________________________________________________________
@brief      The following functions return an int value that calculates the lerp
            transition value corresponding to the resource type and resource 
            amount and timer delay value. They all take no input, and return an
            int output.
*//*__________________________________________________________________________*/
int GetDelayedGold()
{
    return CP_Math_LerpInt(delayedGold, curGold, lerpGold);
}
int GetDelayedFood()
{
    return CP_Math_LerpInt(delayedFood, curFood, lerpFood);
}
int GetDelayedPop()
{
    return CP_Math_LerpInt(delayedPop, curPopulation, lerpPop);
}
int GetDelayedMorale()
{
    return CP_Math_LerpInt(delayedMorale, curMorale, lerpMorale);
}
int GetDelayedBlessing()
{
    return CP_Math_LerpInt(delayedBlessing, curBlessing, lerpBlessing);
}

/*!_____________________________________________________________________________
@brief      This function resets the 'Blessing' value to 0. It takes no input
            and returns no output.
*//*__________________________________________________________________________*/
void ResetBlessing()
{
    curBlessing = 0;
}

/*--------------------
BUILDING FUNCTIONS
---------------------*/
/*!_____________________________________________________________________________
@brief      The following functions increments the current number of buildings
            based on its type that the player currently has in their playing
            field. These functions take no input and returns no output.
*//*__________________________________________________________________________*/
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
    curPopulation += 3;
    SpawnNpc(CP_Vector_Set(810, 470), 1);
    SpawnNpc(CP_Vector_Set(810, 450), 1);
    SpawnNpc(CP_Vector_Set(810, 430), 1);
}
void AddTavern()
{
	numTaverns++;
}

/*!_____________________________________________________________________________
@brief      The following functions decrements the current number of buildings
            based on its type that the player currently has in their playing
            field, but only if the player has at least 1 of the valid building
            to decrement. These functions take no input and returns no output.
*//*__________________________________________________________________________*/
void SubtractMarket()
{
	if (numMarkets > 0) {
		numMarkets--;
	}
}
void SubtractFarm()
{
	if (numFarms > 0) {
		numFarms--;
	}
}
void SubtractHouse()
{
	if (numHouses > 0) {
		numHouses--;
	}
}
void SubtractTavern()
{
	if (numTaverns > 0) {
		numTaverns--;
	}
}

/*!_____________________________________________________________________________
@brief      This function checks for which building type has been added onto the
            field and calls its respective 'add' function. It takes an int value
            corresponding to the building's type index and returns no output.
*//*__________________________________________________________________________*/
void AddNewResourceBuilding(int buildingIndex)
{
    switch (buildingIndex)
    {
    case B_HOUSE_INDEX:
        AddHouse();
        break;
    case B_FARM_INDEX:
        AddFarm();
        break;
    case B_MARKET_INDEX:
        AddMarket();
        break;
    case B_TAVERN_INDEX:
        AddTavern();
        break;
    }
}

/*---------------------------
RESOURCE ANIMATION FUNCTIONS
---------------------------*/
/*!_____________________________________________________________________________
@brief      The following functions create an animation sequence that generates
            each respective resource's VFX and moves them via lerp() to the 
            resource UI. Each function takes in an int for amount of resource
            change, 3 CP_Vector values for the movement, a float value for the
            lifetime of the VFX and a float value for its spawn delay.
*//*__________________________________________________________________________*/

void SpawnGoldGainAnimation(int amount, CP_Vector startPos, CP_Vector checkpoint, CP_Vector endPos, float lifeTime, float spawnDelay)
{
    if (amount > 0)
    {
        SpawnVfxEaseInToEaseOut(1, startPos, checkpoint, endPos, lifeTime, CP_Vector_Set(128, 128), spawnDelay);
        AddDelayedResource(1, amount, lifeTime * 2 + spawnDelay);
    }
    else if (amount < 0)
    {
        SpawnVfxEaseInToEaseOut(2, startPos, checkpoint, endPos, lifeTime, CP_Vector_Set(128, 128), spawnDelay);
        AddDelayedResource(1, amount, lifeTime * 2 + spawnDelay);
    }
}

void SpawnFoodGainAnimation(int amount, CP_Vector startPos, CP_Vector checkpoint, CP_Vector endPos, float lifeTime, float spawnDelay)
{
    if (amount > 0)
    {
        SpawnVfxEaseInToEaseOut(3, startPos, checkpoint, endPos, lifeTime, CP_Vector_Set(128, 128), spawnDelay);
        AddDelayedResource(2, amount, lifeTime * 2 + spawnDelay);
    }
    else if (amount < 0)
    {
        SpawnVfxEaseInToEaseOut(4, startPos, checkpoint, endPos, lifeTime, CP_Vector_Set(128, 128), spawnDelay);
        AddDelayedResource(2, amount, lifeTime * 2 + spawnDelay);
    }
}

void SpawnMoraleGainAnimation(int amount, CP_Vector startPos, CP_Vector checkpoint, CP_Vector endPos, float lifeTime, float spawnDelay)
{
    if (amount > 0)
    {
        SpawnVfxEaseInToEaseOut(5, startPos, checkpoint, endPos, lifeTime, CP_Vector_Set(128, 128), spawnDelay);
        AddDelayedResource(4, amount, lifeTime * 2 + spawnDelay);
    }
    else if (amount < 0)
    {
        SpawnVfxEaseInToEaseOut(6, startPos, checkpoint, endPos, lifeTime, CP_Vector_Set(128, 128), spawnDelay);
        AddDelayedResource(4, amount, lifeTime * 2 + spawnDelay);
    }
}

void SpawnBlessingGainAnimation(int amount, CP_Vector startPos, CP_Vector checkpoint, CP_Vector endPos, float lifeTime, float spawnDelay)
{
    if (amount > 0)
    {
        SpawnVfxEaseInToEaseOut(7, startPos, checkpoint, endPos, lifeTime, CP_Vector_Set(128, 128), spawnDelay);
        AddDelayedResource(5, amount, lifeTime * 2 + spawnDelay);
    }
}

/*!_____________________________________________________________________________
@brief      This function calls for the respective resource spawn animation when
            the player receives resources via an event trigger. It takes in an
            int value and returns no output.
*//*__________________________________________________________________________*/

void ApplyEventResourceAnim(int resourceChange[4])
{
    SpawnGoldGainAnimation(resourceChange[0], CP_Vector_Set(200, 450), CP_Vector_Set(CP_Random_RangeFloat(150,250), CP_Random_RangeFloat(400, 500)), CP_Vector_Set(520, 90), 0.6f, 0);
    SpawnFoodGainAnimation(resourceChange[1], CP_Vector_Set(200, 450), CP_Vector_Set(CP_Random_RangeFloat(150, 250), CP_Random_RangeFloat(400, 500)), CP_Vector_Set(520, 180), 0.6f, 0);
    SpawnMoraleGainAnimation(resourceChange[3], CP_Vector_Set(200, 450), CP_Vector_Set(CP_Random_RangeFloat(150, 250), CP_Random_RangeFloat(400, 500)), CP_Vector_Set(520, 360), 0.6f, 0);
    SpawnNpc(CP_Vector_Set(810, 430), (resourceChange[2]));
    curPopulation += resourceChange[2];
}


/*!_____________________________________________________________________________
@brief      The following function changes the UI value of the respective 
            resources with a counter animation via lerp when there is a change
            in resource value. It takes in an int corresponding to the changed
            value and returns no output.
*//*__________________________________________________________________________*/

void IncreaseGold(int amount)
{
    if (amount != 0)
    {
        delayedGold = CP_Math_LerpInt(delayedGold, curGold, lerpGold);
        lerpGold = 0;
        curGold += amount;
    }
}

void IncreaseFood(int amount)
{
    if (amount != 0)
    {
        delayedFood = CP_Math_LerpInt(delayedFood, curFood, lerpFood);
        lerpFood = 0;
        curFood += amount;
    }
}

void IncreasePop(int amount)
{
    if (amount != 0)
    {
        delayedPop = CP_Math_LerpInt(delayedPop, curPopulation, lerpPop);
        lerpPop = 0;
        curPopulation += amount;
    }
}

void IncreaseMorale(int amount)
{
    delayedMorale = CP_Math_LerpInt(delayedMorale, curMorale, lerpMorale);
    lerpMorale = 0;
    curMorale += amount;
}

void IncreaseBlessing(int amount)
{
    delayedBlessing = CP_Math_LerpInt(delayedBlessing, curBlessing, lerpBlessing);
    lerpBlessing = 0;
    curBlessing += amount;
}

/*!_____________________________________________________________________________
@brief      This function applies the change in the respective resources during 
            an event and calls for the respective AddDelayedResource() function
            for each resources. It takes in an int value and returns no output.
*//*__________________________________________________________________________*/
void ApplyEventResourceChange(int resourceChange[4])
{
    AddDelayedResource(1, resourceChange[0], 0.6f);
    AddDelayedResource(2, resourceChange[1], 0.6f);
    AddDelayedResource(3, resourceChange[2], 0.6f);
    AddDelayedResource(4, resourceChange[3], 0.6f);
}

/*!_____________________________________________________________________________
@brief      This function checks whether the player has enough Gold to pay for
            a cost incurred during their decision phase; if they have enough Gold
            to pay the function returns true, otherwise it will return false.
            It takes in an int value for the cost amount to compare with their
            current Gold and returns a boolean.
*//*__________________________________________________________________________*/
bool IsCostPayable(int costAmt)
{
	if (curGold >= costAmt)
	{
		return true;
	}
	return false;
}

