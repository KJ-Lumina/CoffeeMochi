#include <stdio.h>
#include "cprocessing.h"

// gamestate is required input from main.c!!
int isPopupOnScreen = 0;

void TogglePopup(int identity, int isItShowing);

/* Things left to do:
How to initialise all buildings/buttons
Work on image render into struct*/


struct Button
{
    float width;
    float height;
    float xPos;
    float yPos;
    int identity;
    const char* imagename;
};

struct Building
{
    char buildingName[16];
    int population;
    int goldCost;

};

// all structs are temporarily initialized to 3 in array
struct Button StartMenu[3];
struct Button Main[3] = { { 100,100,200,200,21,"Building" }, { 100,100,900,900,22,"Citizen" }, { 100,100,900,900,23,"Settings" } };
struct Button Popup[1] = { { 256,128,500,500,30,"Popup" } };
struct Building Building[3];


int LoopArray(struct Button array[], CP_Vector mousePos)
{
    int result = 0;
    int arraylength = sizeof(array)/sizeof(int);

    for (int i = 0; i < arraylength; i++)
        if (mousePos.x >= array[i].xPos && mousePos.x <= array[i].xPos + array[i].width &&
            mousePos.y >= array[i].yPos && mousePos.y <= array[i].yPos + array[i].height)
        {
            result = array[i].identity;
            break;
        }
        else
            result = 0;

    return result;
}

//requires OnClickEvent to call
void CheckMouseCollision(int gamestate, CP_Vector mousePos)
{
    int result = 0;
    if (gamestate == 1)
        result = LoopArray(StartMenu,mousePos);
    else if (gamestate == 2)
        result = LoopArray(Main,mousePos);
    else if (gamestate == 3)
        result = LoopArray(Popup,mousePos);

    switch (result)
    {
    case 0:
        if (isPopupOnScreen == 1) {
            TogglePopup(result,0);
            isPopupOnScreen = 0;
        }
        break;

    case 21:
        if (gamestate == 2) {
            TogglePopup(result, 1);
            isPopupOnScreen = 1;
        }
        break;

    case 31:
        break;
    }
}

void TogglePopup(int identity, int isItShowing)
{
    int arrayNumber = 0;
    if (identity == 21)
        arrayNumber = 0;
    else if (identity == 22)
        arrayNumber = 1;
    else if (identity == 23)
        arrayNumber = 2;

    ToggleDrawUI(isItShowing, arrayNumber);
}

void ToggleDrawUI(int isItShowing, int popupArraynumber)
{
    if (isItShowing == 1)
        CP_Image_Draw(Popup[popupArraynumber].imagename, 300, 300, Popup[popupArraynumber].width, Popup[popupArraynumber].height, 255);
}




//All Button call defininitions past this point
