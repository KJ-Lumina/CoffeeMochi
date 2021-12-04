/*!_____________________________________________________________________________
@file       WorldSpaceGrid.h
@author     Travess Tan (travesscheekhang.t@digipen.edu)
@course     CSD1120
@section    B
@team       CoffeeMochi
@brief      This file contains the variables & prototype function definitions that
            interacts with the Grid in Impero.

            Copyright ©2021 DigiPen, All rights reserved.
*//*__________________________________________________________________________*/

#include <stdio.h>
#include "cprocessing.h"


void InitWorldSpaceGrid();
void ScreenToWorldPosition(CP_Vector*);
void ScreenToGridPosition(CP_Vector*);
void WorldToGridPosition(CP_Vector*);

void ReturnToCenter();
void DrawBuildings();
void DrawTileSet();

