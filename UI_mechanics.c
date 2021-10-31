#include <stdio.h>
#include "cprocessing.h"
#include "Common_Headers.h"
#include "UI_mechanics.h"
#include "Buildings.h"

//TEMPPPPPPPPPPPPP FOR PROTOTYPE ONLY

CP_Image basicEvent;
CP_Image advancedEvent;
CARDEVENTS* currentEvent;
float windowWidth;
float windowHeight;
CP_Vector optionAPos;
CP_Vector optionBPos;

//SPRITESHEET tileset_testenemy = { setNextSprite,minX,maxX,minY,maxY,maxSprites,spriteSizeX,spritesizeY };
SPRITESHEET tileset_testenemy = { 0,0,4,0,1,4,64,64 };
CP_Image testenemy;



int CheckUIClick(float xPos, float yPos)
{
    switch (GetGameState())
    {
    case State_MainMenu:
        break;
    case State_Idle:
        // click on basic card
        if (xPos >= windowWidth - 238 && xPos <= windowWidth - 142 && yPos >= windowHeight - 180 && yPos <= windowHeight - 20)
        {
            return 1;
        }
        // click on advanced card ?
        break;
    case State_MakeAChoice:
        // click on option A
        if (xPos >= optionAPos.x - TILEWIDTH / 2 && xPos <= optionAPos.x + TILEWIDTH / 2 && yPos >= optionBPos.y - TILEHEIGHT / 2 && yPos <= optionBPos.y + TILEHEIGHT / 2)
        {
            return 2;
            break;
        }
        
        // click on option B?
        else if (xPos >= optionBPos.x - TILEWIDTH/2  && xPos <= optionBPos.x+TILEWIDTH/2 && yPos >= optionBPos.y -TILEHEIGHT/2 && yPos <= optionBPos.y+TILEHEIGHT / 2)
        {
            return 3;
            break;
        }
        break;
    case State_PlaceYourBuilding:
        break;
    default:
        return 0;
    }
    return 0;
}

void InitUI()
{
    basicEvent = CP_Image_Load("./Assets/basiceventcard.png");
    advancedEvent = CP_Image_Load("./Assets/advancedeventcard.png");
    windowWidth = (float)CP_System_GetWindowWidth();
    windowHeight = (float)CP_System_GetWindowHeight();
    optionAPos = CP_Vector_Set(windowWidth - 190, windowHeight - 300);
    optionBPos = CP_Vector_Set(windowWidth - 70, windowHeight - 300);
}

void UI_SetEvent(CARDEVENTS* newEvent)
{
    currentEvent = newEvent;
}

void DrawUI()
{
    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
    CP_Graphics_DrawRect(windowWidth - 260, 0, windowWidth, windowHeight);
    if (GetGameState() == State_Idle)
    {
        CP_Image_Draw(basicEvent, windowWidth - 190, windowHeight - 100, 160, 160, 255);
        CP_Image_Draw(advancedEvent, windowWidth - 70, windowHeight - 100, 160, 160, 255);
    }
    else if (GetGameState() == State_MakeAChoice)
    {
        CP_Settings_TextSize(20);
        CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
        char buffer[100];
        sprintf_s(buffer, 100, "%s", currentEvent->description);

        CP_Font_DrawText(buffer, 250, 100);

        CP_Image_Draw(*GetBuildingSpriteButtonByIndex(currentEvent->indexOptionA), optionAPos.x-5, optionAPos.y, TILEWIDTH, TILEHEIGHT/2, 255);
        CP_Image_Draw(*GetBuildingSpriteButtonByIndex(currentEvent->indexOptionB), optionBPos.x+5, optionBPos.y, TILEWIDTH, TILEHEIGHT/2, 255);
    }
    else
    {
        
    }
}


void OpenPopup(int identity);


// all structs are temporarily initialized to 3 in array
Button StartMenu[3];
Button Main[3] = { { 100,100,200,200,21,"Building" }, { 100,100,900,900,22,"Citizen" }, { 100,100,900,900,23,"Settings" } };
Button Popup[1] = { { 256,128,500,500,30,"Popup" } };

//Main[3] = { { 100,100,900,900,21,"Building" }, { 100,100,100,100,22,"Citizen" }, { 100,100,100,100,23,"Settings" } };
//Popup[1] = { { 256,128,500,500,30,"Popup" } };



//requires onmouseclick event to call
int CheckMouseColliding(Button array[])
{
    float mousePosX = CP_Input_GetMouseX();
    float mousePosY = CP_Input_GetMouseY();
    int result = 0;
    int i;
    //int arraysize = sizeof(array)/sizeof(array[0]);


    //loop is temp set to 3 due to array temp size
    for (i = 0; i < 3; i++)
        if (mousePosX >= array[i].xPos && mousePosX <= array[i].xPos + array[i].width &&
            mousePosY >= array[i].yPos && mousePosY <= array[i].yPos + array[i].height)
        {
            result = array[i].identity;
            break;
        }
        else
            result = 0;
    
    return result;
        //OpenPopup(result);

}

void MouseCollidingState(int zgamestate)
{
    int result = 0;
    if (zgamestate == 1)
        result = CheckMouseColliding(StartMenu);
    else if (zgamestate == 2)
        result = CheckMouseColliding(Main);
    else if (zgamestate == 3)
        result = CheckMouseColliding(Popup);

    //return result;
}

void OpenPopup(int identity)
{
    int i = 0;
    if (identity == 21)
        i = 0;
    else if (identity == 22)
        i = 1;
    else if (identity == 23)
        i = 2;

    CP_Image_Draw(Popup[i].imagename, 300, 300, Popup[i].width, Popup[i].height, 255);

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

// Draws Animation, float x and y for position, float scale pixel size, delay is animation time, index is enum spritesheet
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

void DrawTempTextResources()
{
    CP_Settings_TextSize(20);
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
    char buffer[100];
    sprintf_s(buffer, 100, "Gold: %d\t\tFood: %d\t\tPopulation: %d", Get_current_gold(), Get_current_food(), Get_current_population());
    CP_Font_DrawText(buffer, 20, 20);
}




