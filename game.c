#include <stdio.h>
#include <stdbool.h>
#include "game.h"
#include "cprocessing.h"
#include "TravessFunctions.h"
#include "WorldSpaceGrid.h"
#include "UI_mechanics.h"



GAMESTATE gameState = State_Idle;
#pragma region Game Options Control
bool AllowMouseDrag = true;
bool AllowTileSet = true;
#pragma endregion



#pragma region  Grid Variables

CP_Vector cursorTile;
CP_Vector cursorGrid;
BUILDING cursorBuilding;
CARDEVENTS currentEvent;
CP_Vector currentMousePos;

bool mouseDrag = false;
CP_Vector mouseDragPos;

#pragma endregion

#pragma region Sprite/Images Declaration
CP_Image grasstile;
CP_Image housetile;
CP_Image wheattile;
CP_Image treetile;
CP_Image tdgrasstile;

#pragma endregion


#pragma region Win & Lose Variable Declaration
int loseCondition_FoodValue;
#pragma endregion



GAMESTATE GetGameState()
{
    return gameState;
}


void DrawCursorTile(void)
{
    CP_Image_Draw(GetBuildingSpriteByIndex(cursorBuilding.spriteIndex), cursorTile.x, cursorTile.y, TILEWIDTH, TILEHEIGHT, 255);
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
    cursorTile = currentMousePos;
    ScreenToWorldPosition(&cursorTile);
    cursorGrid = cursorTile;
    WorldToGridPosition(&cursorGrid);
    //mouseDrag purpose
    if (CP_Input_MouseTriggered(0))
    {
        mouseDragPos = currentMousePos;
    }
}

void AddNewResourceBuilding(int buildingIndex)
{
    switch (buildingIndex)
    {
    case 2:
        AddHouse();
        break;
    case 3:
        AddFarm();
        break;
    case 4:
        AddMarket();
        break;
    }
}

void AdminControlInput()
{
    if (CP_Input_KeyDown(KEY_1))
    {
        if (!IsTileOccupied(cursorTile))
        {
            SetNewBuilding((int)cursorGrid.x, (int)cursorGrid.y, 1);
        }
    }
    else if (CP_Input_KeyDown(KEY_2))
    {
        if (!IsTileOccupied(cursorTile))
        {
            SetNewBuilding((int)cursorGrid.x, (int)cursorGrid.y, 2);
            AddHouse();
        }
    }
    else if (CP_Input_KeyDown(KEY_3))
    {
        if (!IsTileOccupied(cursorTile))
        {
            SetNewBuilding((int)cursorGrid.x, (int)cursorGrid.y, 3);
            AddFarm();
        }
    }
    else if (CP_Input_KeyDown(KEY_4))
    {
        if (!IsTileOccupied(cursorTile))
        {
            SetNewBuilding((int)cursorGrid.x, (int)cursorGrid.y, 4);
            AddMarket();
        }
    }
    else if (CP_Input_KeyDown(KEY_5))
    {
        if (!IsTileOccupied(cursorTile))
        {
            SetNewBuilding((int)cursorGrid.x, (int)cursorGrid.y, 5);
        }
    }
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

CARDEVENTS GetCurrentEvent()
{
    return currentEvent;
}

void MouseClick()
{
    if (gameState == State_PlaceYourBuilding)
    {
        if (!IsTileOccupied(cursorTile))
        {
            SetNewBuilding((int)cursorGrid.x, (int)cursorGrid.y, cursorBuilding.spriteIndex);
            AddNewResourceBuilding(cursorBuilding.spriteIndex);
            EndTurn();
            gameState = State_Idle;
        }
    }
    else
    {
        switch (CheckUIClick(mouseDragPos.x, mouseDragPos.y))
        {
        case 1:
            gameState = State_MakeAChoice;
            currentEvent = GetBasicEvent();
            UI_SetEvent(currentEvent);
            break;
        case 2:
            gameState = State_PlaceYourBuilding;
            cursorBuilding = GetBuildingByIndex(currentEvent.indexOptionA);
            break;
        case 3:
            gameState = State_PlaceYourBuilding;
            cursorBuilding = GetBuildingByIndex(currentEvent.indexOptionB);
            break;
        }
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
    CP_System_SetWindowSize(1600, 900);
    InitWorldSpaceGrid();
    InitBuildings();
    InitSpritesheets();
    InitDeck();
    InitUI();
}

void game_update(void)
{
    UpdateMouseInput();
    MouseDragOrClick();
    CheckKeyInput();

    CP_Graphics_ClearBackground(CP_Color_Create(150, 150, 150, 255));

    if (AllowTileSet) DrawTileSet();
    else DrawAllTiles();

    if (gameState == State_PlaceYourBuilding)
    {
        DrawCursorTile();
    }

    DrawUI();
    DrawTempTextResources();
}

void game_exit(void)
{

}
