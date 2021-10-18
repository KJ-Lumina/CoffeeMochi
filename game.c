#include <stdio.h>
#include <stdbool.h>
#include "cprocessing.h"
#include "TravessFunctions.h"

#define WORLDGRIDX 40
#define WORLDGRIDY 40

float windowsWidth;
float windowsHeight;
CP_Vector worldSpaceOrigin;
float tileWidth = 64;
float tileHeight = 64;

CP_Vector cursorTile;
CP_Vector tilePos;
CP_Vector lastMousePos;
CP_Vector newMousePos;
bool mouseDrag = false;
int worldGrid[WORLDGRIDX][WORLDGRIDY] = { 0 };

CP_Image grasstile;
CP_Image housetile;
CP_Image wheattile;
CP_Image treetile;
CP_Image tdgrasstile;

void UpdateMousePosition()
{
    newMousePos.x = CP_Input_GetMouseX();
    newMousePos.y = CP_Input_GetMouseY();
}

CP_Vector SnapToGrid(float x, float y)
{
    x -= worldSpaceOrigin.x;
    y -= worldSpaceOrigin.y;

    // Snap to box grid
    float tilePosX = (int)(x / tileWidth) * tileWidth;
    float tilePosY = (int)(y / tileHeight) * tileHeight;

    // Snap to grid
    //x = (x - tilePosX) / tileWidth;
    //y = (y - tilePosY) / tileHeight;

    // value below is the grid position of the tile
    //printf("%f %f \n", tilePosX * 2 / tileWidth, tilePosY * 2 / tileHeight);

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


void DrawAllTiles()
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

void DrawCursorTile()
{
    cursorTile = SnapToGrid(newMousePos.x, newMousePos.y);
    CP_Vector gridPos = WorldPositionToGridPosition(cursorTile.x, cursorTile.y);
    //printf("%f,%f\n", gridPos.x, gridPos.y);
    CP_Image_Draw(grasstile, cursorTile.x, cursorTile.y, tileWidth, tileHeight, 255);
}

void ReturnToCenter()
{
    worldSpaceOrigin.x = windowsWidth / 2 - tileWidth * 10;
    worldSpaceOrigin.y = windowsHeight / 2 - tileHeight * 10;
}

void CheckPlayerInput()
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

void BasicPlatform()
{
    for (int i = 7; i < 12; ++i)
    {
        for (int j = 7; j < 12; ++j)
        {
            worldGrid[i][j] = 5;
        }
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

    BasicPlatform();
}

void game_update(void)
{
    CP_Graphics_ClearBackground(CP_Color_Create(150, 150, 150, 255));
    UpdateMousePosition();
    //placing tiles, currently permanent in testing

    
    if (CP_Input_MouseTriggered(0))
    {
        lastMousePos = newMousePos;
    }
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
    
    CheckPlayerInput();

    CP_Graphics_DrawRect(worldSpaceOrigin.x + WORLDGRIDX / 4 * tileWidth - tileWidth, worldSpaceOrigin.y + WORLDGRIDX / 4 * tileHeight - tileHeight, tileWidth, tileHeight);
    DrawAllTiles();
    DrawCursorTile();
    //printf("%f", SnapToGrid(CP_Input_GetMouseX(), CP_Input_GetMouseY()).x);
    
    
}

void game_exit(void)
{

}