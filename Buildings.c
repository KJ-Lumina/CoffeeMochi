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
	sprite_market = CP_Image_Load("./Assets/best_Market.png");
	sprite_tavern = CP_Image_Load("./Assets/best_Tavern.png");
	button_house = CP_Image_Load("./Assets/housebtn.png");
	button_farm = CP_Image_Load("./Assets/farmbtn.png");
	button_grass = CP_Image_Load("./Assets/grassbtn.png");
}

BUILDING* GetBuildingByIndex(int index)
{
	switch (index)
	{
	case B_GRASS_INDEX:
		return &tile_base;
	case B_HOUSE_INDEX:
		return &tile_house;
	case B_FARM_INDEX:
		return &tile_farm;
	case B_MARKET_INDEX:
		return &tile_market;
	case B_TAVERN_INDEX:
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
	case B_GRASS_INDEX:
		return &sprite_grass;
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