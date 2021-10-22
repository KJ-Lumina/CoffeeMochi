#include <stdio.h>
#include "cprocessing.h"
#include "game.h"



CARDEVENTS b_BuildAHouse = { 1, 1, "Our citizens needs a place to sleep. Click on the house below and build it on the grid.", 2, 0, "", "" };
CARDEVENTS b_BuildAFarm = { 2, 1, "Our city needs to eat. Click on the farm below and build it on the grid.", 3, 0, "", "" };


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