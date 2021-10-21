#include <stdio.h>
#include <stdbool.h>
#include "cprocessing.h"
#include "TravessFunctions.h"
#include "Resource_Stats.h"
#include "MainSystem.h"

#define WORLDGRIDX 40
#define WORLDGRIDY 40

#pragma region World Variables Declaration
int turnNumber = 1;
#pragma endregion

#pragma region  Grid Variables
int worldGrid[WORLDGRIDX][WORLDGRIDY] = { 0 };
CP_Vector worldSpaceOrigin;
float windowsWidth;
float windowsHeight;
float tileWidth = 64;
float tileHeight = 64;

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

#pragma region Resources Variables Declaration
int current_gold;
int current_food;
int current_population;
int max_population = 100;
int current_morale;

//Gold Related Variables
int num_of_markets = 0;
int num_of_merchant_citizen = 0;
float bankrupt_debuff = 0.75f;
bool is_bankrupt = false;

//Food Related Variables
int num_of_farms = 0;
int num_of_farmer_citizen = 0;
float starving_debuff = 0.75f;
bool is_starving = false;

//Population Related Variables
int num_of_housing = 0;
float overpopulation_debuff_rate = 0.0f;
bool is_overpopulated = false;

// Function for unhappiness_factor not implemented yet
int unhappiness_factor = 0;
#pragma endregion

#pragma region Win & Lose Variable Declaration
int loseCondition_FoodValue;
#pragma endregion


void DrawUI(void);



#pragma region Grid Functions
CP_Vector SnapToGrid(float x, float y)
{
    x -= worldSpaceOrigin.x;
    y -= worldSpaceOrigin.y;

    // Snap to box grid
    float tilePosX = (int)(x / tileWidth) * tileWidth;
    float tilePosY = (int)(y / tileHeight) * tileHeight;

    tilePosX += worldSpaceOrigin.x + tileWidth / 2;
    tilePosY += worldSpaceOrigin.y + tileHeight / 2;

    return CP_Vector_Set(tilePosX, tilePosY);
}

CP_Vector WorldPositionToGridPosition(float x, float y)
{
    x -= worldSpaceOrigin.x + tileWidth / 2;
    y -= worldSpaceOrigin.y + tileHeight / 2;

    x /= tileWidth;
    y /= tileHeight;

    return CP_Vector_Set(x, y);
}

CP_Vector GridPositionToWorldPosition(float x, float y)
{
    x *= tileWidth;
    y *= tileHeight;

    x += worldSpaceOrigin.x + tileWidth / 2;
    y += worldSpaceOrigin.y + tileHeight / 2;;

    return CP_Vector_Set(x, y);
}

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
                newTile = GridPositionToWorldPosition((float)i, (float)j);
                CP_Image_Draw(grasstile, newTile.x, newTile.y, tileWidth, tileHeight, 255);
                break;

            case 2:
                newTile = GridPositionToWorldPosition((float)i, (float)j);
                CP_Image_Draw(housetile, newTile.x, newTile.y, tileWidth, tileHeight, 255);
                break;

            case 3:
                newTile = GridPositionToWorldPosition((float)i, (float)j);
                CP_Image_Draw(wheattile, newTile.x, newTile.y, tileWidth, tileHeight, 255);
                break;
            case 4:
                newTile = GridPositionToWorldPosition((float)i, (float)j);
                CP_Image_Draw(treetile, newTile.x, newTile.y, tileWidth, tileHeight, 255);
                break;
            case 5:
                newTile = GridPositionToWorldPosition((float)i, (float)j);
                CP_Image_Draw(tdgrasstile, newTile.x, newTile.y, tileWidth, tileHeight, 255);
                break;
            }
        }
    }
}

void DrawCursorTile(void)
{
    cursorTile = SnapToGrid(newMousePos.x, newMousePos.y);
    CP_Image_Draw(grasstile, cursorTile.x, cursorTile.y, tileWidth, tileHeight, 255);
}

#pragma endregion

#pragma region Resource Functions
void UpdateResourceAmount(void) {
    if (current_gold <= 0)
        current_gold = 0;

    if (current_food <= 0)
        current_food = 0;
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
    //Updates and Check for Triggers
    is_bankrupt = gold_generated_per_turn(current_gold, current_population, num_of_markets, num_of_merchant_citizen, num_of_farms, num_of_housing);
    is_starving = food_generated_per_turn(current_food, current_population, num_of_farms, num_of_farmer_citizen);
    is_overpopulated = check_for_overpopulation(current_population, max_population);

    //Check for Game Over Trigger
    if (current_food <= loseCondition_FoodValue)
        GameOver();
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
    worldSpaceOrigin.x = windowsWidth / 2 - tileWidth * 10;
    worldSpaceOrigin.y = windowsHeight / 2 - tileHeight * 10;
}

void CheckKeyInput(void)
{
    if (CP_Input_KeyDown(KEY_F))
    {
        ReturnToCenter();
    }
    if (CP_Input_KeyDown(KEY_1))
    {
        CP_Vector newTileGridPos = WorldPositionToGridPosition(cursorTile.x, cursorTile.y);
        worldGrid[(int)(newTileGridPos.x)][(int)(newTileGridPos.y)] = 1;
    }
    else if (CP_Input_KeyDown(KEY_2))
    {
        CP_Vector newTileGridPos = WorldPositionToGridPosition(cursorTile.x, cursorTile.y);
        worldGrid[(int)(newTileGridPos.x)][(int)(newTileGridPos.y)] = 2;
    }
    else if (CP_Input_KeyDown(KEY_3))
    {
        CP_Vector newTileGridPos = WorldPositionToGridPosition(cursorTile.x, cursorTile.y);
        worldGrid[(int)(newTileGridPos.x)][(int)(newTileGridPos.y)] = 3;
    }
    else if (CP_Input_KeyDown(KEY_4))
    {
        CP_Vector newTileGridPos = WorldPositionToGridPosition(cursorTile.x, cursorTile.y);
        worldGrid[(int)(newTileGridPos.x)][(int)(newTileGridPos.y)] = 4;
    }
    else if (CP_Input_KeyDown(KEY_5))
    {
        CP_Vector newTileGridPos = WorldPositionToGridPosition(cursorTile.x, cursorTile.y);
        worldGrid[(int)(newTileGridPos.x)][(int)(newTileGridPos.y)] = 5;
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
        CP_Vector newTileGridPos = WorldPositionToGridPosition(cursorTile.x, cursorTile.y);
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
    windowsWidth = (float)CP_System_GetWindowWidth();
    windowsHeight = (float)CP_System_GetWindowHeight();
    worldSpaceOrigin.x = windowsWidth / 2 - tileWidth * 9.5f;
    worldSpaceOrigin.y = windowsHeight / 2 - tileHeight * 9.5f;
    
    grasstile = CP_Image_Load("./Assets/grasstile.png");
    housetile = CP_Image_Load("./Assets/housetile.png");
    wheattile = CP_Image_Load("./Assets/wheattile.png");
    treetile = CP_Image_Load("./Assets/treetile.png");
    tdgrasstile = CP_Image_Load("./Assets/TDgrasstile.png");

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
}

void game_exit(void)
{

}
