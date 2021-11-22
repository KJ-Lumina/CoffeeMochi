#include <stdio.h>
#include <stdbool.h>
#include "cprocessing.h"




#define WORLDGRIDX 5
#define WORLDGRIDY 5
#define TILEWIDTH 128.0f
#define TILEHEIGHT 128.0f
#define TILESPRITEWIDTH 128.0f
#define TILESPRITEHEIGHT 160.0f
#define MAPOFFSETX -130

#define MAXTILECOUNT (WORLDGRIDX * WORLDGRIDY)

//Menu & Music
#define BGM CP_SOUND_GROUP_0
#define SFX CP_SOUND_GROUP_1

#define BLUE_PILL 1;
#define RED_PILL 2;

//Building Related Definitions
#define B_GRASS_INDEX -2 // TO BE DELETED
#define NULL_CHOICE -1
#define B_EMPTY_INDEX 0
#define B_HOUSE_INDEX 1
#define B_FARM_INDEX 2
#define B_MARKET_INDEX 3
#define B_TAVERN_INDEX 4

//Events Resource Index
#define NUMBER_OF_MAX_REWARDS 2

#define R_NULL_INDEX 0
#define R_GOLD_INDEX 1
#define R_FOOD_INDEX 2
#define R_POPULATION_INDEX 3
#define R_MORALE_INDEX 4
#define R_BUILDING_HOUSE_INDEX 5
#define R_BUILDING_FARM_INDEX 6
#define R_BUILDING_MARKET_INDEX 7
#define R_BUILDING_TAVERN_INDEX 8


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
#define EVENT_TYPE_REWARD 5


//Resources
// Building costs for every tile are defaulted to the following values
#define MARKET_BUILD_COST 20
#define FARM_BUILD_COST 20
#define HOUSING_BUILD_COST 20
#define TAVERN_BUILD_COST 20

//buttons names in-progress
#define START_GAME 0

void SetGameSceneEndPhase();

typedef enum 
{
	SCENE_SPLASH_DIGIPEN,
	SCENE_SPLASH_COFFEEMOCHI,
	SCENE_MAINMENU,
	SCENE_GAMEENTRY,
	SCENE_GAMELEAVEENTRY,
	SCENE_GAMEPHASE,
	SCENE_ENDPHASE,
}GAMESCENE;

typedef enum
{
	State_GameEntry,
	State_StartOfTurn,
	State_Idle,
	State_CardDraw,
	State_MakeAChoice,
	State_CollectRewards,
	State_PlaceYourBuilding,
	State_DestroyBuilding,
	State_EndOfTurn,
	State_GameOver

}GAMESTATE;

typedef struct
{
	int rewardIndex;
	int rewardAmount;

}REWARD;

typedef struct
{
	int eventIndex; // unique ID
	int eventType; //
	char* description;
	int resourceChangeA[4];
	REWARD resourceRewardA[NUMBER_OF_MAX_REWARDS];
	char* descriptionA;
	int resourceChangeB[4];
	REWARD resourceRewardB[NUMBER_OF_MAX_REWARDS];
	char* descriptionB;
}CARDEVENT;



typedef struct
{
	int eventIndex;
	int cardType;
	int resourceType;
	int resourceAmt;
	char* description;
}REWARDCARD;

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
typedef struct 
{
	CP_Image image;
	CP_Vector startingPos;
	CP_Vector endingPos;
	float totalTime;
	float currentTime;
}MOVINGSPRITES;
float Math_Abs(float x);
int Math_Abs_Int(int x);

/* Forward declarations */
void MainGame_Initialize(void);
void MainGame_Update(void);

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
bool AttemptPlaceBuilding(CP_Vector cursorPos);
int GetOccupiedIndex(int x, int y);
bool IsTileOccupied(CP_Vector);
int GetAllBuildingsPositionByIndex(int index, TILEPOSITION position[]);
void DestroyBuildingBySelectedBuilding();

// UI_Mechanics
bool CheckWithinBounds(CP_Vector position, float width, float height);
bool ClickCheck_CardDraw();
int ClickCheck_CardChoice();
int ClickCheck_Rewards();
void DrawUI_Deck();
void DrawUI(GAMESTATE state);
void DrawTempTextResources();
void UI_SetReward(REWARDCARD* rewardCard, int cardsLeft);
void UI_SetEvent(CARDEVENT*);

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
void AddTavern();
void SubtractMarket();
void SubtractFarm();
void SubtractHouse();
void SubtractTavern();
void GenerateResourcesOnEndTurn();
void ApplyEventResult(int resourceChange[4]);
bool LoseCondition_Resources();

// Card Events
int GetCardsLeft();
CARDEVENT* GetNextEvent();
CARDEVENT* GetCurrentEvent();
BUILDING* GetBuildingByIndex(int);
REWARDCARD* GetRewardByIndex(int index);
CP_Image* GetBuildingSpriteByIndex(int);
CP_Image* GetBuildingSpriteButtonByIndex(int);
CP_Image* GetCardSpriteByIndex(int index);
CARDEVENT* GetEventByIndex(int index);

// Resources
bool IsCostPayable(int costAmt);


// UI VFX
void SpawnLinearVfx(int spriteIndex, CP_Vector startPos, CP_Vector endPos, float lifetime, CP_Vector size, float spawnDelay);
void DrawAllLinearVfx();
void InitVfx();



















