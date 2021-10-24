#include <stdio.h>
#include <stdbool.h>
#include "game.h"
#include "cprocessing.h"
#include "TravessFunctions.h"
#include "grid.h"
#include "UI_mechanics.h"



GAMESTATE gameState = State_Idle;

#pragma region World Variables Declaration
int turnNumber = 1;
#pragma endregion

#pragma region  Grid Variables
int worldGrid[WORLDGRIDX][WORLDGRIDY] = { 0 };
CP_Vector worldSpaceOrigin;
float windowsWidth;
float windowsHeight;
//float TILEWIDTH = 64;
//float TILEHEIGHT = 64;
CP_Vector cursorTile;
BUILDING cursorBuilding;
CARDEVENTS currentEvent;

CP_Vector lastMousePos;
CP_Vector newMousePos;
bool mouseDrag = false;

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


void DrawAllTiles(void)
{
    CP_Vector newTile;
    
    for (int j = 0; j < WORLDGRIDY; ++j)
    {
        for (int i = 0; i < WORLDGRIDX; ++i)
        {
            int tileNum = 1;

            newTile = GridToWorldPosition((float)i, (float)j, worldSpaceOrigin);
            DrawTile(tileNum, newTile.x, newTile.y);

            //switch (worldGrid[i][j])
            //{
            //case 0:
            //    break;

            //case 1:
            //    newTile = GridToWorldPosition((float)i, (float)j, worldSpaceOrigin);
            //    CP_Image_Draw(GetBuildingSpriteByIndex(1), newTile.x, newTile.y, TILEWIDTH, TILEHEIGHT, 255);
            //    break;

            //case 2:
            //    newTile = GridToWorldPosition((float)i, (float)j, worldSpaceOrigin);
            //    CP_Image_Draw(GetBuildingSpriteByIndex(2), newTile.x, newTile.y, TILEWIDTH, TILEHEIGHT, 255);
            //    break;

            //case 3:
            //    newTile = GridToWorldPosition((float)i, (float)j, worldSpaceOrigin);
            //    CP_Image_Draw(GetBuildingSpriteByIndex(3), newTile.x, newTile.y, TILEWIDTH, TILEHEIGHT, 255);
            //    break;
            //case 4:
            //    newTile = GridToWorldPosition((float)i, (float)j, worldSpaceOrigin);
            //    CP_Image_Draw(GetBuildingSpriteByIndex(4), newTile.x, newTile.y, TILEWIDTH, TILEHEIGHT, 255);
            //    break;
            //case 5:
            //    newTile = GridToWorldPosition((float)i, (float)j, worldSpaceOrigin);
            //    CP_Image_Draw(GetBuildingSpriteByIndex(5), newTile.x, newTile.y, TILEWIDTH, TILEHEIGHT, 255);
            //    break;
            //}

            
        }
    }
    DrawAnimation(500, 500, 200, 200, 0.25, TILESET_TESTENEMY);

}


void DrawCursorTile(void)
{
    cursorTile = SnapToGrid(newMousePos.x, newMousePos.y, worldSpaceOrigin);
    CP_Image_Draw(GetBuildingSpriteByIndex(cursorBuilding.spriteIndex), cursorTile.x, cursorTile.y, TILEWIDTH, TILEHEIGHT, 255);
}

#pragma endregion




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
    for (int j = 0; j < WORLDGRIDY; ++j)
    {
        for (int i = 0; i < WORLDGRIDX; ++i)
        {
            switch (worldGrid[i][j])
            {
            case 0:
                break;
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
            }
        }
    }
}
#pragma endregion

void UpdateMouseInput(void)
{
    newMousePos = CP_Vector_Set(CP_Input_GetMouseX(), CP_Input_GetMouseY());
    if (CP_Input_MouseTriggered(0))
    {
        lastMousePos = newMousePos;
    }
}

void ReturnToCenter(void)
{
    worldSpaceOrigin.x = windowsWidth / 2 - TILEWIDTH * 10;
    worldSpaceOrigin.y = windowsHeight / 2 - TILEHEIGHT * 10;
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
    if (CP_Input_KeyDown(KEY_1))
    {
        CP_Vector newTileGridPos = WorldToGridPosition(cursorTile.x, cursorTile.y, worldSpaceOrigin);
        if (worldGrid[(int)(newTileGridPos.x)][(int)(newTileGridPos.y)] == 0)
        {
            worldGrid[(int)(newTileGridPos.x)][(int)(newTileGridPos.y)] = 1;
        }
    }
    else if (CP_Input_KeyDown(KEY_2))
    {
        CP_Vector newTileGridPos = WorldToGridPosition(cursorTile.x, cursorTile.y, worldSpaceOrigin);
        if (worldGrid[(int)(newTileGridPos.x)][(int)(newTileGridPos.y)] == 0)
        {
            worldGrid[(int)(newTileGridPos.x)][(int)(newTileGridPos.y)] = 2;
            AddHouse();
        }
    }
    else if (CP_Input_KeyDown(KEY_3))
    {
        CP_Vector newTileGridPos = WorldToGridPosition(cursorTile.x, cursorTile.y, worldSpaceOrigin);
        if (worldGrid[(int)(newTileGridPos.x)][(int)(newTileGridPos.y)] == 0)
        {
            worldGrid[(int)(newTileGridPos.x)][(int)(newTileGridPos.y)] = 3;
            AddFarm();
        }
    }
    else if (CP_Input_KeyDown(KEY_4))
    {
        CP_Vector newTileGridPos = WorldToGridPosition(cursorTile.x, cursorTile.y, worldSpaceOrigin);
        if (worldGrid[(int)(newTileGridPos.x)][(int)(newTileGridPos.y)] == 0)
        {
            worldGrid[(int)(newTileGridPos.x)][(int)(newTileGridPos.y)] = 4;
            AddMarket();
        }
    }
    else if (CP_Input_KeyDown(KEY_5))
    {
        CP_Vector newTileGridPos = WorldToGridPosition(cursorTile.x, cursorTile.y, worldSpaceOrigin);
        if (worldGrid[(int)(newTileGridPos.x)][(int)(newTileGridPos.y)] == 0)
        {
            worldGrid[(int)(newTileGridPos.x)][(int)(newTileGridPos.y)] = 5;
        }
    }
}

void CreateBasicPlatform(void)
{
    for (int i = 7; i < 12; ++i)
    {
        for (int j = 7; j < 12; ++j)
        {
            worldGrid[i][j] = 1;
        }
    }
}

CARDEVENTS GetCurrentEvent()
{
    return currentEvent;
}

void MouseClick()
{
    if (gameState == State_PlaceYourBuilding)
    {
        CP_Vector newTileGridPos = WorldToGridPosition(cursorTile.x, cursorTile.y, worldSpaceOrigin);
        if (worldGrid[(int)(newTileGridPos.x)][(int)(newTileGridPos.y)] == 1)
        {
            worldGrid[(int)(newTileGridPos.x)][(int)(newTileGridPos.y)] = cursorBuilding.spriteIndex;
            AddNewResourceBuilding(cursorBuilding.spriteIndex);
            EndTurn();
            gameState = State_Idle;
        }
    }
    else
    {
        switch (CheckUIClick(lastMousePos.x, lastMousePos.y))
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
    if (CP_Input_MouseDragged(0))
    {
        mouseDrag = true;
        worldSpaceOrigin.x += newMousePos.x - lastMousePos.x;
        worldSpaceOrigin.y += newMousePos.y - lastMousePos.y;
        lastMousePos = newMousePos;
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

void game_init(void)
{    
    CP_System_SetWindowSize(1600, 900);
    InitBuildings();
    InitSpritesheets();
    InitDeck();
    InitUI();
    windowsWidth = (float)CP_System_GetWindowWidth();
    windowsHeight = (float)CP_System_GetWindowHeight();
    worldSpaceOrigin.x = windowsWidth / 2 - TILEWIDTH * 12.0f;
    worldSpaceOrigin.y = windowsHeight / 2 - TILEHEIGHT * 9.5f;
    


    CreateBasicPlatform();
}

void game_update(void)
{
    UpdateMouseInput();
    MouseDragOrClick();
    CheckKeyInput();

    CP_Graphics_ClearBackground(CP_Color_Create(150, 150, 150, 255));
    DrawAllTiles();
    if (gameState == State_PlaceYourBuilding)
    {
        DrawCursorTile();
    }
    DrawUI();
    CP_Settings_TextSize(20);
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
    char buffer[100];
    sprintf_s(buffer, 100, "Gold: %d\t\tFood: %d\t\tPopulation: %d", Get_current_gold(), Get_current_food(), Get_current_population());
    CP_Font_DrawText(buffer, 20, 20);
}

void game_exit(void)
{

}
