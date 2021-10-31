#include <stdio.h>
#include <stdbool.h>
#include "cprocessing.h"

#define WORLDGRIDX 10
#define WORLDGRIDY 5
#define TILEWIDTH 128.0f
#define TILEHEIGHT 128.0f

#define B_GRASS_INDEX 1
#define B_HOUSE_INDEX 2
#define B_FARM_INDEX 3
#define B_MARKET_INDEX 4

#define BASIC_EVENT 1
#define ADVANCED_EVENT 2
#define NULL_CHOICE -1


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
	int difficulty;
	char description[100];
	int indexOptionA;
	int indexOptionB;
	char* stringOptionA;
	char* stringOptionB;

}CARDEVENTS;

typedef struct
{
	CARDEVENTS cards[20];
	int cardsLeft;

}CARDDECK;

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

void MoveWorldSpaceOrigin(float positionChangeX, float positionChangeY);
void SetNewBuilding(int xPos, int yPos, int buildingIndex);
bool IsTileOccupied(CP_Vector);
void DrawUI();
void InitBuildings(void);
void InitDeck(void);
void InitUI();
void DrawTempTextResources();
void UI_SetEvent(CARDEVENTS*);
int Get_current_gold();
int Get_current_food();
int Get_current_population();
void AddMarket();
void AddFarm();
void AddHouse();
void GenerateResourcesOnEndTurn();
int CheckUIClick(float, float);
CARDEVENTS* GetBasicEvent();
CARDEVENTS* GetCurrentEvent();
BUILDING* GetBuildingByIndex(int);
CP_Image* GetBuildingSpriteByIndex(int);
CP_Image* GetBuildingSpriteButtonByIndex(int);



















