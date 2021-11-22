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
bool AllowAdminControl = false;
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
int rewardCardsLeft[NUMBER_OF_MAX_REWARDS];
int rewardIndex = 0;

float endTurnTimer;


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
    if (Get_current_food() <= loseCondition_FoodValue || Get_current_population() <= loseCondition_PopulationValue) {
        return true;
    }
    return false;
}

void EndTurn() 
{
    GenerateResourcesOnEndTurn();

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
                if (GetCardsLeft() == 0)
                {
                    GameEnd();
                    break;
                }
                else
                {
                    AnimTimer = 0.6f;
                    gameState = State_CardDraw;
                }
            }
            break;
        case State_CardDraw:
            
            break;
        case State_MakeAChoice:
            // made a choice
            switch (ClickCheck_CardChoice())
            {
            case 1:
                // clicked on optionA
                rewardIndex = 0; // Reset Reward Index Before Event Begin
                ApplyEventResult(selectedEvent->resourceChangeA);
                for (int index = 0; index < NUMBER_OF_MAX_REWARDS; ++index)
                {
                    selectedReward[index] = GetRewardByIndex(selectedEvent->resourceRewardA[index].rewardIndex);
                    rewardCardsLeft[index] = selectedEvent->resourceRewardA[index].rewardAmount;
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
                break;
            case 2:
                // clicked on optionB
                ApplyEventResult(selectedEvent->resourceChangeB);
                for (int index = 0; index < NUMBER_OF_MAX_REWARDS; ++index)
                {
                    selectedReward[index] = GetRewardByIndex(selectedEvent->resourceRewardB[index].rewardIndex);
                    rewardCardsLeft[index] = selectedEvent->resourceRewardB[index].rewardAmount;
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
                break;
            }
            break;
        case State_CollectRewards:
            switch (ClickCheck_Rewards())
            {
            case 1:
                // reward is construction
               
                if (rewardCardsLeft[rewardIndex] > 0) 
                {
                    SetCurrentBuilding(GetBuildingByIndex(selectedReward[rewardIndex]->eventIndex));
                    --rewardCardsLeft[rewardIndex];
                    gameState = State_PlaceYourBuilding;
                }
                else if (rewardCardsLeft[rewardIndex] < 0) 
                {
                    SetCurrentBuilding(GetBuildingByIndex(selectedReward[rewardIndex]->eventIndex));
                    ++rewardCardsLeft[rewardIndex];
                    gameState = State_DestroyBuilding;
                }
                    
                break;
            case 2:
                // reward is ongoing?
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
    case State_PlaceYourBuilding:
        DrawCursorTile(currentMousePos);
        break;
    case State_DestroyBuilding:

        DestroyBuildingBySelectedBuilding(); //Destroy 1 building by the building chosen in choice

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
        
        //if (rewardCardsLeft[rewardIndex])
        //{
        //    gameState = State_CollectRewards;
        //}
        //// there is no more rewards
        //else
        //{
        //    ++rewardIndex;
        //    if (rewardIndex != NUMBER_OF_MAX_REWARDS) {
        //        gameState = State_CollectRewards;
        //    }
        //    else {
        //        rewardIndex = 0; //Reset Reward Index
        //        gameState = State_EndOfTurn;

        //    }
        //}

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
                        SpawnLinearVfx(4,tempVector, CP_Vector_Set(tempVector.x, tempVector.y - TILEHEIGHT / 3), 1, CP_Vector_Set(128, 128), animCount * animDelay);
                        ++animCount;
                        break;
                    case B_FARM_INDEX:
                        tempVector.x = i * TILEWIDTH + TILEWIDTH / 2 + worldOrigin.x;
                        tempVector.y = j * TILEHEIGHT + TILEHEIGHT / 3 + worldOrigin.y;
                        SpawnLinearVfx(3, tempVector, CP_Vector_Set(tempVector.x, tempVector.y - TILEHEIGHT / 3), 1, CP_Vector_Set(128, 128), animCount * animDelay);
                        ++animCount;
                        break;
                    case B_MARKET_INDEX:
                        tempVector.x = i * TILEWIDTH + TILEWIDTH / 2 + worldOrigin.x;
                        tempVector.y = j * TILEHEIGHT + TILEHEIGHT / 3 + worldOrigin.y;
                        SpawnLinearVfx(1, tempVector, CP_Vector_Set(tempVector.x, tempVector.y - TILEHEIGHT / 3), 1, CP_Vector_Set(128, 128), animCount * animDelay);
                        ++animCount;
                        break;
                    case B_TAVERN_INDEX:
                        tempVector.x = i * TILEWIDTH + TILEWIDTH / 2 + worldOrigin.x;
                        tempVector.y = j * TILEHEIGHT + TILEHEIGHT / 3 + worldOrigin.y;
                        SpawnLinearVfx(5, tempVector, CP_Vector_Set(tempVector.x, tempVector.y - TILEHEIGHT / 3), 1, CP_Vector_Set(128, 128), animCount * animDelay);
                        ++animCount;
                        break;
                    default:
                        break;
                    }
                }
            }
            endTurnTimer = animCount * animDelay;
        }
        else
        {
            endTurnTimer -= CP_System_GetDt();
            if (endTurnTimer <= 0)
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
    gameState = State_GameEntry;
    InitResources(100,10,0,0);
    InitWorldSpaceGrid();
    InitBuildings();
    InitNpc();
    InitSpritesheets();
    InitDecks();
    InitUI();
    InitVfx();
}

void MainGame_Update(void)
{
    UpdateMouseInput();
    MouseDragOrClick();
    CheckKeyInput();
    // Graphics
    DrawTileSet();
    UpdateAllNpc();
    DrawBuildings();
    DrawAllLinearVfx();
    DrawUI(gameState);
    GameStateControl();
    DrawTempTextResources();
    DrawAllAnimations();

}