 #include <stdio.h>
#include "cprocessing.h"
#include "Common_Headers.h"
#include <math.h>
#include "Npc.h"
#include <stdbool.h>



NPC npcList[100] = { 0 };
float deltaTime;
CP_Vector worldSpaceOrigin;
CP_Image npcSprite;

void InitNpc()
{
	npcSprite = CP_Image_Load("./Assets/best_Tree.png");
}

void SpawnNpc(CP_Vector position, int amtToSpawn)
{
	NPC newNpc = { 1, position, CP_Vector_Set(0, 0), 0, 0, 0, 500, {0} };
	for (int i = 0; i < 100; ++i)
	{
		if (npcList[i].spriteIndex == 0)
		{
			npcList[i] = newNpc;
			return;
		}
	}
}

CP_Vector FindNearestCheckPoint(CP_Vector position)
{
	int checkPointx = (int)(position.x / TILEWIDTH);
	int checkPointy = (int)(position.y / TILEHEIGHT);
	if (position.x - (checkPointx * TILEWIDTH) >= TILEWIDTH / 2)
	{
		checkPointx = checkPointx * 2 + 2;
	}
	else
	{
		checkPointx = checkPointx * 2 + 1;
	}
	if (position.y - (checkPointy * TILEHEIGHT) >= TILEHEIGHT / 2)
	{
		checkPointy = checkPointy * 2 + 2;
	}
	else
	{
		checkPointy = checkPointy * 2 + 1;
	}
	return CP_Vector_Set((float)checkPointx, (float)checkPointy);
}

void CheckpointToWorldPosition(CP_Vector* checkPoint)
{
	if ((int)checkPoint->x % 2 == 1)
	{
		checkPoint->x = ((int)(checkPoint->x - 1) / 2) * TILEWIDTH + TILEWIDTH / 20;
	}
	else
	{
		checkPoint->x = ((int)(checkPoint->x + 1) / 2) * TILEWIDTH - TILEWIDTH / 20;
	}
	if ((int)checkPoint->y % 2 == 1)
	{
		checkPoint->y = ((int)(checkPoint->y - 1) / 2) * TILEHEIGHT + TILEHEIGHT / 20;
	}
	else
	{
		checkPoint->y = ((int)(checkPoint->y + 1) / 2) * TILEHEIGHT - TILEHEIGHT / 20;
	}
}

int GetRandomCheckPoint(int differenceLeft)
{
	if (differenceLeft == 0)
	{
		return 0;
	}
	else if (differenceLeft > 0)
	{
		return CP_Random_RangeInt(1, differenceLeft);
	}
	else
	{
		int returnInt = CP_Random_RangeInt(1, -differenceLeft);
		return -returnInt;
	}
}

void CalculateNextPosition(NPC* npc)
{
	//moving
	if (npc->nextMoveTimer <= 0)
	{
		
		//Snap NPC to destination
		if (npc->distanceLeft <= deltaTime * npc->baseSpeed)
		{
			npc->worldPosition.x += npc->direction.x * npc->distanceLeft;
			npc->worldPosition.y += npc->direction.y * npc->distanceLeft;
			npc->distanceLeft = 0;
			printf("snap");
		}
		//Move NPC
		else
		{
			npc->worldPosition.x += npc->direction.x * npc->baseSpeed * deltaTime;
			npc->worldPosition.y += npc->direction.y * npc->baseSpeed * deltaTime;
			npc->distanceLeft -= npc->baseSpeed * deltaTime;
		}




		//reached destination
		//set new destination
		if (npc->distanceLeft <= 0)
		{
			if (npc->checkPointsLeft == 0)
			{
				//new idle timer
				//npc->nextMoveTimer = CP_Random_RangeFloat(1, 3);
				npc->nextMoveTimer = 0;
				//add nearest checkpoint
				npc->checkPoints[1] = FindNearestCheckPoint(npc->worldPosition);
				printf("Starting - %f, %f|", npc->checkPoints[1].x, npc->checkPoints[1].y);

				//add other checkpoints
				int finalCheckpointx = CP_Random_RangeInt(1, WORLDGRIDX * 2);
				int finalCheckpointy = CP_Random_RangeInt(1, WORLDGRIDY * 2);
				printf("final: %d,%d\n", finalCheckpointx, finalCheckpointy);

				int checkpointDiffX = finalCheckpointx - (int)npc->checkPoints[1].x;
				int checkpointDiffY = finalCheckpointy - (int)npc->checkPoints[1].y;

				int turnCount = 1;
				bool newX = Math_Abs_Int(checkpointDiffX) > Math_Abs_Int(checkpointDiffY);
				//npc->checkPoints[turnCount + 1] = CP_Vector_Set(npc->checkPoints[turnCount].x, npc->checkPoints[turnCount].y + checkpointDiffY);

				while (checkpointDiffX != 0 && checkpointDiffY != 0)
				{
					if (turnCount < 2)
					{
						if (newX)
						{
							int newXpoint = GetRandomCheckPoint(checkpointDiffX);
							npc->checkPoints[turnCount + 1] = CP_Vector_Set(npc->checkPoints[turnCount].x + newXpoint, npc->checkPoints[turnCount].y);
							checkpointDiffX -= newXpoint;
							printf("X first - %f, %f|", npc->checkPoints[turnCount + 1].x, npc->checkPoints[turnCount + 1].y);
							turnCount++;
							newX = !newX;
						}
						else
						{
							int newYpoint = GetRandomCheckPoint(checkpointDiffY);
							npc->checkPoints[turnCount + 1] = CP_Vector_Set(npc->checkPoints[turnCount].x, npc->checkPoints[turnCount].y + newYpoint);
							checkpointDiffY -= newYpoint;
							printf("Y first - %f, %f|", npc->checkPoints[turnCount + 1].x, npc->checkPoints[turnCount + 1].y);
							turnCount++;
							newX = !newX;
						}
					}
					else if (newX)
					{
						//last 2 turns are maxed
						npc->checkPoints[turnCount + 1] = CP_Vector_Set(npc->checkPoints[turnCount].x + checkpointDiffX, npc->checkPoints[turnCount].y);
						printf("X second - %f, %f|", npc->checkPoints[turnCount + 1].x, npc->checkPoints[turnCount + 1].y);
						turnCount++;
						checkpointDiffX = 0;
						npc->checkPoints[turnCount + 1] = CP_Vector_Set(npc->checkPoints[turnCount].x, npc->checkPoints[turnCount].y + checkpointDiffY);
						printf("Y second - %f, %f\n", npc->checkPoints[turnCount + 1].x, npc->checkPoints[turnCount + 1].y);
						turnCount++;
						checkpointDiffY = 0;
					}
					else
					{
						//last 2 turns are maxed
						npc->checkPoints[turnCount + 1] = CP_Vector_Set(npc->checkPoints[turnCount].x, npc->checkPoints[turnCount].y + checkpointDiffY);
						printf("X second - %f, %f|", npc->checkPoints[turnCount + 1].x, npc->checkPoints[turnCount + 1].y);
						turnCount++;
						checkpointDiffY = 0;
						npc->checkPoints[turnCount + 1] = CP_Vector_Set(npc->checkPoints[turnCount].x + checkpointDiffX, npc->checkPoints[turnCount].y);
						printf("Y second - %f, %f\n", npc->checkPoints[turnCount + 1].x, npc->checkPoints[turnCount + 1].y);
						checkpointDiffX = 0;
					}
				}

				if (checkpointDiffX == 0 && checkpointDiffY != 0)
				{
					npc->checkPoints[turnCount + 1] = CP_Vector_Set(npc->checkPoints[turnCount].x, npc->checkPoints[turnCount].y + checkpointDiffY);
					printf("%f, %f", npc->checkPoints[turnCount + 1].x, npc->checkPoints[turnCount + 1].y);
					turnCount++;
					checkpointDiffY = 0;
				}
				if (checkpointDiffX != 0 && checkpointDiffY == 0)
				{
					npc->checkPoints[turnCount + 1] = CP_Vector_Set(npc->checkPoints[turnCount].x + checkpointDiffX, npc->checkPoints[turnCount].y);
					printf("%f, %f\n", npc->checkPoints[turnCount + 1].x, npc->checkPoints[turnCount + 1].y);
					turnCount++;
					checkpointDiffX = 0;
				}

				npc->checkPointsLeft += turnCount;
				printf(" total checkpoints : %d/", npc->checkPointsLeft);
			}
			if (npc->checkPointsLeft > 0)
			{
				npc->checkPoints[0] = npc->checkPoints[1];
				npc->checkPoints[1] = npc->checkPoints[2];
				npc->checkPoints[2] = npc->checkPoints[3];
				npc->checkPoints[3] = npc->checkPoints[4];
				npc->checkPoints[4] = npc->checkPoints[5];
				npc->checkPointsLeft -= 1;
				printf("before convert||%f,%f||", npc->checkPoints[0].x, npc->checkPoints[0].y);
				CheckpointToWorldPosition(&npc->checkPoints[0]);
				printf("after convert||%f,%f||", npc->checkPoints[0].x, npc->checkPoints[0].y);
				//new direction
				npc->direction.x = npc->checkPoints[0].x - npc->worldPosition.x;
				npc->direction.y = npc->checkPoints[0].y - npc->worldPosition.y;
				if (npc->direction.x != 0)
				{
					npc->direction = CP_Vector_Normalize(npc->direction);
					npc->distanceLeft = Math_Abs(npc->checkPoints[0].x - npc->worldPosition.x) / Math_Abs(npc->direction.x);
				}
				else if (npc->direction.y != 0)
				{
					npc->direction = CP_Vector_Normalize(npc->direction);
					npc->distanceLeft = Math_Abs(npc->checkPoints[0].y - npc->worldPosition.y) / Math_Abs(npc->direction.y);
				}
				else
				{
					npc->distanceLeft = 0;
				}
				printf(" difference x|y: %f, %f", Math_Abs(npc->checkPoints[0].x - npc->worldPosition.x), Math_Abs(npc->checkPoints[0].y - npc->worldPosition.y));
				printf(" direction x|y: %f, %f", npc->direction.x, npc->direction.y);
				printf(" distance left:%f|", npc->distanceLeft);
			}
		}
	}
	//idling
	else
	{
		npc->nextMoveTimer -= deltaTime;
	}
}

float CalculateMoveDistance(float baseSpeed, float distance)
{
	if (distance > 5)
	{
		return baseSpeed;
	}
	if (distance <= 2)
	{
		return baseSpeed / 5;
	}
	else
	{
		distance -= 2;
		baseSpeed *= distance / 3;
	}
	return 1;
}

CP_Vector MoveNpc(CP_Vector position, CP_Vector destination, float distance)
{
	CP_Vector direction;
	direction.x = destination.x - position.x;
	direction.y = destination.y - position.y;
	//float magnitude = CP_Vector_Distance(position, destination);

	return CP_Vector_Set(0, 0);

}

void UpdateAllNpc()
{
	worldSpaceOrigin = GetWorldSpaceOrigin();
	deltaTime = CP_System_GetDt();
	for (int i = 0; i < 100; ++i)
	{
		if (npcList[i].spriteIndex != 0)
		{
			CalculateNextPosition(&npcList[i]);
		}
	}

	for (int i = 0; i < 100; ++i)
	{
		if (npcList[i].spriteIndex != 0)
		{
			CP_Image_Draw(npcSprite, npcList[i].worldPosition.x + worldSpaceOrigin.x , npcList[i].worldPosition.y + worldSpaceOrigin.y, 100, 100, 255);
		}
	}
	CP_Image_Draw(npcSprite, worldSpaceOrigin.x, worldSpaceOrigin.y, 100, 100, 255);
}

