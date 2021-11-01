#include <stdio.h>
#include "cprocessing.h"


/*
void WrapText(char *text, int maxCharPerLine)
{
	int charCountInLine = 0;
	int prevSpace = 0;
	for (int i = 0; i < 100; ++i)
	{
		switch (text[i])
		{
		case ' ':
			if (charCountInLine > maxCharPerLine)
			{
				if (i - prevSpace > maxCharPerLine)
				{
					text[i] = '\n';
					charCountInLine = 0;
					prevSpace = i;
				}
				else
				{
					text[prevSpace] = '\n';
					charCountInLine = 0;
					i = prevSpace;
				}
			}
			else
			{
				prevSpace = i;
			}
			break;
		case '\n':
			charCountInLine = 0;
			prevSpace = i;
			break;
		default:
			charCountInLine++;
			break;
		}
	}
}

void DrawWrapText(char* text, int maxCharPerLine, float screenX, float screenY)
{
	WrapText(text, maxCharPerLine);
	char tempChar[100];
	int lineCount = 1;
	int prevLineIndex = 0;

	for (int i = 0; i < 100; ++i)
	{
		switch (text[i])
		{
		case '\n':

			CP_Font_DrawText(tempChar, screenX, screenY + (lineCount * 20));
			prevLineIndex = i;
			memset(tempChar, 0, 100);
			lineCount++;
			break;
		case '\0':
			CP_Font_DrawText(tempChar, screenX, screenY + (lineCount * 5));
			return;
			break;
		default:
			tempChar[i - prevLineIndex] = text[i];
			break;
		}
	}

	
}*/