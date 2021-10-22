#include <stdio.h>
#include "cprocessing.h"
#include "game.h"



//TEMPPPPPPPPPPPPP FOR PROTOTYPE ONLY

CP_Image basicEvent;
CP_Image advancedEvent;
CARDEVENTS *eventCard;
float windowWidth;
float windowHeight;

int CheckUIClick(float xPos, float yPos)
{
    if (xPos >= windowWidth - 238 && xPos <= windowWidth - 142 && yPos >= windowHeight - 180 && yPos <= windowHeight - 20)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void InitUI()
{
    basicEvent = CP_Image_Load("./Assets/basiceventcard.png");
    advancedEvent = CP_Image_Load("./Assets/advancedeventcard.png");
    windowWidth = (float)CP_System_GetWindowWidth();
    windowHeight = (float)CP_System_GetWindowHeight();
}

void DrawUI()
{
    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
    CP_Graphics_DrawRect(windowWidth - 260, 0, windowWidth, windowHeight);
    *eventCard = GetCurrentEvent();
    if (eventCard != NULL)
    {
        printf("passed\n");
        CP_Settings_TextSize(20);
        CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
        char buffer[100];
        printf("passed\n");
        sprintf_s(buffer, 100, "%s", eventCard->description);
        printf("passed\n");
        CP_Font_DrawText(buffer, 20, 20);
    }
    else
    {
        CP_Image_Draw(basicEvent, windowWidth - 190, windowHeight - 100, 160, 160, 255);
        CP_Image_Draw(advancedEvent, windowWidth - 70, windowHeight - 100, 160, 160, 255);
    }
}

//temp gamestate will be toggled in UI file
// gamestate is required input from main.c!!
int gamestate = 2;

//void ClosePopup();
void OpenPopup(int identity);

/* Things left to do:
How to initialise all buildings/buttons
Work on image render into struct*/




// all structs are temporarily initialized to 3 in array
Button StartMenu[3];
Button Main[3] = { { 100,100,200,200,21,"Building" }, { 100,100,900,900,22,"Citizen" }, { 100,100,900,900,23,"Settings" } };
Button Popup[1] = { { 256,128,500,500,30,"Popup" } };
Building Buildings[3];

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

    switch (result)
    {
    case 0:
        if (gamestate == 3) {
            gamestate = 2;
            //ClosePopup();
        }
        break;

    case 21:
        gamestate = 3;
        OpenPopup(result);
        break;

    case 31:
        //cursorTile = SnapToGrid(mousePosX, mousePosY);
        //CP_Image_Draw(housetile, cursorTile.x, cursorTile.y, tileWidth, tileHeight, 255);
        break;
    }

    return result;
}

void MouseCollidingState(int zgamestate)
{
    int result = 0;
    if (zgamestate == 1)
        result = CheckMouseColliding(StartMenu);
    else if (zgamestate == 2)
        result = CheckMouseColliding(Main);
    else if (gamestate == 3)
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





//All Button call defininitions past this point





