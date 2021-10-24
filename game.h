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

#define WORLDGRIDX 40
#define WORLDGRIDY 40
#define TILEWIDTH 128.0f
#define TILEHEIGHT 128.0f

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
CP_Image GetBuildingSpriteButtonByIndex(int);
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



















