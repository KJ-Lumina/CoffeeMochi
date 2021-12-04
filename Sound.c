/*---------------------------------------------------------
 * file:	Sound.c
 * author:	Kelly Liaw
 * email:	liaw.k@digipen.edu
 * co-author: Seow Kai Jun
 * email: s.kaijun@digipen.edu.sg
*
 * brief:	All Sound Related Variables and Functions Calls for the project resides here
*
 * Copyright ?2020 DigiPen, All rights reserved.
* ---------------------------------------------------------*/

#include <stdio.h>
#include "Common_Headers.h"
#include "cprocessing.h"
#include <stdbool.h>

#define PLAY 1
#define STOP 0

typedef struct {
    int soundIndex;
    int soundState;
    int soundLocation;
}
SOUND;

bool SoundInit = false;

//Card Based
CP_Sound CardDeckHover_S;
CP_Sound CardEndAnim_S;
CP_Sound Click_S;

//Card Choices
CP_Sound Choice_LHS;
CP_Sound Choice_RHS;

//Building
CP_Sound Farm_S;
CP_Sound Gold_S;
CP_Sound House_S;
CP_Sound Market_S;
CP_Sound Tavern_S;

//Event
CP_Sound RatE_S;
CP_Sound ThunderE_S;
CP_Sound Peddler_S;
CP_Sound FireE_S;
CP_Sound EarthquakeE_S;

//BGM
CP_Sound BGM_Game;
CP_Sound BGM_Home;
CP_Sound BGM_Lose;
CP_Sound BGM_Win;

float currentVolume = 1.0f;
bool isBGM_Playing = false;

SOUND_SFX previousChoiceSFX;
bool sfxPlayed = false;

//PlayingSound[20];
void InitSound()
{
    if (!SoundInit) {
        

        //Card Based
        CardDeckHover_S = CP_Sound_Load("./Sounds/CardhoverSFX.wav");
        Click_S = CP_Sound_Load("./Sounds/ClickSFX.wav");
        CardEndAnim_S = CP_Sound_Load("./Sounds/CardSFX.wav");

        //Card Choices
        Choice_LHS = CP_Sound_Load("./Sounds/ChoiceLSFX.wav");
        Choice_RHS = CP_Sound_Load("./Sounds/ChoiceRSFX.wav");

        //Building SFX
        Farm_S = CP_Sound_Load("./Sounds/FarmSFX.wav");
        Gold_S = CP_Sound_Load("./Sounds/GoldSFX.wav");
        House_S = CP_Sound_Load("./Sounds/HouseSFX.wav");
        Market_S = CP_Sound_Load("./Sounds/MarketSFX.wav");
        Tavern_S = CP_Sound_Load("./Sounds/TavernSFX.wav");

        //Event SFX
        RatE_S = CP_Sound_Load("./Sounds/RatESFX.wav");
        ThunderE_S = CP_Sound_Load("./Sounds/ThunderESFX.wav");
        Peddler_S = CP_Sound_Load("./Sounds/PeddlerSFX.wav");
        EarthquakeE_S = CP_Sound_Load("./Sounds/EarthquakeESFX.wav");
        FireE_S = CP_Sound_Load("./Sounds/FireESFX.wav");

        //BGMs
        BGM_Game = CP_Sound_Load("./Sounds/BGMgame.wav");
        BGM_Home = CP_Sound_Load("./Sounds/BGMhome.wav");
        BGM_Lose = CP_Sound_Load("./Sounds/BGMlose.wav");
        BGM_Win = CP_Sound_Load("./Sounds/BGMwin.wav");
        
        SoundInit = true;
    }
}

void StopBGM() {
    CP_Sound_StopAll(); //Stop All Sound before playing new ones
    isBGM_Playing = false;
}

void StopSFX() {
    sfxPlayed = false;
}
void PlayBGM(SOUND_BGM sound_BGM) {
    
    if (isBGM_Playing == false) {

        switch (sound_BGM) {

        case Sound_BGM_Home:
            CP_Sound_PlayAdvanced(BGM_Home, currentVolume, 1.0f, TRUE, CP_SOUND_GROUP_0);
            break;

        case Sound_BGM_Game:
            CP_Sound_PlayAdvanced(BGM_Game, currentVolume, 1.0f, TRUE, CP_SOUND_GROUP_0);
            break;

        case Sound_BGM_Lose:
            CP_Sound_PlayAdvanced(BGM_Lose, currentVolume, 1.0f, TRUE, CP_SOUND_GROUP_0);
            break;

        case Sound_BGM_Win:
            CP_Sound_PlayAdvanced(BGM_Win, currentVolume, 1.0f, TRUE, CP_SOUND_GROUP_0);
            break;
        }

        isBGM_Playing = true;
    }
}

void Play_Building_SFX(int buildingIndex) {

    switch (buildingIndex) {

    case B_HOUSE_INDEX:
        CP_Sound_PlayAdvanced(House_S, currentVolume, 1.0f, FALSE, CP_SOUND_GROUP_0);
        break;

    case B_FARM_INDEX:
        CP_Sound_PlayAdvanced(Farm_S, currentVolume / 2, 1.0f, FALSE, CP_SOUND_GROUP_0);
        break;

    case B_MARKET_INDEX:
        CP_Sound_PlayAdvanced(Market_S, currentVolume / 2, 1.0f, FALSE, CP_SOUND_GROUP_0);
        break;

    case B_TAVERN_INDEX:
        CP_Sound_PlayAdvanced(Tavern_S, currentVolume / 2, 1.0f, FALSE, CP_SOUND_GROUP_0);
        break;
    }
}

void Play_Choice_SFX(SOUND_SFX sound_sfx) {

    if (sound_sfx != previousChoiceSFX) sfxPlayed = false;

    if (!sfxPlayed) {

        switch (sound_sfx) {
        case Sound_SFX_Choice_LHS:
            CP_Sound_PlayAdvanced(Choice_LHS, currentVolume/2, 1.0f, FALSE, CP_SOUND_GROUP_0);
            previousChoiceSFX = Sound_SFX_Choice_LHS;
            break;

        case Sound_SFX_Choice_RHS:
            CP_Sound_PlayAdvanced(Choice_RHS, currentVolume / 2, 1.0f, FALSE, CP_SOUND_GROUP_0);
            previousChoiceSFX = Sound_SFX_Choice_RHS;
            break;
        }

        sfxPlayed = true;
    }
}

void Play_Event_SFX(int eventIndex) {

    switch (eventIndex) {
        //Events
        case 9:
            CP_Sound_PlayAdvanced(Peddler_S, currentVolume / 2, 1.0f, FALSE, CP_SOUND_GROUP_0);
            break;

        case 11:
            CP_Sound_PlayAdvanced(ThunderE_S, currentVolume / 2, 1.0f, FALSE, CP_SOUND_GROUP_0);
            break;

        case 19:
            CP_Sound_PlayAdvanced(FireE_S, currentVolume, 1.0f, FALSE, CP_SOUND_GROUP_0);
            break;

        case 24:
            CP_Sound_PlayAdvanced(RatE_S, currentVolume, 1.0f, FALSE, CP_SOUND_GROUP_0);
            break;     

        case 25:
            CP_Sound_PlayAdvanced(ThunderE_S, currentVolume / 2, 1.0f, FALSE, CP_SOUND_GROUP_0);
            break;
    }

}

void Play_SFX_Sound(SOUND_SFX sound_sfx)
{
    switch (sound_sfx)
    {
    case Sound_SFX_CardHover:
        CP_Sound_Play(CardDeckHover_S);
        break;

    case Sound_SFX_Card:
        CP_Sound_Play(CardEndAnim_S);
        break;

  

    case Sound_SFX_Click:
        CP_Sound_Play(Click_S);
        break;

        //Events

    }     
}

float GetVolume() {
    return currentVolume;
}

void SetVolume(float vol) {
    currentVolume = vol;
    printf("Current Volume: %f", currentVolume);
    CP_Sound_SetGroupVolume(CP_SOUND_GROUP_0, currentVolume);
}

//void Shutdown()
//{
//    switch (SoundIndex)
//    {
//    case CARD:
//        CP_Sound_Free(&CardDeckHover_S);
//        return;
//
//    case HOUSE:
//        CP_Sound_Free(&House_S);
//        return;
//
//    case MARKET:
//        CP_Sound_Free(&Market_S);
//        return;
//
//    case FARM:
//        //CP_Sound_Free(&Farm_S);
//        return;
//
//    case GOLD:
//        CP_Sound_Free(&Gold_S);
//        return;
//
//    case RAT:
//        CP_Sound_Play(RatE_S);
//        return;
//    case 7:
//        CP_Sound_Play(BGM_Game);
//        return;
//    }
//}