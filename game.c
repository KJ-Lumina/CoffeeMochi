/*!_____________________________________________________________________________
@file       OngoingEvents.h
@author     Travess Tan (travesscheekhang.t@digipen.edu)
@co-authors Seow Kai Jun (s.kaijun@digipen.edu)
@course     CSD1120
@section    B
@team       CoffeeMochi
@brief      This file contains the variables & function definitions that controls the main game from start to end in Impero

            Copyright ©2021 DigiPen, All rights reserved.
*//*__________________________________________________________________________*/

#include <stdio.h>
#include <stdbool.h>
#include "Common_Headers.h"
#include "cprocessing.h"
#include "TravessFunctions.h"
#include "WorldSpaceGrid.h"
#include "UI_Animations.h"
#include "Npc.h"

#pragma region Game Options Control
bool AllowAdminControl = false;
bool AllowMouseDrag = false;
bool mouseDrag = false;
float MouseCounter = 0;
#pragma endregion

GAMESTATE gameState;
CP_Vector currentMousePos;
CP_Vector mouseDragPos;
int loseCondition_FoodValue = 0;
int loseCondition_PopulationValue = 0;

int GameWin = false;

float AnimTimer = 1;
CARDEVENT* selectedEvent;
REWARDCARD* selectedReward[NUMBER_OF_MAX_REWARDS];
int selectedChoice;
int rewardCardsLeft[NUMBER_OF_MAX_REWARDS];
int rewardIndex = 0;

int endState = 0;
float endTurnTimer = 0;
float animCount = 0;
float animDelay = 0.1f;
CP_Vector tempVector;
CP_Vector worldOrigin;

/*--------------------
End of Game FUNCTIONS
---------------------*/
/*!_____________________________________________________________________________
@brief      This set of functions are used to check if the game has ended
            based on cards left and resources left.
*//*__________________________________________________________________________*/

void GameEnd() {
    GameWin = true; //Proceed with Game Win
    gameState = State_GameOver; //Change GameState to State_GameOver
}

void GameOver() 
{
    //Lose UI Pop Up
    GameWin = false; //Proceed With Game False
    gameState = State_GameOver; //Change GameState to State_GameOver
}

bool LoseCondition_Resources()
{
    //Check for Lose Conditions [Any Resource < 0]
    if (Get_current_food() < 0 || Get_current_population() < 0 || Get_current_gold() < 0 || Get_current_morale() < 0) {
        return true;
    }
    return false;
}

/*--------------------
End of Game FUNCTIONS
---------------------*/
/*!_____________________________________________________________________________
@brief      This function is used to Activate and update end of turn
            events. Also used to check if the game should end here.
*//*__________________________________________________________________________*/

void EndTurn() 
{
    GenerateResourcesOnEndTurn(); 
    OnEndUpdateEvents();

    if (LoseCondition_Resources()) {
        GameOver();
    }
    else {
        gameState = State_GameEntry; //Change GameState to GameEntry if Game is not lost
    }
}


/*--------------------
Player Input FUNCTIONS
---------------------*/
/*!_____________________________________________________________________________
@brief      This set of functions are used to check for player input.
            Player clicking for Game UI interaction.
            Keyboard Inputs for Debugging (If admin control is enabled)
*//*__________________________________________________________________________*/

void UpdateMouseInput()
{
    currentMousePos.x = CP_Input_GetMouseX();
    currentMousePos.y = CP_Input_GetMouseY();
    
    //Update Left Mouse Button Position for Mouse Drag
    if (CP_Input_MouseTriggered(0))
    {
        mouseDragPos = currentMousePos;
    }
}

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
        SpawnAnimation(currentMousePos.x, currentMousePos.y, currentMousePos.x, currentMousePos.y, 200, 200, 1, 0.5f, 1);
    }

    if (CP_Input_KeyTriggered(KEY_E))
    {
        SpawnVfxEaseInToEaseOut(1, currentMousePos, CP_Vector_Set(CP_Random_RangeFloat(-50, 50) + currentMousePos.x, CP_Random_RangeFloat(-50, 50) + currentMousePos.y), CP_Vector_Set(50, 50), 1, CP_Vector_Set(128, 128), 0);
    }

    if (CP_Input_KeyTriggered(KEY_H)) {
        GameOver();
    }

    if (CP_Input_KeyTriggered(KEY_1))
    {
        int moregold[4] = { 10,0,0,0 };
        ApplyEventResourceChange(moregold);
    }
    if (CP_Input_KeyTriggered(KEY_2))
    {
        int morefood[4] = { 0,10,0,0 };
        ApplyEventResourceChange(morefood);
    }
    if (CP_Input_KeyTriggered(KEY_3))
    {
        int morepop[4] = { 0,0,10,0 };
        ApplyEventResourceChange(morepop);
    }
    if (CP_Input_KeyTriggered(KEY_4))
    {
        int moremoral[4] = { 0,0,0,10 };
        ApplyEventResourceChange(moremoral);
    }
    if (CP_Input_KeyTriggered(KEY_5))
    {
        int moregold[4] = { -10,0,0,0 };
        ApplyEventResourceChange(moregold);
    }
    if (CP_Input_KeyTriggered(KEY_6))
    {
        int morefood[4] = { 0,-10,0,0 };
        ApplyEventResourceChange(morefood);
    }
    if (CP_Input_KeyTriggered(KEY_7))
    {
        int morepop[4] = { 0,0,-10,0 };
        ApplyEventResourceChange(morepop);
    }
    if (CP_Input_KeyTriggered(KEY_8))
    {
        int moremoral[4] = { 0,0,0,-10 };
        ApplyEventResourceChange(moremoral);
    }
    if (CP_Input_KeyTriggered(KEY_R))
    {
        RestartGame();
    }
}

void CheckKeyInput()
{
    if (CP_Input_KeyTriggered(KEY_F))
    {
        ReturnToCenter();
    }
    //For Debugging Purpose
    if (AllowAdminControl)
    {
        AdminControlInput();
    }
}

/*--------------------
Mouse Click FUNCTION
---------------------*/
/*!_____________________________________________________________________________
@brief      This function is called when the player has clicked in the game
            It looks for the current state of the game and checks if respective
            UIs or interactables have been clicked on

            State Idle - Clicks on card -> State Card Draw
            State MakeAChoice - Clicks on red/blue -> Starts Event
            State CollectRewards - Clicks on Reward card -> Spawn Rewards
                if no rewards left, State EndOfTurn
            State PlaceYourBuilding - Clicks on tiles -> Construct Building
                if no rewards left, State EndOfTurn
*//*__________________________________________________________________________*/

void MouseClick() //Run when Mouse is Clicked
{
    switch (gameState)
    {
        case State_StartOfTurn:
            //Run Condition on Start of Turn
            gameState = State_Idle;
            break;
        case State_Idle: 
            if (ClickCheck_CardDraw())
            {
                Play_SFX_Sound(Sound_SFX_Click);
                Play_SFX_Sound(Sound_SFX_CardHover);
                AnimTimer = 0.6f;
                gameState = State_CardDraw;
            }
            break;
        case State_MakeAChoice:
            // check for each card choices
            switch (ClickCheck_CardChoice())
            {
            case 1:
                selectedChoice = 1;
                // clicked on optionA
                rewardIndex = 0; // Reset Reward Index Before Event Begin
                ApplyEventResourceAnim(selectedEvent->resourceChangeA);
                for (int index = 0; index < NUMBER_OF_MAX_REWARDS; ++index)
                {
                    selectedReward[index] = GetRewardByIndex(selectedEvent->resourceRewardA[index].rewardIndex);
                    rewardCardsLeft[index] = selectedEvent->resourceRewardA[index].rewardAmount;
                }
                //AnimTimer = 1.3f;
                Play_SFX_Sound(Sound_SFX_Click);
                gameState = State_ResourceChange;
                break;
            case 2:
                selectedChoice = 2;
                // clicked on optionB
                rewardIndex = 0; // Reset Reward Index Before Event Begin
                ApplyEventResourceAnim(selectedEvent->resourceChangeB);
                for (int index = 0; index < NUMBER_OF_MAX_REWARDS; ++index)
                {
                    selectedReward[index] = GetRewardByIndex(selectedEvent->resourceRewardB[index].rewardIndex);
                    rewardCardsLeft[index] = selectedEvent->resourceRewardB[index].rewardAmount;
                }
                //AnimTimer = 1.3f;
                Play_SFX_Sound(Sound_SFX_Click);
                gameState = State_ResourceChange;
                break;
            }
            break;
        case State_CollectRewards:
            switch (ClickCheck_Rewards())
            {
            case BUILD_TYPE_EVENT:
                // reward is construction
                if (AreAllOccupied())
                {
                    gameState = State_EndOfTurn;
                }
                SetBuildingType(GetBuildingByIndex(selectedReward[rewardIndex]->eventIndex));
                --rewardCardsLeft[rewardIndex];
                gameState = State_PlaceYourBuilding;
                break;
            case DESTROY_TYPE_EVENT:
                SetBuildingType(GetBuildingByIndex(selectedReward[rewardIndex]->eventIndex));
                --rewardCardsLeft[rewardIndex];
                gameState = State_DestroyBuilding;
                break;
            case ONGOING_TYPE_EVENT:
                if (selectedEvent->affectedLand[rewardIndex] == 26)
                {
                    int ongoingTurns = 0;
                    if (selectedChoice == 1)
                    {
                        ongoingTurns = selectedEvent->resourceRewardA[1].rewardIndex;
                    }
                    else
                    {
                        ongoingTurns = selectedEvent->resourceRewardB[1].rewardIndex;
                    }
                    for (int i = 0; i < 26; ++i)
                    {
                        if (GetOccupiedIndex((i) % WORLDGRIDY, (i) / WORLDGRIDX) == selectedReward[0]->resourceType)
                        {
                            GenerateEvents(O_RATEVENT, (i) % WORLDGRIDY, (i) / WORLDGRIDX, ongoingTurns);
                        }
                    }
                }
                else
                {
                    int ongoingTurns = 0;
                    if (selectedChoice == 1)
                    {
                        ongoingTurns = selectedEvent->resourceRewardA[1].rewardIndex;
                    }
                    else
                    {
                        ongoingTurns = selectedEvent->resourceRewardB[1].rewardIndex;
                    }
                    while (selectedEvent->affectedLand[rewardIndex] != 0)
                    {
                        int gridPos = selectedEvent->affectedLand[rewardIndex];
                        if (GetOccupiedIndex((gridPos - 1) % WORLDGRIDY, (gridPos - 1) / WORLDGRIDX) == selectedReward[0]->resourceType)
                        {
                            GenerateEvents(O_RATEVENT, (gridPos - 1) % WORLDGRIDY, (gridPos - 1) / WORLDGRIDX, ongoingTurns);
                        }
                        rewardIndex++;
                    }
                }
                gameState = State_EndOfTurn;
            }
            break;
        case State_PlaceYourBuilding:
            if (AttemptPlaceBuilding(currentMousePos))
            {
                switch(rewardIndex) //Prevention for going out of bounds
                {
                case 0: //Check Reward Index = 0
                    if (rewardCardsLeft[rewardIndex] == 0) //Once first reward is empty, goes down to 2nd array slot in the Selected Reward
                    {
                        if (rewardCardsLeft[++rewardIndex] == 0) //Checks for 2nd Reward Index 
                        {
                            gameState = State_EndOfTurn;
                        }
                        else
                        {
                            gameState = State_CollectRewards;
                        }
                    }
                    else
                    {
                        gameState = State_CollectRewards;
                    }
                    break;
                case 1: //Check Reward Index = 1 
                    if (rewardCardsLeft[rewardIndex] == 0)
                    {
                        gameState = State_EndOfTurn;
                    }
                    else
                    {
                        gameState = State_CollectRewards;
                    }
                    break;
                default:
                    gameState = State_EndOfTurn;
                    break;
                }

            }
            break;
    }
}

/*--------------------
Main Game Control FUNCTION
---------------------*/
/*!_____________________________________________________________________________
@brief      This function is the main game loop, controlling the game state
            Main uses for this loop is to stall game states for animations
            to run.
            Further comments are written within the function.
*//*__________________________________________________________________________*/
void GameStateControl()
{
    DrawGridIndicator(currentMousePos);
    switch (gameState)
    {
    //------------------------------------------------------------------------------------------
    // Allows for gold card to exit animation.
    // Checks if blessing points are enough for gold card, if so draw a gold card,
    // if not, draw a normal card.
    //------------------------------------------------------------------------------------------
    case State_GameEntry:
        AnimTimer = 1.0f;
        if (Get_current_blessing() >= 100)
        {
            UI_SetGoldCard(); 
            EventSetGoldenCard();
            ResetBlessing();
        }
        else
        {
            UI_SetNormalCard();
        }
        gameState = State_StartOfTurn;
        break;
    //------------------------------------------------------------------------------------------
    // Allows for draw card to enter animation
    //------------------------------------------------------------------------------------------
    case State_StartOfTurn:
        AnimTimer -= CP_System_GetDt();
        if (AnimTimer < 0)
        {
            gameState = State_Idle;
        }
        break;
    //------------------------------------------------------------------------------------------
    // Allows for card draw animation
    // Sets events based on card draw
    //------------------------------------------------------------------------------------------
    case State_CardDraw:
        AnimTimer -= CP_System_GetDt();
        if (AnimTimer <= 0)
        {
            Play_SFX_Sound(Sound_SFX_Card);
            gameState = State_MakeAChoice;         
            selectedEvent = GetNextEvent();
            Play_Event_SFX(selectedEvent->eventIndex);
            UI_SetEvent(selectedEvent);
        }
        break;
    //------------------------------------------------------------------------------------------
    // Allows for resouces bar change animation
    // Sets rewards based on choice selected
    //------------------------------------------------------------------------------------------
    case State_ResourceChange:
        AnimTimer -= CP_System_GetDt();
        if (AnimTimer <= 0)
        {
            if (rewardCardsLeft[rewardIndex])
            { 
                if (selectedReward[rewardIndex]->cardType == EVENT_TYPE_REWARD)
                {
                    RollForcedCard(selectedReward[0]->resourceType, rewardCardsLeft[0]);
                    // no reward cards
                    AnimTimer = 0.6f;
                    gameState = State_EndOfTurn;
                }
                else
                {
                    // reward cards exist
                    UI_SetReward(selectedReward[rewardIndex], rewardCardsLeft[0] + rewardCardsLeft[1]);
                    gameState = State_CollectRewards;
                }
            }
            else
            {
                // no reward cards
                AnimTimer = 0.6f;
                gameState = State_EndOfTurn;
            }
        }
        break;
    //------------------------------------------------------------------------------------------
    // Draws constructing building.
    //------------------------------------------------------------------------------------------
    case State_PlaceYourBuilding:
        DrawCursorTile(currentMousePos);
        break;
    //------------------------------------------------------------------------------------------
    // Destroys building based on event
    //------------------------------------------------------------------------------------------
    case State_DestroyBuilding:
        DestroyBuildingBySelectedBuilding(selectedReward[rewardIndex]->resourceType); //Destroy 1 building by the building chosen in choice
        switch (rewardIndex) //Prevention for going out of bounds
        {
        case 0: //Check Reward Index = 0
            if (rewardCardsLeft[rewardIndex] == 0)
            {
                if (rewardCardsLeft[++rewardIndex] == 0) {
                    gameState = State_EndOfTurn;
                }
                else {
                    gameState = State_CollectRewards;
                }
            }
            else //If there is more buidling to destroy
            {
                gameState = State_CollectRewards;
            }
            break;

        case 1: //Check Reward Index = 1
            if (rewardCardsLeft[rewardIndex] == 0)
            {
                gameState = State_EndOfTurn;
            }
            else //If there is more building to destroy
            {
                gameState = State_CollectRewards;
            }
            break;

        default:
            gameState = State_EndOfTurn;
            break;
        }
        break;

    //------------------------------------------------------------------------------------------
    // End of turn goes through a series of animations before it returns to Start of turn again.
    //------------------------------------------------------------------------------------------
    case State_EndOfTurn:
        endTurnTimer -= CP_System_GetDt();
        if (endTurnTimer <= 0)
        {
            switch (endState)
            {
            //------------------------------------------------------------------------------------------
            // This marks the first animation. Resources generated from buildings are collected
            // and stored.
            // It loops through all buildings on the grid, checks for status, if not broken
            // or destroyed by any events, proceeed to generate resources.
            // 
            // animCount is used to calculate number of animations to draw and delay next state
            // accordingly.
            //------------------------------------------------------------------------------------------
            case 0:
                rewardIndex = 0;  // Reset Reward Index Before Event Begin
                worldOrigin = GetWorldSpaceOrigin();
                for (int j = 0; j < WORLDGRIDY; ++j)
                {
                    for (int i = 0; i < WORLDGRIDX; ++i)
                    {
                        switch (GetOccupiedIndex(i, j))
                        {
                            case B_HOUSE_INDEX:
                                tempVector.x = i * TILEWIDTH + TILEWIDTH / 2 + worldOrigin.x;
                                tempVector.y = j * TILEHEIGHT + TILEHEIGHT / 3 + worldOrigin.y;

                                // Checks if any events are affecting this house, if so lose morale.
                                if (CheckCurrent(B_HOUSE_INDEX, i, j))
                                {
                                    SpawnMoraleGainAnimation(-1, tempVector, CP_Vector_Set(tempVector.x, tempVector.y - TILEHEIGHT / 3), CP_Vector_Set(520, 360), 0.6f, animCount * animDelay);
                                }
                                ++animCount;
                                break;
                            case B_FARM_INDEX:
                                tempVector.x = i * TILEWIDTH + TILEWIDTH / 2 + worldOrigin.x;
                                tempVector.y = j * TILEHEIGHT + TILEHEIGHT / 3 + worldOrigin.y;
                                // Check if any events are affecting this farm, if not, generate food
                                if (!CheckCurrent(B_FARM_INDEX, i, j))
                                {
                                    SpawnFoodGainAnimation(farm_food_prediction(), tempVector, CP_Vector_Set(tempVector.x, tempVector.y - TILEHEIGHT / 3), CP_Vector_Set(520, 180), 0.6f, animCount* animDelay);
                                }
                                ++animCount;
                                break;
                            case B_MARKET_INDEX:
                                tempVector.x = i * TILEWIDTH + TILEWIDTH / 2 + worldOrigin.x;
                                tempVector.y = j * TILEHEIGHT + TILEHEIGHT / 3 + worldOrigin.y;
                                // Check if any events are affecting this market, if not, generate gold
                                if (!CheckCurrent(B_MARKET_INDEX, i, j))
                                {
                                    SpawnGoldGainAnimation(market_gold_prediction(), tempVector, CP_Vector_Set(tempVector.x, tempVector.y - TILEHEIGHT / 3), CP_Vector_Set(520, 90), 0.6f, animCount* animDelay);
                                }
                                ++animCount;
                                break;
                            case B_TAVERN_INDEX:
                                tempVector.x = i * TILEWIDTH + TILEWIDTH / 2 + worldOrigin.x;
                                tempVector.y = j * TILEHEIGHT + TILEHEIGHT / 3 + worldOrigin.y;
                                // Check if any events are affecting this tavern, if not, generate morale
                                if (!CheckCurrent(B_TAVERN_INDEX, i, j))
                                {
                                    SpawnMoraleGainAnimation(tavern_morale_preduction(), tempVector, CP_Vector_Set(tempVector.x, tempVector.y - TILEHEIGHT / 3), CP_Vector_Set(520, 360), 0.6f, animCount * animDelay);
                                }
                                ++animCount;
                                break;
                            default:
                                break;
                        }
                    }
                }
                endTurnTimer = animCount * animDelay + 0.5f;
                endState = 1;
                break;
            //------------------------------------------------------------------------------------------
            // This marks the second animation. Food is consumed for every existing npc in the game.
            //
            // animCount is used to calculate number of animations to draw and delay next state
            // accordingly.
            //------------------------------------------------------------------------------------------
            case 1:
                animCount = 0;
                worldOrigin = GetWorldSpaceOrigin();
                for (int i = 0; i < MAXNPC; ++i)
                {
                    if (GetNpc(i).x != 0)
                    {
                        tempVector = GetNpc(i);
                        tempVector = CP_Vector_Set(tempVector.x + worldOrigin.x, tempVector.y + worldOrigin.y);
                        SpawnFoodGainAnimation(-1, tempVector, CP_Vector_Set(tempVector.x, tempVector.y - 40), CP_Vector_Set(520, 180), 0.6f, animCount * animDelay);
                        ++animCount;
                    }
                }
                endTurnTimer = animCount * animDelay + 1.5f;
                endState = 2;
                break;
            //------------------------------------------------------------------------------------------
            // This marks the third animation. Blessing points for the gold card is generated
            // for every existing npc in the game
            //
            // animCount is used to calculate number of animations to draw and delay next state
            // accordingly.
            //------------------------------------------------------------------------------------------
            case 2:
                animCount = 0;
                worldOrigin = GetWorldSpaceOrigin();
                for (int i = 0; i < MAXNPC; ++i)
                {
                    if (GetNpc(i).x != 0)
                    {
                        tempVector = GetNpc(i);
                        tempVector = CP_Vector_Set(tempVector.x + worldOrigin.x, tempVector.y + worldOrigin.y);
                        SpawnBlessingGainAnimation(1, tempVector, CP_Vector_Set(tempVector.x, tempVector.y - 40), CP_Vector_Set(100, 450), 0.8f, animCount * animDelay);
                        ++animCount;
                    }
                }
                endTurnTimer = animCount * animDelay + 3.0f;
                endState = 3;
                UI_SetBlessingsTimer(endTurnTimer + 2.2f);
                break;
            //------------------------------------------------------------------------------------------
            // This marks the final stage for End Of Turn state. Checks if the game is still running
            // or should the game end.
            //------------------------------------------------------------------------------------------
            case 3:
                endState = 0;
                if (GetCardsLeft() == 0)
                {
                    GameEnd();
                }
                else
                {
                    EndTurn();  //State Set to Start Turn is in EndTurn()       
                }
                break;
            }
        }
        break;
    //------------------------------------------------------------------------------------------
    // Game has ended. Change game phase in main.c accordingly.
    //------------------------------------------------------------------------------------------
    case State_GameOver:
        SetGameSceneEndPhase(GameWin);
        gameState = State_End;
        break;
    }
}

/*--------------------
Click Or Drag FUNCTION
---------------------*/
/*!_____________________________________________________________________________
@brief      This function checks if the player is dragging or clicking.
            Originally intended to allow moving the map around if the
            world were to be bigger than the given screen space.
            Allows to expansion of tiles.
*//*__________________________________________________________________________*/


void MouseDragOrClick(void)
{
    if (AllowMouseDrag)
    {   // To deferentiate between drag and click (Check for Mouse Drag)
        if (CP_Input_MouseDragged(0))
        {
            mouseDrag = true;
            MoveWorldSpaceOrigin(currentMousePos.x - mouseDragPos.x, currentMousePos.y - mouseDragPos.y);
            mouseDragPos = currentMousePos;
        }
        // To deferentiate between drag and click (Check for Mouse Click)
        if (CP_Input_MouseClicked() && !mouseDrag)
        {
            MouseClick();
        }
        // Check if its at end of mouse drag
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

/*--------------------
Game Initialize FUNCTION
---------------------*/
/*!_____________________________________________________________________________
@brief      This function is called when the game starts or restarts.
            Sets everything to default state.
*//*__________________________________________________________________________*/
void MainGame_Initialize(void)
{
    gameState = State_GameEntry;
    InitResources(100,40,0,50);
    InitWorldSpaceGrid();
    InitBuildings();
    InitNpc();
    InitSpritesheets();
    InitDecks();
    InitUI();
    InitVfx();
    InitOngoingEvents(); 
}

/*--------------------
Main Game Update FUNCTION
---------------------*/
/*!_____________________________________________________________________________
@brief      This function serves as the Game Update, called every frame.
            It will call every main functions necessary to keep the game
            loop running.
*//*__________________________________________________________________________*/
void MainGame_Update(void)
{
    //Get Inputs Start
    UpdateResources();
    UpdateMouseInput();
    MouseDragOrClick();
    CheckKeyInput();
    //Get Inputs End
   
    //Graphics Start
    DrawTileSet();
    UpdateAllNpc();
    DrawUI(gameState);
    DrawBuildings();
    DrawOngoingEvents();
    GameStateControl();
    DrawTempTextResources();
    DrawAllVfx();
    DrawAllAnimations();
    //Graphic End
}
