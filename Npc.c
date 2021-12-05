/*!_____________________________________________________________________________
@file       Npc.c
@author     Travess Tan (travesscheekhang.t@digipen.edu)
@course     CSD1120
@section    B
@team       CoffeeMochi
@brief      This file contains the NPC related variables & function definitions
			that are used in Impero.

			Copyright ©2021 DigiPen, All rights reserved.
*//*__________________________________________________________________________*/

#include <stdio.h>
#include "cprocessing.h"
#include "Common_Headers.h"
#include <math.h>
#include "Npc.h"
#include <stdbool.h>

NPC npcList[MAXNPC] = { 0 };
float deltaTime;
CP_Vector worldSpaceOrigin;
CP_Image npcSprite;


/*!_____________________________________________________________________________
@brief      This function loads images and clears all npcs in the game.
*//*__________________________________________________________________________*/
void InitNpc()
{
	npcSprite = CP_Image_Load("./Assets/best_npc.png");

	for (int i = 0; i < MAXNPC; i++)
	{
		npcList[i] = (NPC){ 0 };
	}
}

/*!_____________________________________________________________________________
@brief      This function spawns npcs with given locations.
*//*__________________________________________________________________________*/
void SpawnNpc(CP_Vector position, int amtToSpawn)
{
	for (int i = 0; i < amtToSpawn; ++i)
	{
		NPC newNpc = { 1, position, CP_Vector_Set(0, 0), 0, 0, 0, CP_Random_RangeFloat(60,120), {0} };
		for (int j = 0; j < MAXNPC; ++j)
		{
			if (npcList[j].spriteIndex == 0)
			{
				npcList[j] = newNpc;
				return;
			}
		}
	}
}

/*!_____________________________________________________________________________
@brief      This function searches for nearest position on the grid.
			This is called when the npc is first spawned,
			to ensure it moves to within the grid space from outside
			the map.

			Checkpoints are allocated to 4 corners of each tile.
*//*__________________________________________________________________________*/
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

/*!_____________________________________________________________________________
@brief      This function converts simple npc checkpoints to world positions.
*//*__________________________________________________________________________*/
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

/*!_____________________________________________________________________________
@brief      This function returns a random number between 1 and given parameter
			This is to create a turning point for the npc to create a variety
			of movements before reaching the destination.
*//*__________________________________________________________________________*/
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

/*!_____________________________________________________________________________
@brief      This function calculates npc's new position and stores it within
			its struct object.

			Further comments are within the function.
*//*__________________________________________________________________________*/
void CalculateNextPosition(NPC* npc)
{
	//-------------------------------------------------------
	// Nextmovetimer determines whether the npc is currently
	// moving or idling. If it is below 0, it will move and
	// calculate its new position.
	//-------------------------------------------------------
	if (npc->nextMoveTimer <= 0)
	{
		//Snap NPC to destination when near to prevent issues.
		if (npc->distanceLeft <= deltaTime * npc->baseSpeed)
		{
			npc->worldPosition.x += npc->direction.x * npc->distanceLeft;
			npc->worldPosition.y += npc->direction.y * npc->distanceLeft;
			npc->distanceLeft = 0;
		}
		//Move NPC
		else
		{
			npc->worldPosition.x += npc->direction.x * npc->baseSpeed * deltaTime;
			npc->worldPosition.y += npc->direction.y * npc->baseSpeed * deltaTime;
			npc->distanceLeft -= npc->baseSpeed * deltaTime;
		}

		//-------------------------------------------------------
		// Npc has reached its destination. 
		//-------------------------------------------------------
		if (npc->distanceLeft <= 0)
		{
			//-------------------------------------------------------
			// Npc has reached its final destination. 
			// First checkpoint is to search for the nearest 
			// checkpoint within the grid to prevent npc from leaving
			// the game at all times.
			//-------------------------------------------------------
			if (npc->checkPointsLeft == 0)
			{
				//new idle timer
				npc->nextMoveTimer = CP_Random_RangeFloat(1, 3);
				//add nearest checkpoint
				npc->checkPoints[1] = FindNearestCheckPoint(npc->worldPosition);

				//-------------------------------------------------------
				// Checkpoints are created by first randomizing a corner
				// in any of the tile in the game
				//-------------------------------------------------------
				int finalCheckpointx = CP_Random_RangeInt(1, WORLDGRIDX * 2);
				int finalCheckpointy = CP_Random_RangeInt(1, WORLDGRIDY * 2);

				int checkpointDiffX = finalCheckpointx - (int)npc->checkPoints[1].x;
				int checkpointDiffY = finalCheckpointy - (int)npc->checkPoints[1].y;

				int turnCount = 1;
				bool newX = Math_Abs_Int(checkpointDiffX) > Math_Abs_Int(checkpointDiffY);

				while (checkpointDiffX != 0 && checkpointDiffY != 0)
				{
					//-------------------------------------------------------
					// 2 random checkpoints are now added between the final
					// checkpoint and the current npc's position
					// 
					// Checkpoints are calculated in such a way where it 
					// will move horizontally then vertically.
					// Variable newX determines if last movement was a 
					// horizontal or vertical one.
					//-------------------------------------------------------
					if (turnCount < 2)
					{
						if (newX)
						{
							int newXpoint = GetRandomCheckPoint(checkpointDiffX);
							npc->checkPoints[turnCount + 1] = CP_Vector_Set(npc->checkPoints[turnCount].x + newXpoint, npc->checkPoints[turnCount].y);
							checkpointDiffX -= newXpoint;
							turnCount++;
							newX = !newX;
						}
						else
						{
							int newYpoint = GetRandomCheckPoint(checkpointDiffY);
							npc->checkPoints[turnCount + 1] = CP_Vector_Set(npc->checkPoints[turnCount].x, npc->checkPoints[turnCount].y + newYpoint);
							checkpointDiffY -= newYpoint;
							turnCount++;
							newX = !newX;
						}
					}
					//-------------------------------------------------------
					// After the first 2 random checkpoints are added, the
					// last 2 checkpoints will ensure it reaches the final
					// destination.
					//-------------------------------------------------------
					else if (newX)
					{
						//last 2 turns are maxed
						npc->checkPoints[turnCount + 1] = CP_Vector_Set(npc->checkPoints[turnCount].x + checkpointDiffX, npc->checkPoints[turnCount].y);
						turnCount++;
						checkpointDiffX = 0;
						npc->checkPoints[turnCount + 1] = CP_Vector_Set(npc->checkPoints[turnCount].x, npc->checkPoints[turnCount].y + checkpointDiffY);
						turnCount++;
						checkpointDiffY = 0;
					}
					else
					{
						//last 2 turns are maxed
						npc->checkPoints[turnCount + 1] = CP_Vector_Set(npc->checkPoints[turnCount].x, npc->checkPoints[turnCount].y + checkpointDiffY);
						turnCount++;
						checkpointDiffY = 0;
						npc->checkPoints[turnCount + 1] = CP_Vector_Set(npc->checkPoints[turnCount].x + checkpointDiffX, npc->checkPoints[turnCount].y);
						checkpointDiffX = 0;
					}
				}

				//----------------------------------------------------------
				// These 2 if statements check if the character has already
				// reached the horizontal or vertical axis position.
				// The next checkpoint will then move it to the final 
				// destination.
				//----------------------------------------------------------
				if (checkpointDiffX == 0 && checkpointDiffY != 0)
				{
					npc->checkPoints[turnCount + 1] = CP_Vector_Set(npc->checkPoints[turnCount].x, npc->checkPoints[turnCount].y + checkpointDiffY);
					turnCount++;
					checkpointDiffY = 0;
				}
				if (checkpointDiffX != 0 && checkpointDiffY == 0)
				{
					npc->checkPoints[turnCount + 1] = CP_Vector_Set(npc->checkPoints[turnCount].x + checkpointDiffX, npc->checkPoints[turnCount].y);
					turnCount++;
					checkpointDiffX = 0;
				}

				npc->checkPointsLeft += turnCount;
			}

			//-------------------------------------------------------
			// Npc has not reached its final destination. Move all
			// checkpoints forward and calculate its new direction
			// and distance left.
			//-------------------------------------------------------
			if (npc->checkPointsLeft > 0)
			{
				npc->checkPoints[0] = npc->checkPoints[1];
				npc->checkPoints[1] = npc->checkPoints[2];
				npc->checkPoints[2] = npc->checkPoints[3];
				npc->checkPoints[3] = npc->checkPoints[4];
				npc->checkPoints[4] = npc->checkPoints[5];
				npc->checkPointsLeft -= 1;
				CheckpointToWorldPosition(&npc->checkPoints[0]);
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
			}
		}
	}
	//idling
	else
	{
		npc->nextMoveTimer -= deltaTime;
	}
}


/*!_____________________________________________________________________________
@brief      This function moves all existing npcs in the game and draws
			the image with its respective positions.
*//*__________________________________________________________________________*/
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
}

/*!_____________________________________________________________________________
@brief      This function returns the position of an npc struct with
			given index parameter. If npc is not spawned in with the 
			given index, it will return a zero vector.
*//*__________________________________________________________________________*/
CP_Vector GetNpc(int index)
{
	if (npcList[index].spriteIndex == 0)
	{
		return CP_Vector_Set(0,0);
	}
	else
	{
		return npcList[index].worldPosition;
	}
}

