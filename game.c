#include <stdio.h>
#include <stdbool.h>
#include "Common_Headers.h"
#include "cprocessing.h"
#include "TravessFunctions.h"
#include "WorldSpaceGrid.h"
#include "UI_mechanics.h"
#include "Npc.h"


#pragma region Game Options Control
bool AllowMouseDrag = false;
bool mouseDrag = false;
#pragma endregion

GAMESTATE gameState;
GAMEPHASE gamePhase;
CP_Vector currentMousePos;
CP_Vector mouseDragPos;
int loseCondition_FoodValue;
int loseCondition_PopulationValue;
float AnimTimer = 1;



GAMESTATE GetGameState()
{
    return gameState;
}

#pragma region Turn & Win Lose Functions
//Trigger Turn Start Functions Calls
void StartTurn() 
{

}

//Trigger Turn End Functions Calls
void GameOver() 
{
    //Lose UI Pop Up
    printf("The game has ended!");
}

void EndTurn() 
{
    if (gamePhase == PHASE_GAMEPHASE) {
        GenerateResourcesOnEndTurn();
    }

    if (gamePhase == PHASE_ENDPHASE) {
        GameOver();
    }
    /*if (LoseCondition_Resources())
        GameOver();*/
}

bool LoseCondition_Resources() {
    if (Get_current_food() <= loseCondition_FoodValue || Get_current_population() <= loseCondition_FoodValue) {
        return 1;
    }

    return 0;
}
#pragma endregion

void UpdateMouseInput()
{
    currentMousePos.x = CP_Input_GetMouseX();
    currentMousePos.y = CP_Input_GetMouseY();
    
    //mouseDrag purpose
    if (CP_Input_MouseTriggered(0))
    {
        mouseDragPos = currentMousePos;
    }
}

//Debug instant commands
void AdminControlInput()
{
    if (CP_Input_KeyTriggered(KEY_Q))
    {
        CP_Vector spawnPoint = currentMousePos;
        ScreenToWorldPosition(&spawnPoint);
        SpawnNpc(spawnPoint, 1);
    }
    //testing animations
    if (CP_Input_KeyTriggered(KEY_W))
    {
        SpawnAnimation(currentMousePos.x, currentMousePos.y, 200, 200, 1, 0.5f, 1);
    }
}


void CheckKeyInput()
{
    if (CP_Input_KeyTriggered(KEY_F))
    {
        ReturnToCenter();
    }
    if (CP_Input_KeyTriggered(KEY_SPACE))
    {
        EndTurn();
    }
    
    // Debugging
    AdminControlInput();
}

void MouseClick()
{
    if (gamePhase == PHASE_MAINMENU) return; //Run Main Menu UI stuff

    switch (gameState)
    {
        case State_StartOfTurn:
            //Run Condition on Start of Turn
            gameState = State_Idle;
            break;

        case State_Idle:
            if (ClickCheckCardDraw())
            {
                if (GetCardsLeft() == 0)
                {
                    ++gamePhase;
                    ChangeDeckByPhase(gamePhase);
                    if (gamePhase == PHASE_ENDPHASE)
                    {
                        gameState = State_EndOfTurn;
                    }
                    else
                    {
                        gameState = State_MakeAChoice;
                        UI_SetEvent(GetNextEvent(gamePhase));
                    }
                }
                else
                {
                    AnimTimer = 0.6f;
                    gameState = State_CardDraw;
                }
            }
            break;
        case State_CardDraw:
            
            break;
        case State_MakeAChoice:
            switch (ClickCheckCardChoice())
            {
            case 1:
                gameState = State_PlaceYourBuilding;
                break;
            case 2:
                gameState = State_EndOfTurn;
                break;
            }
            break;
        case State_PlaceYourBuilding:

            if (AttemptPlaceBuilding(currentMousePos))
            {
                gameState = State_EndOfTurn;
            }
            break;

        case State_EndOfTurn:

            break;
    }
}

void GameStateControl() 
{
    DrawGridIndicator(currentMousePos);
    switch (gameState)
    {
        case State_StartOfTurn:
            gameState = State_Idle;
            break;
        case State_Idle:
            break;
        case State_CardDraw:
            AnimTimer -= CP_System_GetDt();
            if (AnimTimer <= 0)
            {
                gameState = State_MakeAChoice;
                UI_SetEvent(GetNextEvent(gamePhase));
            }
            break;
        case State_MakeAChoice:
            break;
        case State_PlaceYourBuilding:
            DrawCursorTile(currentMousePos);
            break;
        case State_EndOfTurn:
            EndTurn();
            gameState = State_StartOfTurn;
            break;
    }
}

void MouseDragOrClick(void)
{
    if (AllowMouseDrag)
    {
        if (CP_Input_MouseDragged(0))
        {
            mouseDrag = true;
            MoveWorldSpaceOrigin(currentMousePos.x - mouseDragPos.x, currentMousePos.y - mouseDragPos.y);
            mouseDragPos = currentMousePos;
        }
        // To deferentiate between drag and click
        if (CP_Input_MouseClicked() && !mouseDrag)
        {
            MouseClick();
        }
        // end of drag
        if (CP_Input_MouseReleased(0) && mouseDrag)
        {
            mouseDrag = false;
        }
    }
    else if (CP_Input_MouseClicked())
    {
        MouseClick();
    }
}

void game_init(void)
{    
    CP_System_ShowConsole();
    CP_System_SetWindowSize(1600, 900);
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_TOP);
    gameState = State_Idle;
    gamePhase = PHASE_BUILDPHASE;
    InitResources(100);
    InitWorldSpaceGrid();
    InitBuildings();
    InitNpc();
    InitSpritesheets();
    InitDecks();
    InitUI();
}

void game_update(void)
{
    UpdateMouseInput();
    MouseDragOrClick();
    CheckKeyInput();
    // Graphics
    CP_Graphics_ClearBackground(CP_Color_Create(150, 150, 150, 255));
    DrawTileSet();
    DrawBuildings();
    UpdateAllNpc();
    GameStateControl();
    DrawUI(gameState);
    DrawTempTextResources();
    DrawAllAnimations();
}

void game_exit(void)
{

}
