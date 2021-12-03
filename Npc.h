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

