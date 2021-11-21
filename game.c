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
float MouseCounter = 0;
#pragma endregion

GAMESTATE gameState;
CP_Image game_Background;
CP_Vector currentMousePos;
CP_Vector mouseDragPos;
int loseCondition_FoodValue = 0;
int loseCondition_PopulationValue = 0;

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

//End Game Functions
void GameEnd() {
    gameState = State_GameOver;
    printf("The game has ended.");
}

void GameOver() 
{
    //Lose UI Pop Up
    gameState = State_GameOver;
    printf("You lose. Ran out of ****");
}

void EndTurn() 
{
    GenerateResourcesOnEndTurn();

    if (LoseCondition_Resources()) {
        GameOver();
    }
}

bool LoseCondition_Resources() {
    if (Get_current_food() <= loseCondition_FoodValue || Get_current_population() <= loseCondition_PopulationValue) {
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
   // if (MouseCounter > 0) return;

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
                    if (isTutorial) {
                        SwapDeckToMain(isTutorial);
                        isTutorial = false;
                    }
                    else {
                        GameEnd();
                        gameState = State_EndOfTurn;
                        break;
                    }

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
                    printf("reward exist");
                    printf(" reward number: %d ", rewardCardsLeft[rewardIndex]);
                    UI_SetReward(selectedReward[rewardIndex], rewardCardsLeft[rewardIndex]);
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
                    printf("reward exist");
                    printf(" reward number: %d ", rewardCardsLeft[rewardIndex]);
                    UI_SetReward(selectedReward[rewardIndex], rewardCardsLeft[rewardIndex]);
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
            printf(" reward number 2: %d ", rewardCardsLeft[rewardIndex]);
            switch (ClickCheck_Rewards())
            {
            case 1:
                // reward is construction
          
                if (rewardCardsLeft[rewardIndex] > 0) {
                    printf("Place building");
                    SetCurrentBuilding(GetBuildingByIndex(selectedReward[rewardIndex]->eventIndex));
                    --rewardCardsLeft[rewardIndex];
                    UI_SetReward(selectedReward[rewardIndex], rewardCardsLeft[rewardIndex]);
                    gameState = State_PlaceYourBuilding;
                }
                else if (rewardCardsLeft[rewardIndex] < 0) {
                    printf("Destroy Building");
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
            
          

            break;

        case State_EndOfTurn:

            break;
    }
   // MouseCounter = 0.5f;
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
        case State_DestroyBuilding:

            DestroyBuildingBySelectedBuilding();

            //there is more building to destroy
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
                }
                else {
                    rewardIndex = 0; //Reset Reward Index
                    gameState = State_EndOfTurn;
                }
            }

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