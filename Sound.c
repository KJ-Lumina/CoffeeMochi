#include <stdio.h>
#include "cprocessing.h"
#include <stdbool.h>

#define PLAY 1
#define STOP 0

typedef struct{
	int soundIndex;
	int soundState;
	int soundLocation;
}
SOUND;


void Init()
{

    mySound = CP_Sound_Load("./Assets/sounds/placeholderButtonClick.mp3");
}

void PlaySound()
{
    if (CP_Input_MouseTriggered(MOUSE_BUTTON_1))
        CP_Sound_Play(mySound);
    return;
}

void Shutdown()
{
    CP_Sound_Free(&mySound);
}