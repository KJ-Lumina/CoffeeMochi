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
	CARDEVENTS cards[20];
	int cardsLeft;

}CARDDECK;

typedef struct 
{
	float width;
	float height;
	float xPos;
	float yPos;
	int identity;
	const char* imagename;
}Button;


#define B_GRASS_INDEX 1
#define B_HOUSE_INDEX 2
#define B_FARM_INDEX 3
#define B_MARKET_INDEX 4

#define BASIC_EVENT 1
#define ADVANCED_EVENT 2
#define NULL_CHOICE -1


#define WORLDGRIDX 40
#define WORLDGRIDY 40
#define TILEWIDTH 128.0f
#define TILEHEIGHT 128.0f

float Math_Clamp_Float(float target, float min, float max);
int Math_Clamp_Int(int target, int min, int max);

GAMESTATE GetGameState();
CP_Vector GetWorldSpaceOrigin();
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



















