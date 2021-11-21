#include <stdio.h>
#include "cprocessing.h"
#include "Common_Headers.h"
#include <stdlib.h>
#include "Menu.h"



int OptionsOpen = FALSE;
CP_Image OptionMenu; //Might consider moving to UI_Mechanics?

void StartGame(GAMEPHASE gamePhase) {
	gamePhase = PHASE_BUILDPHASE;
}

void OpenOptions() {

	OptionsOpen = !OptionsOpen;

	if (OptionsOpen) {
		//CP IMAGE DRAW OPTION MENU
	}
	else {

	}

}

void ChangeVolume(float vol, CP_SOUND_GROUP soundGroup) {

	CP_Sound_SetGroupVolume(soundGroup, vol);

}

void ExitGame() {
	CP_Engine_Terminate();
}


