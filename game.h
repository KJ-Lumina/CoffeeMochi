#include <stdio.h>
#include "cprocessing.h"


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
	const char* name;
	int spriteIndex;
	int goldGen;
	int foodGen;
	int moraleGen;
}BUILDING;

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
	float width;
	float height;
	float xPos;
	float yPos;
	int identity;
	const char* imagename;
}Button;

typedef struct 
{
	char buildingName[16];
	int population;
	int goldCost;
}Building;

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

#define WORLDGRIDX 40
#define WORLDGRIDY 40
#define TILEWIDTH 64.0f
#define TILEHEIGHT 64.0f

GAMESTATE GetGameState();
CP_Vector SnapToGrid(float, float, CP_Vector);
CP_Vector WorldToGridPosition(float, float, CP_Vector);
CP_Vector GridToWorldPosition(float, float, CP_Vector);
void DrawUI();
void InitBuildings(void);
void InitDeck(void);
void InitUI();
void UI_SetEvent(CARDEVENTS);
BUILDING GetBuildingByIndex(int);
CP_Image GetBuildingSpriteByIndex(int);
int Get_current_gold();
int Get_current_food();
int Get_current_population();
void AddMarket();
void AddFarm();
void AddHouse();
void GenerateResourcesOnEndTurn();
int CheckUIClick(float, float);
CARDEVENTS GetBasicEvent();
CARDEVENTS GetCurrentEvent();
void DrawAnimation(float x, float y, float scaleX, float scaleY, float delay, int index);
SPRITESHEET GetSpriteAnimationByIndex(int index);
CP_Image GetSpriteSheetByIndex(int index);
void InitSpritesheets(void); 
















