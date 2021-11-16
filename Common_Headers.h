#include <stdio.h>
#include <stdbool.h>
#include "cprocessing.h"

#define WORLDGRIDX 10
#define WORLDGRIDY 5
#define TILEWIDTH 128.0f
#define TILEHEIGHT 128.0f

#define MAXTILECOUNT (WORLDGRIDX * WORLDGRIDY)


#define R_NULL_INDEX 0
#define R_GOLD_INDEX 1
#define R_FOOD_INDEX 2
#define R_POPULATION_INDEX 3
#define R_MORALE_INDEX 4
#define R_BUILDING_HOUSE_INDEX 5
#define R_BUILDING_FARM_INDEX 6
#define R_BUILDING_MARKET_INDEX 7
#define R_BUILDING_TAVERN_INDEX 8


#define NULL_CHOICE -1
#define B_GRASS_INDEX 1
#define B_HOUSE_INDEX 2
#define B_FARM_INDEX 3
#define B_MARKET_INDEX 4
#define B_TAVERN_INDEX 5

#define E_INCREASE_RESOURCE 1
#define E_DECREASE_RESOURCE 2
#define E_DESTROY_BUILDING 3

#define NULL_EVENT 0
#define BASIC_EVENT 1
#define ADVANCED_EVENT 2

#define NULL_TYPE_EVENT 0
#define BUILD_TYPE_EVENT 1
#define RESOURCE_TYPE_EVENT 2
#define DESTROY_TYPE_EVENT 3
#define ONGOING_TYPE_EVENT 4

//buttons names in-progress
#define START_GAME 0


typedef enum {
	PHASE_MAINMENU,
	PHASE_BUILDPHASE,
	PHASE_GAMEPHASE,
	PHASE_ENDPHASE

}GAMEPHASE;

typedef enum
{
	State_MainMenu,
	State_StartOfTurn,
	State_Idle,
	State_MakeAChoice,
	State_PlaceYourBuilding,
	State_EndOfTurn,
}GAMESTATE;


typedef const struct {

	int indexOption;
	int optionType;

}CARDOPTION;

typedef struct
{
	int eventIndex;
	int eventDifficultyType;
	int eventType;
	char description[100];

	int indexOptionA;
	int optionAmountA;
	int optionTypeA;
	int costTypeA;
	int costAmountA;
	char* descriptionA;

	int indexOptionB;
	int optionAmountB;
	int optionTypeB;
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
	bool isUsed;
	float width;
	float height;
	float xPos;
	float yPos;
	int isSplashScreenActive;
	int isSettingsActive;
	int index;

}BUTTON;

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
	float timeToDeath; //time drawn on screen before it disappears
	float posX;
	float posY;
	float scaleX;
	float scaleY;
	float timeElapse;
	int index;
	int isInfiniteLoop;
}SPRITESHEET;

typedef struct
{
	const char* imagename;
	int maxSpritesX;
	int maxSpritesY;

}TILEMAP;

typedef struct
{
	int positionX;
	int positionY;

}TILEPOSITION;

float Math_Abs(float x);
int Math_Abs_Int(int x);

GAMESTATE GetGameState();

// Initialization
void InitBuildings();
void InitDecks();
void InitUI();
void InitResources(int startingGold);
void GameOver();

// WorldGridSpace
CP_Vector GetWorldSpaceOrigin();
void MoveWorldSpaceOrigin(float positionChangeX, float positionChangeY);
void DrawGridIndicator(CP_Vector cursorPosition);
void DrawCursorTile(CP_Vector cursorPos);
float CalculateUnitsToBorder(CP_Vector position, CP_Vector directionUnit);
void SetNewBuilding(int xPos, int yPos, int buildingIndex);
void SetCurrentBuilding(BUILDING* newBuilding);
void SetCurrentAmountToBuild(int buildAmount);
bool AttemptPlaceBuilding(CP_Vector cursorPos);
bool IsTileOccupied(CP_Vector);
int GetAllBuildingsPositionByIndex(int index, TILEPOSITION position[]);

// UI_Mechanics
void DrawUI_Deck();
void DrawUI_CardDrawn();
void DrawUI_Constructing();
void DrawTempTextResources();
void UI_SetEvent(CARDEVENT*);
int CheckUIClick(float, float);

// Resources
void Set_current_gold(int gold);
void Set_current_food(int food);
void Set_current_population(int population);
void Set_additional_morale(int addMorale);
int Get_current_gold();
int Get_current_food();
int Get_current_population();
int Get_current_morale();
int Get_additional_morale();
void AddNewResourceBuilding(int buildingIndex);
void AddMarket();
void AddFarm();
void AddHouse();
void SubtractMarket();
void SubtractFarm();
void SubtractHouse();
void GenerateResourcesOnEndTurn();

// Card Events
int GetCardsLeft();
CARDEVENT* GetNextEvent(GAMEPHASE gamePhase);
CARDEVENT* GetCurrentEvent();
BUILDING* GetBuildingByIndex(int);
CP_Image* GetBuildingSpriteByIndex(int);
CP_Image* GetBuildingSpriteButtonByIndex(int);
void ChangeDeckByPhase(GAMEPHASE currentGamePhase);



















