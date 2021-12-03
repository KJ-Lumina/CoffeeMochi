#include <stdio.h>
#include <stdbool.h>
#include "cprocessing.h"
#include "WorldSpaceGrid.h"
#include "Common_Headers.h"
#include "Buildings.h"


float windowsWidth;
float windowsHeight;

int buildingGrid[WORLDGRIDX][WORLDGRIDY] = { 0 };
CP_Vector worldSpaceOrigin;
CP_Vector tempTile;
CP_Vector cursorTile;
CP_Image gridIndicator;
CP_Image gridIndicatorgreen;
CP_Image gridIndicatorred;

BUILDING* selectedBuilding;

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
    gridIndicator = CP_Image_Load("./Assets/GridIndicator.png");
    gridIndicatorgreen = CP_Image_Load("./Assets/GridIndicatorgreen.png");
    gridIndicatorred = CP_Image_Load("./Assets/GridIndicatorred.png");
    for (int i = 0; i < WORLDGRIDX; ++i)
    {
        for (int j = 0; j < WORLDGRIDY; ++j)
        {
            buildingGrid[i][j] = 0;
        }
    }
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
bool IsWithinGrid(CP_Vector position)
{
    float xPos = position.x - worldSpaceOrigin.x;
    float yPos = position.y - worldSpaceOrigin.y;
    if (xPos < 0 || yPos < 0 || xPos > WORLDGRIDX * TILEWIDTH || yPos > WORLDGRIDY * TILEHEIGHT)
    {
        return false;
    }
    return true;
}
void ScreenToWorldPosition(CP_Vector* position)
{
    position->x -= worldSpaceOrigin.x;
    position->y -= worldSpaceOrigin.y;
    position->x = CP_Math_ClampInt((int)(position->x / TILEWIDTH), 0, WORLDGRIDX - 1) * TILEWIDTH;
    position->y = CP_Math_ClampInt((int)(position->y / TILEHEIGHT), 0, WORLDGRIDY - 1) * TILEHEIGHT;
    position->x += worldSpaceOrigin.x + TILEWIDTH / 2;
    position->y += worldSpaceOrigin.y + TILEHEIGHT / 2;
}
void ScreenToGridPosition(CP_Vector* position)
{
    position->x -= worldSpaceOrigin.x;
    position->y -= worldSpaceOrigin.y;

    // Snap to box grid
    
    position->x = (float)CP_Math_ClampInt((int)(position->x / TILEWIDTH), 0, WORLDGRIDX - 1);
    position->y = (float)CP_Math_ClampInt((int)(position->y / TILEHEIGHT), 0, WORLDGRIDY - 1);
}

void WorldToGridPosition(CP_Vector* position)
{
    position->x -= worldSpaceOrigin.x + TILEWIDTH / 2;
    position->y -= worldSpaceOrigin.y + TILEHEIGHT / 2;

    position->x /= TILEWIDTH;
    position->y /= TILEHEIGHT;

    position->x = (float)CP_Math_ClampInt((int)position->x, 0, WORLDGRIDX - 1);
    position->y = (float)CP_Math_ClampInt((int)position->y, 0, WORLDGRIDY - 1);
}
void GridToWorldPosition(CP_Vector* position)
{
    position->x *= TILEWIDTH;
    position->y *= TILEHEIGHT;

    position->x += worldSpaceOrigin.x + TILEWIDTH / 2;
    position->y += worldSpaceOrigin.y + TILEHEIGHT / 2;;

}

float CalculateUnitsToBorder(CP_Vector position, CP_Vector directionUnit)
{
    float dirUnitsToXBorder = 0;
    float dirUnitsToYBorder = 0;
    if (directionUnit.x > 0)
    {
        dirUnitsToXBorder = (WORLDGRIDX * TILEWIDTH - position.x) / directionUnit.x;
    }
    else
    {
        dirUnitsToXBorder = position.x / Math_Abs(directionUnit.x);
    }
    if (directionUnit.y > 0)
    {
        dirUnitsToYBorder = (WORLDGRIDY * TILEHEIGHT - position.y) / directionUnit.y;
    }
    else
    {
        dirUnitsToYBorder = position.y / Math_Abs(directionUnit.y);
    }
    return dirUnitsToXBorder > dirUnitsToYBorder ? dirUnitsToYBorder : dirUnitsToXBorder;
}

void DestroyBuildingBySelectedBuilding(int buildingIndex) {

    TILEPOSITION tile_positions[MAXTILECOUNT];
    int length = GetAllBuildingsPositionByIndex(buildingIndex, tile_positions);

    switch (buildingIndex) {
    case B_HOUSE_INDEX:
        SubtractHouse();
        break;

    case B_FARM_INDEX:
        SubtractFarm();
        break;

    case B_MARKET_INDEX:
        SubtractMarket();
        break;

    case B_TAVERN_INDEX:
        SubtractTavern();
        break;
    }

    if (length > 0) {
        unsigned int lowerBounds = 0;
        unsigned int upperBounds = length - 1;

        unsigned int randIndex = CP_Random_RangeInt(lowerBounds, upperBounds);

        SetNewBuilding((tile_positions + randIndex)->positionX, (tile_positions + randIndex)->positionY, B_EMPTY_INDEX); //Replace it with nothing (Empty Sqaure)
    }
}

void SetNewBuilding(int x, int y, int buildingIndex)
{
    buildingGrid[x][y] = buildingIndex;
}
void SetBuildingType(BUILDING* newBuilding)
{
    selectedBuilding = newBuilding;
}

int GetOccupiedIndex(int x, int y)
{
    return buildingGrid[x][y];
}

bool IsTileOccupied(CP_Vector position)
{
    if (buildingGrid[(int)position.x][(int)position.y] == 0)
    {
        return false;
    }
    return true;
}

bool AttemptPlaceBuilding(CP_Vector cursorPosition)
{
    if (IsWithinGrid(cursorPosition))
    {
        ScreenToGridPosition(&cursorPosition);
        if (!IsTileOccupied(cursorPosition))
        {
            Play_SFX_Sound(Sound_SFX_Click);
            SetNewBuilding((int)cursorPosition.x, (int)cursorPosition.y, selectedBuilding->spriteIndex);
            AddNewResourceBuilding(selectedBuilding->spriteIndex);
            return 1;
        }
    }
    return 0;
}




void ReturnToCenter()
{
    worldSpaceOrigin.x = windowsWidth / 2 - TILEWIDTH * WORLDGRIDX / 2 + MAPOFFSETX;
    worldSpaceOrigin.y = windowsHeight / 2 - TILEHEIGHT * WORLDGRIDY / 2 + MAPOFFSETY;
}

void DrawGridIndicator(CP_Vector cursorPosition)
{
    if (IsWithinGrid(cursorPosition))
    {
        ScreenToWorldPosition(&cursorPosition);
        CP_Image_Draw(gridIndicator, cursorPosition.x, cursorPosition.y, TILEWIDTH, TILEHEIGHT, 255);
    }
}

void DrawCursorTile(CP_Vector cursorPosition)
{
    if (IsWithinGrid(cursorPosition))
    {
        ScreenToGridPosition(&cursorPosition);
        if (IsTileOccupied(cursorPosition))
        {
            GridToWorldPosition(&cursorPosition);
            CP_Image_Draw(gridIndicatorred, cursorPosition.x, cursorPosition.y, TILEWIDTH, TILEHEIGHT, 255);
        }
        else
        {
            GridToWorldPosition(&cursorPosition);
            CP_Image_Draw(gridIndicatorgreen, cursorPosition.x, cursorPosition.y, TILEWIDTH, TILEHEIGHT, 255);
        }
        CP_Image_Draw(*GetBuildingSpriteByIndex(selectedBuilding->spriteIndex), cursorPosition.x, cursorPosition.y, TILESPRITEWIDTH, TILESPRITEHEIGHT, 255);
    }
    else
    {
        CP_Image_Draw(*GetBuildingSpriteByIndex(selectedBuilding->spriteIndex), cursorPosition.x, cursorPosition.y, TILESPRITEWIDTH, TILESPRITEHEIGHT, 255);
    }
}

int GetAllBuildingsPositionByIndex(int index, TILEPOSITION position[]) {

    int arrayIndex = 0;


    for (int j = 0; j < WORLDGRIDY; ++j)
    {
        for (int i = 0; i < WORLDGRIDX; ++i)
        {
            if (buildingGrid[i][j] == index) {

                position[arrayIndex].positionY = j;
                position[arrayIndex].positionX = i;
                ++arrayIndex;

            }
        }
    }

    return arrayIndex;

}

// Draw all structures
void DrawBuildings()
{
    for (int j = 0; j < WORLDGRIDY; ++j)
    {
        for (int i = 0; i < WORLDGRIDX; ++i)
        {
            tempTile.x = (float)i;
            tempTile.y = (float)j;
            switch (buildingGrid[i][j])
            {
            case 0:
                break;

            case 1:
                GridToWorldPosition(&tempTile);
                CP_Image_Draw(*GetBuildingSpriteByIndex(1), tempTile.x, tempTile.y, TILESPRITEWIDTH, TILESPRITEHEIGHT, 255);
                break;

            case 2:
                GridToWorldPosition(&tempTile);
                CP_Image_Draw(*GetBuildingSpriteByIndex(2), tempTile.x, tempTile.y, TILESPRITEWIDTH, TILESPRITEHEIGHT, 255);
                break;

            case 3:
                GridToWorldPosition(&tempTile);
                CP_Image_Draw(*GetBuildingSpriteByIndex(3), tempTile.x, tempTile.y, TILESPRITEWIDTH, TILESPRITEHEIGHT, 255);
                break;
            case 4:
                GridToWorldPosition(&tempTile);
                CP_Image_Draw(*GetBuildingSpriteByIndex(4), tempTile.x, tempTile.y, TILESPRITEWIDTH, TILESPRITEHEIGHT, 255);
                break;
            case 5:
                GridToWorldPosition(&tempTile);
                CP_Image_Draw(*GetBuildingSpriteByIndex(5), tempTile.x, tempTile.y, TILESPRITEWIDTH, TILESPRITEHEIGHT, 255);
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
            GridToWorldPosition(&tempTile);

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

            CP_Image_DrawSubImage(tilemap, tempTile.x, tempTile.y + 16, tilesetWidth, tilesetHeight, tilesetWidth * varX, tilesetHeight * varY, 
                tilesetWidth * (varX + 1), tilesetHeight * (varY + 1), 255);
        }
    }
}
