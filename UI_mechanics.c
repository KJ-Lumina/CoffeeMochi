#include <stdio.h>
#include <stdbool.h>
#include "cprocessing.h"
#include "Common_Headers.h"
#include "UI_mechanics.h"
#include "Buildings.h"
#include "UI_Text.h"

#define BUTTONARRAYSIZE 3

// SPRITES
CP_Image EventCard;
MOVINGSPRITES EventCardAnim;
CP_Image image_CardBack;
CP_Image EventCardDeck;
//CP_Image EventGauntletClose;
CP_Image image_CardFlipped;
CP_Image image_CardA;
CP_Image image_CardB;

CARDEVENT* selectedEvent;
REWARDCARD* selectedReward;
int rewardCardsLeft;
float windowWidth;
float windowHeight;
CP_Vector optionAPos;
CP_Vector optionBPos;
bool IsAViable;
bool IsBViable;
char textDescBuffer[100];

//SPRITESHEET tileset_testenemy = { setNextSprite,minX,maxX,minY,maxY,maxSprites,spriteSizeX,spritesizeY,timeToDeath,posX,PosY,scaleX,scaleY,timeElapse,index,isInfiniteLoop };
SPRITESHEET tileset_testenemy = { 0,0,4,0,1,4,64,64,10,100,100,200,200,0,1,0 };
CP_Image testenemy;
//Button normalinitialize = {width,height,xPos,yPos,isSplashScreenActive,isSettingActive,index}
BUTTON start_game = { true,100,100,200,200,1,0,START_GAME };

//using for testing spawner anims
float delta = 0;





void InitUI()
{
    windowWidth = (float)CP_System_GetWindowWidth();
    windowHeight = (float)CP_System_GetWindowHeight();
    image_CardBack = CP_Image_Load("./ImperoArtAssets/Impero_CardBack.png");
    EventCardDeck = CP_Image_Load("./Assets/WIP CARDDECK.png");
    EventCardAnim = (MOVINGSPRITES){ image_CardBack, CP_Vector_Set(windowWidth - 130, (windowHeight / 2) + 230), CP_Vector_Set(windowWidth - 130, (windowHeight / 2) - 60), 0.6f, 0 };
    //EventGauntletClose = CP_Image_Load("./Assets/gauntletclose.png");
    image_CardFlipped = CP_Image_Load("./ImperoArtAssets/Impero_CardFlip.png");
    image_CardA = CP_Image_Load("./ImperoArtAssets/Impero_CardBlue.png");
    image_CardB = CP_Image_Load("./ImperoArtAssets/Impero_CardRed.png");

    optionAPos = CP_Vector_Set(windowWidth - 176, windowHeight / 2 - 60);
    optionBPos = CP_Vector_Set(windowWidth - 90, windowHeight / 2 - 60);
}

void UI_SetEvent(CARDEVENT* newEvent)
{
    selectedEvent = newEvent;
    IsAViable = IsCostPayable(selectedEvent->resourceChangeA[0]);
    IsBViable = IsCostPayable(selectedEvent->resourceChangeB[0]);
}

void UI_SetReward(CARDEVENT* newEvent, bool optionA)
{
    if (optionA)
    {
        selectedReward = GetRewardByIndex(newEvent->resourceRewardA[0]);
        rewardCardsLeft = newEvent->resourceRewardA[1];
    }
    else
    {
        selectedReward = GetRewardByIndex(newEvent->resourceRewardB[0]);
        rewardCardsLeft = newEvent->resourceRewardB[1];
    }
}

bool ClickCheck_CardDraw()
{
    if (CheckWithinBounds(EventCardAnim.startingPos, 185, 243))
    {
        return true;
    }
    return false;
}

int ClickCheck_CardChoice()
{
    if (CheckWithinBounds(optionAPos, 120, 320))
    {
        if (IsAViable)
        {
            return 1;
        }
        else
        {
            printf("u broke dude LMAO");
            return 0;
        }
    }
    else if (CheckWithinBounds(optionBPos, 120, 320))
    {
        if (IsBViable)
        {
            return 2;
        }
        else
        {
            printf("u broke dude LMAO");
            return 0;
        }
    }
    return 0;
}

int ClickCheck_Rewards()
{
    if (CheckWithinBounds(CP_Vector_Set(windowWidth - 130, windowHeight / 2 - 60), 180, 240))
    {
        if (selectedReward->cardType == BUILD_TYPE_EVENT)
        {
            return 1;
        }
        else if (selectedReward->cardType == ONGOING_TYPE_EVENT)
        {
            return 2;
        }
    }
    return 0;
}


/*void DrawUI_GauntletClose()
{
    // Draw Gauntlet
    CP_Image_Draw(EventGauntletClose, windowWidth - 130, windowHeight / 2 - 60, 240, 255, 255);
}*/

void DrawUI_GauntletOpen()
{
    // Draw Gauntlet
    // Hovering A
    if (CheckWithinBounds(optionAPos, 90, 243))
    {
        CP_Image_Draw(image_CardA, windowWidth - 130, windowHeight / 2 - 60, 185, 243, 255);
        CP_Settings_TextSize(20);
        CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
        CP_Font_DrawTextBox(selectedEvent->descriptionA, windowWidth - 255, 140, 250);
    }
    // Hovering B
    else if (CheckWithinBounds(optionBPos, 90, 243))
    {
        CP_Image_Draw(image_CardB, windowWidth - 130, windowHeight / 2 - 60, 185, 243, 255);
        CP_Settings_TextSize(20);
        CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
        CP_Font_DrawTextBox(selectedEvent->descriptionB, windowWidth - 255, 140, 250);
    }
    // Not Hovering
    else
    {
        CP_Image_Draw(image_CardFlipped, windowWidth - 130, windowHeight / 2 - 60, 185, 243, 255);
        CP_Settings_TextSize(20);
        CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
        CP_Font_DrawTextBox(selectedEvent->description, windowWidth - 255, 140, 250);
    }
}

void DrawUI_Deck()
{
    // Draw back of card
    CP_Image_Draw(EventCardDeck, windowWidth - 130, (windowHeight / 2) + 240, 240, 360, 255);
}

void DrawUI_TopPile()
{
    // Hovering Deck
    if (CheckWithinBounds(EventCardAnim.startingPos, 185, 243))
    {
        EventCardAnim.currentTime += CP_System_GetDt();
    }
    else
    {
        EventCardAnim.currentTime -= CP_System_GetDt();
    }
    EventCardAnim.currentTime = CP_Math_ClampFloat(EventCardAnim.currentTime, 0, EventCardAnim.totalTime / 8);
    CP_Image_Draw(EventCardAnim.image, EventCardAnim.startingPos.x , CP_Math_LerpFloat(EventCardAnim.startingPos.y, EventCardAnim.endingPos.y, EventCardAnim.currentTime / EventCardAnim.totalTime), 185, 243, 255);
}

void DrawUI_TopPileInsert()
{
    EventCardAnim.currentTime += CP_System_GetDt();
    EventCardAnim.currentTime = CP_Math_ClampFloat(EventCardAnim.currentTime, 0, EventCardAnim.totalTime);
    CP_Image_Draw(EventCardAnim.image, EventCardAnim.startingPos.x, CP_Math_LerpFloat(EventCardAnim.startingPos.y, EventCardAnim.endingPos.y, EventCardAnim.currentTime / EventCardAnim.totalTime), 185, 243, 255);
}

void DrawUI_Default()
{
    // Create black background
    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
    CP_Graphics_DrawRect(windowWidth - 260, 0, windowWidth, windowHeight);
}

void DrawUI_RewardCards()
{
    CP_Image_Draw(*GetCardSpriteByIndex(selectedReward->eventIndex), windowWidth - 130, windowHeight / 2 - 60, 185, 243, 255);
}

void DrawUI(GAMESTATE state)
{
    DrawUI_Default();
    switch (state)
    {
    case State_StartOfTurn:
        DrawUI_Deck();
        DrawUI_TopPile();
        //DrawUI_GauntletClose();
        break;
    case State_Idle:
        DrawUI_Deck();
        DrawUI_TopPile();
        //DrawUI_GauntletClose();
        break;
    case State_CardDraw:
        DrawUI_Deck();
        DrawUI_TopPileInsert();
        //DrawUI_GauntletClose();
        break;
    case State_MakeAChoice:
        DrawUI_Deck();
        DrawUI_GauntletOpen();
        break;
    case State_CollectRewards:
        DrawUI_Deck();
        DrawUI_RewardCards();
        break;
    case State_PlaceYourBuilding:
        DrawUI_Deck();
        //DrawUI_GauntletClose();
        break;
    case State_EndOfTurn:
        DrawUI_Deck();
        //DrawUI_GauntletClose();
        EventCardAnim.currentTime = 0;
        break;
    }
}


// all structs are temporarily initialized to 3 in array
BUTTON AllButtons[BUTTONARRAYSIZE];
//Button Main[3] = { { 100,100,200,200,0,0,"Building" }, { 100,100,900,900,22,"Citizen" }, { 100,100,900,900,23,"Settings" } };

BUTTON GetButtonIndex(int index)
{
    switch (index)
    {
    case 0:
        return start_game;
    default:
        return start_game;
    }

}

void InitButtons(void)
{
    int numOfButtons = 1;

    for (int i = 0; i < numOfButtons; ++i)
    {
        AllButtons[i].width = GetButtonIndex(i).width;
        AllButtons[i].height = GetButtonIndex(i).height;
        AllButtons[i].xPos = GetButtonIndex(i).xPos;
        AllButtons[i].yPos = GetButtonIndex(i).yPos;
        AllButtons[i].isSplashScreenActive = GetButtonIndex(i).isSplashScreenActive;
        AllButtons[i].isSettingsActive = GetButtonIndex(i).isSettingsActive;
        AllButtons[i].index = GetButtonIndex(i).index;

    }

}

//requires onmouseclick event to call
int CheckMouseColliding(BUTTON buttonArray[], CP_Vector mousePos, int isSplashScreenActive, int isSettingsActive)
{
    float mousePosX = mousePos.x;
    float mousePosY = mousePos.y;
    int mouseClickFailed = 0;


    for (int i = 0; i < BUTTONARRAYSIZE; i++)
    {
        if (buttonArray[i].isUsed)
        {
            if (mousePosX >= buttonArray[i].xPos && mousePosX <= buttonArray[i].xPos + buttonArray[i].width &&
                mousePosY >= buttonArray[i].yPos && mousePosY <= buttonArray[i].yPos + buttonArray[i].height &&
                buttonArray[i].isSplashScreenActive == isSplashScreenActive &&
                buttonArray[i].isSettingsActive == isSettingsActive)
            {
                if (isSplashScreenActive == 1)
                {
                    //SplashScreenButtons(array[i].imagename);
                    return buttonArray[i].index;
                }
                else if (isSettingsActive == 1)
                {
                    //SettingsButtons(array[i].imagename);
                    return buttonArray[i].index;
                }
                else if (isSplashScreenActive == 0 && isSettingsActive == 0)
                {
                    //MainButtons(array[i].imagename);
                    return buttonArray[i].index;
                }
            }
        }
    }

    return mouseClickFailed;
}


void InitSpritesheets(void)
{
    //edit this when more spritesheets
    numOfSpritesheets = 1;

    testenemy = CP_Image_Load("./Assets/testenemy.png");



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
    default:
        return testenemy;
    }
}

// Draws Animation without stopping, float x and y for position, float scale pixel size, delay is animation time, index is enum spritesheet
void DrawAnimation(float x, float y, float scaleX, float scaleY, float delay, int index)
{

    timeElapse[index] += CP_System_GetDt();


    if (timeElapse[index] >= delay)
    {
        timeElapse[index] -= delay;
        setNextSprite[index] = 1;
    }

    if (setNextSprite[index] == 1)
    {
        if (minX[index] == maxX[index] - 1)
        {
            minX[index] = 0;
        }
        else
        {
            minX[index]++;
        }

        if (minY[index] == maxY[index] - 1)
        {
            minY[index] = 0;
        }
        else
        {
            minY[index]++;
        }

        setNextSprite[index] = 0;
    }

    //CP_Image_DrawSubImage(GetBuildingSpriteByIndex(7), WORLDGRIDX, WORLDGRIDY, TILEWIDTH, TILEHEIGHT, 0, 512, 128, 672, 255);
    CP_Image_DrawSubImage(GetSpriteSheetByIndex(index), x, y, scaleX, scaleY, spriteSizeX[index] * minX[index], spriteSizeY[index] * minY[index], spriteSizeX[index] * (minX[index] + 1), spriteSizeY[index] * (minY[index] + 1), 255);

}

SPRITESHEET AllAnims[100] = { 0 };

// Saves an animation into the AllAnims array to be rendered. If infiniteLoop put 1. timeToDeath is how long 1 cycle of animation will take in case of infiniteloop.
void SpawnAnimation(float x, float y, float scaleX, float scaleY, int index, float timeToDeath, int isInfinteLoop)
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

    if (isInfinteLoop)
    {
        AllAnims[i].isInfiniteLoop = 1;
        AllAnims[i].timeElapse = 0;
    }
}

float getDelta = 0;
float delay = 0;
int animSize = 99;
int currentSprite = 0;

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
                CP_Image_DrawSubImage(GetSpriteSheetByIndex(AllAnims[i].index), AllAnims[i].posX, AllAnims[i].posY, AllAnims[i].scaleX, AllAnims[i].scaleY, AllAnims[i].spriteSizeX * AllAnims[i].minX, AllAnims[i].spriteSizeY * AllAnims[i].minY, AllAnims[i].spriteSizeX * (AllAnims[i].minX + 1), AllAnims[i].spriteSizeY * (AllAnims[i].minY + 1), 255);
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

            CP_Image_DrawSubImage(GetSpriteSheetByIndex(AllAnims[i].index), AllAnims[i].posX, AllAnims[i].posY, AllAnims[i].scaleX, AllAnims[i].scaleY, AllAnims[i].spriteSizeX * AllAnims[i].minX, AllAnims[i].spriteSizeY * AllAnims[i].minY, AllAnims[i].spriteSizeX * (AllAnims[i].minX + 1), AllAnims[i].spriteSizeY * (AllAnims[i].minY + 1), 255);

        }

    }
}

//testing animation fucntion
void ConstantAnimSpawner(float time)
{
    delta += CP_System_GetDt();
    if (delta >= time)
    {
        delta -= time;
        SpawnAnimation(100, 100, 200, 200, 1, 4, 0);
    }
}


void DrawTempTextResources()
{
    CP_Settings_TextSize(20);
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
    char buffer[200];
    sprintf_s(buffer, 100, "Gold: %d\t\tFood: %d\t\tPopulation: %d\t\tMorale: %d", Get_current_gold(), Get_current_food(), Get_current_population(), (Get_current_morale() + Get_additional_morale()));
    CP_Font_DrawText(buffer, 200, 20);
}

