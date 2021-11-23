#include <stdio.h>
#include <stdbool.h>
#include "cprocessing.h"



SPRITESHEET GetSpriteAnimationByIndex(int index);
CP_Image GetSpriteSheetByIndex(int index);
void InitSpritesheets(void);
void DrawAnimation(float x, float y, float scaleX, float scaleY, float delay, int index);
void SpawnAnimation(float x, float y, float scaleX, float scaleY, int index, float timeToDeath, int isInfinteLoop);

//have to add more in for more spritesheets
enum SpriteAnimationIndex
{
    TILESET_TESTENEMY = 1
    ,TILESET_STARS = 2 
};

typedef struct
{
    int spriteIndex;
    CP_Vector startPos;
    CP_Vector endPos;
    float curlifetime;
    float lifetime;
    CP_Vector size;
    float spawnDelay;
}LINEARVFX;
