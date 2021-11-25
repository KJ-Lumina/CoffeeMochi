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
bool IsAViable;
bool IsBViable;
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
CP_Image image_barBG;
CP_Image image_resourcetext;
float goldLerp;
float foodLerp;
float popuLerp;
float moraleLerp;

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
    image_redZone = CP_Image_Load("./Assets/RedZone.png");
    image_descbox = CP_Image_Load("./ImperoArtAssets/Impero_Textbox.png");
    image_descboxcover = CP_Image_Load("./ImperoArtAssets/textboxcover.png");

    optionAPos = CP_Vector_Set(windowWidth - 176, windowHeight / 2 - 60);
    optionBPos = CP_Vector_Set(windowWidth - 90, windowHeight / 2 - 60);

    image_CardFlash = CP_Image_Load("./ImperoArtAssets/Impero_Cardflash.png");
    image_CardHighlight = CP_Image_Load("./ImperoArtAssets/Impero_Cardhighlight.png");
    image_ResourceBars = CP_Image_Load("./ImperoArtAssets/ResourceBarAssets/Impero_ResourceBars.png");
    image_goldbar = CP_Image_Load("./ImperoArtAssets/ResourceBarAssets/goldbar.png");
    image_foodbar = CP_Image_Load("./ImperoArtAssets/ResourceBarAssets/foodbar.png");
    image_populationbar = CP_Image_Load("./ImperoArtAssets/ResourceBarAssets/populationbar.png");
    image_moralebar = CP_Image_Load("./ImperoArtAssets/ResourceBarAssets/moralebar.png");
    image_barBG = CP_Image_Load("./ImperoArtAssets/ResourceBarAssets/barBackground.png");
    image_resourcetext = CP_Image_Load("./ImperoArtAssets/ResourceBarAssets/resourcetext.png");
    goldLerp = 0.0f;
    foodLerp = -0.2f;
    popuLerp = -0.4f;
    moraleLerp = -0.4f;
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
    UIrewardCardsLeft = abs(cardsLeft);
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
        return UIselectedChoice;
    }
    else if (CheckWithinBounds(optionBPos, 120, 320))
    {
        UIselectedChoice = RED_PILL;
        cardhighlightTimer[0] = 1;
        return UIselectedChoice;
    }
    return 0;
}
int ClickCheck_Rewards()
{
    if (CheckWithinBounds(CP_Vector_Set(1485 - 15.0f * UIrewardCardsLeft, 390), 150 + UIrewardCardsLeft * rewardCardGap, 240))
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
        CP_Image_Draw(image_CardA, windowWidth - 130, windowHeight / 2 - 60, 185, 243, 255);
        DrawUI_TextDesc(UIselectedEvent->descriptionA);
        DrawUI_Title(UIselectedEvent->title);
    }
    // Hovering B
    else if (CheckWithinBounds(optionBPos, 90, 243))
    {
        CP_Image_Draw(image_CardB, windowWidth - 130, windowHeight / 2 - 60, 185, 243, 255);
        DrawUI_TextDesc(UIselectedEvent->descriptionB);
        DrawUI_Title(UIselectedEvent->title);
    }
    // Not Hovering
    else
    {
        CP_Image_Draw(image_CardFlipped, windowWidth - 130, windowHeight / 2 - 60, 185, 243, 255);
        DrawUI_TextDesc(UIselectedEvent->description);
        DrawUI_Title(UIselectedEvent->title);
    }
}
void DrawUI_SelectedOption()
{
    cardhighlightTimer[0] -= CP_System_GetDt();
    switch (UIselectedChoice)
    {
    case 1:
        CP_Image_Draw(image_CardA, windowWidth - 130, windowHeight / 2 - 60, 185, 243, 255);
        CP_Image_Draw(image_CardHighlight, windowWidth - 130, windowHeight / 2 - 60, 185, 243, CP_Math_LerpInt(0, 180, cardhighlightTimer[0]));
        DrawUI_Title(UIselectedEvent->title);
        DrawUI_TextDesc(UIselectedEvent->descriptionA);
        break;
    case 2:
        CP_Image_Draw(image_CardB, windowWidth - 130, windowHeight / 2 - 60, 185, 243, 255);
        CP_Image_Draw(image_CardHighlight, windowWidth - 130, windowHeight / 2 - 60, 185, 243, CP_Math_LerpInt(0, 180, cardhighlightTimer[0]));
        DrawUI_Title(UIselectedEvent->title);
        DrawUI_TextDesc(UIselectedEvent->descriptionB);
        break;
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
        CP_Image_Draw(*GetCardSpriteByType(UIselectedReward->cardType), 1470 + offsetX + rewardCardGap * i, 390, 185, 243, 255);
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
        DrawUI_TextDesc(UIselectedReward->description);
    }
    else
    {
        DrawUI_TextDesc("Click on the card below to claim your reward.");
    }
}
void DrawUI_Textbox()
{
    CP_Image_Draw(image_descbox, 1370, 130, 439, 244, 255);
}
void DrawUI_TextDesc(const char* text)
{
    CP_Settings_TextSize(20);
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
    CP_Font_DrawTextBox(text, 1220, 90, 300);
}
void DrawUI_Title(const char* text)
{
    CP_Settings_TextSize(40);
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
    CP_Font_DrawTextBox(text, 1170, 40, 400);
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
        CP_Image_Draw(image_redZone, 800 + MAPOFFSETX, 450, 640, 640, (int)alphaLerp);
        break;
    default:
        while (UIselectedEvent->affectedLand[affectedIndex] != 0)
        {
            CP_Image_Draw(image_redZone, ((float)((UIselectedEvent->affectedLand[affectedIndex] - 1) % WORLDGRIDY) - (WORLDGRIDX) / 2) * TILEWIDTH + MAPOFFSETX + 800,
                    ((float)((UIselectedEvent->affectedLand[affectedIndex] - 1) / WORLDGRIDY) - (WORLDGRIDX) / 2) * TILEHEIGHT + 450, TILEWIDTH, TILEHEIGHT, (int)alphaLerp);
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
        DrawUI_TopPile();
        break;
    case State_Idle:
        DrawUI_Textbox();
        DrawUI_TextDesc("Click on the deck below to draw an event card");
        DrawUI_Deck();
        DrawUI_TopPile();
        break;
    case State_CardDraw:
        DrawUI_Textbox();
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



void DrawTempTextResources()
{
    if (goldLerp < (float)Get_current_gold() / 200)
    {
        goldLerp += CP_System_GetDt() / 4;
        if (goldLerp > (float)Get_current_gold() / 200)
            goldLerp = (float)Get_current_gold() / 200;
    }
    else if(goldLerp > (float)Get_current_gold() / 200)
    {
        goldLerp -= CP_System_GetDt() / 4;
        if (goldLerp < (float)Get_current_gold() / 200)
            goldLerp = (float)Get_current_gold() / 200;
    }
    if (foodLerp < (float)Get_current_food() / 200)
    {
        foodLerp += CP_System_GetDt() / 4;
        if (foodLerp > (float)Get_current_food() / 200)
            foodLerp = (float)Get_current_food() / 200;
    }
    else if (foodLerp > (float)Get_current_food() / 200)
    {
        foodLerp -= CP_System_GetDt() / 4;
        if (foodLerp < (float)Get_current_food() / 200)
            foodLerp = (float)Get_current_food() / 200;
    }

    if (popuLerp < (float)Get_current_population() / 200)
    {
        popuLerp += CP_System_GetDt() / 4;
        if (popuLerp > (float)Get_current_population() / 200)
            popuLerp = (float)Get_current_population() / 200;
    }
    else if (popuLerp > (float)Get_current_population() / 200)
    {
        popuLerp -= CP_System_GetDt() / 4;
        if (popuLerp < (float)Get_current_population() / 200)
            popuLerp = (float)Get_current_population() / 200;
    }

    if (moraleLerp < (float)Get_current_morale() / 200)
    {
        moraleLerp += CP_System_GetDt() / 4;
        if (moraleLerp > (float)Get_current_morale() / 200)
            moraleLerp = (float)Get_current_morale() / 200;
    }
    else if (moraleLerp > (float)Get_current_morale() / 200)
    {
        moraleLerp -= CP_System_GetDt() / 4;
        if (moraleLerp < (float)Get_current_morale() / 200)
            moraleLerp = (float)Get_current_morale() / 200;
    }
    CP_Image_Draw(image_barBG, 100, 90, 200, 60, 255);
    CP_Image_Draw(image_barBG, 100, 180, 200, 60, 255);
    CP_Image_Draw(image_barBG, 100, 270, 200, 60, 255);
    CP_Image_Draw(image_barBG, 100, 360, 200, 60, 255);
    CP_Image_Draw(image_goldbar, CP_Math_LerpFloat(-100, 100, goldLerp), 90, 200, 60, 255);
    CP_Image_Draw(image_foodbar, CP_Math_LerpFloat(-100, 100, foodLerp), 180, 200, 60, 255);
    CP_Image_Draw(image_populationbar, CP_Math_LerpFloat(-100, 100, popuLerp), 270, 200, 60, 255);
    CP_Image_Draw(image_moralebar, CP_Math_LerpFloat(-100, 100, moraleLerp), 360, 200, 60, 255);
    CP_Image_Draw(image_resourcetext, 100, 200, 200, 400, 255);

    //CP_Image_Draw(image_ResourceBars, 800, 450, 1600, 900, 255);


    CP_Settings_TextSize(40);
    

    if (Get_current_gold() < 0)
    {
        CP_Settings_Fill(CP_Color_Create(200, 0, 0, 255));
        sprintf_s(resourceBuffer, 20, "(In Debt)");
        CP_Font_DrawText(resourceBuffer, 60, 68);
    }

    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));

    //Draw Text for current gold amount
    sprintf_s(resourceBuffer, 20, "%d", Get_current_gold());
    CP_Font_DrawText(resourceBuffer, 140, 70);
    
    //Draw Text For current food amount
    sprintf_s(resourceBuffer, 20, "%d", Get_current_food());
    CP_Font_DrawText(resourceBuffer, 140, 160);

    //Draw Text for current population amount
    sprintf_s(resourceBuffer, 20, "%d", Get_current_population());
    CP_Font_DrawText(resourceBuffer, 140, 250);

    //Draw Text for current morale
    sprintf_s(resourceBuffer, 20, "%d", Get_current_morale() + Get_additional_morale());
    CP_Font_DrawText(resourceBuffer, 140, 340);

    //Draw text for number of cards left
    sprintf_s(resourceBuffer, 20, "Cards Left: %d", GetCardsLeft());
    CP_Font_DrawText(resourceBuffer, 1450, 850);


}

