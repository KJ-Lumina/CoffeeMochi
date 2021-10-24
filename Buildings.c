#include <stdio.h>
#include "cprocessing.h"
#include "game.h"




BUILDING tile_base		= { "grass",	1, 0, 0, 0 };
BUILDING tile_house		= { "House",	2, 0, 0, 0 };
BUILDING tile_farm		= { "Farm",		3, 0, 1, 0 };
BUILDING tile_market	= { "Market",	4, 1, 0, 0 };
BUILDING tile_tavern	= { "Tavern",	5, 0, 0, 10 };

CP_Image sprite_grass;
CP_Image sprite_house;
CP_Image sprite_farm;
CP_Image sprite_market;
CP_Image sprite_tavern;

void InitBuildings(void)
{
	sprite_grass = CP_Image_Load("./Assets/grasstile.png");
	sprite_house = CP_Image_Load("./Assets/housetile.png");
	sprite_farm = CP_Image_Load("./Assets/wheattile.png");
	sprite_market = CP_Image_Load("./Assets/treetile.png");
	sprite_tavern = CP_Image_Load("./Assets/TDgrasstile.png");
}

BUILDING GetBuildingByIndex(int index)
{
	switch (index)
	{
	case 1:
		return tile_base;
	case 2:
		return tile_house;
	case 3:
		return tile_farm;
	case 4:
		return tile_market;
	case 5:
		return tile_tavern;
	default:
		return tile_base;
		break;
	}
}

CP_Image GetBuildingSpriteByIndex(int index)
{
	switch (index)
	{
	case 1:
		return sprite_grass;
	case 2:
		return sprite_house;
	case 3:
		return sprite_farm;
	case 5:
		return sprite_market;
	case 6:
		return sprite_tavern;
	default:
		return sprite_grass;
	}
}