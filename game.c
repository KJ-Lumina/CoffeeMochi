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
bool AllowMouseDrag = false;
bool mouseDrag = false;
#pragma endregion

GAMESTATE gameState;
CP_Image game_Background;
CP_Vector currentMousePos;
CP_Vector mouseDragPos;
int loseCondition_FoodValue;
int loseCondition_PopulationValue;

bool isTutorial = true;

float AnimTimer = 1;
CARDEVENT* selectedEvent;
REWARDCARD* selectedReward[NUMBER_OF_MAX_REWARDS];
int rewardCardsLeft[NUMBER_OF_MAX_REWARDS];
int rewardIndex = 0;


#pragma region Turn & Win Lose Functions
//Trigger Turn Start Functions Calls
void StartTurn() 
{

}

//Trigger Turn End Functions Calls
void GameOver() 
{
    //Lose UI Pop Up
    printf("The game has ended!");
}

void EndTurn() 
{
    GenerateResourcesOnEndTurn();

    if (gameState == State_GameOver) {
        GameOver();
    }
    /*if (LoseCondition_Resources())
        GameOver();*/
}

bool LoseCondition_Resources() {
    if (Get_current_food() <= loseCondition_FoodValue || Get_current_population() <= loseCondition_FoodValue) {
        return 1;
    }

    return 0;
}
#pragma endregion

void DrawBackground()
{
    CP_Image_Draw(game_Background, 800, 450, 1600, 900, 255);
}

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
}


void CheckKeyInput()
{
    if (CP_Input_KeyTriggered(KEY_F))
    {
        ReturnToCenter();
    }
    if (CP_Input_KeyTriggered(KEY_SPACE))
    {
        EndTurn();
    }
    
    // Debugging
    AdminControlInput();
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
                    SwapDeckToMain(isTutorial);
                    if (isTutorial == true) isTutorial = false;
                    //Check if End Game
                    gameState = State_MakeAChoice;
                    selectedEvent = GetNextEvent(isTutorial);
                    UI_SetEvent(selectedEvent);
                    
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
                ApplyEventResult(selectedEvent->resourceChangeA);
                for (int index = 0; index < NUMBER_OF_MAX_REWARDS; ++index)
                {
                    selectedReward[index] = GetRewardByIndex(selectedEvent->resourceRewardA[index].rewardIndex);
                    rewardCardsLeft[index] = selectedEvent->resourceRewardA[index].rewardAmount;
                }
                
                if (rewardCardsLeft[rewardIndex])
                {
                    // reward cards exist
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

            //Fail Safe Check
            if (rewardCardsLeft[rewardIndex] == 0) {
                rewardIndex = 0;
                gameState = State_EndOfTurn; 
            }

            UI_SetReward(selectedReward[rewardIndex], rewardCardsLeft[rewardIndex]);

            switch (ClickCheck_Rewards())
            {
            case 1:
                // reward is construction
          
                if (rewardCardsLeft[rewardIndex] > 0) {
                    SetCurrentBuilding(GetBuildingByIndex(selectedReward[rewardIndex]->eventIndex));
                    --rewardCardsLeft[rewardIndex];
                    UI_SetReward(selectedReward[rewardIndex], rewardCardsLeft[rewardIndex]);
                    gameState = State_PlaceYourBuilding;
                }
                else if (rewardCardsLeft[rewardIndex] < 0) {
                    SetCurrentBuilding(GetBuildingByIndex(selectedReward[rewardIndex]->eventIndex));
                    ++rewardCardsLeft[rewardIndex];
                    UI_SetReward(selectedReward[rewardIndex], rewardCardsLeft[rewardIndex]);
                    gameState = State_DestroyBuilding;
                }
                    
                break;
            case 2:
                // reward is ongoing?
                break;
            }
            break;
        case State_PlaceYourBuilding:

            printf("reward card left: %d" ,rewardCardsLeft[rewardIndex]);

            if (AttemptPlaceBuilding(currentMousePos))
            {
                // there is more rewards to collect
                if (rewardCardsLeft[rewardIndex])
                {
                    gameState = State_CollectRewards;
                }
                // there is no more rewards
                else
                {
                    ++rewardIndex;
                    if (rewardIndex != NUMBER_OF_MAX_REWARDS) {
                        gameState = State_CollectRewards;
                        MouseClick();
                        printf("Enter");
                    }
                    else {
                        rewardIndex = 0; //Reset Reward Index
                        gameState = State_EndOfTurn;
                    }
                   
                }
            }
            break;
        case State_DestroyBuilding:
            
            DestroyBuildingBySelectedBuilding();
            //there is more building to destroy
            if (rewardCardsLeft)
            {
                gameState = State_CollectRewards;
            }
            // there is no more rewards
            else
            {
                ++rewardIndex;
                if (rewardIndex != NUMBER_OF_MAX_REWARDS) {
                    gameState = State_CollectRewards;
                    MouseClick();
                }
                else {
                    rewardIndex = 0; //Reset Reward Index
                    gameState = State_EndOfTurn;
                }
            }

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
                UI_SetEvent(GetNextEvent(isTutorial));
            }
            break;
        case State_MakeAChoice:
            break;
        case State_PlaceYourBuilding:
            DrawCursorTile(currentMousePos);
            break;
        case State_EndOfTurn:
            EndTurn();
            gameState = State_StartOfTurn;
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
    CP_System_ShowConsole();
    gameState = State_Idle;
    game_Background = CP_Image_Load("./ImperoArtAssets/Impero_GameBG.png");
    InitResources(100);
    InitWorldSpaceGrid();
    InitBuildings();
    InitNpc();
    InitSpritesheets();
    InitDecks();
    InitUI();
}

void MainGame_Update(void)
{
    UpdateMouseInput();
    MouseDragOrClick();
    CheckKeyInput();
    // Graphics
    DrawBackground();
    DrawTileSet();
    DrawBuildings();
    UpdateAllNpc();
    DrawUI(gameState);
    GameStateControl();
    DrawTempTextResources();
    DrawAllAnimations();
}