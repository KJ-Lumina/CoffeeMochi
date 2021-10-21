#include <stdio.h>
#include "cprocessing.h"


#define WORLDGRIDX 40
#define WORLDGRIDY 40
#define TILEWIDTH 64.0f
#define TILEHEIGHT 64.0f

#pragma region Grid Functions
CP_Vector SnapToGrid(float x, float y, CP_Vector worldSpaceOrigin)
{
    x -= worldSpaceOrigin.x;
    y -= worldSpaceOrigin.y;
  
    // Snap to box grid
    float tilePosX = (int)(x / TILEWIDTH) * TILEWIDTH;
    float tilePosY = (int)(y / TILEHEIGHT) * TILEHEIGHT;

    tilePosX += worldSpaceOrigin.x + TILEWIDTH / 2;
    tilePosY += worldSpaceOrigin.y + TILEHEIGHT / 2;

    return CP_Vector_Set(tilePosX, tilePosY);
}

CP_Vector WorldToGridPosition(float x, float y, CP_Vector worldSpaceOrigin)
{
    x -= worldSpaceOrigin.x + TILEWIDTH / 2;
    y -= worldSpaceOrigin.y + TILEHEIGHT / 2;

    x /= TILEWIDTH;
    y /= TILEHEIGHT;

    return CP_Vector_Set(x, y);
}

CP_Vector GridToWorldPosition(float x, float y, CP_Vector worldSpaceOrigin)
{
    x *= TILEWIDTH;
    y *= TILEHEIGHT;

    x += worldSpaceOrigin.x + TILEWIDTH / 2;
    y += worldSpaceOrigin.y + TILEHEIGHT / 2;;

    return CP_Vector_Set(x, y);
}

