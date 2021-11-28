#include <stdio.h>
#include "cprocessing.h"
#include <stdbool.h>
#include "Sound.h"

#define PLAY 1
#define STOP 0

#define CARD 0 
#define HOUSE 1
#define MARKET 2
#define FARM 3
#define TAVERN 4
#define GOLD 5
#define RAT 6


typedef struct {
    int soundIndex;
    int soundState;
    int soundLocation;
}
SOUND;

CP_Sound CardDeck_S;
CP_Sound Click_S;
CP_Sound Gold_S;
CP_Sound House_S;
CP_Sound Market_S;
CP_Sound RatE_S;
CP_Sound Tavern_S;
CP_Sound Crops_S;
CP_Sound BGM_Test;
//PlayingSound[20];
void InitSound()
{
    CP_Sound_StopAll(); //Stop All Sound before playing new ones
    CardDeck_S = CP_Sound_Load("./Sounds/CardSFX.wav3");
    Click_S = CP_Sound_Load("./Sounds/ClickSFX.wav3");
    Gold_S = CP_Sound_Load("./Sounds/GoldSFX.wav3");
    House_S = CP_Sound_Load("./Sounds/HouseSFX.wav3");
    Market_S = CP_Sound_Load("./Sounds/MarketSFX.wav3");
    RatE_S = CP_Sound_Load("./Sounds/RatSFX.wav3");
    Tavern_S = CP_Sound_Load("./Sounds/TavernSFX.wav3");
    Crops_S = CP_Sound_Load("./Sounds/HouseSFX.wav3");
    BGM_Test = CP_Sound_Load("./ImperoArtAssets/Music/Test_BGMT.wav");
    LoopSound();
    
}
void LoopSound() {
    CP_Sound_PlayAdvanced(BGM_Test, 1.0f, 1.0f, TRUE, CP_SOUND_GROUP_1);
}

void PlaySound(int SoundIndex)
{
    if (CP_Input_MouseTriggered(MOUSE_BUTTON_1))
    {
        CP_Sound_Play(Click_S);
        CP_Sound_Free(&Click_S);
    }
    return;
    switch (SoundIndex)
    {
    case CARD:
        CP_Sound_Play(CardDeck_S);
        return;

    case HOUSE:
        CP_Sound_Play(House_S);
        return;

    case MARKET:
        CP_Sound_Play(Market_S);
        return;

    case FARM:
        //CP_Sound_Play(Farm_S);
        return;

    case GOLD:
        CP_Sound_Play(Gold_S);
        return;

    case RAT:
        CP_Sound_Play(RatE_S);
        return;
    }

}

void Shutdown(int SoundIndex)
{
    switch (SoundIndex)
    {
    case CARD:
        CP_Sound_Free(&CardDeck_S);
        return;

    case HOUSE:
        CP_Sound_Free(&House_S);
        return;

    case MARKET:
        CP_Sound_Free(&Market_S);
        return;

    case FARM:
        //CP_Sound_Free(&Farm_S);
        return;

    case GOLD:
        CP_Sound_Free(&Gold_S);
        return;

    case RAT:
        CP_Sound_Play(RatE_S);
        return;
    case 7:
        CP_Sound_Play(BGM_Test);
        return;
    }


}