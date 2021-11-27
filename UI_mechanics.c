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


//Button normalinitialize = {width,height,xPos,yPos,isSplashScreenActive,isSettingActive,index}
BUTTON start_game = { true,100,100,200,200,1,0,START_GAME };




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

extern int rewardIndex;


void InitUI()
{
    windowWidth = (float)CP_System_GetWindowWidth();
    windowHeight = (float)CP_System_GetWindowHeight();
    image_CardBack = CP_Image_Load("./ImperoArtAssets/Impero_CardBack.png");
    image_CardDeck = CP_Image_Load("./ImperoArtAssets/Impero_CardDeck.png");
    
    image_CardFlipped = CP_Image_Load("./ImperoArtAssets/Impero_CardFlip.png");
    image_CardA = CP_Image_Load("./ImperoArtAssets/Impero_CardBlue.png");
    image_CardB = CP_Image_Load("./ImperoArtAssets/Impero_CardRed.png");
    image_redZone = CP_Image_Load("./Assets/RedZone.png");
    image_descbox = CP_Image_Load("./ImperoArtAssets/ResourceBarAssets/Impero_Textbox.png");
    image_descboxcover = CP_Image_Load("./ImperoArtAssets/ResourceBarAssets/Impero_TextboxBG.png");

    cardSeletorPos = CP_Vector_Set(200, 450);
    optionAPos = CP_Vector_Set(cardSeletorPos.x - 43, cardSeletorPos.y);
    optionBPos = CP_Vector_Set(cardSeletorPos.x + 43, cardSeletorPos.y);
    EventCardAnim = (MOVINGSPRITES){ image_CardBack, CP_Vector_Set(130, (windowHeight / 2) + 230), CP_Vector_Set(cardSeletorPos.x, cardSeletorPos.y), 0.6f, 0 };

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
        return UIselectedReward->cardType;
    }
    return 0;
}

void DrawUI_OptionSelector()
{
    // Hovering A
    if (CheckWithinBounds(optionAPos, 90, 243))
    {
        CP_Image_Draw(image_CardA, cardSeletorPos.x, cardSeletorPos.y, 185, 243, 255);
        DrawUI_TextDesc(UIselectedEvent->descriptionA);
        DrawUI_Title(UIselectedEvent->title);
        UI_SetResourceAffected(UIselectedEvent->resourceChangeA);
    }
    // Hovering B
    else if (CheckWithinBounds(optionBPos, 90, 243))
    {
        CP_Image_Draw(image_CardB, cardSeletorPos.x, cardSeletorPos.y, 185, 243, 255);
        DrawUI_TextDesc(UIselectedEvent->descriptionB);
        DrawUI_Title(UIselectedEvent->title);
        UI_SetResourceAffected(UIselectedEvent->resourceChangeB);
    }
    // Not Hovering
    else
    {
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
void DrawUI_Deck()
{
    // Draw back of card
    //CP_Image_Draw(image_CardDeck, windowWidth - 130, (windowHeight / 2) + 260, 228, 309, 255);
}
void DrawUI_TopCard()
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
    CP_Image_Draw(EventCardAnim.image, CP_Math_LerpFloat(EventCardAnim.startingPos.x, EventCardAnim.endingPos.x, EventCardAnim.currentTime / EventCardAnim.totalTime),
        CP_Math_LerpFloat(EventCardAnim.startingPos.y, EventCardAnim.endingPos.y, EventCardAnim.currentTime / EventCardAnim.totalTime), 185, 243, 255);


}
void DrawUI_TopCardTransition()
{
    EventCardAnim.currentTime += CP_System_GetDt();
    EventCardAnim.currentTime = CP_Math_ClampFloat(EventCardAnim.currentTime, 0, EventCardAnim.totalTime);
    CP_Image_Draw(EventCardAnim.image, EaseOutSine(EventCardAnim.startingPos.x, EventCardAnim.endingPos.x, EventCardAnim.currentTime / EventCardAnim.totalTime),
        EaseOutSine(EventCardAnim.startingPos.y, EventCardAnim.endingPos.y, EventCardAnim.currentTime / EventCardAnim.totalTime), 185, 243, 255);

    cardflashTimer = 0;
    cardhighlightTimer[0] = 0;
    cardhighlightTimer[1] = 0;
    cardhighlightTimer[2] = 0;
    cardhighlightTimer[3] = 0;
    cardhighlightTimer[4] = 0;
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
        DrawUI_TextDesc(UIselectedReward->description);
    }
    else
    {
        DrawUI_TextDesc("Click on the card below to claim your reward.");
    }
}
void DrawUI_Textbox()
{
    CP_Image_Draw(image_descboxcover, 260, 190, 455, 270, 255);
    CP_Image_Draw(image_descbox, 260, 190, 455, 270, 255);
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
    CP_Font_DrawTextBox(text, 110, 105, 300);
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

void DrawUI(GAMESTATE state)
{
    switch (state)
    {
    case State_GameEntry:
        break;
    case State_StartOfTurn:
        DrawUI_Textbox();
        DrawUI_Deck();
        DrawUI_TopCard();
        break;
    case State_Idle:
        DrawUI_Textbox();
        DrawUI_Title("Click on the card below to start an event");
        DrawUI_Deck();
        DrawUI_TopCard();
        break;
    case State_CardDraw:
        DrawUI_Textbox();
        DrawUI_Deck();
        DrawUI_TopCardTransition();
        break;
    case State_MakeAChoice:
        DrawUI_Textbox();
        DrawUI_Deck();
        DrawUI_OptionSelector();
        DrawUI_AffectedLand();
        break;
    case State_ResourceChange:
        DrawUI_Textbox();
        DrawUI_Deck();
        DrawUI_SelectedOption();
        break;
    case State_CollectRewards:
        DrawUI_Textbox();
        DrawUI_Deck();
        DrawUI_RewardCards(false);
        break;
    case State_PlaceYourBuilding:
        DrawUI_Textbox();
        DrawUI_Deck();
        DrawUI_RewardCards(true);
        break;
    case State_DestroyBuilding:
        DrawUI_Textbox();
        DrawUI_Deck();
        DrawUI_RewardCards(true);
        break;
    case State_EndOfTurn:
        DrawUI_Textbox();
        DrawUI_Deck();
        EventCardAnim.currentTime = 0;
        break;
    }
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
int resourceAffectedLerp = 0;
bool ralerpinc = true;

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
    sprintf_s(resourceBuffer, 20, "%d", GetDelayedGold());
    CP_Font_DrawText(resourceBuffer, 660, 74);
    
    //Draw Text For current food amount
    sprintf_s(resourceBuffer, 20, "%d", GetDelayedFood());
    CP_Font_DrawText(resourceBuffer, 660, 164);

    //Draw Text for current population amount
    sprintf_s(resourceBuffer, 20, "%d", GetDelayedPop());
    CP_Font_DrawText(resourceBuffer, 660, 254);

    //Draw Text for current morale
    sprintf_s(resourceBuffer, 20, "%d", GetDelayedMorale() + Get_additional_morale());
    CP_Font_DrawText(resourceBuffer, 660, 344);

    //Draw text for number of cards left
    sprintf_s(resourceBuffer, 20, "%d", GetCardsLeft());
    CP_Font_DrawText(resourceBuffer, 505, 769);


}

