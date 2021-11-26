#include <stdio.h>
#include <stdbool.h>
#include "cprocessing.h"



SPRITESHEET GetSpriteAnimationByIndex(int index);
CP_Image GetSpriteSheetByIndex(int index);
void InitSpritesheets(void);
void DrawAnimation(float x, float y, float scaleX, float scaleY, float delay, int index);


//have to add more in for more spritesheets
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