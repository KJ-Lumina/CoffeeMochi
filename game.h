#include <stdio.h>
#include "cprocessing.h"

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

CP_Vector SnapToGrid(float, float, CP_Vector);
CP_Vector WorldToGridPosition(float, float, CP_Vector);
CP_Vector GridToWorldPosition(float, float, CP_Vector);
void DrawUI();
void InitBuildings(void);
void InitDeck(void);
void InitUI();
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















