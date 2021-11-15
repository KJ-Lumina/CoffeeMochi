#include <stdio.h>
#include "cprocessing.h"
#include "Common_Headers.h"
#include "UI_mechanics.h"
#include "Buildings.h"
#include "UI_Text.h"

//TEMPPPPPPPPPPPPP FOR PROTOTYPE ONLY

CP_Image basicEvent;
CP_Image advancedEvent;
CARDEVENT* selectedEvent;
float windowWidth;
float windowHeight;
CP_Vector optionAPos;
CP_Vector optionBPos;
char textDescBuffer[100];

//SPRITESHEET tileset_testenemy = { setNextSprite,minX,maxX,minY,maxY,maxSprites,spriteSizeX,spritesizeY,timeToDeath,posX,PosY,scaleX,scaleY,timeElapse,index,isInfiniteLoop };
SPRITESHEET tileset_testenemy = { 0,0,4,0,1,4,64,64,10,100,100,200,200,0,1,0 };
CP_Image testenemy;
//Button normalinitialize = {width,height,xPos,yPos,isSplashScreenActive,isSettingActive,index}
BUTTON start_game = { 100,100,200,200,1,0,START_GAME };

//using for testing spawner anims
float delta = 0;



int CheckUIClick(float xPos, float yPos)
{
    switch (GetGameState())
    {
    case State_MainMenu:
        break;
    case State_Idle:
        // click on card
        if (xPos >= windowWidth - 238 && xPos <= windowWidth - 142 && yPos >= windowHeight - 180 && yPos <= windowHeight - 20)
        {
            return 1;
        }
        break;
    case State_MakeAChoice:
        // click on option A
        if (xPos >= optionAPos.x - TILEWIDTH / 2 && xPos <= optionAPos.x + TILEWIDTH / 2 && yPos >= optionBPos.y - TILEHEIGHT / 2 && yPos <= optionBPos.y + TILEHEIGHT / 2)
        {
            TILEPOSITION tile_positions[MAXTILECOUNT];
            int randIndex = 0;
            unsigned int upperBounds = 0;
            unsigned int lowerBounds = 0;
            int length;

            //Check for Pre-Requiste 
            switch (selectedEvent->costTypeA)
            {
                //RESOURCE COST
            case R_GOLD_INDEX:
                if ((Get_current_gold() - selectedEvent->costAmountA) < 0) return 0;
                Set_current_gold(Get_current_gold() - selectedEvent->costAmountA);
                break;

            case R_FOOD_INDEX:
                if ((Get_current_food() - selectedEvent->costAmountA) < 0) return 0;
                Set_current_food(Get_current_food() - selectedEvent->costAmountA);
                break;

            case R_POPULATION_INDEX:
                if ((Get_current_population() - selectedEvent->costAmountA) < 0) return 0;
                Set_current_population(Get_current_population() - selectedEvent->costAmountA);
                break;

            case R_MORALE_INDEX:
                if ((Get_current_morale() - selectedEvent->costAmountA) < 0) return 0;
                Set_additional_morale(Get_additional_morale() - selectedEvent->costAmountA);
                break;

            case R_BUILDING_HOUSE_INDEX:
                SubtractHouse();           

                length = GetAllBuildingsPositionByIndex(B_HOUSE_INDEX, tile_positions);

                if (length > 0) {
                    lowerBounds = 0;
                    upperBounds = length - 1;

                    randIndex = CP_Random_RangeInt(lowerBounds, upperBounds);

                    SetNewBuilding((tile_positions + randIndex)->positionX, (tile_positions + randIndex)->positionY, B_GRASS_INDEX);
                }

                break;

            case R_BUILDING_FARM_INDEX:
                SubtractFarm();

                length = GetAllBuildingsPositionByIndex(B_HOUSE_INDEX, tile_positions);

                if (length > 0) {
                    lowerBounds = 0;
                    upperBounds = length - 1;

                    randIndex = CP_Random_RangeInt(lowerBounds, upperBounds);

                    SetNewBuilding((tile_positions + randIndex)->positionX, (tile_positions + randIndex)->positionY, B_GRASS_INDEX);
                }
                break;

            case R_BUILDING_MARKET_INDEX:
                SubtractMarket();

                length = GetAllBuildingsPositionByIndex(B_HOUSE_INDEX, tile_positions);

                if (length > 0) {
                    lowerBounds = 0;
                    upperBounds = length - 1;

                    randIndex = CP_Random_RangeInt(lowerBounds, upperBounds);

                    SetNewBuilding((tile_positions + randIndex)->positionX, (tile_positions + randIndex)->positionY, B_GRASS_INDEX);
                }
                break;

            case R_BUILDING_TAVERN_INDEX:
                //Subtract Tavern
                break;

            default:
                break;
            }

            //Run the Event depending on the event type if the Pre-Requiste is met
            switch (selectedEvent->eventType) {
            case BUILD_TYPE_EVENT:
                SetCurrentBuilding(GetBuildingByIndex(selectedEvent->indexOptionA));
                SetCurrentAmountToBuild(selectedEvent->optionAmountA);
                return 1;
                break;

            case RESOURCE_TYPE_EVENT:              

                switch (selectedEvent->optionTypeA) {
                case R_GOLD_INDEX:

                    if (selectedEvent->indexOptionA == E_INCREASE_RESOURCE) {
                        Set_current_gold(Get_current_gold() + selectedEvent->optionAmountA);
                    }
                    else if (selectedEvent->indexOptionA == E_DECREASE_RESOURCE) {
                        Set_current_gold(Get_current_gold() - selectedEvent->optionAmountA);
                    }
                 
                    break;

                case R_FOOD_INDEX:

                    if (selectedEvent->indexOptionA == E_INCREASE_RESOURCE) {
                        Set_current_food(Get_current_food() + selectedEvent->optionAmountA);
                    }
                    else if (selectedEvent->indexOptionA == E_DECREASE_RESOURCE) {
                        Set_current_food(Get_current_food() - selectedEvent->optionAmountA);
                    }

                    break;

                case R_POPULATION_INDEX:

                    if (selectedEvent->indexOptionA == E_INCREASE_RESOURCE) {
                        Set_current_population(Get_current_population() + selectedEvent->optionAmountA);
                    }
                    else if (selectedEvent->indexOptionA == E_DECREASE_RESOURCE) {
                        Set_current_population(Get_current_population() - selectedEvent->optionAmountA);
                    }

                    break;
                
                default:
                    break;
                }

                return 2;
                break;

            case ONGOING_TYPE_EVENT:

                break;

            default:
                break;

            }
             
            break;
        }
        
        // click on option B?
        else if (xPos >= optionBPos.x - TILEWIDTH/2  && xPos <= optionBPos.x+TILEWIDTH/2 && yPos >= optionBPos.y -TILEHEIGHT/2 && yPos <= optionBPos.y+TILEHEIGHT / 2)
        {
            TILEPOSITION tile_positions[MAXTILECOUNT];
            int randIndex = 0;
            unsigned int upperBounds = 0;
            unsigned int lowerBounds = 0;
            int length;

            //Check for Pre-Requiste 
            switch (selectedEvent->costTypeB)
            {
            //RESOURECE RELATED TYPE COST
            case R_GOLD_INDEX:
                if ((Get_current_gold() - selectedEvent->costAmountB) < 0) return 0;
                Set_current_gold(Get_current_gold() - selectedEvent->costAmountB);
                break;

            case R_FOOD_INDEX:
                if ((Get_current_food() - selectedEvent->costAmountB) < 0) return 0;
                Set_current_food(Get_current_food() - selectedEvent->costAmountB);
                break;

            case R_POPULATION_INDEX:
                if ((Get_current_population() - selectedEvent->costAmountB) < 0) return 0;
                Set_current_population(Get_current_population() - selectedEvent->costAmountB);
                break;

            case R_MORALE_INDEX:
                if ((Get_current_morale() - selectedEvent->costAmountB) < 0) return 0;
                Set_additional_morale(Get_additional_morale() - selectedEvent->costAmountB);
                break;


            //BUILDING RELATED TYPE COST
            case R_BUILDING_HOUSE_INDEX:
                SubtractHouse();

                length = GetAllBuildingsPositionByIndex(B_HOUSE_INDEX, tile_positions);

                if (length > 0) {
                    lowerBounds = 0;
                    upperBounds = length - 1;

                    randIndex = CP_Random_RangeInt(lowerBounds, upperBounds);

                    SetNewBuilding((tile_positions + randIndex)->positionX, (tile_positions + randIndex)->positionY, B_GRASS_INDEX);
                }

                break;

            case R_BUILDING_FARM_INDEX:
                SubtractFarm();

                length = GetAllBuildingsPositionByIndex(B_HOUSE_INDEX, tile_positions);

                if (length > 0) {
                    lowerBounds = 0;
                    upperBounds = length - 1;

                    randIndex = CP_Random_RangeInt(lowerBounds, upperBounds);

                    SetNewBuilding((tile_positions + randIndex)->positionX, (tile_positions + randIndex)->positionY, B_GRASS_INDEX);
                }
                break;

            case R_BUILDING_MARKET_INDEX:
                SubtractMarket();

                length = GetAllBuildingsPositionByIndex(B_HOUSE_INDEX, tile_positions);

                if (length > 0) {
                    lowerBounds = 0;
                    upperBounds = length - 1;

                    randIndex = CP_Random_RangeInt(lowerBounds, upperBounds);

                    SetNewBuilding((tile_positions + randIndex)->positionX, (tile_positions + randIndex)->positionY, B_GRASS_INDEX);
                }
                break;

            case R_BUILDING_TAVERN_INDEX:
                //Subtract Tavern
                break;

            default:
                break;
            }

            //Run the Event depending on the event type if the Pre-Requiste is met
            switch (selectedEvent->eventType) {
            case BUILD_TYPE_EVENT:
                SetCurrentBuilding(GetBuildingByIndex(selectedEvent->indexOptionB));
                SetCurrentAmountToBuild(selectedEvent->optionAmountB);
                return 1;
                break;

            case RESOURCE_TYPE_EVENT:

                printf("Test4");

                switch (selectedEvent->optionTypeA) {
                case R_GOLD_INDEX:

                    if (selectedEvent->indexOptionA == E_INCREASE_RESOURCE) {
                        Set_current_gold(Get_current_gold() + selectedEvent->optionAmountB);
                    }
                    else if (selectedEvent->indexOptionA == E_DECREASE_RESOURCE) {
                        Set_current_gold(Get_current_gold() - selectedEvent->optionAmountB);
                    }

                    break;

                case R_FOOD_INDEX:

                    if (selectedEvent->indexOptionA == E_INCREASE_RESOURCE) {
                        Set_current_food(Get_current_food() + selectedEvent->optionAmountB);
                    }
                    else if (selectedEvent->indexOptionA == E_DECREASE_RESOURCE) {
                        Set_current_food(Get_current_food() - selectedEvent->optionAmountB);
                    }

                    break;

                case R_POPULATION_INDEX:

                    if (selectedEvent->indexOptionA == E_INCREASE_RESOURCE) {
                        Set_current_population(Get_current_population() + selectedEvent->optionAmountB);
                    }
                    else if (selectedEvent->indexOptionA == E_DECREASE_RESOURCE) {
                        Set_current_population(Get_current_population() - selectedEvent->optionAmountB);
                    }

                    break;

                default:
                    break;
                }

                return 2;
                break;


            case DESTROY_TYPE_EVENT:

                switch (selectedEvent->optionTypeA) {

                    case R_BUILDING_HOUSE_INDEX:

                        SubtractHouse();

                        break;
                    
                    case R_BUILDING_FARM_INDEX:

                        SubtractFarm();

                        break;

                    case R_BUILDING_MARKET_INDEX:

                        SubtractMarket();

                        break;

                    case R_BUILDING_TAVERN_INDEX:

                        //ADD IN TAVERN SUBTRACT

                        break;
                }

                break;

            case ONGOING_TYPE_EVENT:

                break;

            default:
                break;

            }

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

void DrawEventUI()
{
    CP_Settings_TextSize(20);
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
    //printf("test4");
    //printf("%d", selectedEvent->eventIndex);
    //sprintf_s(textDescBuffer, 100, "%s", currentEvent->description);
    CP_Font_DrawTextBox(selectedEvent->description, windowWidth - 250, 300, 250);
    //printf("test5");
    CP_Image_Draw(*GetBuildingSpriteButtonByIndex(selectedEvent->indexOptionA), optionAPos.x - 5, optionAPos.y, TILEWIDTH, TILEHEIGHT / 2, 255);
    //printf("test6");
    CP_Image_Draw(*GetBuildingSpriteButtonByIndex(selectedEvent->indexOptionB), optionBPos.x + 5, optionBPos.y, TILEWIDTH, TILEHEIGHT / 2, 255);
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

void UI_SetEvent(CARDEVENT* newEvent)
{
    selectedEvent = newEvent;
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
        DrawEventUI();
    }
    else
    {
        
    }
}



// all structs are temporarily initialized to 3 in array
BUTTON AllButtons[3];
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
int CheckMouseColliding(BUTTON array[], CP_Vector mousePos, int isSplashScreenActive, int isSettingsActive)
{
    float mousePosX = mousePos.x;
    float mousePosY = mousePos.y;
    int arraysize = sizeof(array) / sizeof(int);
    int mouseClickFailed = 0;


    for (int i = 0; i < arraysize; i++)
    {
        if (mousePosX >= array[i].xPos && mousePosX <= array[i].xPos + array[i].width &&
            mousePosY >= array[i].yPos && mousePosY <= array[i].yPos + array[i].height &&
            array[i].isSplashScreenActive == isSplashScreenActive &&
            array[i].isSettingsActive == isSettingsActive)
        {
            if (isSplashScreenActive == 1)
            {
                //SplashScreenButtons(array[i].imagename);
                return array[i].index;
            }
            else if (isSettingsActive == 1)
            {
                //SettingsButtons(array[i].imagename);
                return array[i].index;
            }
            else if (isSplashScreenActive == 0 && isSettingsActive == 0)
            {
                //MainButtons(array[i].imagename);
                return array[i].index;
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
    char buffer[100];
    sprintf_s(buffer, 100, "Gold: %d\t\tFood: %d\t\tPopulation: %d\t\tMorale: %d", Get_current_gold(), Get_current_food(), Get_current_population(), (Get_current_morale() + Get_additional_morale()));
    CP_Font_DrawText(buffer, 20, 20);
}

