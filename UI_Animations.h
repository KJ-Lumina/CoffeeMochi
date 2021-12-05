/*!_____________________________________________________________________________
@file       UI_Animations.h
@author     Lee Xin Qian (xinqian.lee@digipen.edu)
@co-authors Travess Tan (travesscheekhang.t@digipen.edu)
@course     CSD1120
@section    B
@team       CoffeeMochi
@brief      This file contains the prototype function definitions that are used in
            animation sequences of Impero.

            Copyright ©2021 DigiPen, All rights reserved.
*//*__________________________________________________________________________*/

#include <stdio.h>
#include <stdbool.h>
#include "cprocessing.h"

SPRITESHEET GetSpriteAnimationByIndex(int index);
CP_Image GetSpriteSheetByIndex(int index);
void InitSpritesheets(void);
void DrawAnimation(float x, float y, float scaleX, float scaleY, float delay, int index);

enum SpriteAnimationIndex
{
    TILESET_NULL,
    TILESET_TESTENEMY,
    TILESET_STARS,
    TILESET_STARFALL
};

typedef struct
{
    int spriteIndex;
    int movementIndex;
    CP_Vector startPos;
    CP_Vector endPos;
    float curlifetime;
    float lifetime;
    CP_Vector size;
    float spawnDelay;
}LINEARVFX;

float EaseInQuad(float start, float end, float value);