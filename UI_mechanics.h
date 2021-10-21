#include <stdio.h>
#include "cprocessing.h"

//temp gamestate will be toggled in UI file
// gamestate is required input from main.c!!
int gamestate = 2;

//void ClosePopup();
void OpenPopup(int identity);

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

//Main[3] = { { 100,100,900,900,21,"Building" }, { 100,100,100,100,22,"Citizen" }, { 100,100,100,100,23,"Settings" } };
//Popup[1] = { { 256,128,500,500,30,"Popup" } };



//requires onmouseclick event to call
int CheckMouseColliding(struct Button array[])
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
