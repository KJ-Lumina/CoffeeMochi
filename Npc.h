#include <stdio.h>
#include "cprocessing.h"


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
void SpawnNpc(CP_Vector position, int amtToSpawn);
void CalculateNextPosition(NPC* npc);
void UpdateAllNpc();

