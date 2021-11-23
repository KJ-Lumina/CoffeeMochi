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


int curGold = 100;
int curFood = 40;
int curPopulation = 0;
int initPopulation = 100;
int curMorale = 50;
int additionalMorale = 0;

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

	curGold += (numMarkets * GOLD_AMT_FROM_MARKETS) - (numTaverns * TAVERN_UPKEEP_COST);
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

	curFood += (numFarms * FOOD_AMT_FROM_FARMS) - (numHouses * FOOD_CONSUMPTION_PER_PAX);
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

    currentMoraleStatus = isPoor + isStarved;

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
	}
	
}

//TEMPORARY FOR PROTOTYPE ONLYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY

void InitResources(int startingGold, int startingFood, int startingPopulation, int startingMorale) {
	curGold = startingGold;
    curFood = startingFood;
    curPopulation = startingPopulation;
    curMorale = startingMorale;
    additionalMorale = 0;
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

void GenerateResourcesOnEndTurn()
{
	Gold_generated_per_turn();
	Food_generated_per_turn();
	Population_per_turn();
	//Morale_per_turn();
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

void ApplyEventResult(int resourceChange[4])
{
    
    //curGold += Lerp_Resources(curGold, curGold + resourceChange[0], 2.0f);
    curGold += resourceChange[0];
    curFood += resourceChange[1];
    curPopulation += resourceChange[2];
    curMorale += resourceChange[3];

    /*if (resourceChange[0] != 0)
    {
        resourceChange[0] > 0 ? DrawanimgoldIncrease : Drawnimgolddecrease
    }*/
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
