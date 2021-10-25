#include <stdio.h>
#include "cprocessing.h"



#pragma region Grid Functions

CP_Vector SnapToGrid(float x, float y, CP_Vector worldSpaceOrigin)
{
    x -= worldSpaceOrigin.x;
    y -= worldSpaceOrigin.y;
  
    // Snap to box grid
    float tilePosX = (int)(x / TILEWIDTH) * TILEWIDTH;
    float tilePosY = (int)(y / TILEHEIGHT) * TILEHEIGHT;

    tilePosX = Math_Clamp_Float(tilePosX, 0, (WORLDGRIDX - 1) * TILEWIDTH);
    tilePosY = Math_Clamp_Float(tilePosY, 0, (WORLDGRIDX - 1) * TILEWIDTH);

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

    x = (float)Math_Clamp_Int((int)x, 0, WORLDGRIDX);
    y = (float)Math_Clamp_Int((int)y, 0, WORLDGRIDY);

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

