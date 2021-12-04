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
CP_Image OptionsButtonImage;
CP_Image OptionsButtonImageHover;
CP_Image SettingsButtonImage;
CP_Image SettingsButtonImageHover;
CP_Image ExitButtonImage;
CP_Image ExitButtonImageHover;
CP_Image CreditsButtonImage;
CP_Image CreditsButtonImageHover;
CP_Image HowToPlayButtonImage;
CP_Image HowToPlayButtonImageHover;
CP_Image game_Background;
CP_Image game_UIBackground;

bool ShowWinScreen = false;
CP_Image GameWinScreen;
CP_Image GameLoseScreen;

CP_Image ReturnToMainMenuButton;
CP_Image ReturnToMainMenuButtonHover;
CP_Image ResumeGameButton;
CP_Image ResumeGameButtonHover;
CP_Image RestartGameButton;
CP_Image RestartGameButtonHover;
CP_Image CreditsScreenImage;
CP_Image HowToPlayScreenImage;
CP_Image OptionsScreenImage;
CP_Image ResolutionBtn_1600;
CP_Image ResolutionBtn_1920;
CP_Image Vol_Slider;
float sliderMinPos = 620;
float sliderMaxPos = 1050;
CP_Vector currentSliderPos;
float current_Volume = 1.0f;
int scrollSpeed = 6;
int isOptionsOpen = false;
int isCreditRolling = false;

float splashdigipentimer = 0;
float splashcoffeemochitimer = 0;

float mainScreenYLerpStart = 1350;
float mainScreenYLerpEnd = -600;
float exitScreenYLerpStart = 1350;
float exitScreenYLerpEnd = -600;
float titleImageYLerpStart = 250;
float titleImageYLerpEnd = -700;

int fadeInAlpha = 0;
float entryDuration = 2;
float exitDuration = 2;
float currentTimer;
CP_Image whiteFlash;

void ExitGame() 
{
	CP_Engine_Terminate();
}


void game_init(void)
{

	CP_System_SetWindowSize(1600, 900);
	//CP_System_FullscreenAdvanced(1600, 900); //Enable for full screen
	accFont = CP_Font_Load("./Assets/accid.ttf");
	CP_Font_Set(accFont);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_TOP);
	windowsWidth = (float)CP_System_GetWindowWidth();
	windowsHeight = (float)CP_System_GetWindowHeight();

	//START FROM BEGINNING
	gameScene = SCENE_SPLASH_DIGIPEN;

	//Splash Screen Assets
	Splash_Digipen = CP_Image_Load("./ImperoArtAssets/Impero_Digipen.png");
	Splash_CoffeeMochi = CP_Image_Load("./ImperoArtAssets/CoffeeMochi_BG.png");

	//Main Menu Assets
	mainScreenImage = CP_Image_Load("./ImperoArtAssets/MainMenuAssets/Impero_MainMenu_BG.png");
	titleImage = CP_Image_Load("./ImperoArtAssets/MainMenuAssets/Impero_TitleTrim.png");
	StartButtonImage = CP_Image_Load("./ImperoArtAssets/MainMenuAssets/Impero_StartButton.png");
	StartButtonImageHover = CP_Image_Load("./ImperoArtAssets/MainMenuAssets/Impero_StartButtonHover.png");
	OptionsButtonImage = CP_Image_Load("./ImperoArtAssets/MainMenuAssets/Impero_OptionsButton.png");
	OptionsButtonImageHover = CP_Image_Load("./ImperoArtAssets/MainMenuAssets/Impero_OptionsButtonHover.png");
	SettingsButtonImage = CP_Image_Load("./ImperoArtAssets/MainMenuAssets/Impero_SettingsButton.png");
	SettingsButtonImageHover = CP_Image_Load("./ImperoArtAssets/MainMenuAssets/Impero_SettingsButtonHover.png");
	ExitButtonImage = CP_Image_Load("./ImperoArtAssets/MainMenuAssets/Impero_QuitButton.png");
	ExitButtonImageHover = CP_Image_Load("./ImperoArtAssets/MainMenuAssets/Impero_QuitButtonHover.png");
	CreditsButtonImage = CP_Image_Load("./ImperoArtAssets/MainMenuAssets/Impero_CreditsButton.png");
	CreditsButtonImageHover = CP_Image_Load("./ImperoArtAssets/MainMenuAssets/Impero_CreditsButtonHover.png");
	HowToPlayButtonImage = CP_Image_Load("./ImperoArtAssets/MainMenuAssets/Impero_HowtoplayButton.png");
	HowToPlayButtonImageHover = CP_Image_Load("./ImperoArtAssets/MainMenuAssets/Impero_HowtoplayButtonHover.png");
	game_Background = CP_Image_Load("./ImperoArtAssets/Impero_GameBG.png");
	game_UIBackground = CP_Image_Load("./ImperoArtAssets/UI bg.png"); 

	//Win & Lose Screen
	GameWinScreen = CP_Image_Load("./ImperoArtAssets/OtherMenuAssets/Impero_winscreen.png");
	GameLoseScreen = CP_Image_Load("./ImperoArtAssets/OtherMenuAssets/Impero_losescreen.png");

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
	
	//Other Type of Menus
	CreditsScreenImage = CP_Image_Load("./ImperoArtAssets/OtherMenuAssets/Impero_Credits.png");
	HowToPlayScreenImage = CP_Image_Load("./ImperoArtAssets/OtherMenuAssets/Impero_HowToPlay.png");

	//Additional Screen Images
	whiteFlash = CP_Image_Load("./Assets/WhiteFlash.png");
	InitSpritesheets();
	InitSound();
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
		PlayBGM(Sound_BGM_Game);
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
				StopBGM();
				Play_SFX_Sound(Sound_SFX_Click);
				isOptionsOpen = false;			
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
				Play_SFX_Sound(Sound_SFX_Click);
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
				Play_SFX_Sound(Sound_SFX_Click);
				isOptionsOpen = false;
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
			CP_Image_Draw(Splash_Digipen, 800, 450, 1600, 900, CP_Math_LerpInt(0, 255, splashdigipentimer / 1)); //Lerp Through Alpha of the Splash Screen
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
			CP_Image_Draw(Splash_CoffeeMochi, 800, 450, 1600, 900, CP_Math_LerpInt(0, 255, splashcoffeemochitimer / 1)); //Lerp Through Alpha of the Splash Screen
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
		PlayBGM(Sound_BGM_Home);
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
		if (CheckWithinBounds(CP_Vector_Set(800, 550), 328, 99))
		{
			CP_Image_Draw(StartButtonImageHover, 800, 550, 328, 99, 255);
			if (CP_Input_MouseClicked())
			{
				Play_SFX_Sound(Sound_SFX_Click);
				gameScene = SCENE_GAMEENTRY;
				currentTimer = 0;
			}
		}
		else
		{
			//crazy ass star near start button (Main menu animations)
			ConstantAnimSpawner(3, 2, 5.f, 970, 970, 500, 500, 300, 300, 0.75f, 0, 0);
			CP_Image_Draw(StartButtonImage, 800, 550, 328, 99, 255);
		}

		// How To Play Button
		if (CheckWithinBounds(CP_Vector_Set(275, 700), 328, 99))
		{
			CP_Image_Draw(HowToPlayButtonImageHover, 275, 700, 328, 99, 255);
			if (CP_Input_MouseClicked()) {
				Play_SFX_Sound(Sound_SFX_Click);
				gameScene = SCENE_HOWTOPLAY;
				currentTimer = 0;
			}
		}
		else
		{
			CP_Image_Draw(HowToPlayButtonImage, 275, 700, 328, 99, 255);
		}

		// Optionss button
		if (CheckWithinBounds(CP_Vector_Set(625, 700), 328, 99))
		{
			CP_Image_Draw(OptionsButtonImageHover, 625, 700, 328, 99, 255);
			if (CP_Input_MouseClicked())
			{
				Play_SFX_Sound(Sound_SFX_Click);
				gameScene = SCENE_OPTIONS;
				currentTimer = 0;
			}
		}
		else
		{
			CP_Image_Draw(OptionsButtonImage, 625, 700, 328, 99, 255);
		}

		// Credit Button
		if (CheckWithinBounds(CP_Vector_Set(975, 700), 328, 99))
		{
			CP_Image_Draw(CreditsButtonImageHover, 975, 700, 328, 99, 255);
			if (CP_Input_MouseClicked()) {
				Play_SFX_Sound(Sound_SFX_Click);
				gameScene = SCENE_CREDITS;
				currentTimer = 0;
			}
		}
		else
		{
			CP_Image_Draw(CreditsButtonImage, 975, 700, 328, 99, 255);
		}

		// Exit button
		if (CheckWithinBounds(CP_Vector_Set(1325, 700), 328, 99))
		{
			CP_Image_Draw(ExitButtonImageHover, 1325, 700, 328, 99, 255);
			if (CP_Input_MouseClicked()) {
				StopBGM();
				Play_SFX_Sound(Sound_SFX_Click);
				ExitGame();
			}
		}
		else
		{
			CP_Image_Draw(ExitButtonImage, 1325, 700, 328, 99, 255);
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
				Play_SFX_Sound(Sound_SFX_Click);
				gameScene = SCENE_MAINMENU;
				currentTimer = 0;
			}
		}
		else {
			CP_Image_Draw(ReturnToMainMenuButton, 800, 700, 281, 87, 255);
		}
		
		CP_Image_Draw(Vol_Slider, currentSliderPos.x, currentSliderPos.y, 35, 58, 255);
		CP_Image_Draw(ResolutionBtn_1600, 800, 365, 241,90, 255);
		AdjustVolumeSlider(); //Update the Volume Slider Position and Volume
	}

	else if (gameScene == SCENE_CREDITS) {

		CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));
		CP_Image_Draw(CreditsScreenImage, 800, 1000, 1600, 2000, 255);
		
		if (CP_Input_MouseTriggered(0) && isCreditRolling == false) {
			isCreditRolling = true;	
		}

		if (CP_Input_MouseDown(0)) {
			scrollSpeed = 2;
		}
		else {
			scrollSpeed = 6;
		}

		if (isCreditRolling) {
			if (currentTimer <= 1) {
				currentTimer += CP_System_GetDt() / scrollSpeed;
				CP_Image_Draw(CreditsScreenImage, 800, CP_Math_LerpFloat(1000, -100, currentTimer), 1600, 2000, 255);

			}

			if (currentTimer > 1) {
				CP_Image_Draw(CreditsScreenImage, 800, -100, 1600, 2000, 255);
				if (CheckWithinBounds(CP_Vector_Set(1300, 475), 281, 87))
				{
					CP_Image_Draw(ReturnToMainMenuButtonHover, 1300, 475, 281, 87, 255);
					if (CP_Input_MouseClicked())
					{
						Play_SFX_Sound(Sound_SFX_Click);
						gameScene = SCENE_MAINMENU;
						isCreditRolling = false;
						scrollSpeed = 6;
						currentTimer = 0;
					}
				}
				else {
					CP_Image_Draw(ReturnToMainMenuButton, 1300, 475, 281, 87, 255);
				}
			}
		}
	}

	else if (gameScene == SCENE_HOWTOPLAY) {
		CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));

		CP_Image_Draw(HowToPlayScreenImage, 800, 450, 1600, 900, 255);

		if (CheckWithinBounds(CP_Vector_Set(1425, 75), 281, 87))
		{
			CP_Image_Draw(ReturnToMainMenuButtonHover, 1425, 75, 281, 87, 255);
			if (CP_Input_MouseClicked())
			{
				Play_SFX_Sound(Sound_SFX_Click);
				gameScene = SCENE_MAINMENU;
				currentTimer = 0;
			}
		}

		else {
			CP_Image_Draw(ReturnToMainMenuButton, 1425, 75, 281, 87, 255);
		}
	}

	else if (gameScene == SCENE_GAMEENTRY)
	{
		if (currentTimer <= 3)
		{
			currentTimer += CP_System_GetDt();
			CP_Image_Draw(mainScreenImage, windowsWidth / 2, CP_Math_LerpFloat(mainScreenYLerpStart, mainScreenYLerpEnd, currentTimer / entryDuration), 1600, 2700, 255);
			CP_Image_Draw(titleImage, windowsWidth / 2, CP_Math_LerpFloat(titleImageYLerpStart, titleImageYLerpEnd, currentTimer / entryDuration), 985, 440, 255);
			CP_Image_Draw(ExitButtonImageHover, 1325, 700, 328, 99, CP_Math_LerpInt(255, 0, (currentTimer * 2)));
			CP_Image_Draw(CreditsButtonImageHover, 975, 700, 328, 99, CP_Math_LerpInt(255, 0, (currentTimer * 2)));
			CP_Image_Draw(OptionsButtonImageHover, 625, 700, 328, 99, CP_Math_LerpInt(255, 0, (currentTimer * 2)));
			CP_Image_Draw(HowToPlayButtonImageHover, 275, 700, 328, 99, CP_Math_LerpInt(255, 0, (currentTimer * 2)));
			CP_Image_Draw(StartButtonImageHover, 800, 550, 328, 99, CP_Math_LerpInt(255, 0, (currentTimer * 2)));
			fadeInAlpha = CP_Math_LerpInt(0, 255, ((currentTimer - (entryDuration / 2)) / (entryDuration / 2)));

			if (currentTimer >= entryDuration / 2)
			{				
				CP_Image_Draw(whiteFlash, windowsWidth / 2, windowsHeight / 2, 1600, 900, fadeInAlpha);
			}

			if (currentTimer >= entryDuration) {
				DrawIntroNarritive(fadeInAlpha);
			}
		}

		if (currentTimer >= entryDuration)
		{
			CP_Image_Draw(whiteFlash, windowsWidth / 2, windowsHeight / 2, 1600, 900, 255);

			DrawIntroNarritive(255);

			if (CP_Input_MouseClicked()) {
				Play_SFX_Sound(Sound_SFX_Click);
				MainGame_Initialize();
				currentTimer = 2;
				gameScene = SCENE_GAMEENTRY_EXIT;
			}
		}
	}
	else if (gameScene == SCENE_GAMEENTRY_EXIT)
	{
		if (currentTimer <= 2)
		{
			currentTimer -= CP_System_GetDt();
			fadeInAlpha = CP_Math_LerpInt(0, 255, ((currentTimer - (entryDuration / 2)) / (entryDuration / 2)));
			DrawIntroNarritive(fadeInAlpha);
			CP_Image_Draw(whiteFlash, windowsWidth / 2, windowsHeight / 2, 1600, 900, fadeInAlpha);
		}
		if (currentTimer <= 0)
		{
			CP_Image_Draw(whiteFlash, windowsWidth / 2, windowsHeight / 2, 1600, 900, 0);
			DrawIntroNarritive(0);
			currentTimer = 0;
			StopBGM();
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
			StopBGM();
			gameScene = SCENE_GAMEPHASE;
		}

	}
	else if (gameScene == SCENE_ENDENTRY)
	{
		if (currentTimer <= 3)
		{		
			currentTimer -= CP_System_GetDt();
			CP_Image_Draw(mainScreenImage, windowsWidth / 2, CP_Math_LerpFloat(exitScreenYLerpStart, exitScreenYLerpEnd, currentTimer / exitDuration), 1600, 2700, 255);

			if (currentTimer >= exitDuration / 2)
			{
				currentTimer -= CP_System_GetDt()/2;
				CP_Image_Draw(whiteFlash, windowsWidth / 2, windowsHeight / 2, 1600, 900, CP_Math_LerpInt(0, 255, ((currentTimer - (exitDuration / 2)) / (exitDuration / 2))));
			}

		}
		if (currentTimer <= 0)
		{
			currentTimer = 0;
			StopBGM();
			gameScene = SCENE_ENDPHASE;
		}		
	}
	else if (gameScene == SCENE_ENDPHASE)
	{	
		if (ShowWinScreen) {
			PlayBGM(Sound_BGM_Win);
			CP_Image_Draw(GameWinScreen, 800, 450, 1600, 900, 255);
		}
		else {
			PlayBGM(Sound_BGM_Lose);
			CP_Image_Draw(GameLoseScreen, 800, 450, 1600, 900, 255);
		}

		if (CheckWithinBounds(CP_Vector_Set(1100, 700), 281, 87))
		{
			CP_Image_Draw(ReturnToMainMenuButtonHover, 1100, 700, 281, 87, 255);
			if (CP_Input_MouseClicked())
			{
				StopBGM();
				Play_SFX_Sound(Sound_SFX_Click);
				gameScene = SCENE_MAINMENU;
				currentTimer = 0;
			}
		}
		else {
			CP_Image_Draw(ReturnToMainMenuButton, 1100, 700, 281, 87, 255);
		}

		if (CheckWithinBounds(CP_Vector_Set(500, 700), 281, 87))
		{
			CP_Image_Draw(RestartGameButtonHover, 500, 700, 281, 87, 255);
			if (CP_Input_MouseClicked())
			{
				StopBGM();
				Play_SFX_Sound(Sound_SFX_Click);
				RestartGame();
				currentTimer = 0;
			}
		}
		else {
			CP_Image_Draw(RestartGameButton, 500, 700, 281, 87, 255);
		}
	}
}

void game_exit(void)
{

}

void DrawIntroNarritive(int alpha) 
{
		

	CP_Settings_TextSize(60);

	CP_Settings_Fill(CP_Color_Create(128, 128, 128, alpha));

	//Draw Text for Starting Narrative
	CP_Font_DrawTextBox("In the year 1300 A.D. in an unknown kingdom.\n The king has been dethroned and exiled due to a revolt against him.\n However, the citizens have high expectations of you, the heir apparent to lead the country into prosperity.\n\n You would do well to not betray their expectations. Good luck.\n\n\n\n - Click Anywhere to Continue -", 350, 150, 900);
}

void SetGameSceneEndPhase(int isWin)
{
	currentTimer = 3;
	ShowWinScreen = isWin;
	gameScene = SCENE_ENDENTRY;
}

void OpenOptions()
{
	if (!isOptionsOpen) isOptionsOpen = true;
}

bool adjustingVol = false;

void AdjustVolumeSlider() {

	float mouseX = CP_Input_GetMouseX();

	if (!adjustingVol)
	{
		if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT) && CheckWithinBounds(currentSliderPos, 35, 58))
		{
			adjustingVol = true;
		}
	}
	if (adjustingVol)
	{
		if (CP_Input_MouseReleased(MOUSE_BUTTON_LEFT))
		{
			adjustingVol = false;
		}
		else
		{
			float volClamped = CP_Math_ClampFloat(mouseX, sliderMinPos, sliderMaxPos);
			currentSliderPos = CP_Vector_Set(volClamped, currentSliderPos.y);

			SetVolume((volClamped - sliderMinPos) / (sliderMaxPos - sliderMinPos));
		}
	}
}

