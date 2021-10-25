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

typedef struct
{
	int setNextSprite;
	float minX; //parameters for recurring sprites x-axis
	float maxX; //parameters for recurring sprites x-axis
	float minY;
	float maxY;
	int maxSprites; //number of sprites in a spritesheet
	float spriteSizeX; //pixels on a sprite grid x-axis
	float spriteSizeY; //pixels on a sprite grid y-axis
}SPRITESHEET;

typedef struct
{
	const char* imagename;  
	int maxSpritesX;
	int maxSpritesY;

}TILEMAP;

SPRITESHEET GetSpriteAnimationByIndex(int index);
CP_Image GetSpriteSheetByIndex(int index);
void InitSpritesheets(void);
void DrawAnimation(float x, float y, float scaleX, float scaleY, float delay, int index);
void DrawTile(int gridPosX, int gridPosY);

//have to add more in for more spritesheets
enum SpriteAnimationIndex {
	TILESET_TESTENEMY = 0
};

