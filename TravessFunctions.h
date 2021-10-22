#include <stdio.h>
#include "cprocessing.h"
#include <stdbool.h>

#define DEBUG 0
#if DEBUG
#define LOG(...) printf(__VA_ARGS__)
#else
#define LOG(...)
#endif

bool ScreenToGridPosition(int* selCol, int* selRow, int cellSize, int screenSize)
{
	float CursorX = CP_Input_GetMouseX();
	float CursorY = CP_Input_GetMouseY();
    if (CursorX >= screenSize || CursorY >= screenSize)
    {
        printf("%f %f\n", CursorX, CursorY);
        return false;
    }

	*selCol = (int)(CursorX / cellSize);
	*selRow = (int)(CursorY / cellSize);

	//CP_Settings_Fill(CP_Color_Create(0, 255, 0, 255));
	//CP_Graphics_DrawCircle((float)(xGrid * cellSize + cellSize/2), (float)(yGrid * cellSize + cellSize / 2), 10);
	printf("true");
	return true;
}

float Math_Abs(float x)
{
	if (x < 0)
	{
		return -x;
	}
	return x;
}















