#include <stdio.h>
#include "cprocessing.h"
#include <stdbool.h>
#include "Common_Headers.h"
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
#define GOLD_AMT_FROM_MARKETS 3
#define TAX_AMOUNT 1

// For now Food generated from 'Farm' tiles and 'farmer' citizens 
// and consumption amount is defaulted to the following values
// Subject to adjustments if Levels/Ranks are to be implemented
#define FOOD_AMT_FROM_FARMS 7
#define FOOD_CONSUMPTION_PER_PAX 5

#define PAX_PER_HOUSING 1
#define INCREMENT_PERCENTAGE_PER_PAX 1
#define PAX_LOW_MODIFIER 0.1
#define PAX_MEDIUM_MODIFIER 0.25

// Upkeep costs for every tile are defaulted to the following values
// Subject to adjustments if Levels/Ranks are to be implemented
#define TAVERN_UPKEEP_COST 5
//#define FARM_UPKEEP_COST 5
//#define HOUSING_UPKEEP_COST 5


#define HIGH_MORALE 0
#define MEDIUM_MORALE 1
#define LOW_MORALE 2

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

void Set_additional_morale(int addMorale) {
	additionalMorale = addMorale;
}

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

//int Lerp_Resources(int start, int end, float duration)
//{   
//    float dt = duration / CP_System_GetDt();
//    float t = 0;
//    while (t <= duration)
//    {
//        t += dt;
//        return (int)((end - start)/dt);
//    }
//    return 0;
//}

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
    //IncreaseGold((numMarkets * GOLD_AMT_FROM_MARKETS) - (numTaverns * TAVERN_UPKEEP_COST));
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
    //IncreaseFood((numFarms * FOOD_AMT_FROM_FARMS) - (numHouses * FOOD_CONSUMPTION_PER_PAX));
}

void Population_per_turn()
{
    /*
	//curPopulation = initPopulation + (numHouses * PAX_PER_HOUSING);
    if (!isStarved) {
        curPopulation += numHouses * PAX_PER_HOUSING * INCREMENT_PERCENTAGE_PER_PAX;
    }
    else {
        curPopulation -= numHouses * PAX_PER_HOUSING * INCREMENT_PERCENTAGE_PER_PAX;
    }

    switch (currentMoraleStatus) {

    case MEDIUM_MORALE:
        curPopulation -= (int)(curPopulation * PAX_MEDIUM_MODIFIER);
        break;

    case LOW_MORALE:
        curPopulation -= (int)(curPopulation * PAX_LOW_MODIFIER);
        break;

    }*/
    curPopulation =  numHouses * PAX_PER_HOUSING * 10;

}

void Gold_check()
{
	if (curGold < 30)
		isPoor = TRUE;
	else
		isPoor = FALSE;
}

void Food_check()
{
	if (curFood < (curPopulation * 2))
		isStarved = TRUE;
	else
		isStarved = FALSE;
}


void Morale_per_turn()
{
	//for now morale calculation is simply based on whether 2 conditions are true, if we want to add
	//event effects then we will just have to add a seperate counter for it
	//Tavern morale modifier affected by Morale debuff i.e. More debuff, lower Morale increase

    /*currentMoraleStatus = isPoor + isStarved;

	switch (currentMoraleStatus)
	{
		case HIGH_MORALE:
			curMorale = curPopulation + (numTaverns * tavernModifier);
			break;
		case MEDIUM_MORALE:
			curMorale = (curPopulation/MEDIUM_MORALE) + (numTaverns * (tavernModifier - MEDIUM_MORALE));
			break;
		case LOW_MORALE:
			curMorale = (curPopulation/LOW_MORALE) + (numTaverns * (tavernModifier - LOW_MORALE));
			break;
	}*/
    //IncreaseMorale(numTaverns * 2);
}

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
void AddTavern()
{
	numTaverns++;
}

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

void ResetBlessing()
{
    curBlessing = 0;
}

void GenerateResourcesOnEndTurn()
{
	Gold_generated_per_turn();
	Food_generated_per_turn();
	Population_per_turn();
	Morale_per_turn();
}

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

void ApplyEventResourceAnim(int resourceChange[4])
{
    SpawnGoldGainAnimation(resourceChange[0], CP_Vector_Set(200, 450), CP_Vector_Set(CP_Random_RangeFloat(150,250), CP_Random_RangeFloat(400, 500)), CP_Vector_Set(520, 90), 0.6f, 0);
    SpawnFoodGainAnimation(resourceChange[1], CP_Vector_Set(200, 450), CP_Vector_Set(CP_Random_RangeFloat(150, 250), CP_Random_RangeFloat(400, 500)), CP_Vector_Set(520, 180), 0.6f, 0);
    SpawnMoraleGainAnimation(resourceChange[3], CP_Vector_Set(200, 450), CP_Vector_Set(CP_Random_RangeFloat(150, 250), CP_Random_RangeFloat(400, 500)), CP_Vector_Set(520, 360), 0.6f, 0);
}

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

void ApplyEventResourceChange(int resourceChange[4])
{
    AddDelayedResource(1, resourceChange[0], 0.6f);
    AddDelayedResource(2, resourceChange[1], 0.6f);
    AddDelayedResource(3, resourceChange[2], 0.6f);
    AddDelayedResource(4, resourceChange[3], 0.6f);
}

bool IsCostPayable(int costAmt)
{
	if (curGold >= costAmt)
	{
		return true;
	}
	return false;
}




/*// click on option A
        if (xPos >= optionAPos.x - 60 && xPos <= optionAPos.x + 60 && yPos >= optionBPos.y - 160 && yPos <= optionBPos.y + 160)
        {

            //Check for Pre-Requiste 
            switch (selectedEvent->costTypeA)
            {
            case R_NULL_INDEX:
                //Do Nothing
                break;
                //RESOURCE COST
            case R_GOLD_INDEX:
                if ((Get_current_gold() - selectedEvent->costAmountA) < 0) return 0;
                Set_current_gold(Get_current_gold() - selectedEvent->costAmountA);
                break;

            case R_FOOD_INDEX:
                if ((Get_current_food() - selectedEvent->costAmountA) < 0) return 0;
                Set_current_food(Get_current_food() - selectedEvent->costAmountA);
                break;

            case R_POPULATION_INDEX:
                if ((Get_current_population() - selectedEvent->costAmountA) < 0) return 0;
                Set_current_population(Get_current_population() - selectedEvent->costAmountA);
                break;

            case R_MORALE_INDEX:
                if ((Get_current_morale() - selectedEvent->costAmountA) < 0) return 0;
                Set_additional_morale(Get_additional_morale() - selectedEvent->costAmountA);
                break;

            case R_BUILDING_HOUSE_INDEX:
                SubtractHouse();
                DestroyBuildingByIndex(B_HOUSE_INDEX);

                break;

            case R_BUILDING_FARM_INDEX:
                SubtractFarm();
                DestroyBuildingByIndex(B_FARM_INDEX);

                break;

            case R_BUILDING_MARKET_INDEX:
                SubtractMarket();
                DestroyBuildingByIndex(B_MARKET_INDEX);

                break;

            case R_BUILDING_TAVERN_INDEX:
                SubtractTavern();
                DestroyBuildingByIndex(B_TAVERN_INDEX);

                break;

            default:
                break;
            }

            //Run the Event depending on the event type if the Pre-Requiste is met
            switch (selectedEvent->eventType) {
            case BUILD_TYPE_EVENT:
                if (selectedEvent->indexOptionA != NULL_CHOICE) {
                    SetCurrentBuilding(GetBuildingByIndex(selectedEvent->indexOptionA));
                    SetCurrentAmountToBuild(selectedEvent->optionAmountA);
                    return 1;
                }
                else {
                    return 2;
                }           
                break;

            case RESOURCE_TYPE_EVENT:

                switch (selectedEvent->optionTypeA) {

                case R_NULL_INDEX:

                    break;

                case R_GOLD_INDEX:

                    if (selectedEvent->indexOptionA == E_INCREASE_RESOURCE) {
                        Set_current_gold(Get_current_gold() + selectedEvent->optionAmountA);
                    }
                    else if (selectedEvent->indexOptionA == E_DECREASE_RESOURCE) {
                        Set_current_gold(Get_current_gold() - selectedEvent->optionAmountA);
                    }

                    break;

                case R_FOOD_INDEX:

                    if (selectedEvent->indexOptionA == E_INCREASE_RESOURCE) {
                        Set_current_food(Get_current_food() + selectedEvent->optionAmountA);
                    }
                    else if (selectedEvent->indexOptionA == E_DECREASE_RESOURCE) {
                        Set_current_food(Get_current_food() - selectedEvent->optionAmountA);
                    }

                    break;

                case R_POPULATION_INDEX:

                    if (selectedEvent->indexOptionA == E_INCREASE_RESOURCE) {
                        Set_current_population(Get_current_population() + selectedEvent->optionAmountA);
                    }
                    else if (selectedEvent->indexOptionA == E_DECREASE_RESOURCE) {
                        Set_current_population(Get_current_population() - selectedEvent->optionAmountA);
                    }

                    break;

                default:
                    break;
                }

                return 2;
                break;

            case DESTROY_TYPE_EVENT:

                switch (selectedEvent->optionTypeA) {

                case R_BUILDING_HOUSE_INDEX:
                    SubtractHouse();
                    DestroyBuildingByIndex(B_HOUSE_INDEX);

                    break;

                case R_BUILDING_FARM_INDEX:
                    SubtractFarm();
                    DestroyBuildingByIndex(B_FARM_INDEX);

                    break;

                case R_BUILDING_MARKET_INDEX:
                    SubtractMarket();
                    DestroyBuildingByIndex(B_MARKET_INDEX);

                    break;

                case R_BUILDING_TAVERN_INDEX:
                    SubtractTavern();
                    DestroyBuildingByIndex(B_TAVERN_INDEX);

                    break;
                }
                return 2;
                break;

            case ONGOING_TYPE_EVENT:

                break;

            default:
                break;

            }

            break;
        }*/
