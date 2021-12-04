/*!_____________________________________________________________________________
@file       Npc.h
@author     Travess Tan (travesscheekhang.t@digipen.edu)
@course     CSD1120
@section    B
@team       CoffeeMochi
@brief      This file contains the NPC related variables & prototype function definitions
			that are used in Impero.

			Copyright ©2021 DigiPen, All rights reserved.
*//*__________________________________________________________________________*/

#include <stdio.h>
#include "cprocessing.h"

#define MAXNPC 100

typedef struct {
	int spriteIndex;
	CP_Vector worldPosition;
	CP_Vector direction;
	int checkPointsLeft;
	float distanceLeft;
	float nextMoveTimer;
	float baseSpeed;
	CP_Vector checkPoints[6];
}NPC;


void InitNpc();

void CalculateNextPosition(NPC* npc);
void UpdateAllNpc();
CP_Vector GetNpc(int index);

