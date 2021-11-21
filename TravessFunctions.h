#include <stdio.h>
#include "cprocessing.h"
#include <stdbool.h>

#define DEBUG 0
#if DEBUG
#define LOG(...) printf(__VA_ARGS__)
#else
#define LOG(...)
#endif

float Math_Abs(float x)
{
	if (x < 0)
	{
		return -x;
	}
	return x;
}

int Math_Abs_Int(int x)
{
	if (x < 0)
	{
		return -x;
	}
	return x;
}

void LerpTowards()
{

}

bool CheckWithinBounds(CP_Vector position, float width, float height)
{
	float mouseX = CP_Input_GetMouseX();
	float mouseY = CP_Input_GetMouseY();

	if (mouseX >= position.x - (width / 2) && mouseX <= position.x + (width / 2) && mouseY >= position.y - (height / 2) && mouseY <= position.y + (height / 2))
	{
		return true;
	}
	return false;
}
















