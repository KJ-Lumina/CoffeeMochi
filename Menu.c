#include <stdio.h>
#include "cprocessing.h"
#include "Common_Headers.h"
#include <stdlib.h>
#include "Menu.h"

CP_Font accFont;
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
CP_Image game_UIBackground;

CP_Image ReturnToMainMenuButton;
CP_Image ReturnToMainMenuButtonHover;
CP_Image ResumeGameButton;
CP_Image ResumeGameButtonHover;
CP_Image RestartGameButton;
CP_Image RestartGameButtonHover;
CP_Image OptionsScreenImage;
CP_Image ResolutionBtn_1600;
CP_Image ResolutionBtn_1920;
CP_Image Vol_Slider;
float sliderMinPos = 620;
float sliderMaxPos = 1050;
CP_Vector currentSliderPos;
float current_Volume = 1.0f;
bool isOptionsOpen = false;

CP_Sound Test_BGM; //Delete after use

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
	accFont = CP_Font_Load("./Assets/accid.ttf");
	CP_Font_Set(accFont);
	//CP_System_FullscreenAdvanced(1600, 900); //Enable for full screen
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_TOP);
	windowsWidth = (float)CP_System_GetWindowWidth();
	windowsHeight = (float)CP_System_GetWindowHeight();

	//START FROM BEGINNING
	//gameScene = SCENE_SPLASH_DIGIPEN;
	//SKIP TO GAME
	MainGame_Initialize();
	gameScene = SCENE_GAMEPHASE;
	//CHANGEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
	Splash_Digipen = CP_Image_Load("./ImperoArtAssets/Impero_Digipen.png");
	Splash_CoffeeMochi = CP_Image_Load("./ImperoArtAssets/CoffeeMochi_BG.png");

	//Main Menu Assets
	mainScreenImage = CP_Image_Load("./ImperoArtAssets/MainMenuAssets/Impero_MainMenu_BG.png");
	titleImage = CP_Image_Load("./ImperoArtAssets/MainMenuAssets/Impero_TitleTrim.png");
	StartButtonImage = CP_Image_Load("./ImperoArtAssets/MainMenuAssets/Impero_StartButton.png");
	StartButtonImageHover = CP_Image_Load("./ImperoArtAssets/MainMenuAssets/Impero_StartButtonHover.png");
	SettingsButtonImage = CP_Image_Load("./ImperoArtAssets/MainMenuAssets/Impero_SettingsButton.png");
	SettingsButtonImageHover = CP_Image_Load("./ImperoArtAssets/MainMenuAssets/Impero_SettingsButtonHover.png");
	ExitButtonImage = CP_Image_Load("./ImperoArtAssets/MainMenuAssets/Impero_ExitButton.png");
	ExitButtonImageHover = CP_Image_Load("./ImperoArtAssets/MainMenuAssets/Impero_ExitButtonHover.png");
	game_Background = CP_Image_Load("./ImperoArtAssets/Impero_GameBG.png");
	game_UIBackground = CP_Image_Load("./ImperoArtAssets/UI bg.png"); 

	//Options Assets
	ReturnToMainMenuButton = CP_Image_Load("./ImperoArtAssets/OtherMenuAssets/Impero_ReturnButton.png");
	ReturnToMainMenuButtonHover = CP_Image_Load("./ImperoArtAssets/OtherMenuAssets/Impero_ReturnButtonHover.png");
	ResumeGameButton = CP_Image_Load("./ImperoArtAssets/OtherMenuAssets/Impero_ResumeButton.png");
	ResumeGameButtonHover = CP_Image_Load("./ImperoArtAssets/OtherMenuAssets/Impero_ResumeButtonHover.png");
	RestartGameButton = CP_Image_Load("./ImperoArtAssets/OtherMenuAssets/Impero_RestartButton.png");
	RestartGameButtonHover = CP_Image_Load("./ImperoArtAssets/OtherMenuAssets/Impero_RestartButtonHover.png");
	OptionsScreenImage = CP_Image_Load("./ImperoArtAssets/OtherMenuAssets/Impero_Options.png");
	ResolutionBtn_1600 = CP_Image_Load("./ImperoArtAssets/OtherMenuAssets/Impero_1600.png");
	ResolutionBtn_1920 = CP_Image_Load("./ImperoArtAssets/OtherMenuAssets/Impero_1920.png");
	Vol_Slider = CP_Image_Load("./ImperoArtAssets/OtherMenuAssets/Impero_slider.png");
	currentSliderPos = CP_Vector_Set(1050, 508);
	
	whiteFlash = CP_Image_Load("./Assets/WhiteFlash.png");
	InitSpritesheets();

	Test_BGM = CP_Sound_Load("./ImperoArtAssets/Music/Test_BGM.wav");
	//CP_Sound_PlayAdvanced(Test_BGM, 1.0f, 1.0f, TRUE, CP_SOUND_GROUP_0);
}

void RestartGame()
{
	currentTimer = 0;
	gameScene = SCENE_RESTART;
}

void game_update(void)
{
	CP_Graphics_ClearBackground(CP_Color_Create(150, 150, 150, 255));
	
	if (gameScene == SCENE_GAMEPHASE)
	{
		CP_Image_Draw(game_Background, 800, 450, 1600, 900, 255);
		CP_Image_Draw(game_UIBackground, 275, 450, 550, 900, 255);
		MainGame_Update();
		if (currentTimer < 1)
		{
			currentTimer += CP_System_GetDt();
			CP_Image_Draw(whiteFlash, windowsWidth / 2, windowsHeight / 2, 1600, 900, CP_Math_LerpInt(255, 0, (currentTimer)));
		}

		if (isOptionsOpen) {
			gameScene = SCENE_GAMEOPTIONS;
		}
	}
	else if (gameScene == SCENE_GAMEOPTIONS) {

		CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));

		CP_Image_Draw(OptionsScreenImage, 800, 450, 1600, 900, 255);

		//Return to main menu
		if (CheckWithinBounds(CP_Vector_Set(1150, 700), 281, 87))
		{
			CP_Image_Draw(ReturnToMainMenuButtonHover, 1150, 700, 281, 87, 255);
			if (CP_Input_MouseClicked())
			{
				gameScene = SCENE_MAINMENU;
				currentTimer = 0;
			}
		}
		else {
			CP_Image_Draw(ReturnToMainMenuButton, 1150, 700, 281, 87, 255);
		}

		if (CheckWithinBounds(CP_Vector_Set(450, 700), 281, 87))
		{
			CP_Image_Draw(ResumeGameButtonHover, 450, 700, 281, 87, 255);
			if (CP_Input_MouseClicked())
			{
				isOptionsOpen = false;
				gameScene = SCENE_GAMEPHASE;
			}
		}
		else {
			CP_Image_Draw(ResumeGameButton, 450, 700, 281, 87, 255);
		}

		if (CheckWithinBounds(CP_Vector_Set(800, 700), 281, 87))
		{
			CP_Image_Draw(RestartGameButtonHover, 800, 700, 281, 87, 255);
			if (CP_Input_MouseClicked())
			{
				RestartGame();
				currentTimer = 0;
			}
		}
		else {
			CP_Image_Draw(RestartGameButton, 800, 700, 281, 87, 255);
		}

		CP_Image_Draw(Vol_Slider, currentSliderPos.x, currentSliderPos.y, 35, 58, 255);
		CP_Image_Draw(ResolutionBtn_1600, 800, 365, 241, 90, 255);
		AdjustVolumeSlider();

	}
	else if (gameScene == SCENE_SPLASH_DIGIPEN)
	{
		CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));
		splashdigipentimer += CP_System_GetDt();
		if (CP_Input_MouseClicked())
		{
			gameScene = SCENE_SPLASH_COFFEEMOCHI;
		}
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
		if (CP_Input_MouseClicked())
		{
			gameScene = SCENE_MAINMENU;
		}
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
		ConstantAnimSpawner(1, 2, 1, 30, 1550, 10, 500, 50, 50, 0.4f, 1, 0);
		ConstantAnimSpawner(1, 2, 1, 30, 1550, 10, 500, 50, 50, 0.4f, 1, 0);
		ConstantAnimSpawner(2, 3, 5, 600, 1800, 0, 500, 50, 50, 1.75f, 1, 1);
		ConstantAnimSpawner(1, 2, 1, 30, 1550, 10, 600, 100, 100, 0.75f, 1, 0);
		if (CP_Input_KeyTriggered(KEY_L))
		{
			SpawnAnimation(1550, 10, 0, 600, 100, 100, 3, 1, 0);
		}
		CP_Image_Draw(mainScreenImage, 800, mainScreenYLerpStart, 1600, 2700, 255);
		DrawAllAnimations();
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
			//crazy ass star near start button
			ConstantAnimSpawner(3, 2, 5.f, 1370, 1370, 645, 645, 300, 300, 0.75f, 0, 0);
			CP_Image_Draw(StartButtonImage, 1200, 700, 328, 99, 255);
		}
		// Settings button
		if (CheckWithinBounds(CP_Vector_Set(800, 700), 328, 99))
		{
			CP_Image_Draw(SettingsButtonImageHover, 800, 700, 328, 99, 255);
			if (CP_Input_MouseClicked())
			{
				gameScene = SCENE_OPTIONS;
				currentTimer = 0;
			}
		}
		else
		{
			CP_Image_Draw(SettingsButtonImage, 800, 700, 328, 99, 255);
		}
		// Exit button
		if (CheckWithinBounds(CP_Vector_Set(400, 700), 328, 99))
		{
			CP_Image_Draw(ExitButtonImageHover, 400, 700, 328, 99, 255);
			if (CP_Input_MouseClicked()) {
				ExitGame();
			}
		}
		else
		{
			CP_Image_Draw(ExitButtonImage, 400, 700, 328, 99, 255);
		}
	}
	else if (gameScene == SCENE_OPTIONS) {

		CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));

		CP_Image_Draw(OptionsScreenImage, 800, 450, 1600, 900, 255);

		if (CheckWithinBounds(CP_Vector_Set(800, 700), 281, 87))
		{
			CP_Image_Draw(ReturnToMainMenuButtonHover, 800, 700, 281, 87, 255);
			if (CP_Input_MouseClicked())
			{
				gameScene = SCENE_MAINMENU;
				currentTimer = 0;
			}
		}
		else {
			CP_Image_Draw(ReturnToMainMenuButton, 800, 700, 281, 87, 255);
		}
		
		CP_Image_Draw(Vol_Slider, currentSliderPos.x, currentSliderPos.y, 35, 58, 255);
		CP_Image_Draw(ResolutionBtn_1600, 800, 365, 241,90, 255);
		AdjustVolumeSlider();


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
			currentTimer = 0;
			gameScene = SCENE_GAMEPHASE;
		}
	}
	else if (gameScene == SCENE_RESTART)
	{
		currentTimer += CP_System_GetDt();
		if (currentTimer <= 1)
		{
			CP_Image_Draw(game_Background, 800, 450, 1600, 900, 255);
			MainGame_Update();
			CP_Image_Draw(whiteFlash, windowsWidth / 2, windowsHeight / 2, 1600, 900, CP_Math_LerpInt(0, 255, (currentTimer)));
		}
		else
		{
			CP_Image_Draw(whiteFlash, windowsWidth / 2, windowsHeight / 2, 1600, 900, 255);
			MainGame_Initialize();
			currentTimer = 0;
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

void OpenOptions()
{
	if (!isOptionsOpen) isOptionsOpen = true;
}
void ChangeVolume(float vol)
{
	current_Volume = vol;
	CP_Sound_SetGroupVolume(CP_SOUND_GROUP_0, current_Volume);
}

void AdjustVolumeSlider() {

	float mouseX = CP_Input_GetMouseX();
	//float mouseY = CP_Input_GetMouseY();
	CP_Vector previousSliderPos = currentSliderPos;

	if (CP_Input_MouseDragged(MOUSE_BUTTON_LEFT)) {		
		if (CheckWithinBounds(currentSliderPos, 35, 58)) {
			float sliderMovement = mouseX - previousSliderPos.x;
			int withinMaxPos = currentSliderPos.x + sliderMovement <= sliderMaxPos;
			int withinMinPos = currentSliderPos.x + sliderMovement >= sliderMinPos;
			if (withinMaxPos && withinMinPos) {
				float newSliderPosX = currentSliderPos.x + sliderMovement;
				currentSliderPos = CP_Vector_Set(newSliderPosX, currentSliderPos.y);
			}

			float vol_percentageChange = (sliderMovement / (sliderMaxPos - sliderMinPos));
			
			float newVol = current_Volume + vol_percentageChange;

			if (newVol <= 0.0f) {
				newVol = 0.0f;
			}
			else if (newVol >= 1.0f) {
				newVol = 1.0f;
			}

			ChangeVolume(newVol);
		}

	}

}

