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

float Math_Clamp_Float(float target, float min, float max)
{
	if (target <= min)
	{
		return min;
	}
	else if (target >= max)
	{
		return max;
	}
	else
	{
		return target;
	}
}

int Math_Clamp_Int(int target, int min, int max)
{
	if (target <= min)
	{
		return min;
	}
	else if (target >= max)
	{
		return max;
	}
	else
	{
		return target;
	}
}















