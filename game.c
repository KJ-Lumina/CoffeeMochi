#include <stdio.h>
#include <stdbool.h>
#include "Common_Headers.h"
#include "cprocessing.h"
#include "TravessFunctions.h"
#include "WorldSpaceGrid.h"
#include "UI_mechanics.h"
#include "Npc.h"



GAMESTATE gameState = State_Idle;
GAMEPHASE gamePhase = PHASE_EVENTLOOP; //Suppose to start with Build
#pragma region Game Options Control
bool AllowMouseDrag = true;
#pragma endregion

CP_Vector currentMousePos;
CP_Vector mouseDragPos;
bool mouseDrag = false;


#pragma region Win & Lose Variable Declaration
int loseCondition_FoodValue;
int loseCondition_PopulationValue;
#pragma endregion


#define ONEVENTCARDCLICK 1


GAMESTATE GetGameState()
{
    return gameState;
}

#pragma region Turn & Win Lose Functions
//Trigger Turn Start Functions Calls
void StartTurn(void) 
{

}

//Trigger Turn End Functions Calls
void GameOver(void) 
{
    //Lose UI Pop Up
    printf("The game has ended!");
    gameState = State_GameOver;
}

void EndTurn(void) 
{
    GenerateResourcesOnEndTurn();
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

void UpdateMouseInput(void)
{
    currentMousePos.x = CP_Input_GetMouseX();
    currentMousePos.y = CP_Input_GetMouseY();
    
    //mouseDrag purpose
    if (CP_Input_MouseTriggered(0))
    {
        mouseDragPos = currentMousePos;
    }
}

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
    /*
    if (CP_Input_KeyDown(KEY_1))
    {
        if (!IsTileOccupied(cursorTile))
        {
            SetNewBuilding((int)selectedGrid.x, (int)selectedGrid.y, 1);
        }
    }
    */
}


void CheckKeyInput(void)
{
    if (CP_Input_KeyTriggered(KEY_F))
    {
        ReturnToCenter();
    }
    if (CP_Input_KeyTriggered(KEY_SPACE))
    {
        EndTurn();
    }
    
    // PLAYTESTING
    AdminControlInput();
}

void MouseClick()
{
    if (gamePhase == PHASE_BUILDPHASE) { //For Starting Build Phase (2 Farms, 2 House, 2 Market)



    }else if (gamePhase == PHASE_EVENTLOOP) {
        switch (gameState)
        {
        case State_StartOfTurn:
            //Run Condition on Start of Turn
            gameState = State_Idle;
            break;

        case State_Idle:
            if (CheckUIClick(currentMousePos.x, currentMousePos.y) == 1)
            {
                gameState = State_MakeAChoice;
                UI_SetEvent(GetNextEvent());
            }
            break;
        case State_MakeAChoice:
            switch (CheckUIClick(currentMousePos.x, currentMousePos.y))
            {
            case 1:
                gameState = State_PlaceYourBuilding;
                break;
            case 2:
                gameState = State_EndOfTurn;
                break;
            }
            break;
        case State_PlaceYourBuilding:

            if (AttemptPlaceBuilding(currentMousePos))
            {
                gameState = State_EndOfTurn;
            }
            break;

        case State_EndOfTurn:

            break;
        }

       
    }
}

void GameStateControl() 
{
    DrawGridIndicator(currentMousePos);
    switch (gameState)
    {
    case State_StartOfTurn:
        //Run Condition on Start of Turn
        gameState = State_Idle;
        break;

    case State_Idle:

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
    else
    {
        MouseClick();
    }
    
}

void game_init(void)
{    
    CP_System_ShowConsole();

    CP_System_SetWindowSize(1600, 900);
    InitResources(100);
    InitWorldSpaceGrid();
    InitBuildings();
    InitNpc();
    InitSpritesheets();
    InitDecks();
    InitUI();
}

void game_update(void)
{
    UpdateMouseInput();
    MouseDragOrClick();
    CheckKeyInput();

    CP_Graphics_ClearBackground(CP_Color_Create(150, 150, 150, 255));
    DrawTileSet();
    DrawBuildings();
    UpdateAllNpc();
    GameStateControl();
    DrawUI();
    DrawTempTextResources();

    DrawAllAnimations();

}

void game_exit(void)
{

}
