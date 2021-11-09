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
            //Check for Pre-Requiste 
            switch (selectedEvent->costTypeA)
            {
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
            //Check for Pre-Requiste 
            switch (selectedEvent->costTypeB)
            {
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
    sprintf_s(buffer, 100, "Gold: %d\t\tFood: %d\t\tPopulation: %d\t\tMorale: %d", Get_current_gold(), Get_current_food(), Get_current_population(), (Get_current_morale() + Get_additional_morale()));
    CP_Font_DrawText(buffer, 20, 20);
}




