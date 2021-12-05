/*!_____________________________________________________________________________
@file       Building.c
@author     Erron Quek (e.quek@digipen.edu)
@co-authors Travess Tan (travesscheekhang.t@digipen.edu)
@course     CSD1120
@section    B
@team       CoffeeMochi
@brief      This file contains the all building related variable & function definitions in Impero 

			Copyright ©2021 DigiPen, All rights reserved.
*//*__________________________________________________________________________*/

#include <stdio.h>
#include <stdbool.h>
#include "cprocessing.h"
#include "Common_Headers.h"

typedef struct
{
	CP_Vector position;
	int buildingIndex;
} CONSTBUILDING;


BUILDING tile_house		= { "House",	B_HOUSE_INDEX, 0, 0, 0 };
BUILDING tile_farm		= { "Farm",		B_FARM_INDEX, 0, 1, 0 };
BUILDING tile_market	= { "Market",	B_MARKET_INDEX, 1, 0, 0 };
BUILDING tile_tavern	= { "Tavern",	B_TAVERN_INDEX, 0, 0, 10 };

CONSTBUILDING buildingArray[WORLDGRIDX * WORLDGRIDY] = { 0 };

CP_Image sprite_grass;
CP_Image sprite_house;
CP_Image sprite_farm;
CP_Image sprite_market;
CP_Image sprite_tavern;
CP_Image button_house;
CP_Image button_farm;
CP_Image button_grass;
/*--------------------
SET BUILDING FUNCTIONS
---------------------*/
/*!_____________________________________________________________________________
@brief      This function initializes the every respective building's CP_Image
			with its respective sprite image. It takes no input and returns no
			output.
*//*__________________________________________________________________________*/
void InitBuildings(void)
{
	sprite_grass = CP_Image_Load("./Assets/grasstile.png");
	sprite_house = CP_Image_Load("./ImperoArtAssets/Impero_House.png");
	sprite_farm = CP_Image_Load("./ImperoArtAssets/Impero_ShittyFarm.png");
	sprite_market = CP_Image_Load("./ImperoArtAssets/Impero_Market.png");
	sprite_tavern = CP_Image_Load("./ImperoArtAssets/Impero_Tavern.png");
	button_house = CP_Image_Load("./Assets/housebtn.png");
	button_farm = CP_Image_Load("./Assets/farmbtn.png");
	button_grass = CP_Image_Load("./Assets/grassbtn.png");
}

/*!_____________________________________________________________________________
@brief      These 2 functions AddBuilding and RemoveBuilding searches for a
			building's CP_Vector 'pos' and adds or removes the building 
			accordingly. For AddBuilding it will first ensure that the tile 
			is empty before adding the building indicated by 'index'; for 
			RemoveBuilding, it will search for the indicated pos and set the tile
			back to an empty tile. 
			AddBuilding takes an int and a CP_Vector and returns nothing.
			RemoveBuilding takes a CP_Vector and returns nothing.
*//*__________________________________________________________________________*/
void AddBuilding(int index, CP_Vector pos)
{
	for (int i = 0; i < (WORLDGRIDX * WORLDGRIDY); i++)
	{
		if (buildingArray[i].buildingIndex == 0)
		{
			buildingArray[i].buildingIndex = index;
			buildingArray[i].position = pos;
		}
	}
}

void RemoveBuilding(CP_Vector pos)
{
	for (int i = 0; i < (WORLDGRIDX * WORLDGRIDY); i++)
	{
		if (buildingArray[i].position.x == pos.x && buildingArray[i].position.y == pos.y)
			buildingArray[i].buildingIndex = 0;
	}
}

/*----------------------
OTHER BUILDING FUNCTIONS
-----------------------*/
/*!_____________________________________________________________________________
@brief      This function searches for the closest building type 'index' from
			the CP_Vector 'location'. It takes in an int and a CP_Vector and
			returns a CP_Vector of the closest building found.
*//*__________________________________________________________________________*/

CP_Vector FindNearestBuilding(int index, CP_Vector location)
{
	float closestDist = 1000.0f;
	int nearest = 0;
	for (int i = 0; i < (WORLDGRIDX * WORLDGRIDY); i++)
	{
		if (buildingArray[i].buildingIndex == index && CP_Vector_Distance(location, buildingArray[i].position) < closestDist)
		{
			closestDist = CP_Vector_Distance(location, buildingArray[i].position);
			nearest = i;
		}
	}
	return buildingArray[nearest].position;
}

/*!_____________________________________________________________________________
@brief      This function counts the total number of building type 'index' that
			is currently on the field. It takes an input of int, and returns
			an int of the total count.
*//*__________________________________________________________________________*/
int CountBuilding(int index)
{
	int count = 0;
	for (int i = 0; i < (WORLDGRIDX * WORLDGRIDY); i++)
	{
		if (buildingArray[i].buildingIndex == index)
			count++;
	}
	return count;
}

/*!_____________________________________________________________________________
@brief      This function returns the memory address of a building tile 'index'
			It takes an input of int and returns a pointer to the first address
			of the object BUILDING.
*//*__________________________________________________________________________*/
BUILDING* GetBuildingByIndex(int index)
{
	switch (index)
	{
	case B_HOUSE_INDEX:
		return &tile_house;
	case B_FARM_INDEX:
		return &tile_farm;
	case B_MARKET_INDEX:
		return &tile_market;
	case B_TAVERN_INDEX:
		return &tile_tavern;
	default:
		return &tile_house;
		break;
	}
}

/*!_____________________________________________________________________________
@brief      This function returns the memory address of a CP_Image of a tile 
			'index'. It takes an input of int and returns a pointer to the 
			first address of a CP_Image.
*//*__________________________________________________________________________*/
CP_Image* GetBuildingSpriteByIndex(int index)
{
	switch (index)
	{
	case B_HOUSE_INDEX:
		return &sprite_house;
	case B_FARM_INDEX:
		return &sprite_farm;
	case B_MARKET_INDEX:
		return &sprite_market;
	case B_TAVERN_INDEX:
		return &sprite_tavern;
	default:
		return &sprite_grass;
	}
}

/*!_____________________________________________________________________________
@brief      This function returns the memory address of a CP_Image button of a 
			tile 'index'. It takes an input of int and returns a pointer to the
			first address of a CP_Image.
*//*__________________________________________________________________________*/
CP_Image* GetBuildingSpriteButtonByIndex(int index)
{
	switch (index)
	{
	case 1:
		return &button_grass;		
	case 2:
		return &button_house;
	case 3:
		return &button_farm;
	case 5:
		return &sprite_market;
	case 6:
		return &sprite_tavern;
	default:
		return &button_grass;
	}
}