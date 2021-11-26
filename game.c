#include <stdio.h>
#include <stdbool.h>
#include "Common_Headers.h"
#include "cprocessing.h"
#include "TravessFunctions.h"
#include "WorldSpaceGrid.h"
#include "UI_mechanics.h"
#include "UI_Animations.h"
#include "Npc.h"



#pragma region Game Options Control
bool AllowAdminControl = true;
bool AllowMouseDrag = false;
bool mouseDrag = false;
float MouseCounter = 0;
#pragma endregion

GAMESTATE gameState;
CP_Vector currentMousePos;
CP_Vector mouseDragPos;
int loseCondition_FoodValue = 0;
int loseCondition_PopulationValue = 0;

//bool isTutorial = true;

float AnimTimer = 1;
CARDEVENT* selectedEvent;
REWARDCARD* selectedReward[NUMBER_OF_MAX_REWARDS];
int selectedChoice;
int rewardCardsLeft[NUMBER_OF_MAX_REWARDS];
int rewardIndex = 0;

float endTurnTimer = 0;


#pragma region Turn & Win Lose Functions
//Trigger Turn Start Functions Calls
void StartTurn() 
{

}

//End Game Functions
void GameEnd() {
    gameState = State_GameOver;
}

void GameOver() 
{
    //Lose UI Pop Up
    gameState = State_GameOver;
}

bool LoseCondition_Resources()
{
    if (Get_current_food() <= loseCondition_FoodValue || Get_current_population() <= loseCondition_PopulationValue || Get_current_gold() < 0 || Get_current_morale() < 0) {
        return true;
    }
    return false;
}

void EndTurn() 
{
    GenerateResourcesOnEndTurn();
    OnEndUpdateEvents();

    if (LoseCondition_Resources()) {
        GameOver();
    }
    else {
        gameState = State_StartOfTurn;
    }
}
#pragma endregion

void UpdateMouseInput()
{
    currentMousePos.x = CP_Input_GetMouseX();
    currentMousePos.y = CP_Input_GetMouseY();
    
    //mouseDrag purpose
    if (CP_Input_MouseTriggered(0))
    {
        mouseDragPos = currentMousePos;
    }
}

//Debug instant commands
void AdminControlInput()
{
    if (CP_Input_KeyTriggered(KEY_Q))
    {
        CP_Vector spawnPoint = currentMousePos;
        ScreenToWorldPosition(&spawnPoint);
        SpawnNpc(spawnPoint, 1);
    }
    //testing animations
    if (CP_Input_KeyTriggered(KEY_W))
    {
        SpawnAnimation(currentMousePos.x, currentMousePos.y, 200, 200, 1, 0.5f, 1);
    }

    if (CP_Input_KeyTriggered(KEY_E)) 
    {
        SpawnVfxEaseInToEaseOut(1, currentMousePos, CP_Vector_Set(CP_Random_RangeFloat(-50, 50) + currentMousePos.x, CP_Random_RangeFloat(-50, 50) + currentMousePos.y), CP_Vector_Set(50, 50), 1, CP_Vector_Set(128, 128), 0);
    }

    if (CP_Input_KeyTriggered(KEY_1))
    {
        int moregold[4] = { 10,0,0,0 };
        ApplyEventResourceChange(moregold);
    }
    if (CP_Input_KeyTriggered(KEY_2))
    {
        int morefood[4] = { 0,10,0,0 };
        ApplyEventResourceChange(morefood);
    }
    if (CP_Input_KeyTriggered(KEY_3))
    {
        int morepop[4] = { 0,0,10,0 };
        ApplyEventResourceChange(morepop);
    }
    if (CP_Input_KeyTriggered(KEY_4))
    {
        int moremoral[4] = { 0,0,0,10 };
        ApplyEventResourceChange(moremoral);
    }
    if (CP_Input_KeyTriggered(KEY_5))
    {
        int moregold[4] = { -10,0,0,0 };
        ApplyEventResourceChange(moregold);
    }
    if (CP_Input_KeyTriggered(KEY_6))
    {
        int morefood[4] = { 0,-10,0,0 };
        ApplyEventResourceChange(morefood);
    }
    if (CP_Input_KeyTriggered(KEY_7))
    {
        int morepop[4] = { 0,0,-10,0 };
        ApplyEventResourceChange(morepop);
    }
    if (CP_Input_KeyTriggered(KEY_8))
    {
        int moremoral[4] = { 0,0,0,-10 };
        ApplyEventResourceChange(moremoral);
    }
    if (CP_Input_KeyTriggered(KEY_R))
    {
        RestartGame();
    }
}
void CheckKeyInput()
{
    if (CP_Input_KeyTriggered(KEY_F))
    {
        ReturnToCenter();
    }
    // Debugging
    if (AllowAdminControl)
    {
        AdminControlInput();
    }
}
void MouseClick()
{
    switch (gameState)
    {
        case State_StartOfTurn:
            //Run Condition on Start of Turn
            gameState = State_Idle;
            break;
        case State_Idle:
            if (ClickCheck_CardDraw())
            {
                AnimTimer = 0.6f;
                gameState = State_CardDraw;
            }
            break;
        case State_CardDraw:
            
            break;
        case State_MakeAChoice:
            // made a choice
            switch (ClickCheck_CardChoice())
            {
            case 1:
                selectedChoice = 1;
                // clicked on optionA
                rewardIndex = 0; // Reset Reward Index Before Event Begin
                ApplyEventResourceAnim(selectedEvent->resourceChangeA);
                for (int index = 0; index < NUMBER_OF_MAX_REWARDS; ++index)
                {
                    selectedReward[index] = GetRewardByIndex(selectedEvent->resourceRewardA[index].rewardIndex);
                    rewardCardsLeft[index] = selectedEvent->resourceRewardA[index].rewardAmount;
                }
                //AnimTimer = 1.3f;
                gameState = State_ResourceChange;
                break;
            case 2:
                selectedChoice = 2;
                // clicked on optionB
                rewardIndex = 0; // Reset Reward Index Before Event Begin
                ApplyEventResourceAnim(selectedEvent->resourceChangeB);
                for (int index = 0; index < NUMBER_OF_MAX_REWARDS; ++index)
                {
                    selectedReward[index] = GetRewardByIndex(selectedEvent->resourceRewardB[index].rewardIndex);
                    rewardCardsLeft[index] = selectedEvent->resourceRewardB[index].rewardAmount;
                }
                //AnimTimer = 1.3f;
                gameState = State_ResourceChange;
                break;
            }
            break;
        case State_CollectRewards:
            switch (ClickCheck_Rewards())
            {
            case BUILD_TYPE_EVENT:
                // reward is construction
                SetBuildingType(GetBuildingByIndex(selectedReward[rewardIndex]->eventIndex));
                --rewardCardsLeft[rewardIndex];
                gameState = State_PlaceYourBuilding;
                break;
            case DESTROY_TYPE_EVENT:
                SetBuildingType(GetBuildingByIndex(selectedReward[rewardIndex]->eventIndex));
                --rewardCardsLeft[rewardIndex];
                gameState = State_DestroyBuilding;
                break;
            case ONGOING_TYPE_EVENT:
                while (selectedEvent->affectedLand[rewardIndex] != 0)
                {
                    int gridPos = selectedEvent->affectedLand[rewardIndex];
                    if (GetOccupiedIndex((gridPos - 1) % WORLDGRIDY, (gridPos - 1) / WORLDGRIDX) == selectedReward[0]->resourceType)
                    {
                        GenerateEvents(O_RATEVENT, (gridPos - 1) % WORLDGRIDY, (gridPos - 1) / WORLDGRIDX);
                    }
                    rewardIndex++;
                }
                gameState = State_EndOfTurn;
                break;
            }
            break;
        case State_PlaceYourBuilding:
            if (AttemptPlaceBuilding(currentMousePos))
            {
                switch(rewardIndex) //Prevention for going out of bounds
                {
                case 0: //Check Reward Index = 0
                    if (rewardCardsLeft[rewardIndex] == 0) //Once first reward is empty, goes down to 2nd array slot in the Selected Reward
                    {
                        if (rewardCardsLeft[++rewardIndex] == 0) //Checks for 2nd Reward Index 
                        {
                            gameState = State_EndOfTurn;
                        }
                        else
                        {
                            gameState = State_CollectRewards;
                        }
                    }
                    else
                    {
                        gameState = State_CollectRewards;
                    }
                    break;
                case 1: //Check Reward Index = 1 
                    if (rewardCardsLeft[rewardIndex] == 0)
                    {
                        gameState = State_EndOfTurn;
                    }
                    else
                    {
                        gameState = State_CollectRewards;
                    }
                    break;
                default:
                    gameState = State_EndOfTurn;
                    break;
                }

            }
            break;
        case State_DestroyBuilding:
            
            break;

        case State_EndOfTurn:

            break;
    }
}
void GameStateControl() 
{
    DrawGridIndicator(currentMousePos);
    switch (gameState)
    {
    case State_GameEntry:
        gameState = State_Idle;
        break;
    case State_StartOfTurn:
        gameState = State_Idle;
        break;
    case State_Idle:
        break;
    case State_CardDraw:
        AnimTimer -= CP_System_GetDt();
        if (AnimTimer <= 0)
        {
            gameState = State_MakeAChoice;
            selectedEvent = GetNextEvent();
            UI_SetEvent(selectedEvent);
        }
        break;
    case State_MakeAChoice:
        break;
    case State_ResourceChange:
        AnimTimer -= CP_System_GetDt();
        if (AnimTimer <= 0)
        {
            switch (selectedChoice)
            {
            case 1:
                //ApplyEventResourceChange(selectedEvent->resourceChangeA);
                break;
            case 2:
                //ApplyEventResourceChange(selectedEvent->resourceChangeB);
                break;
            }
            if (rewardCardsLeft[rewardIndex])
            {
                // reward cards exist
                UI_SetReward(selectedReward[rewardIndex], rewardCardsLeft[0] + rewardCardsLeft[1]);
                gameState = State_CollectRewards;
            }
            else
            {
                // no reward cards
                gameState = State_EndOfTurn;
            }
        }
        break;
    case State_CollectRewards:
        break;
    case State_PlaceYourBuilding:
        DrawCursorTile(currentMousePos);
        break;
    case State_DestroyBuilding:
        DestroyBuildingBySelectedBuilding(selectedReward[rewardIndex]->resourceType); //Destroy 1 building by the building chosen in choice
        switch (rewardIndex) //Prevention for going out of bounds
        {
        case 0: //Check Reward Index = 0
            if (rewardCardsLeft[rewardIndex] == 0)
            {
                if (rewardCardsLeft[++rewardIndex] == 0) {
                    gameState = State_EndOfTurn;
                }
                else {
                    gameState = State_CollectRewards;
                }
            }
            else //If there is more buidling to destroy
            {
                gameState = State_CollectRewards;
            }
            break;

        case 1: //Check Reward Index = 1
            if (rewardCardsLeft[rewardIndex] == 0)
            {
                gameState = State_EndOfTurn;
            }
            else //If there is more building to destroy
            {
                gameState = State_CollectRewards;
            }
            break;

        default:
            gameState = State_EndOfTurn;
            break;
        }   
        break;
    case State_EndOfTurn:
        if (endTurnTimer <= 0)
        {
            rewardIndex = 0;  // Reset Reward Index Before Event Begin
            float animCount = 0;
            float animDelay = 0.1f;
            CP_Vector worldOrigin = GetWorldSpaceOrigin();
            CP_Vector tempVector;
            for (int j = 0; j < WORLDGRIDY; ++j)
            {
                for (int i = 0; i < WORLDGRIDX; ++i)
                {
                    switch (GetOccupiedIndex(i, j))
                    {
                    case B_HOUSE_INDEX:
                        tempVector.x = i * TILEWIDTH + TILEWIDTH / 2 + worldOrigin.x;
                        tempVector.y = j * TILEHEIGHT + TILEHEIGHT / 3 + worldOrigin.y;
                        if (CheckCurrent(B_HOUSE_INDEX, i, j))
                        {
                            // house broken, lose morale
                            SpawnMoraleGainAnimation(-1, tempVector, CP_Vector_Set(tempVector.x, tempVector.y - TILEHEIGHT / 3), CP_Vector_Set(520, 360), 0.6f, animCount * animDelay);
                            //SpawnVfxEaseInToEaseOut(6, tempVector, CP_Vector_Set(tempVector.x, tempVector.y - TILEHEIGHT / 3), CP_Vector_Set(50, 180), 0.6f, CP_Vector_Set(128, 128), animCount * animDelay);
                        }
                        // house consume food
                        SpawnFoodGainAnimation(-1, tempVector, CP_Vector_Set(tempVector.x, tempVector.y - TILEHEIGHT / 3), CP_Vector_Set(520, 180), 0.6f, animCount * animDelay);
                        ++animCount;
                        break;
                    case B_FARM_INDEX:
                        tempVector.x = i * TILEWIDTH + TILEWIDTH / 2 + worldOrigin.x;
                        tempVector.y = j * TILEHEIGHT + TILEHEIGHT / 3 + worldOrigin.y;
                        // farm broken, no food
                        if (CheckCurrent(B_FARM_INDEX, i, j))
                        {
                            //SpawnFoodGainAnimation(0, tempVector, CP_Vector_Set(tempVector.x, tempVector.y - TILEHEIGHT / 3), CP_Vector_Set(520, 180), 0.6f, animCount * animDelay);
                        }
                        else
                        {
                            // farm generate food
                            SpawnFoodGainAnimation(1, tempVector, CP_Vector_Set(tempVector.x, tempVector.y - TILEHEIGHT / 3), CP_Vector_Set(520, 180), 0.6f, animCount* animDelay);
                        }
                        ++animCount;
                        break;
                    case B_MARKET_INDEX:
                        tempVector.x = i * TILEWIDTH + TILEWIDTH / 2 + worldOrigin.x;
                        tempVector.y = j * TILEHEIGHT + TILEHEIGHT / 3 + worldOrigin.y;
                        // market broke, no gold
                        if (CheckCurrent(B_MARKET_INDEX, i, j))
                        {
                            
                        }
                        else
                        {
                            // market generate gold
                            SpawnGoldGainAnimation(1, tempVector, CP_Vector_Set(tempVector.x, tempVector.y - TILEHEIGHT / 3), CP_Vector_Set(520, 90), 0.6f, animCount* animDelay);
                        }
                        ++animCount;
                        break;
                    case B_TAVERN_INDEX:
                        tempVector.x = i * TILEWIDTH + TILEWIDTH / 2 + worldOrigin.x;
                        tempVector.y = j * TILEHEIGHT + TILEHEIGHT / 3 + worldOrigin.y;
                        // tavern broken, no morale
                        if (CheckCurrent(B_TAVERN_INDEX, i, j))
                        {

                        }
                        else
                        {
                            // tavern generate morale
                            SpawnMoraleGainAnimation(0, tempVector, CP_Vector_Set(tempVector.x, tempVector.y - TILEHEIGHT / 3), CP_Vector_Set(520, 360), 0.6f, animCount* animDelay);
                        }
                        ++animCount;
                        break;
                    default:
                        break;
                    }
                }
            }
            endTurnTimer = animCount * animDelay + 1.0f;
        }
        else
        {
            endTurnTimer -= CP_System_GetDt();
            if (GetCardsLeft() == 0)
            {
                GameEnd();
            }
            else if (endTurnTimer <= 0)
            {
                EndTurn();  //State Set to Start Turn is in EndTurn()       
            }
        }
        break;
    case State_GameOver:
        SetGameSceneEndPhase();
        break;
    }
}
void MouseDragOrClick(void)
{
    if (AllowMouseDrag)
    {
        if (CP_Input_MouseDragged(0))
        {
            mouseDrag = true;
            MoveWorldSpaceOrigin(currentMousePos.x - mouseDragPos.x, currentMousePos.y - mouseDragPos.y);
            mouseDragPos = currentMousePos;
        }
        // To deferentiate between drag and click
        if (CP_Input_MouseClicked() && !mouseDrag)
        {
            MouseClick();
        }
        // end of drag
        if (CP_Input_MouseReleased(0) && mouseDrag)
        {
            mouseDrag = false;
        }
    }
    else if (CP_Input_MouseClicked())
    {
        MouseClick();
    }
}

void MainGame_Initialize(void)
{
    //CP_System_FullscreenAdvanced(1600, 900); //Enable for full screen
    gameState = State_GameEntry;
    InitResources(100,40,0,50);
    InitWorldSpaceGrid();
    InitBuildings();
    InitNpc();
    InitSpritesheets();
    InitDecks();
    InitUI();
    InitVfx();
    InitOngoingEvents();
}

void MainGame_Update(void)
{
    UpdateResources();
    UpdateMouseInput();
    MouseDragOrClick();
    CheckKeyInput();
    // Graphics
    DrawTileSet();
    UpdateAllNpc();
    DrawUI(gameState);
    DrawBuildings();
    DrawOngoingEvents();
    GameStateControl();
    DrawTempTextResources();
    DrawAllLinearVfx();
    DrawAllAnimations();
}