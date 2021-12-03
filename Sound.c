#include <stdio.h>
#include "Common_Headers.h"
#include "cprocessing.h"
#include <stdbool.h>
#include "Sound.h"


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

    case Sound_SFX_Choice_LHS:
        CP_Sound_Play(Choice_LHS);
        break;

    case Sound_SFX_Choice_RHS:
        CP_Sound_Play(Choice_RHS);
        break;

    case Sound_SFX_Click:
        CP_Sound_Play(Click_S);
        break;

    //Events

    case Sound_SFX_EarthQuake:
        CP_Sound_Play(EarthquakeE_S);
        break;

    case Sound_SFX_Burning:
        CP_Sound_Play(FireE_S);
        break;

    case Sound_SFX_Peddler:
        CP_Sound_Play(Peddler_S);
        break;

    case Sound_SFX_Thunder:
        CP_Sound_Play(ThunderE_S);
        break;

    case Sound_SFX_Rat:
        CP_Sound_Play(RatE_S);
        break;
       
    //Buildings
    case Sound_SFX_House:
        CP_Sound_Play(House_S);
        break;

    case Sound_SFX_Market:
        CP_Sound_Play(Market_S);
        break;

    case Sound_SFX_Farm:
        CP_Sound_Play(Farm_S);
        break;

    case Sound_SFX_Gold:
        CP_Sound_Play(Gold_S);
        break;

    case Sound_SFX_Tavern:
        CP_Sound_Play(Tavern_S);
        break;
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