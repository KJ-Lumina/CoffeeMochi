#include <stdio.h>
#include <stdbool.h>
#include <math.h>
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
CP_Image image_CardDeck;
CP_Image image_CardFlipped;
CP_Image image_CardA;
CP_Image image_CardB;

CARDEVENT* UIselectedEvent;
REWARDCARD* UIselectedReward;
int UIrewardCardsLeft;
float rewardCardGap = 30;
float windowWidth;
float windowHeight;
CP_Vector optionAPos;
CP_Vector optionBPos;
bool IsAViable;
bool IsBViable;
char textDescBuffer[100];


//Button normalinitialize = {width,height,xPos,yPos,isSplashScreenActive,isSettingActive,index}
BUTTON start_game = { true,100,100,200,200,1,0,START_GAME };




// reward card effect, to be optimized later
CP_Image image_CardFlash;
CP_Image image_CardHighlight;
float cardflashTimer = 0;
float cardhighlightTimer[5];

// resource bars
CP_Image image_ResourceBars;

extern int rewardIndex;


void InitUI()
{
    windowWidth = (float)CP_System_GetWindowWidth();
    windowHeight = (float)CP_System_GetWindowHeight();
    image_CardBack = CP_Image_Load("./ImperoArtAssets/Impero_CardBack.png");
    image_CardDeck = CP_Image_Load("./ImperoArtAssets/Impero_CardDeck.png");
    EventCardAnim = (MOVINGSPRITES){ image_CardBack, CP_Vector_Set(windowWidth - 130, (windowHeight / 2) + 230), CP_Vector_Set(windowWidth - 130, (windowHeight / 2) - 60), 0.6f, 0 };
    image_CardFlipped = CP_Image_Load("./ImperoArtAssets/Impero_CardFlip.png");
    image_CardA = CP_Image_Load("./ImperoArtAssets/Impero_CardBlue.png");
    image_CardB = CP_Image_Load("./ImperoArtAssets/Impero_CardRed.png");

    optionAPos = CP_Vector_Set(windowWidth - 176, windowHeight / 2 - 60);
    optionBPos = CP_Vector_Set(windowWidth - 90, windowHeight / 2 - 60);

    image_CardFlash = CP_Image_Load("./ImperoArtAssets/Impero_Cardflash.png");
    image_CardHighlight = CP_Image_Load("./ImperoArtAssets/Impero_Cardhighlight.png");
    image_ResourceBars = CP_Image_Load("./ImperoArtAssets/ResourceBarAssets/Impero_ResourceBars.png");
}

void UI_SetEvent(CARDEVENT* newEvent)
{
    UIselectedEvent = newEvent;
    IsAViable = true;   // IsCostPayable(selectedEvent->resourceChangeA[0]);
    IsBViable = true;   // IsCostPayable(selectedEvent->resourceChangeB[0]);
}

void UI_SetReward(REWARDCARD* rewardCard, int cardsLeft)
{
    UIselectedReward = rewardCard;
    UIrewardCardsLeft = cardsLeft;
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
            return BLUE_PILL;
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
            return RED_PILL;
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
    if (CheckWithinBounds(CP_Vector_Set(1485 - 15.0f * UIrewardCardsLeft, 390), 150 + UIrewardCardsLeft * rewardCardGap, 240))
    {
        if (UIselectedReward->cardType == BUILD_TYPE_EVENT)
        {
            --UIrewardCardsLeft;
            return 1;
        }
        else if (UIselectedReward->cardType == ONGOING_TYPE_EVENT)
        {
            --UIrewardCardsLeft;
            return 2;
        }
    }
    return 0;
}

void DrawUI_GauntletOpen()
{
    // Hovering A
    if (CheckWithinBounds(optionAPos, 90, 243))
    {
        CP_Image_Draw(image_CardA, windowWidth - 130, windowHeight / 2 - 60, 185, 243, 255);
        CP_Settings_TextSize(20);
        CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
        CP_Font_DrawTextBox(UIselectedEvent->descriptionA, 1330, 140, 250);
    }
    // Hovering B
    else if (CheckWithinBounds(optionBPos, 90, 243))
    {
        CP_Image_Draw(image_CardB, windowWidth - 130, windowHeight / 2 - 60, 185, 243, 255);
        CP_Settings_TextSize(20);
        CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
        CP_Font_DrawTextBox(UIselectedEvent->descriptionB, 1330, 140, 250);
    }
    // Not Hovering
    else
    {
        CP_Image_Draw(image_CardFlipped, windowWidth - 130, windowHeight / 2 - 60, 185, 243, 255);
        CP_Settings_TextSize(20);
        CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
        CP_Font_DrawTextBox(UIselectedEvent->description, 1330, 140, 250);
    }
}

void DrawUI_Deck()
{
    // Draw back of card
    CP_Image_Draw(image_CardDeck, windowWidth - 130, (windowHeight / 2) + 260, 228, 309, 255);
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

void DrawUI_RewardCards(bool rewardPicked)
{
    float offsetX = -(abs(UIrewardCardsLeft) - 1) * rewardCardGap / 2;
    float deltaTime = CP_System_GetDt();
    for (int i = 0; i < abs(UIrewardCardsLeft); ++i)
    {
        CP_Image_Draw(*GetCardSpriteByIndex(UIselectedReward->eventIndex), 1470 + offsetX + rewardCardGap * i, 390, 185, 243, 255);
        cardflashTimer += deltaTime;
        CP_Image_Draw(image_CardFlash, 1470 + offsetX + rewardCardGap * i, 390, 185, 243, CP_Math_LerpInt(255, 0, cardflashTimer));
        if (i != abs(UIrewardCardsLeft) - 1)
        {
            if (CheckWithinBounds(CP_Vector_Set(1470 + offsetX + rewardCardGap * i - 77.5f, 390), 30, 243) && cardhighlightTimer[i] <= 1)
            {
                cardhighlightTimer[i] += deltaTime;
            }
            else if (cardhighlightTimer[i] > 0)
            {
                cardhighlightTimer[i] -= deltaTime;
            }
            CP_Image_Draw(image_CardHighlight, 1470 + offsetX + rewardCardGap * i, 390, 185, 243, CP_Math_LerpInt(0, 180, cardhighlightTimer[i] * 2));
        }
        // First reward card
        else
        {
            if (CheckWithinBounds(CP_Vector_Set(1470 + offsetX + rewardCardGap * i, 390), 185, 243) && cardhighlightTimer[i] <= 1)
            {
                cardhighlightTimer[i] += deltaTime;
            }
            else if (cardhighlightTimer[i] > 0)
            {
                cardhighlightTimer[i] -= deltaTime;
            }
            CP_Image_Draw(image_CardHighlight, 1470 + offsetX + rewardCardGap * i, 390, 185, 243, CP_Math_LerpInt(0, 180, cardhighlightTimer[i] * 2));
        }
    }

    if (rewardPicked)
    {
        CP_Settings_TextSize(20);
        CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
        CP_Font_DrawTextBox(UIselectedReward->description, 1330, 140, 250);
    }
    else
    {
        CP_Settings_TextSize(20);
        CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
        CP_Font_DrawTextBox("Click on the card below to claim your reward.", 1330, 140, 250);
    }
}

void DrawUI(GAMESTATE state)
{
    switch (state)
    {
    case State_StartOfTurn:
        DrawUI_Deck();
        DrawUI_TopPile();
        break;
    case State_Idle:
        DrawUI_Deck();
        DrawUI_TopPile();
        break;
    case State_CardDraw:
        DrawUI_Deck();
        DrawUI_TopPileInsert();
        cardflashTimer = 0;
        cardhighlightTimer[0] = 0;
        cardhighlightTimer[1] = 0;
        cardhighlightTimer[2] = 0;
        cardhighlightTimer[3] = 0;
        cardhighlightTimer[4] = 0;
        break;
    case State_MakeAChoice:
        DrawUI_Deck();
        DrawUI_GauntletOpen();
        break;
    case State_CollectRewards:
        DrawUI_Deck();
        DrawUI_RewardCards(false);
        break;
    case State_PlaceYourBuilding:
        DrawUI_Deck();
        DrawUI_RewardCards(true);
        break;
    case State_DestroyBuilding:
        DrawUI_Deck();
        DrawUI_RewardCards(true);
        break;
    case State_EndOfTurn:
        DrawUI_Deck();
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

char resourceBuffer[20];
void DrawTempTextResources()
{
    CP_Image_Draw(image_ResourceBars, 800, 450, 1600, 900, 255);


    CP_Settings_TextSize(40);
    

    if (Get_current_gold() < 0)
    {
        CP_Settings_Fill(CP_Color_Create(200, 0, 0, 255));
        sprintf_s(resourceBuffer, 20, "(In Debt)");
        CP_Font_DrawText(resourceBuffer, 60, 68);
    }

    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));

    sprintf_s(resourceBuffer, 20, "%d", Get_current_gold());
    CP_Font_DrawText(resourceBuffer, 150, 68);

    

    sprintf_s(resourceBuffer, 20, "%d", Get_current_food());
    CP_Font_DrawText(resourceBuffer, 150, 156);

    sprintf_s(resourceBuffer, 20, "%d", Get_current_population());
    CP_Font_DrawText(resourceBuffer, 150, 243);

    sprintf_s(resourceBuffer, 20, "%d", Get_current_morale() + Get_additional_morale());
    CP_Font_DrawText(resourceBuffer, 150, 332);


    // LUL
    sprintf_s(resourceBuffer, 20, "Cards Left: %d", GetCardsLeft());
    CP_Font_DrawText(resourceBuffer, 1500, 850);


}

