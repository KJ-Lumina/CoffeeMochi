/*!_____________________________________________________________________________
@file       Common_Headers.h
@author     Travess Tan (travesscheekhang.t@digipen.edu)
@co-authors Lee Xin Qian (xinqian.lee@digipen.edu)
			Seow Kai Jun (s.kaijun@digipen.edu)
@course     CSD1120
@section    B
@team       CoffeeMochi
@brief      This file contains the variable & function definition for all the different menus in Impero

			Copyright ©2021 DigiPen, All rights reserved.
*//*__________________________________________________________________________*/

#include <stdio.h>
#include <stdbool.h>
#include "cprocessing.h"

#define WORLDGRIDX 5
#define WORLDGRIDY 5
#define TILEWIDTH 128.0f
#define TILEHEIGHT 128.0f
#define TILESPRITEWIDTH 128.0f
#define TILESPRITEHEIGHT 160.0f
#define MAPOFFSETX 325
#define MAPOFFSETY 25

#define MAXTILECOUNT (WORLDGRIDX * WORLDGRIDY)

#define BLUE_PILL 1;
#define RED_PILL 2;

//Building Related Definitions
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


#define E_INCREASE_RESOURCE 1
#define E_DECREASE_RESOURCE 2
#define E_DESTROY_BUILDING 3

#define O_RATEVENT 9


#define NULL_TYPE_EVENT 0
#define BUILD_TYPE_EVENT 1
#define RESOURCE_TYPE_EVENT 2
#define DESTROY_TYPE_EVENT 3
#define ONGOING_TYPE_EVENT 4
#define GOLDEN_TYPE_EVENT 5
#define TEXT_TYPE_EVENT 6
#define EVENT_TYPE_REWARD 7


//Resources
// Building costs for every tile are defaulted to the following values
#define MARKET_BUILD_COST 20
#define FARM_BUILD_COST 20
#define HOUSING_BUILD_COST 20
#define TAVERN_BUILD_COST 20

//buttons names in-progress
#define START_GAME 0


typedef enum 
{
	SCENE_SPLASH_DIGIPEN,
	SCENE_SPLASH_COFFEEMOCHI,
	SCENE_MAINMENU,
	SCENE_CREDITS,
	SCENE_HOWTOPLAY,
	SCENE_OPTIONS,
	SCENE_GAMEENTRY,
	SCENE_GAMEENTRY_EXIT,
	SCENE_RESTART,
	SCENE_GAMEPHASE,
	SCENE_GAMEOPTIONS,
	SCENE_ENDENTRY,
	SCENE_ENDPHASE,
}GAMESCENE;

typedef enum
{
	State_GameEntry,
	State_StartOfTurn,
	State_Idle,
	State_CardDraw,
	State_MakeAChoice,
	State_ResourceChange,
	State_CollectRewards,
	State_PlaceYourBuilding,
	State_DestroyBuilding,
	State_EndOfTurn,
	State_GameOver,
	State_End

}GAMESTATE;

typedef enum {

	Sound_SFX_CardHover,
	Sound_SFX_Card,
	Sound_SFX_Choice_LHS,
	Sound_SFX_Choice_RHS,
	Sound_SFX_Click,
	Sound_SFX_EarthQuake,
	Sound_SFX_Burning,
	Sound_SFX_Peddler,
	Sound_SFX_Rat,
	Sound_SFX_Thunder,
	Sound_SFX_Gold,
	Sound_SFX_House,
	Sound_SFX_Farm,
	Sound_SFX_Tavern,
	Sound_SFX_Market,

}SOUND_SFX;

typedef enum {

	Sound_BGM_Home,
	Sound_BGM_Game,
	Sound_BGM_Lose,
	Sound_BGM_Win

}SOUND_BGM;

typedef struct
{
	int rewardIndex;
	int rewardAmount;

}REWARD;



typedef struct
{
	int eventIndex; // unique ID
	int eventType; //
	char* title;
	char* description;
	int resourceChangeA[4];
	REWARD resourceRewardA[NUMBER_OF_MAX_REWARDS];
	char* descriptionA;
	char* resultDescA;
	int resourceChangeB[4];
	REWARD resourceRewardB[NUMBER_OF_MAX_REWARDS];
	char* descriptionB;
	char* resultDescB;
	int affectedLand[25];
}CARDEVENT;


typedef struct
{
	int eventIndex;
	int cardType;
	int resourceType;
	char* rewardDesc;
	char* collectedDesc;
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
	int setNextSprite;
	float minX; //parameters for recurring sprites x-axis
	float maxX; //parameters for recurring sprites x-axis
	float minY;
	float maxY;
	int maxSprites; //number of sprites in a spritesheet
	float spriteSizeX; //pixels on a sprite grid x-axis
	float spriteSizeY; //pixels on a sprite grid y-axis
	float scaleX;
	float scaleY;
	int index;
	int isInfiniteLoop;
	float posX;
	float posY;
	float endPosX;
	float endPosY;
	float timeElapse;
	float timeToDeath; //time drawn on screen before it disappears
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
void InitResources(int startingGold, int startingFood, int startingPopulation, int startingMorale);
void GameOver();

// WorldGridSpace
CP_Vector GetWorldSpaceOrigin();
void MoveWorldSpaceOrigin(float positionChangeX, float positionChangeY);
void GridToWorldPosition(CP_Vector*);
void DrawGridIndicator(CP_Vector cursorPosition);
void DrawCursorTile(CP_Vector cursorPos);
void SetNewBuilding(int xPos, int yPos, int buildingIndex);
void SetBuildingType(BUILDING* newBuilding);
bool AttemptPlaceBuilding(CP_Vector cursorPos);
int GetOccupiedIndex(int x, int y);
bool IsTileOccupied(CP_Vector);
int GetAllBuildingsPositionByIndex(int index, TILEPOSITION position[]);
void DestroyBuildingBySelectedBuilding(int buildingIndex);
bool AreAllOccupied();

// UI_Mechanics
bool CheckWithinBounds(CP_Vector position, float width, float height);
bool ClickCheck_CardDraw();
int ClickCheck_CardChoice();
int ClickCheck_Rewards();
void DrawUI(GAMESTATE state);
void DrawUI_TextDesc(const char* text); 
void DrawUI_Title(const char* text);
void DrawTempTextResources();
void UI_SetReward(REWARDCARD* rewardCard, int cardsLeft);
void UI_SetEvent(CARDEVENT*);
void UI_SetBlessingsTimer(float timer);
void UI_SetGoldCard();
void UI_SetNormalCard();
void DrawUI_Settings();

//Sound
void InitSound();
void SetVolume(float vol);
float GetVolume();
void PlayBGM(SOUND_BGM sound_BGM);
void Play_SFX_Sound(SOUND_SFX sound_sfx);
void Play_Building_SFX(int buildingIndex);
void Play_Choice_SFX(SOUND_SFX sound_sfx);
void Play_Event_SFX(int eventIndex);
void StopBGM();
void StopSFX();


// Resources
void UpdateResources();
void Set_current_gold(int gold);
void Set_current_food(int food);
void Set_current_population(int population);
void Set_additional_morale(int addMorale);
int Get_current_blessing();
int Get_current_blessing();
int Get_current_gold();
int Get_current_food();
int Get_current_population();
int Get_current_morale();
int Get_additional_morale();
int market_gold_prediction();
int farm_food_prediction();
int tavern_morale_preduction();
void AddNewResourceBuilding(int buildingIndex);
void AddMarket();
void AddFarm();
void AddHouse();
void AddTavern();
void SubtractMarket();
void SubtractFarm();
void SubtractHouse();
void SubtractTavern();
void ResetBlessing();
void GenerateResourcesOnEndTurn();
bool LoseCondition_Resources();
bool IsCostPayable(int costAmt);
void ApplyEventResourceAnim(int resourceChange[4]);
void ApplyEventResourceChange(int resourceChange[4]);
void IncreaseGold(int amount);
void IncreaseFood(int amount);
void IncreasePop(int amount);
void IncreaseMorale(int amount);
void IncreaseBlessing(int amount);
int GetDelayedGold();
int GetDelayedFood();
int GetDelayedPop();
int GetDelayedMorale();
int GetDelayedBlessing();
void SpawnGoldGainAnimation(int amount, CP_Vector startPos, CP_Vector checkpoint, CP_Vector endPos, float lifeTime, float spawnDelay);
void SpawnFoodGainAnimation(int amount, CP_Vector startPos, CP_Vector checkpoint, CP_Vector endPos, float lifeTime, float spawnDelay);
void SpawnMoraleGainAnimation(int amount, CP_Vector startPos, CP_Vector checkpoint, CP_Vector endPos, float lifeTime, float spawnDelay);
void SpawnBlessingGainAnimation(int amount, CP_Vector startPos, CP_Vector checkpoint, CP_Vector endPos, float lifeTime, float spawnDelay);

// Card Events
int GetCardsLeft();
CARDEVENT* GetNextEvent();
CARDEVENT* GetCurrentEvent();
BUILDING* GetBuildingByIndex(int);
REWARDCARD* GetRewardByIndex(int index);
CP_Image* GetBuildingSpriteByIndex(int);
CP_Image* GetBuildingSpriteButtonByIndex(int);
CP_Image* GetCardSpriteByType(int type);
CARDEVENT* GetEventByIndex(int index);
void EventSetGoldenCard();
void RollForcedCard(int cardIndex, int chance);
void RandomizeDeck();

// NPC 
void SpawnNpc(CP_Vector position, int amtToSpawn);

// Ongoing Events
void InitOngoingEvents();
void GenerateEvents(int eventIndex, int xPos, int yPos, int turnsAlive);// generate the event of rat|| any ongoing events 
void DrawOngoingEvents();
void OnEndUpdateEvents();//end turn, turn reduce by 1 
bool CheckCurrent(int typeofeffect, int x, int y);

// UI VFX
void SpawnLinearVfx(int spriteIndex, CP_Vector startPos, CP_Vector endPos, float lifetime, CP_Vector size, float spawnDelay);
void SpawnVfxEaseInToEaseOut(int spriteIndex, CP_Vector startPos, CP_Vector checkpoint, CP_Vector endPos, float lifetime, CP_Vector size, float spawnDelay);
void DrawAllVfx();
void InitVfx();

//Menu CallBacks
void SetGameSceneEndPhase(int isWin);
void OpenOptions();
void RestartGame();
 
//graphics Animspawn
void ConstantAnimSpawner(int counterIndex, int index, float time, int lowerX, int upperX, int lowerY, int upperY, float scaleX, float scaleY, float timeToDeath, int isTimeVariance, int isLerp);
void SpawnAnimation(float x, float y, float endx, float endy, float scaleX, float scaleY, int index, float timeToDeath, int isInfinteLoop);
void InitSpritesheets(void);
void DrawAllAnimations(void);

float EaseInQuad(float start, float end, float value);
float EaseInSine(float start, float end, float value);
float EaseOutSine(float start, float end, float value);



















