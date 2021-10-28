#include <stdio.h>
#include <stdbool.h>
#include "cprocessing.h"
#include "WorldSpaceGrid.h"
#include "game.h"


float windowsWidth;
float windowsHeight;
int worldGrid[WORLDGRIDX][WORLDGRIDY] = { 0 };
CP_Vector worldSpaceOrigin;
CP_Vector tempTile;



TILEMAP tilemap_world = { "TilesetGrass", 3, 3 };
CP_Image tilemap;
float tilesetWidth = 128;
float tilesetHeight = 160;

int varX = 0;
int varY = 0;

void InitWorldSpaceGrid()
{
    windowsWidth = (float)CP_System_GetWindowWidth();
    windowsHeight = (float)CP_System_GetWindowHeight();
    ReturnToCenter();
    tilemap = CP_Image_Load("./Assets/TilesetGrass.png");
}
CP_Vector GetWorldSpaceOrigin()
{
    return worldSpaceOrigin;
}
void MoveWorldSpaceOrigin(float positionChangeX, float positionChangeY) 
{
    worldSpaceOrigin.x += positionChangeX;
    worldSpaceOrigin.y += positionChangeY;
}
CP_Vector ScreenToWorldPosition(CP_Vector position)
{
    position.x -= worldSpaceOrigin.x;
    position.y -= worldSpaceOrigin.y;

    // Snap to box grid
    float tilePosX = (int)(position.x / TILEWIDTH) * TILEWIDTH;
    float tilePosY = (int)(position.y / TILEHEIGHT) * TILEHEIGHT;

    tilePosX = Math_Clamp_Float(tilePosX, 0, (WORLDGRIDX - 1) * TILEWIDTH);
    tilePosY = Math_Clamp_Float(tilePosY, 0, (WORLDGRIDY - 1) * TILEWIDTH);

    tilePosX += worldSpaceOrigin.x + TILEWIDTH / 2;
    tilePosY += worldSpaceOrigin.y + TILEHEIGHT / 2;

    return CP_Vector_Set(tilePosX, tilePosY);
}
CP_Vector WorldToGridPosition(CP_Vector position)
{
    position.x -= worldSpaceOrigin.x + TILEWIDTH / 2;
    position.y -= worldSpaceOrigin.y + TILEHEIGHT / 2;

    position.x /= TILEWIDTH;
    position.y /= TILEHEIGHT;

    position.x = (float)Math_Clamp_Int((int)position.x, 0, WORLDGRIDX);
    position.y = (float)Math_Clamp_Int((int)position.y, 0, WORLDGRIDY);

    return CP_Vector_Set(position.x, position.y);
}
CP_Vector GridToWorldPosition(CP_Vector position)
{
    position.x *= TILEWIDTH;
    position.y *= TILEHEIGHT;

    position.x += worldSpaceOrigin.x + TILEWIDTH / 2;
    position.y += worldSpaceOrigin.y + TILEHEIGHT / 2;;

    return CP_Vector_Set(position.x, position.y);
}


void SetNewBuilding(int x, int y, int buildingIndex)
{
    worldGrid[x][y] = buildingIndex;
}

int GetOccupiedIndex(int x, int y)
{
    return worldGrid[x][y];
}

bool IsTileOccupied(CP_Vector position)
{
    if (worldGrid[(int)position.x][(int)position.y] == 1)
    {
        return false;
    }
    return true;
}

void ReturnToCenter()
{
    worldSpaceOrigin.x = windowsWidth / 2 - TILEWIDTH * WORLDGRIDX / 2 - 130;
    worldSpaceOrigin.y = windowsHeight / 2 - TILEHEIGHT * WORLDGRIDY / 2;
}

// traditional method on drawing tiles
void DrawAllTiles(void)
{
    for (int j = 0; j < WORLDGRIDY; ++j)
    {
        for (int i = 0; i < WORLDGRIDX; ++i)
        {
            tempTile.x = (float)i;
            tempTile.y = (float)j;
            switch (worldGrid[i][j])
            {
            case 0:
                break;

            case 1:
                tempTile = GridToWorldPosition(tempTile);
                CP_Image_Draw(GetBuildingSpriteByIndex(1), tempTile.x, tempTile.y, TILEWIDTH, TILEHEIGHT, 255);
                break;

            case 2:
                tempTile = GridToWorldPosition(tempTile);
                CP_Image_Draw(GetBuildingSpriteByIndex(2), tempTile.x, tempTile.y, TILEWIDTH, TILEHEIGHT, 255);
                break;

            case 3:
                tempTile = GridToWorldPosition(tempTile);
                CP_Image_Draw(GetBuildingSpriteByIndex(3), tempTile.x, tempTile.y, TILEWIDTH, TILEHEIGHT, 255);
                break;
            case 4:
                tempTile = GridToWorldPosition(tempTile);
                CP_Image_Draw(GetBuildingSpriteByIndex(4), tempTile.x, tempTile.y, TILEWIDTH, TILEHEIGHT, 255);
                break;
            case 5:
                tempTile = GridToWorldPosition(tempTile);
                CP_Image_Draw(GetBuildingSpriteByIndex(5), tempTile.x, tempTile.y, TILEWIDTH, TILEHEIGHT, 255);
                break;
            }
        }
    }
}

// Tileset method on drawing tiles
// valid tilenumbers are from 1 to 9
void DrawTileSet()
{
    for (int j = 0; j < WORLDGRIDY; ++j)
    {
        for (int i = 0; i < WORLDGRIDX; ++i)
        {
            tempTile.x = (float)i;
            tempTile.y = (float)j;
            tempTile = GridToWorldPosition(tempTile);

            switch (i)
            {
            case 0:
                varX = 0;
                break;
            case WORLDGRIDX - 1:
                varX = 2;
                break;
            default:
                varX = 1;
                break;
            }

            switch (j)
            {
            case 0:
                varY = 0;
                break;
            case WORLDGRIDY - 1:
                varY = 2;
                break;
            default:
                varY = 1;
                break;
            }

            CP_Image_DrawSubImage(tilemap, tempTile.x, tempTile.y, tilesetWidth, tilesetHeight, tilesetWidth * varX, tilesetHeight * varY, 
                tilesetWidth * (varX + 1), tilesetHeight * (varY + 1), 255);
        }
    }
}
