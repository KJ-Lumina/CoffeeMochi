#include <stdio.h>
#include "cprocessing.h"
#include "Common_Headers.h"
#include <stdlib.h>
#include "Menu.h"


float windowsWidth;
float windowsHeight;
GAMESCENE gameScene;

CP_Image Splash_Digipen;
CP_Image Splash_CoffeeMochi;
CP_Image mainScreenImage;
CP_Image titleImage;
CP_Image StartButtonImage;
CP_Image StartButtonImageHover;
CP_Image SettingsButtonImage;
CP_Image SettingsButtonImageHover;
CP_Image ExitButtonImage;
CP_Image ExitButtonImageHover;
CP_Image game_Background;

float splashdigipentimer = 0;
float splashcoffeemochitimer = 0;

float mainScreenYLerpStart = 1350;
float mainScreenYLerpEnd = -600;
float titleImageYLerpStart = 250;
float titleImageYLerpEnd = -700;

float entryDuration = 2;
float currentTimer;
CP_Image whiteFlash;

void ExitGame() 
{
	CP_Engine_Terminate();
}


void game_init(void)
{
	
	CP_System_SetWindowSize(1600, 900);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_TOP);
	windowsWidth = (float)CP_System_GetWindowWidth();
	windowsHeight = (float)CP_System_GetWindowHeight();

	//START FROM BEGINNING
	gameScene = SCENE_SPLASH_DIGIPEN;
	//SKIP TO GAME
	//MainGame_Initialize();
	//gameScene = SCENE_GAMEPHASE;
	//CHANGEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
	Splash_Digipen = CP_Image_Load("./ImperoArtAssets/Impero_Digipen.png");
	Splash_CoffeeMochi = CP_Image_Load("./ImperoArtAssets/CoffeeMochi_BG.png");


	mainScreenImage = CP_Image_Load("./ImperoArtAssets/MainMenuAssets/Impero_MainMenu_BG.png");
	titleImage = CP_Image_Load("./ImperoArtAssets/MainMenuAssets/Impero_TitleTrim.png");

	StartButtonImage = CP_Image_Load("./ImperoArtAssets/MainMenuAssets/Impero_StartButton.png");
	StartButtonImageHover = CP_Image_Load("./ImperoArtAssets/MainMenuAssets/Impero_StartButtonHover.png");
	SettingsButtonImage = CP_Image_Load("./ImperoArtAssets/MainMenuAssets/Impero_SettingsButton.png");
	SettingsButtonImageHover = CP_Image_Load("./ImperoArtAssets/MainMenuAssets/Impero_SettingsButtonHover.png");
	ExitButtonImage = CP_Image_Load("./ImperoArtAssets/MainMenuAssets/Impero_ExitButton.png");
	ExitButtonImageHover = CP_Image_Load("./ImperoArtAssets/MainMenuAssets/Impero_ExitButtonHover.png");
	game_Background = CP_Image_Load("./ImperoArtAssets/Impero_GameBG.png");

	whiteFlash = CP_Image_Load("./Assets/WhiteFlash.png");
}

void game_update(void)
{
	CP_Graphics_ClearBackground(CP_Color_Create(150, 150, 150, 255));
	if (gameScene == SCENE_GAMEPHASE)
	{
		CP_Image_Draw(game_Background, 800, 450, 1600, 900, 255);
		MainGame_Update();
	}
	else if (gameScene == SCENE_SPLASH_DIGIPEN)
	{
		CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));
		splashdigipentimer += CP_System_GetDt();
		if (splashdigipentimer < 1)
		{
			CP_Image_Draw(Splash_Digipen, 800, 450, 1600, 900, CP_Math_LerpInt(0, 255, splashdigipentimer / 1));
		}
		else if (splashdigipentimer < 3)
		{
			CP_Image_Draw(Splash_Digipen, 800, 450, 1600, 900, 255);
		}
		else if (splashdigipentimer < 4)
		{
			CP_Image_Draw(Splash_Digipen, 800, 450, 1600, 900, CP_Math_LerpInt(255, 0, (splashdigipentimer - 3) / 1));
		}
		else
		{
			gameScene = SCENE_SPLASH_COFFEEMOCHI;
		}
	}
	else if (gameScene == SCENE_SPLASH_COFFEEMOCHI)
	{
		CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));
		splashcoffeemochitimer += CP_System_GetDt();
		if (splashcoffeemochitimer < 1)
		{
			CP_Image_Draw(Splash_CoffeeMochi, 800, 450, 1600, 900, CP_Math_LerpInt(0, 255, splashcoffeemochitimer / 1));
		}
		else if (splashcoffeemochitimer < 3)
		{
			CP_Image_Draw(Splash_CoffeeMochi, 800, 450, 1600, 900, 255);
		}
		else if (splashcoffeemochitimer < 4)
		{
			CP_Image_Draw(Splash_CoffeeMochi, 800, 450, 1600, 900, CP_Math_LerpInt(255, 0, (splashcoffeemochitimer - 3) / 1));
		}
		else
		{
			gameScene = SCENE_MAINMENU;
		}
	}
	else if (gameScene == SCENE_MAINMENU)
	{
		CP_Image_Draw(mainScreenImage, 800, mainScreenYLerpStart, 1600, 2700, 255);
		CP_Image_Draw(titleImage, 800, titleImageYLerpStart, 985, 440, 255);
		// Play button
		if (CheckWithinBounds(CP_Vector_Set(1200, 700), 328, 99))
		{
			CP_Image_Draw(StartButtonImageHover, 1200, 700, 328, 99, 255);
			if (CP_Input_MouseClicked())
			{
				gameScene = SCENE_GAMEENTRY;
				currentTimer = 0;
			}
		}
		else
		{
			CP_Image_Draw(StartButtonImage, 1200, 700, 328, 99, 255);
		}
		// Settings button
		if (CheckWithinBounds(CP_Vector_Set(800, 700), 328, 99))
		{
			CP_Image_Draw(SettingsButtonImageHover, 800, 700, 328, 99, 255);
		}
		else
		{
			CP_Image_Draw(SettingsButtonImage, 800, 700, 328, 99, 255);
		}
		// Exit button
		if (CheckWithinBounds(CP_Vector_Set(400, 700), 328, 99))
		{
			CP_Image_Draw(ExitButtonImageHover, 400, 700, 328, 99, 255);
		}
		else
		{
			CP_Image_Draw(ExitButtonImage, 400, 700, 328, 99, 255);
		}
	}
	else if (gameScene == SCENE_GAMEENTRY)
	{
		if (currentTimer <= 3)
		{
			currentTimer += CP_System_GetDt();
			CP_Image_Draw(mainScreenImage, windowsWidth / 2, CP_Math_LerpFloat(mainScreenYLerpStart, mainScreenYLerpEnd, currentTimer / entryDuration), 1600, 2700, 255);
			CP_Image_Draw(titleImage, windowsWidth / 2, CP_Math_LerpFloat(titleImageYLerpStart, titleImageYLerpEnd, currentTimer / entryDuration), 985, 440, 255);
			CP_Image_Draw(StartButtonImage, 1200, 700, 328, 99, CP_Math_LerpInt(255, 0, (currentTimer * 2)));
			CP_Image_Draw(SettingsButtonImage, 800, 700, 328, 99, CP_Math_LerpInt(255, 0, (currentTimer * 2)));
			CP_Image_Draw(ExitButtonImage, 400, 700, 328, 99, CP_Math_LerpInt(255, 0, (currentTimer * 2)));

			if (currentTimer >= entryDuration / 2)
			{
				CP_Image_Draw(whiteFlash, windowsWidth / 2, windowsHeight / 2, 1600, 900, CP_Math_LerpInt(0, 255, (currentTimer - (entryDuration / 2) / (entryDuration / 2))));
			}
		}
		if (currentTimer >= entryDuration)
		{
			CP_Image_Draw(whiteFlash, windowsWidth / 2, windowsHeight / 2, 1600, 900, 255);
			MainGame_Initialize();
			gameScene = SCENE_GAMELEAVEENTRY;
			currentTimer = 0;
		}
	}
	else if (gameScene == SCENE_GAMELEAVEENTRY)
	{
		currentTimer += CP_System_GetDt();
		CP_Image_Draw(whiteFlash, windowsWidth / 2, windowsHeight / 2, 1600, 900, CP_Math_LerpInt(255, 0, (currentTimer)));
		if (currentTimer >= 1)
		{
			gameScene = SCENE_GAMEPHASE;
		}
	}
	else if (gameScene == SCENE_ENDPHASE)
	{
		currentTimer += CP_System_GetDt();
		CP_Image_Draw(whiteFlash, windowsWidth / 2, windowsHeight / 2, 1600, 900, CP_Math_LerpInt(0, 255, (currentTimer)));
		if (currentTimer >= 1)
		{
			currentTimer = 0;
			gameScene = SCENE_MAINMENU;
		}
	}
}

void game_exit(void)
{

}

void SetGameSceneEndPhase()
{
	currentTimer = 0;
	gameScene = SCENE_ENDPHASE;
}

int OptionsOpen = FALSE;
CP_Image OptionMenu; //Might consider moving to UI_Mechanics?

void OpenOptions()
{
	OptionsOpen = !OptionsOpen;

	if (OptionsOpen)
	{
		//CP IMAGE DRAW OPTION MENU
	}
	else
	{

	}
}
void ChangeVolume(float vol, CP_SOUND_GROUP soundGroup)
{
	CP_Sound_SetGroupVolume(soundGroup, vol);
}
