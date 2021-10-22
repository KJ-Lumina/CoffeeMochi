#include <stdio.h>
#include "cprocessing.h"
#include "game.h"



CARDEVENTS b_BuildAHouse = { 1, 1, "Hey governor, our citizens needs a place to sleep. Could you build a house?", 3, 0, "", "" };
CARDEVENTS b_BuildAFarm = { 2, 1, "Hey governor, our city needs to eat. Could you build a farm for food production?", 3, 0, "", "" };


CARDEVENTS basicDeck[20];


void InitDeck()
{
	basicDeck[0] = b_BuildAHouse;
	basicDeck[1] = b_BuildAFarm;

}

CARDEVENTS GetBasicEvent()
{
	return basicDeck[0];
}


void GetAdvancedEvent()
{

}