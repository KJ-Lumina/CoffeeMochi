#include <stdio.h>
#include <stdbool.h>
#include "Common_Headers.h"
#include "cprocessing.h"
#include "TravessFunctions.h"
#include "WorldSpaceGrid.h"
#include "UI_mechanics.h"



GAMESTATE gameState = State_Idle;
#pragma region Game Options Control
bool AllowMouseDrag = true;
#pragma endregion

CP_Vector currentMousePos;
CP_Vector mouseDragPos;
bool mouseDrag = false;


#pragma region Win & Lose Variable Declaration
int loseCondition_FoodValue;
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
    switch (gameState)
    {
    case State_Idle:
        if (CheckUIClick(currentMousePos.x, currentMousePos.y) == 1)
        {
            gameState = State_MakeAChoice;
            UI_SetEvent(GetNextEvent());
        }
        break;
    case State_MakeAChoice:
        //make specific functions for ui
        printf("lolnew");
        if (CheckUIClick(currentMousePos.x, currentMousePos.y) == 1)
        {
            gameState = State_PlaceYourBuilding;
        }
        break;
    case State_PlaceYourBuilding:     

        if (AttemptPlaceBuilding(currentMousePos))
        {
            EndTurn();
            gameState = State_Idle;
        }
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
    InitWorldSpaceGrid();
    InitBuildings();
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
    switch (gameState)
    {
    case State_PlaceYourBuilding:
        DrawCursorTile(currentMousePos);
        break;
    }
    DrawUI();
    DrawTempTextResources();
}

void game_exit(void)
{

}
