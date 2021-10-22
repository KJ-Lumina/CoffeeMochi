#include <stdio.h>
#include "CProcessing.h"

// hardcode amt of sprites, add 1 more when more spritesheets :(
int numOfSpritesheets;
int setNextSprite[1];
float minX[1];
float maxX[1];
float minY[1];
float maxY[1];
int maxSprites[1];
float spriteSizeX[1];
float spriteSizeY[1];
float timeElapse[1];

SPRITESHEET GetSpriteAnimationByIndex(int index);
CP_Image GetSpriteSheetByIndex(int index);
void InitSpritesheets(void);

//have to add more in for more spritesheets
enum SpriteAnimationIndex {
	TILESET_TESTENEMY = 0
};

