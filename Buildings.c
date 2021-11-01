#include <stdio.h>
#include "cprocessing.h"
#include "Common_Headers.h"
#include "Buildings.h"





BUILDING tile_base		= { "grass",	B_GRASS_INDEX, 0, 0, 0 };
BUILDING tile_house		= { "House",	B_HOUSE_INDEX, 0, 0, 0 };
BUILDING tile_farm		= { "Farm",		B_FARM_INDEX, 0, 1, 0 };
BUILDING tile_market	= { "Market",	B_MARKET_INDEX, 1, 0, 0 };
BUILDING tile_tavern	= { "Tavern",	5, 0, 0, 10 };

CP_Image sprite_grass;
CP_Image sprite_house;
CP_Image sprite_farm;
CP_Image sprite_market;
CP_Image sprite_tavern;
CP_Image button_house;
CP_Image button_farm;
CP_Image button_grass;


void InitBuildings(void)
{
	sprite_grass = CP_Image_Load("./Assets/grasstile.png");
	sprite_house = CP_Image_Load("./Assets/best_House.png");
	sprite_farm = CP_Image_Load("./Assets/best_Farm.png");
	sprite_market = CP_Image_Load("./Assets/best_Tree.png");
	sprite_tavern = CP_Image_Load("./Assets/best_Tavern.png");
	button_house = CP_Image_Load("./Assets/housebtn.png");
	button_farm = CP_Image_Load("./Assets/farmbtn.png");
	button_grass = CP_Image_Load("./Assets/grassbtn.png");
}

BUILDING* GetBuildingByIndex(int index)
{
	switch (index)
	{
	case 1:
		return &tile_base;
	case 2:
		return &tile_house;
	case 3:
		return &tile_farm;
	case 4:
		return &tile_market;
	case 5:
		return &tile_tavern;
	default:
		return &tile_base;
		break;
	}
}

CP_Image* GetBuildingSpriteByIndex(int index)
{
	switch (index)
	{
	case 1:
		return &sprite_grass;

	case 2:
		return &sprite_house;
	case 3:
		return &sprite_farm;
	case 5:
		return &sprite_market;
	case 6:
		return &sprite_tavern;
	default:
		return &sprite_grass;
	}
}
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