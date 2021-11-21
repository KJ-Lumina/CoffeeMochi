#include <stdio.h>
#include <stdbool.h>
#include "cprocessing.h"



SPRITESHEET GetSpriteAnimationByIndex(int index);
CP_Image GetSpriteSheetByIndex(int index);
void InitSpritesheets(void);
void DrawAnimation(float x, float y, float scaleX, float scaleY, float delay, int index);
void ConstantAnimSpawner(float time);
void DrawAllAnimations(void);
void SpawnAnimation(float x, float y, float scaleX, float scaleY, int index, float timeToDeath, int isInfinteLoop);

//have to add more in for more spritesheets
enum SpriteAnimationIndex
{
    TILESET_TESTENEMY = 1
};
