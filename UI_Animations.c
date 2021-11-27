#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "cprocessing.h"
#include "Common_Headers.h"
#include "UI_Animations.h"

#define NATURAL_LOG_OF_2    0.693147181f
#define PI                3.14159265358979323846f  /* pi */

//SPRITESHEET tileset_testenemy = { setNextSprite,minX,maxX,minY,maxY,maxSprites,spriteSizeX,spritesizeY,timeToDeath,posX,PosY,scaleX,scaleY,timeElapse,index,isInfiniteLoop };
//SPRITESHEET tileset_testenemy = { setNextSprite,minX,maxX,minY,maxY,maxSprites,spriteSizeX,spritesizeY,scaleX,scaleY,index,isInfiniteLoop,posX,PosY,endposX,endposY,timeElapse,timeToDeath};
SPRITESHEET tileset_testenemy = { 0,0,4,0,1,4,64,64,200,200,1,0,100,100, 100, 100, 0,10 };
SPRITESHEET tileset_stars = { 0,0,10,0,1,10,64,64,100,100,2,0,100,100, 100, 100, 0, 10 };
SPRITESHEET tileset_stars_falling = { 0,0,10,0,1,10,64,64,100,100,2,0,100,100, 100, 100, 0, 10 };
CP_Image testenemy;
CP_Image stars;

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

// using for testing spawner anims
float spawnerAnimDelta = 0;


void InitSpritesheets()
{
    //edit this when more spritesheets
    numOfSpritesheets = 2;

    testenemy = CP_Image_Load("./Assets/testenemy.png");
    stars = CP_Image_Load("./Assets/Impero_Sprite_Stars.png");

    for (int i = 0; i < numOfSpritesheets; ++i)
    {
        minX[i] = GetSpriteAnimationByIndex(i).minX;
        maxX[i] = GetSpriteAnimationByIndex(i).maxX;
        minY[i] = GetSpriteAnimationByIndex(i).minY;
        maxY[i] = GetSpriteAnimationByIndex(i).maxY;
        maxSprites[i] = GetSpriteAnimationByIndex(i).maxSprites;
        spriteSizeX[i] = GetSpriteAnimationByIndex(i).spriteSizeX;
        spriteSizeY[i] = GetSpriteAnimationByIndex(i).spriteSizeY;
    }

}
SPRITESHEET GetSpriteAnimationByIndex(int index)
{
    switch (index)
    {
    case TILESET_TESTENEMY:
        return tileset_testenemy;
    case TILESET_STARS:
        return tileset_stars;
    case TILESET_STARFALL:
        return tileset_stars_falling;
    default:
        return tileset_testenemy;
    }
}
CP_Image GetSpriteSheetByIndex(int index)
{
    switch (index)
    {
    case 1:
        return testenemy;
    case 2:
        return stars;
    case 3:
        return stars;
    default:
        return testenemy;
    }
}

SPRITESHEET AllAnims[100] = { 0 };
float getDelta = 0;
float delay = 0;
int animSize = 99;
int currentSprite = 0;
// Draws Animation without stopping, float x and y for position, float scale pixel size, delay is animation time, index is enum spritesheet
void DrawAnimation(float x, float y, float scaleX, float scaleY, float newDelay, int newIndex)
{

    timeElapse[newIndex] += CP_System_GetDt();


    if (timeElapse[newIndex] >= newDelay)
    {
        timeElapse[newIndex] -= newDelay;
        setNextSprite[newIndex] = 1;
    }

    if (setNextSprite[newIndex] == 1)
    {
        if (minX[newIndex] == maxX[newIndex] - 1)
        {
            minX[newIndex] = 0;
        }
        else
        {
            minX[newIndex]++;
        }

        if (minY[newIndex] == maxY[newIndex] - 1)
        {
            minY[newIndex] = 0;
        }
        else
        {
            minY[newIndex]++;
        }

        setNextSprite[newIndex] = 0;
    }

    //CP_Image_DrawSubImage(GetBuildingSpriteByIndex(7), WORLDGRIDX, WORLDGRIDY, TILEWIDTH, TILEHEIGHT, 0, 512, 128, 672, 255);
    CP_Image_DrawSubImage(GetSpriteSheetByIndex(newIndex), x, y, scaleX, scaleY, spriteSizeX[newIndex] * minX[newIndex], spriteSizeY[newIndex] * minY[newIndex], spriteSizeX[newIndex] * (minX[newIndex] + 1), spriteSizeY[newIndex] * (minY[newIndex] + 1), 255);

}
// Saves an animation into the AllAnims array to be rendered. If infiniteLoop put 1. timeToDeath is how long 1 cycle of animation will take in case of infiniteloop.
void SpawnAnimation(float x, float y, float endx, float endy, float scaleX, float scaleY, int index, float timeToDeath, int isInfinteLoop)
{
    int i = 0;
    for (i = 0; i < 99; ++i)
    {
        if (AllAnims[i].timeToDeath == 0)
        {
            break;
        }
    }

    AllAnims[i].timeToDeath = timeToDeath;
    AllAnims[i].posX = x;
    AllAnims[i].posY = y;
    AllAnims[i].scaleX = scaleX;
    AllAnims[i].scaleY = scaleY;
    AllAnims[i].setNextSprite = GetSpriteAnimationByIndex(index).setNextSprite;
    AllAnims[i].minX = GetSpriteAnimationByIndex(index).minX;
    AllAnims[i].minY = GetSpriteAnimationByIndex(index).minY;
    AllAnims[i].maxX = GetSpriteAnimationByIndex(index).maxX;
    AllAnims[i].maxY = GetSpriteAnimationByIndex(index).maxY;
    AllAnims[i].maxSprites = GetSpriteAnimationByIndex(index).maxSprites;
    AllAnims[i].spriteSizeX = GetSpriteAnimationByIndex(index).spriteSizeX;
    AllAnims[i].spriteSizeY = GetSpriteAnimationByIndex(index).spriteSizeY;
    AllAnims[i].timeElapse = GetSpriteAnimationByIndex(index).timeElapse;
    AllAnims[i].index = index;
    AllAnims[i].isInfiniteLoop = 0;
    AllAnims[i].endPosX = endx;
    AllAnims[i].endPosY = endy;

    if (isInfinteLoop)
    {
        AllAnims[i].isInfiniteLoop = 1;
        AllAnims[i].timeElapse = 0;
    }
}
// Draws out all Anims in the array until the animation's death, where it will be removed
void DrawAllAnimations(void)
{

    for (int i = 0; i < animSize; ++i)
    {
        if (AllAnims[i].spriteSizeX > 0 && (AllAnims[i].timeElapse >= AllAnims[i].timeToDeath))
        {
            AllAnims[i].timeToDeath = 0;
            AllAnims[i].posX = 0;
            AllAnims[i].posY = 0;
            AllAnims[i].scaleX = 0;
            AllAnims[i].scaleY = 0;
            AllAnims[i].setNextSprite = 0;
            AllAnims[i].minX = 0;
            AllAnims[i].minY = 0;
            AllAnims[i].maxX = 0;
            AllAnims[i].maxY = 0;
            AllAnims[i].maxSprites = 0;
            AllAnims[i].spriteSizeX = 0;
            AllAnims[i].spriteSizeY = 0;
            AllAnims[i].timeElapse = 0;
            AllAnims[i].index = 0;
            AllAnims[i].isInfiniteLoop = 0;
        }

        if (AllAnims[i].isInfiniteLoop == 0)
        {
            delay = AllAnims[i].timeToDeath / (AllAnims[i].maxSprites);
            getDelta = CP_System_GetDt();
            AllAnims[i].timeElapse += getDelta;

            currentSprite = (int)(AllAnims[i].timeElapse / delay);

            if (currentSprite >= 1)
            {
                for (int j = 0; j < currentSprite; j++)
                {
                    AllAnims[i].minX = (float)currentSprite;
                    AllAnims[i].minY = (float)currentSprite;

                    if (AllAnims[i].minX >= AllAnims[i].maxX - 1)
                    {
                        AllAnims[i].minX = 0;
                    }

                    if (AllAnims[i].minY >= AllAnims[i].maxY - 1)
                    {
                        AllAnims[i].minY = 0;
                    }

                }
            }

            if ((currentSprite + 1) <= AllAnims[i].maxSprites)
            {
                //CP_Image_DrawSubImage(GetBuildingSpriteByIndex(7), WORLDGRIDX, WORLDGRIDY, TILEWIDTH, TILEHEIGHT, 0, 512, 128, 672, 255);
                CP_Image_DrawSubImage(GetSpriteSheetByIndex(AllAnims[i].index), CP_Math_LerpFloat(AllAnims[i].posX, AllAnims[i].endPosX, AllAnims[i].timeElapse / AllAnims[i].timeToDeath),
                    CP_Math_LerpFloat(AllAnims[i].posY, AllAnims[i].endPosY, AllAnims[i].timeElapse / AllAnims[i].timeToDeath), AllAnims[i].scaleX, AllAnims[i].scaleY, AllAnims[i].spriteSizeX * AllAnims[i].minX, AllAnims[i].spriteSizeY * AllAnims[i].minY, AllAnims[i].spriteSizeX * (AllAnims[i].minX + 1), AllAnims[i].spriteSizeY * (AllAnims[i].minY + 1), 255);
            }
        }

        else if (AllAnims[i].isInfiniteLoop == 1)
        {
            AllAnims[i].timeElapse += getDelta;
            delay = AllAnims[i].timeToDeath / (AllAnims[i].maxSprites);

            if (AllAnims[i].timeElapse >= delay)
            {
                AllAnims[i].timeElapse -= delay;
                AllAnims[i].setNextSprite = 1;
            }

            if (AllAnims[i].setNextSprite == 1)
            {
                if (AllAnims[i].minX == AllAnims[i].maxX - 1)
                {
                    AllAnims[i].minX = 0;
                }
                else
                {
                    AllAnims[i].minX++;
                }

                if (AllAnims[i].minY == AllAnims[i].maxY - 1)
                {
                    AllAnims[i].minY = 0;
                }
                else
                {
                    AllAnims[i].minY++;
                }

                AllAnims[i].setNextSprite = 0;
            }
            // AllAnims[i].posX
            //CP_Image_DrawSubImage(GetSpriteSheetByIndex(AllAnims[i].index), AllAnims[i].posX
            //    , AllAnims[i].posY, AllAnims[i].scaleX, AllAnims[i].scaleY, AllAnims[i].spriteSizeX * AllAnims[i].minX, AllAnims[i].spriteSizeY * AllAnims[i].minY, AllAnims[i].spriteSizeX * (AllAnims[i].minX + 1), AllAnims[i].spriteSizeY * (AllAnims[i].minY + 1), 255);
            CP_Image_DrawSubImage(GetSpriteSheetByIndex(AllAnims[i].index), CP_Math_LerpFloat(AllAnims[i].posX, AllAnims[i].endPosX, AllAnims[i].timeElapse / AllAnims[i].timeToDeath),
                CP_Math_LerpFloat(AllAnims[i].posY, AllAnims[i].endPosY, AllAnims[i].timeElapse / AllAnims[i].timeToDeath), AllAnims[i].scaleX, AllAnims[i].scaleY, AllAnims[i].spriteSizeX * AllAnims[i].minX, AllAnims[i].spriteSizeY * AllAnims[i].minY, AllAnims[i].spriteSizeX * (AllAnims[i].minX + 1), AllAnims[i].spriteSizeY * (AllAnims[i].minY + 1), 255);
        }

    }
}


float counter1AnimDelta = 0;
float counter2AnimDelta = 0;
float counter3AnimDelta = 0;
//repeated spawnanim function
void ConstantAnimSpawner(int counterIndex, int index, float time, int lowerX, int upperX, int lowerY, int upperY, float scaleX, float scaleY, float timeToDeath, int isTimeVariance, int isLerp)
{
    float posX = 0;
    float posY = 0;
    float endposX = 0;
    float endposY = 0;
    float totaltime = time; 
    bool check = CP_Random_GetBool();
    switch (counterIndex)
    {
    case 1:
        counter1AnimDelta += CP_System_GetDt();
        if (counter1AnimDelta < totaltime)
        {
            return;
        }
        counter1AnimDelta -= totaltime;
        break;
    case 2:
        counter2AnimDelta += CP_System_GetDt();
        if (counter2AnimDelta < totaltime)
        {
            return;
        }
        counter2AnimDelta -= totaltime;
        break;
    case 3:
        counter3AnimDelta += CP_System_GetDt();
        if (counter3AnimDelta < totaltime)
        {
            return;
        }
        counter3AnimDelta -= totaltime;
        break;
    }

    posX = (float)CP_Random_RangeInt(lowerX, upperX);
    posY = (float)CP_Random_RangeInt(lowerY, upperY);
    if (isLerp)
    {
        endposX = posX - (float)CP_Random_RangeInt(300, upperX);
        endposY = posY + (float)CP_Random_RangeInt(200, upperY);
        //posX += 200;
        //posY -= 200;
    }
    else 
    {
        endposX = posX;
        endposY = posY;
    }

    SpawnAnimation(posX, posY, endposX, endposY, scaleX, scaleY, index, timeToDeath, 0);
    if (isTimeVariance)
    {
        check = CP_Random_GetBool();
        if (check)
        {
            totaltime += CP_Random_GetFloat();

        }
        else
        {
            totaltime -= CP_Random_GetFloat();
        }  
    }
    if (totaltime <= 0)
    {
        totaltime = time;
    }
    else if (totaltime >= (time * 2))
    {
        totaltime = time;
    }
    
}



CP_Image addGold;
CP_Image minusGold;
CP_Image addFood;
CP_Image loseFood;
CP_Image addMorale;
CP_Image minusMorale;
LINEARVFX vfxList[50];

void InitVfx()
{
    addGold = CP_Image_Load("./Assets/addGold2.png");
    minusGold = CP_Image_Load("./Assets/minusGold2.png");
    addFood = CP_Image_Load("./Assets/addFood2.png");
    loseFood = CP_Image_Load("./Assets/minusFood2.png");
    addMorale = CP_Image_Load("./Assets/addMorale2.png");
    minusMorale = CP_Image_Load("./Assets/minusMorale2.png");
    for (int i = 0; i < 50; ++i)
    {
        vfxList[i] = (LINEARVFX){ 0 };
    }
}

CP_Image* GetVfxSpriteByIndex(int index)
{
    switch (index)
    {
    case 1:
        return &addGold;
    case 2:
        return &minusGold;
    case 3:
        return &addFood;
    case 4:
        return &loseFood;
    case 5:
        return &addMorale;
    case 6:
        return &minusMorale;
    default:
        return &addGold;
    }
}



void SpawnLinearVfx(int spriteIndex, CP_Vector startPos, CP_Vector endPos, float lifetime, CP_Vector size, float spawnDelay)
{
    for (int i = 0; i < 50; ++i)
    {
        if (vfxList[i].spriteIndex == 0)
        {
            vfxList[i].spriteIndex = spriteIndex;
            vfxList[i].startPos = startPos;
            vfxList[i].endPos = endPos;
            vfxList[i].lifetime = lifetime;
            vfxList[i].curlifetime = 0;
            vfxList[i].size = size;
            vfxList[i].spawnDelay = spawnDelay;
            return;
        }
    }
}

//Movement types
float EaseInQuad(float start, float end, float value)
{
    end -= start;
    return end * value * value + start;
}

float EaseInSine(float start, float end, float value)
{
    end -= start;
    return -end * cosf(value * (PI * 0.5f)) + end + start;
}

float EaseOutSine(float start, float end, float value)
{
    end -= start;
    return end * sinf(value * (PI * 0.5f)) + start;
}

void DrawAllVfx()
{
    float deltatVfx = CP_System_GetDt();
    for (int i = 0; i < 50; ++i)
    {
        if (vfxList[i].spriteIndex != 0)
        {
            if (vfxList[i].spawnDelay >= 0)
            {
                vfxList[i].spawnDelay -= deltatVfx;
            }
            else
            {
                vfxList[i].curlifetime += deltatVfx;
                if (vfxList[i].curlifetime < vfxList[i].lifetime)
                {
                    switch (vfxList[i].movementIndex)
                    {
                    // Linear movement
                    case 0:
                        CP_Image_Draw(*GetVfxSpriteByIndex(vfxList[i].spriteIndex)
                            , CP_Math_LerpFloat(vfxList[i].startPos.x, vfxList[i].endPos.x, vfxList[i].curlifetime / vfxList[i].lifetime)
                            , CP_Math_LerpFloat(vfxList[i].startPos.y, vfxList[i].endPos.y, vfxList[i].curlifetime / vfxList[i].lifetime)
                            , vfxList[i].size.x, vfxList[i].size.y, 255);
                        break;
                    // Ease in Sine
                    case 1:
                        CP_Image_Draw(*GetVfxSpriteByIndex(vfxList[i].spriteIndex)
                            , EaseInSine(vfxList[i].startPos.x, vfxList[i].endPos.x, vfxList[i].curlifetime / vfxList[i].lifetime)
                            , EaseInSine(vfxList[i].startPos.y, vfxList[i].endPos.y, vfxList[i].curlifetime / vfxList[i].lifetime)
                            , vfxList[i].size.x, vfxList[i].size.y, 255);
                        break;
                    // Ease out Sine
                    case 2:
                        CP_Image_Draw(*GetVfxSpriteByIndex(vfxList[i].spriteIndex)
                            , EaseOutSine(vfxList[i].startPos.x, vfxList[i].endPos.x, vfxList[i].curlifetime / vfxList[i].lifetime)
                            , EaseOutSine(vfxList[i].startPos.y, vfxList[i].endPos.y, vfxList[i].curlifetime / vfxList[i].lifetime)
                            , vfxList[i].size.x, vfxList[i].size.y, 255);
                        break;
                    // Ease in Quad
                    default:
                        CP_Image_Draw(*GetVfxSpriteByIndex(vfxList[i].spriteIndex)
                            , EaseInQuad(vfxList[i].startPos.x, vfxList[i].endPos.x, vfxList[i].curlifetime / vfxList[i].lifetime)
                            , EaseInQuad(vfxList[i].startPos.y, vfxList[i].endPos.y, vfxList[i].curlifetime / vfxList[i].lifetime)
                            , vfxList[i].size.x, vfxList[i].size.y, 255);
                        break;
                    }
                }
                else
                {
                    vfxList[i].spriteIndex = 0;
                }
            }
        }
    }
}

void SpawnVfxEaseInToEaseOut(int spriteIndex, CP_Vector startPos, CP_Vector checkpoint, CP_Vector endPos, float lifetime, CP_Vector size, float spawnDelay)
{
    int count = 2;
    for (int i = 0; i < 50; ++i)
    {
        if (vfxList[i].spriteIndex == 0)
        {
            if (count == 2)
            {
                vfxList[i].spriteIndex = spriteIndex;
                vfxList[i].movementIndex = 1;
                vfxList[i].startPos = startPos;
                vfxList[i].endPos = checkpoint;
                vfxList[i].lifetime = lifetime;
                vfxList[i].curlifetime = 0;
                vfxList[i].size = size;
                vfxList[i].spawnDelay = spawnDelay;
                --count;
            }
            else if (count == 1)
            {
                vfxList[i].spriteIndex = spriteIndex;
                vfxList[i].movementIndex = 2;
                vfxList[i].startPos = checkpoint;
                vfxList[i].endPos = endPos;
                vfxList[i].lifetime = lifetime;
                vfxList[i].curlifetime = 0;
                vfxList[i].size = size;
                vfxList[i].spawnDelay = spawnDelay + lifetime;
                return;
            }
        }
    }
}







