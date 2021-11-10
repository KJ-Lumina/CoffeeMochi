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
















