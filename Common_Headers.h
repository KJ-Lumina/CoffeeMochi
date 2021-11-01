#include <stdio.h>
#include <stdbool.h>
#include "cprocessing.h"

#define WORLDGRIDX 10
#define WORLDGRIDY 5
#define TILEWIDTH 128.0f
#define TILEHEIGHT 128.0f


#define R_NULL_INDEX 0
#define R_GOLD_INDEX 1
#define R_FOOD_INDEX 2

#define NULL_CHOICE -1
#define B_GRASS_INDEX 1
#define B_HOUSE_INDEX 2
#define B_FARM_INDEX 3
#define B_MARKET_INDEX 4

#define NULL_EVENT 0
#define BASIC_EVENT 1
#define ADVANCED_EVENT 2




typedef enum
{
	State_MainMenu,
	State_Idle,
	State_MakeAChoice,
	State_PlaceYourBuilding,
	State_GameOver
}GAMESTATE;

typedef struct
{
	int eventIndex;
	int eventType;
	char description[100];

	int indexOptionA;
	int buildAmountA;
	int costTypeA;
	int costAmountA;
	char* descriptionA;

	int indexOptionB;
	int buildAmountB;
	int costTypeB;
	int costAmountB;
	char* descriptionB;


}CARDEVENT;


typedef struct
{
	const char* name;
	int spriteIndex;
	int goldGen;
	int foodGen;
	int moraleGen;
}BUILDING;

typedef struct 
{
	float width;
	float height;
	float xPos;
	float yPos;
	int identity;
	const char* imagename;
}Button;

typedef struct
{
	int setNextSprite;
	float minX; //parameters for recurring sprites x-axis
	float maxX; //parameters for recurring sprites x-axis
	float minY;
	float maxY;
	int maxSprites; //number of sprites in a spritesheet
	float spriteSizeX; //pixels on a sprite grid x-axis
	float spriteSizeY; //pixels on a sprite grid y-axis
}SPRITESHEET;

typedef struct
{
	const char* imagename;
	int maxSpritesX;
	int maxSpritesY;

}TILEMAP;

float Math_Clamp_Float(float target, float min, float max);
int Math_Clamp_Int(int target, int min, int max);

GAMESTATE GetGameState();

// Initialization
void InitBuildings();
void InitDecks();
void InitUI();
void GameOver();

// WorldGridSpace
void MoveWorldSpaceOrigin(float positionChangeX, float positionChangeY);
void DrawCursorTile(CP_Vector cursorPos);
void SetNewBuilding(int xPos, int yPos, int buildingIndex);
void SetCurrentBuilding(BUILDING* newBuilding);
bool AttemptPlaceBuilding(CP_Vector cursorPos);
bool IsTileOccupied(CP_Vector);
void DrawUI();

void DrawTempTextResources();
void UI_SetEvent(CARDEVENT*);
int Get_current_gold();
int Get_current_food();
int Get_current_population();
void AddNewResourceBuilding(int buildingIndex);
void AddMarket();
void AddFarm();
void AddHouse();
void GenerateResourcesOnEndTurn();
int CheckUIClick(float, float);
CARDEVENT* GetNextEvent();
CARDEVENT* GetCurrentEvent();
BUILDING* GetBuildingByIndex(int);
CP_Image* GetBuildingSpriteByIndex(int);
CP_Image* GetBuildingSpriteButtonByIndex(int);



















