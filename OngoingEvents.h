/*!_____________________________________________________________________________
@file       OngoingEvents.h
@author     Kelly Liaw (liaw.k@digipen.edu)
@co-authors Travess Tan (travesscheekhang.t@digipen.edu)
@course     CSD1120
@section    B
@team       CoffeeMochi
@brief      This file contains the prototype function definitions that are used in
			the ongoing events in Impero.

			Copyright ©2021 DigiPen, All rights reserved.
*//*__________________________________________________________________________*/

#include <stdio.h>
#include "cprocessing.h"

typedef struct {
	int eventIndex;
	CP_Image sprite;
	int positionx;
	int positiony;
	int typeOfEffect;//index of which building it will affect
	int turn;//number of turn it will stay
	int alive;
	char description[100];
}ONGOING;


void RemoveEvent(int index);//check if  turn left is 0, if 0, remove event from array 