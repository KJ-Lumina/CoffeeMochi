/*!_____________________________________________________________________________
@file       UI_mechanics.c
@author     Lee Xin Qian (xinqian.lee@digipen.edu)
@co-authors Travess Tan
@course     CSD1120
@section    B
@team       CoffeeMochi
@brief      This file contains the function definitions that are used in
            the user interfaces of Impero.
*//*__________________________________________________________________________*/


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
MOVINGSPRITES cardDraw;
CP_Image image_CardNormal;
CP_Image image_CardDeck;
CP_Image image_CardFlipped;
CP_Image image_CardA;
CP_Image image_CardB;
CP_Image image_CardGrey;
CP_Image image_CardGold;


CP_Image image_redZone;
CARDEVENT* UIselectedEvent;
REWARDCARD* UIselectedReward;
int UIrewardCardsLeft;
float rewardCardGap = 30;
float windowWidth;
float windowHeight;
CP_Vector optionAPos;
CP_Vector optionBPos;
CP_Vector cardSeletorPos;

int UIselectedChoice;
char textDescBuffer[100];
CP_Image image_descbox;
CP_Image image_descboxcover;

//Gold Card
CP_Image blessCard;
MOVINGSPRITES blessingCard;
float blessFill = 0;
float blessTimer = 0;



//Button normalinitialize = {width,height,xPos,yPos,isSplashScreenActive,isSettingActive,index}
BUTTON start_game = { true,100,100,200,200,1,0,START_GAME };


CP_Image settingsButton;
CP_Image settingsButtonHover;

// reward card effect, to be optimized later
CP_Image image_CardFlash;
CP_Image image_CardHighlight;
float cardflashTimer = 0;
float cardhighlightTimer[5];

// resource bars
CP_Image image_ResourceBars;
CP_Image image_goldbar;
CP_Image image_foodbar;
CP_Image image_populationbar;
CP_Image image_moralebar;
CP_Image image_barBGgold;
CP_Image image_barBGfood;
CP_Image image_barBGpop;
CP_Image image_barBGmorale;


int nullResourceChange[4] = { 0 };
bool goldAffected = false;
bool foodAffected = false;
bool popAffected = false;
bool moraleAffected = false;

char resourceBuffer[20];
int resourceAffectedLerp = 0;
bool ralerpinc = true;

extern int rewardIndex;


void InitUI()
{
    windowWidth = (float)CP_System_GetWindowWidth();
    windowHeight = (float)CP_System_GetWindowHeight();
    image_CardNormal = CP_Image_Load("./ImperoArtAssets/Impero_CardBack.png");
    image_CardDeck = CP_Image_Load("./ImperoArtAssets/Impero_CardDeck.png");
    image_CardGrey = CP_Image_Load("./ImperoArtAssets/Impero_CardGrey.png");
    image_CardGold = CP_Image_Load("./ImperoArtAssets/Impero_CardGold.png");

    image_CardFlipped = CP_Image_Load("./ImperoArtAssets/Impero_CardFlip.png");
    image_CardA = CP_Image_Load("./ImperoArtAssets/Impero_CardBlue.png");
    image_CardB = CP_Image_Load("./ImperoArtAssets/Impero_CardRed.png");
    image_redZone = CP_Image_Load("./Assets/RedZone.png");
    image_descbox = CP_Image_Load("./ImperoArtAssets/ResourceBarAssets/Impero_Textbox.png");
    image_descboxcover = CP_Image_Load("./ImperoArtAssets/ResourceBarAssets/Impero_TextboxBG.png");

    settingsButton = CP_Image_Load("./ImperoArtAssets/ResourceBarAssets/Impero_SettingsButton.png");
    settingsButtonHover = CP_Image_Load("./ImperoArtAssets/ResourceBarAssets/Impero_SettingsButtonHover.png");

    cardSeletorPos = CP_Vector_Set(250, 450);
    optionAPos = CP_Vector_Set(cardSeletorPos.x - 43, cardSeletorPos.y);
    optionBPos = CP_Vector_Set(cardSeletorPos.x + 43, cardSeletorPos.y);
    cardDraw = (MOVINGSPRITES){ image_CardNormal, CP_Vector_Set(130, (windowHeight / 2) + 230), CP_Vector_Set(cardSeletorPos.x, cardSeletorPos.y), 0.6f, 1 };
    blessingCard = (MOVINGSPRITES){ image_CardNormal, CP_Vector_Set(-100, cardSeletorPos.y), CP_Vector_Set(100, cardSeletorPos.y), 0.6f, 0 };

    image_CardFlash = CP_Image_Load("./ImperoArtAssets/Impero_Cardflash.png");
    image_CardHighlight = CP_Image_Load("./ImperoArtAssets/Impero_Cardhighlight.png");

    image_goldbar = CP_Image_Load("./ImperoArtAssets/ResourceBarAssets/Impero_GoldBar.png");
    image_foodbar = CP_Image_Load("./ImperoArtAssets/ResourceBarAssets/Impero_FoodBar.png");
    image_populationbar = CP_Image_Load("./ImperoArtAssets/ResourceBarAssets/Impero_PopBar.png");
    image_moralebar = CP_Image_Load("./ImperoArtAssets/ResourceBarAssets/Impero_MoraleBar.png");

    image_barBGgold = CP_Image_Load("./ImperoArtAssets/ResourceBarAssets/Impero_GoldBarBG.png");
    image_barBGfood = CP_Image_Load("./ImperoArtAssets/ResourceBarAssets/Impero_FoodBarBG.png");
    image_barBGpop = CP_Image_Load("./ImperoArtAssets/ResourceBarAssets/Impero_PopBarBG.png");
    image_barBGmorale = CP_Image_Load("./ImperoArtAssets/ResourceBarAssets/Impero_MoraleBarBG.png");
    blessFill = 0;
    blessTimer = 0;
}
void UI_SetEvent(CARDEVENT* newEvent)
{
    UIselectedEvent = newEvent;
}
void UI_SetReward(REWARDCARD* rewardCard, int cardsLeft)
{
    UIselectedReward = rewardCard;
    UIrewardCardsLeft = abs(cardsLeft);
}
void UI_SetResourceAffected(int resourceChange[4])
{
    goldAffected = (bool)resourceChange[0];
    foodAffected = (bool)resourceChange[1];
    popAffected = (bool)resourceChange[2];
    moraleAffected = (bool)resourceChange[3];
}
void UI_SetBlessingsTimer(float timer)
{
    blessTimer = timer;
}
void UI_SetGoldCard()
{
    cardDraw.image = image_CardGold;
}
void UI_SetNormalCard()
{
    cardDraw.image = image_CardNormal;
}

bool ClickCheck_CardDraw()
{
    if (CheckWithinBounds(cardDraw.startingPos, 185, 243))
    {
        return true;
    }
    return false;
}
int ClickCheck_CardChoice()
{
    if (CheckWithinBounds(optionAPos, 120, 320))
    {
        UIselectedChoice = BLUE_PILL;
        cardhighlightTimer[0] = 1;
        UI_SetResourceAffected(nullResourceChange);
        return UIselectedChoice;
    }
    else if (CheckWithinBounds(optionBPos, 120, 320))
    {
        UIselectedChoice = RED_PILL;
        cardhighlightTimer[0] = 1;
        UI_SetResourceAffected(nullResourceChange);
        return UIselectedChoice;
    }
    return 0;
}
int ClickCheck_Rewards()
{
    if (CheckWithinBounds(CP_Vector_Set(cardSeletorPos.x + 15.0f - 15.0f * UIrewardCardsLeft, cardSeletorPos.y), 150 + UIrewardCardsLeft * rewardCardGap, 240))
    {
        --UIrewardCardsLeft;
        Play_SFX_Sound(Sound_SFX_Click);
        return UIselectedReward->cardType;
    }
    return 0;
}

void DrawUI_OptionSelector()
{
    // Hovering A
    if (CheckWithinBounds(optionAPos, 90, 243))
    {
        Play_Choice_SFX(Sound_SFX_Choice_LHS);
        CP_Image_Draw(image_CardA, cardSeletorPos.x, cardSeletorPos.y, 185, 243, 255);
        DrawUI_TextDesc(UIselectedEvent->descriptionA);
        DrawUI_Title(UIselectedEvent->title);
        UI_SetResourceAffected(UIselectedEvent->resourceChangeA);
    }
    // Hovering B
    else if (CheckWithinBounds(optionBPos, 90, 243))
    {
        Play_Choice_SFX(Sound_SFX_Choice_RHS);
        CP_Image_Draw(image_CardB, cardSeletorPos.x, cardSeletorPos.y, 185, 243, 255);
        DrawUI_TextDesc(UIselectedEvent->descriptionB);
        DrawUI_Title(UIselectedEvent->title);
        UI_SetResourceAffected(UIselectedEvent->resourceChangeB);
    }
    // Not Hovering
    else
    {
        StopSFX();
        CP_Image_Draw(image_CardFlipped, cardSeletorPos.x, cardSeletorPos.y, 185, 243, 255);
        DrawUI_TextDesc(UIselectedEvent->description);
        DrawUI_Title(UIselectedEvent->title);
        UI_SetResourceAffected(nullResourceChange);
    }
}
void DrawUI_SelectedOption()
{
    cardhighlightTimer[0] -= CP_System_GetDt();
    switch (UIselectedChoice)
    {
    case 1:
        CP_Image_Draw(image_CardA, cardSeletorPos.x, cardSeletorPos.y, 185, 243, 255);
        CP_Image_Draw(image_CardHighlight, cardSeletorPos.x, cardSeletorPos.y, 185, 243, CP_Math_LerpInt(0, 180, cardhighlightTimer[0]));
        DrawUI_Title(UIselectedEvent->title);
        DrawUI_TextDesc(UIselectedEvent->descriptionA);
        break;
    case 2:
        CP_Image_Draw(image_CardB, cardSeletorPos.x, cardSeletorPos.y, 185, 243, 255);
        CP_Image_Draw(image_CardHighlight, cardSeletorPos.x, cardSeletorPos.y, 185, 243, CP_Math_LerpInt(0, 180, cardhighlightTimer[0]));
        DrawUI_Title(UIselectedEvent->title);
        DrawUI_TextDesc(UIselectedEvent->descriptionB);
        break;
    }
}
void DrawUI_TopCard()
{
    // Hovering Deck
    if (CheckWithinBounds(cardDraw.startingPos, 185, 243))
    {
        cardDraw.currentTime += CP_System_GetDt();
    }
    else
    {
        cardDraw.currentTime -= CP_System_GetDt();
    }
    cardDraw.currentTime = CP_Math_ClampFloat(cardDraw.currentTime, 0, cardDraw.totalTime / 8);
    CP_Image_Draw(cardDraw.image, CP_Math_LerpFloat(cardDraw.startingPos.x, cardDraw.endingPos.x, cardDraw.currentTime / cardDraw.totalTime),
        CP_Math_LerpFloat(cardDraw.startingPos.y, cardDraw.endingPos.y, cardDraw.currentTime / cardDraw.totalTime), 185, 243, 255);


}
void DrawUI_cardDraw(bool entry)
{
    if (entry)
    {
        cardDraw.currentTime -= CP_System_GetDt();
        cardDraw.currentTime = CP_Math_ClampFloat(cardDraw.currentTime, 0, 0.5);
        CP_Image_Draw(cardDraw.image, cardDraw.startingPos.x,
            EaseInSine(cardDraw.startingPos.y, 1000, cardDraw.currentTime / 0.5f), 185, 243, 255);

    }
    else
    {
        cardDraw.currentTime += CP_System_GetDt();
        cardDraw.currentTime = CP_Math_ClampFloat(cardDraw.currentTime, 0, cardDraw.totalTime);
        CP_Image_Draw(cardDraw.image, EaseOutSine(cardDraw.startingPos.x, cardDraw.endingPos.x, cardDraw.currentTime / cardDraw.totalTime),
            EaseOutSine(cardDraw.startingPos.y, cardDraw.endingPos.y, cardDraw.currentTime / cardDraw.totalTime), 185, 243, 255);

        cardflashTimer = 0;
        cardhighlightTimer[0] = 0;
        cardhighlightTimer[1] = 0;
        cardhighlightTimer[2] = 0;
        cardhighlightTimer[3] = 0;
        cardhighlightTimer[4] = 0;
    }
}
void DrawUI_RewardCards(bool rewardPicked)
{
    float offsetX = -(abs(UIrewardCardsLeft) - 1) * rewardCardGap / 2;
    float deltaTime = CP_System_GetDt();
    for (int i = 0; i < abs(UIrewardCardsLeft); ++i)
    {
        CP_Image_Draw(*GetCardSpriteByType(UIselectedReward->cardType), cardSeletorPos.x + offsetX + rewardCardGap * i, cardSeletorPos.y, 185, 243, 255);
        cardflashTimer += deltaTime;
        CP_Image_Draw(image_CardFlash, cardSeletorPos.x + offsetX + rewardCardGap * i, cardSeletorPos.y, 185, 243, CP_Math_LerpInt(255, 0, cardflashTimer));
        if (i != abs(UIrewardCardsLeft) - 1)
        {
            if (CheckWithinBounds(CP_Vector_Set(cardSeletorPos.x + offsetX + rewardCardGap * i - 77.5f, cardSeletorPos.y), 30, 243) && cardhighlightTimer[i] <= 1)
            {
                cardhighlightTimer[i] += deltaTime;
            }
            else if (cardhighlightTimer[i] > 0)
            {
                cardhighlightTimer[i] -= deltaTime;
            }
            CP_Image_Draw(image_CardHighlight, cardSeletorPos.x + offsetX + rewardCardGap * i, cardSeletorPos.y, 185, 243, CP_Math_LerpInt(0, 180, cardhighlightTimer[i] * 2));
        }
        // First reward card
        else
        {
            if (CheckWithinBounds(CP_Vector_Set(cardSeletorPos.x + offsetX + rewardCardGap * i, cardSeletorPos.y), 185, 243) && cardhighlightTimer[i] <= 1)
            {
                cardhighlightTimer[i] += deltaTime;
            }
            else if (cardhighlightTimer[i] > 0)
            {
                cardhighlightTimer[i] -= deltaTime;
            }
            CP_Image_Draw(image_CardHighlight, cardSeletorPos.x + offsetX + rewardCardGap * i, cardSeletorPos.y, 185, 243, CP_Math_LerpInt(0, 180, cardhighlightTimer[i] * 2));
        }
    }

    if (rewardPicked)
    {
        DrawUI_TextDesc(UIselectedReward->collectedDesc);
    }
    else
    {
        DrawUI_TextDesc(UIselectedReward->rewardDesc);
    }
}

void DrawUI_EventResult()
{
    DrawUI_Title(UIselectedEvent->title);
    if (UIselectedChoice == 1)
    {
        DrawUI_TextDesc(UIselectedEvent->resultDescA);
    }
    else
    {
        DrawUI_TextDesc(UIselectedEvent->resultDescB);
    }
}

void DrawUI_Textbox()
{
    CP_Image_Draw(image_descboxcover, 260, 190, 455, 270, 255);
    CP_Image_Draw(image_descbox, 260, 190, 455, 270, 255);
}

void DrawUI_Settings() {

    if (CheckWithinBounds(CP_Vector_Set(1540, 60), 98, 93)) {
        CP_Image_Draw(settingsButtonHover, 1540, 60, 98, 93, 255);
        if (CP_Input_MouseClicked()) {
            Play_SFX_Sound(Sound_SFX_Click);
            OpenOptions();
        }
    }
    else {
        CP_Image_Draw(settingsButton, 1540, 60, 98, 93, 255);
    }

}


void DrawUI_TextDesc(const char* text)
{
    CP_Settings_TextSize(30);
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
    CP_Font_DrawTextBox(text, 110, 155, 300);
}


void DrawUI_Title(const char* text)
{
    CP_Settings_TextSize(50);
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
    CP_Font_DrawTextBox(text, 100, 105, 330);
}
void DrawUI_AffectedLand()
{
    static float alphaLerp = 0;
    static bool inc = true;
    int affectedIndex = 0;
    if (inc)
    {
        alphaLerp += CP_System_GetDt() * 200;
        if (alphaLerp > 180)
            inc = false;
    }
    else
    {
        alphaLerp -= CP_System_GetDt() * 200;
        if (alphaLerp < 0)
            inc = true;
    }
    switch (UIselectedEvent->affectedLand[0])
    {
    case 0:
        break;
    case 26:
        CP_Image_Draw(image_redZone, 800 + MAPOFFSETX, 450 + MAPOFFSETY, 640, 640, (int)alphaLerp);
        break;
    default:
        while (UIselectedEvent->affectedLand[affectedIndex] != 0)
        {
            CP_Image_Draw(image_redZone, ((float)((UIselectedEvent->affectedLand[affectedIndex] - 1) % WORLDGRIDY) - (WORLDGRIDX) / 2) * TILEWIDTH + MAPOFFSETX + 800,
                    ((float)((UIselectedEvent->affectedLand[affectedIndex] - 1) / WORLDGRIDY) - (WORLDGRIDX) / 2) * TILEHEIGHT + 450 + MAPOFFSETY, TILEWIDTH, TILEHEIGHT, (int)alphaLerp);
            affectedIndex++;
        }

        break;
    }
}
void DrawUI_BlessFill()
{
    if (blessTimer <= 0)
    {
        return;
    }
    else if (blessTimer > 1)
    {
        blessTimer -= CP_System_GetDt();
        blessingCard.currentTime += CP_System_GetDt();
        blessingCard.currentTime = CP_Math_ClampFloat(blessingCard.currentTime, 0, blessingCard.totalTime);

        //gold card
        CP_Image_Draw(image_CardGold, EaseInSine(blessingCard.startingPos.x, blessingCard.endingPos.x, blessingCard.currentTime / blessingCard.totalTime),
            EaseInSine(blessingCard.startingPos.y, blessingCard.endingPos.y, blessingCard.currentTime / blessingCard.totalTime), 185, 243, 255);
        //greyscale
        CP_Image_DrawSubImage(image_CardGrey, EaseInSine(blessingCard.startingPos.x, blessingCard.endingPos.x, blessingCard.currentTime / blessingCard.totalTime),
            EaseInSine(blessingCard.startingPos.y - Get_current_blessing() * 1.215f, blessingCard.endingPos.y - Get_current_blessing() * 1.215f, blessingCard.currentTime / blessingCard.totalTime),
            185, 243 - Get_current_blessing() * 2.43f, 0, 0, 185, 243 - Get_current_blessing() * 2.43f, 255);
    }
    else
    {
        blessTimer -= CP_System_GetDt();
        blessingCard.currentTime -= CP_System_GetDt();
        blessingCard.currentTime = CP_Math_ClampFloat(blessingCard.currentTime, 0, blessingCard.totalTime);
        //gold card
        CP_Image_Draw(image_CardGold, EaseInSine(blessingCard.startingPos.x, blessingCard.endingPos.x, blessingCard.currentTime / blessingCard.totalTime),
            EaseInSine(blessingCard.startingPos.y, blessingCard.endingPos.y, blessingCard.currentTime / blessingCard.totalTime), 185, 243, 255);
        //grey scale
        CP_Image_DrawSubImage(image_CardGrey, EaseInSine(blessingCard.startingPos.x, blessingCard.endingPos.x, blessingCard.currentTime / blessingCard.totalTime),
            EaseInSine(blessingCard.startingPos.y - Get_current_blessing() * 1.215f, blessingCard.endingPos.y - Get_current_blessing() * 1.215f, blessingCard.currentTime / blessingCard.totalTime),
            185, 243 - Get_current_blessing() * 2.43f, 0, 0, 185, 243 - Get_current_blessing() * 2.43f, 255);
    }
}

void DrawUI(GAMESTATE state)
{
    switch (state)
    {
    case State_GameEntry:
        DrawUI_Settings();
        DrawUI_Textbox();
        DrawUI_BlessFill(false);
        break;
    case State_StartOfTurn:
        DrawUI_Settings();
        DrawUI_Textbox();
        DrawUI_BlessFill(false);
        DrawUI_cardDraw(true);
        break;
    case State_Idle:
        DrawUI_Settings();
        DrawUI_Textbox();
        DrawUI_Title("Click on the card below to start an event");
        DrawUI_TopCard();
        break;
    case State_CardDraw:
        DrawUI_Settings();
        DrawUI_Textbox();
        DrawUI_cardDraw(false);
        break;
    case State_MakeAChoice:
        DrawUI_Settings();
        DrawUI_Textbox();
        DrawUI_OptionSelector();
        DrawUI_AffectedLand();
        break;
    case State_ResourceChange:
        DrawUI_Settings();
        DrawUI_Textbox();
        DrawUI_SelectedOption();
        break;
    case State_CollectRewards:
        DrawUI_Settings();
        DrawUI_Textbox();
        DrawUI_RewardCards(false);
        break;
    case State_PlaceYourBuilding:
        DrawUI_Settings();
        DrawUI_Textbox();
        DrawUI_RewardCards(true);
        break;
    case State_DestroyBuilding:
        DrawUI_Settings();
        DrawUI_Textbox();
        DrawUI_RewardCards(true);
        break;
    case State_EndOfTurn:
        DrawUI_Settings();
        DrawUI_Textbox();
        DrawUI_EventResult();
        DrawUI_BlessFill(true);
        cardDraw.currentTime = 1;
        break;
    }
    //independent draws
    DrawUI_BlessFill();
}

BUTTON AllButtons[BUTTONARRAYSIZE];


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

//un-used code, initialize buttons
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

//un-used code, initial skeleton function for arranging button collisions
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
                    return buttonArray[i].index;
                }
                else if (isSettingsActive == 1)
                {
                    return buttonArray[i].index;
                }
                else if (isSplashScreenActive == 0 && isSettingsActive == 0)
                {
                    return buttonArray[i].index;
                }
            }
        }
    }
    return mouseClickFailed;
}



void DrawTempTextResources()
{
    if (ralerpinc)
    {
        resourceAffectedLerp += 10;
        if (resourceAffectedLerp >= 255)
        {
            ralerpinc = false;
        }
    }
    else
    {
        resourceAffectedLerp -= 10;
        if (resourceAffectedLerp <= 60)
        {
            ralerpinc = true;
        }
    }

    //bar background
    CP_Image_Draw(image_barBGgold, 615, 90, 200, 60, 255);
    CP_Image_Draw(image_barBGfood, 615, 180, 200, 60, 255);
    CP_Image_Draw(image_barBGpop, 615, 270, 200, 60, 255);
    CP_Image_Draw(image_barBGmorale, 615, 360, 200, 60, 255);

    float maxBarSize = 380;
    //resource bar
    if (goldAffected)
    {
        CP_Image_Draw(image_goldbar, 520, 90, CP_Math_LerpFloat(0, maxBarSize, (float)GetDelayedGold() / 200), 60, resourceAffectedLerp);
    }
    else
    {
        CP_Image_Draw(image_goldbar, 520, 90, CP_Math_LerpFloat(0, maxBarSize, (float)GetDelayedGold() / 200), 60, 255);

    }
    if (foodAffected)
    {
        CP_Image_Draw(image_foodbar, 520, 180, CP_Math_LerpFloat(0, maxBarSize, (float)GetDelayedFood() / 200), 60, resourceAffectedLerp);
    }
    else
    {
        CP_Image_Draw(image_foodbar, 520, 180, CP_Math_LerpFloat(0, maxBarSize, (float)GetDelayedFood() / 200), 60, 255);
    }
    if (popAffected)
    {
        CP_Image_Draw(image_populationbar, 520, 270, CP_Math_LerpFloat(0, maxBarSize, (float)GetDelayedPop() / 200), 60, resourceAffectedLerp);
    }
    else
    {
        CP_Image_Draw(image_populationbar, 520, 270, CP_Math_LerpFloat(0, maxBarSize, (float)GetDelayedPop() / 200), 60, 255);
    }
    if (moraleAffected)
    {
        CP_Image_Draw(image_moralebar, 520, 360, CP_Math_LerpFloat(0, maxBarSize, (float)GetDelayedMorale() / 200), 60, resourceAffectedLerp);
    }
    else
    {
        CP_Image_Draw(image_moralebar, 520, 360, CP_Math_LerpFloat(0, maxBarSize, (float)GetDelayedMorale() / 200), 60, 255);
    }




    CP_Settings_TextSize(40);
    

    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));

    //Draw Text for current gold amount
    sprintf_s(resourceBuffer, 20, "%d", Get_current_gold());
    CP_Font_DrawText(resourceBuffer, 660, 74);
    
    //Draw Text For current food amount
    sprintf_s(resourceBuffer, 20, "%d", Get_current_food());
    CP_Font_DrawText(resourceBuffer, 660, 164);

    //Draw Text for current population amount
    sprintf_s(resourceBuffer, 20, "%d", Get_current_population());
    CP_Font_DrawText(resourceBuffer, 660, 254);

    //Draw Text for current morale
    sprintf_s(resourceBuffer, 20, "%d", Get_current_morale() + Get_additional_morale());
    CP_Font_DrawText(resourceBuffer, 660, 344);

    //Draw text for number of cards left
    sprintf_s(resourceBuffer, 20, "%d", GetCardsLeft());
    CP_Font_DrawText(resourceBuffer, 505, 769);


}

