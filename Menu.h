/*!_____________________________________________________________________________
@file       Menu.h
@author     Travess Tan (travesscheekhang.t@digipen.edu)
@co-authors Lee Xin Qian (xinqian.lee@digipen.edu)
            Seow Kai Jun (s.kaijun@digipen.edu)
@course     CSD1120
@section    B
@team       CoffeeMochi
@brief      This file contains the prototype function definition
            for all the different menus in Impero

            Copyright ©2021 DigiPen, All rights reserved.
*//*__________________________________________________________________________*/

#include <stdio.h>
#include "CProcessing.h"

/*--------------------
OPEN OPTIONS
---------------------*/
/*!_____________________________________________________________________________
@brief      This prototype function toggle the bool variable isOptionsOpen to manage
            if the Options Menu is Open.
*//*__________________________________________________________________________*/
void OpenOptions();

/*--------------------
EXIT GAME
---------------------*/
/*!_____________________________________________________________________________
@brief      This prototype function is called to exit the game
*//*__________________________________________________________________________*/
void ExitGame();

/*--------------------
DRAW INTRODUCTION NARRATIVE
---------------------*/
/*!_____________________________________________________________________________
@brief      This prototype function draw text for the introduction of Impero
            on to the screen at a specific font size
*//*__________________________________________________________________________*/
void DrawIntroNarritive(int alpha);

/*--------------------
ADJUST VOLUME SLIDER
---------------------*/
/*!_____________________________________________________________________________
@brief      This prototype function adjust the position of the volume sliders by the
            mouse position and set the adjusted volume.
*//*__________________________________________________________________________*/
void AdjustVolumeSlider();