#include <stdio.h>
#include <stdbool.h>
#include "cprocessing.h"
#include "TravessFunctions.h"
#include "grid.h"



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

CP_Vector SnapToGrid(float, float, CP_Vector);
CP_Vector WorldToGridPosition(float, float, CP_Vector);
CP_Vector GridToWorldPosition(float, float, CP_Vector);
void DrawUI(void);
void InitBuildings(void);
CP_Image GetBuildingSpriteByIndex(int);
int Get_current_gold();
int Get_current_food();
int Get_current_population();
void AddMarket();
void AddFarm();
void AddHouse();
void GenerateResourcesOnEndTurn();




void DrawAllTiles(void)
{
    CP_Vector newTile;
    for (int j = 0; j < WORLDGRIDY; ++j)
    {
        for (int i = 0; i < WORLDGRIDX; ++i)
        {
            switch (worldGrid[i][j])
            {
            case 0:
                break;

            case 1:
                newTile = GridToWorldPosition((float)i, (float)j, worldSpaceOrigin);
                CP_Image_Draw(GetBuildingSpriteByIndex(1), newTile.x, newTile.y, TILEWIDTH, TILEHEIGHT, 255);
                break;

            case 2:
                newTile = GridToWorldPosition((float)i, (float)j, worldSpaceOrigin);
                CP_Image_Draw(GetBuildingSpriteByIndex(2), newTile.x, newTile.y, TILEWIDTH, TILEHEIGHT, 255);
                break;

            case 3:
                newTile = GridToWorldPosition((float)i, (float)j, worldSpaceOrigin);
                CP_Image_Draw(GetBuildingSpriteByIndex(3), newTile.x, newTile.y, TILEWIDTH, TILEHEIGHT, 255);
                break;
            case 4:
                newTile = GridToWorldPosition((float)i, (float)j, worldSpaceOrigin);
                CP_Image_Draw(GetBuildingSpriteByIndex(4), newTile.x, newTile.y, TILEWIDTH, TILEHEIGHT, 255);
                break;
            case 5:
                newTile = GridToWorldPosition((float)i, (float)j, worldSpaceOrigin);
                CP_Image_Draw(GetBuildingSpriteByIndex(5), newTile.x, newTile.y, TILEWIDTH, TILEHEIGHT, 255);
                break;
            }
        }
    }
}

void DrawCursorTile(void)
{
    cursorTile = SnapToGrid(newMousePos.x, newMousePos.y, worldSpaceOrigin);
    CP_Image_Draw(grasstile, cursorTile.x, cursorTile.y, TILEWIDTH, TILEHEIGHT, 255);
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
            worldGrid[i][j] = 5;
        }
    }
}

void MouseDrag(void)
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
        CP_Vector newTileGridPos = WorldToGridPosition(cursorTile.x, cursorTile.y, worldSpaceOrigin);
        worldGrid[(int)(newTileGridPos.x)][(int)(newTileGridPos.y)] = 1;
    }
    // end of drag
    if (CP_Input_MouseReleased(0) && mouseDrag)
    {
        mouseDrag = false;
    }
}

void game_init(void)
{    
    CP_System_SetWindowSize(900, 600);
    InitBuildings();
    windowsWidth = (float)CP_System_GetWindowWidth();
    windowsHeight = (float)CP_System_GetWindowHeight();
    worldSpaceOrigin.x = windowsWidth / 2 - TILEWIDTH * 9.5f;
    worldSpaceOrigin.y = windowsHeight / 2 - TILEHEIGHT * 9.5f;
    


    CreateBasicPlatform();
}

void game_update(void)
{
    UpdateMouseInput();
    MouseDrag();
    CheckKeyInput();

    CP_Graphics_ClearBackground(CP_Color_Create(150, 150, 150, 255));
    DrawAllTiles();
    DrawCursorTile();

    //DrawUI();
    CP_Settings_TextSize(20);
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
    char buffer[100];
    sprintf_s(buffer, 100, "Gold: %d\t\tFood: %d\t\tPopulation: %d", Get_current_gold(), Get_current_food(), Get_current_population());
    CP_Font_DrawText(buffer, 20, 20);
}

void game_exit(void)
{

}
