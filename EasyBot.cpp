#include "EasyBot.h"

EasyBot::EasyBot()
{
	setBotName("Bronze Bot");
}

EasyBot::~EasyBot()
{
}

/****************************************************************
FUNCTION:   strategy()

ARGUMENTS:  Ship * hS[] - array of pointers to human ships

RETURNS:    None

NOTES:		EasyBot's implementation of strategy() which simply
			fires at random locations that are available from
			options vector.
****************************************************************/
void EasyBot::strategy(Ship * hS[])
{
	//Similar to HardBot's beginning of strategy()

	srand(time(0));	//seed random time
	int o = rand() % options.size();

	//Assign strategy()'s x & y
	sx = options[o].x;
	sy = options[o].y;

	//Remove point from bot's options vector
	options[o] = options.back();
	options.pop_back();

	switch (gridDefense[sx][sy])
	{
	case water: botMsg = getBotName() + " missed";
		gridDefense[sx][sy] = miss;
		break;
	case ship: botMsg = getBotName() + " hit you";
		gridDefense[sx][sy] = hit;
		sx2 = sx;
		sy2 = sy;
		findCpuHit(hS);	//pops those coordinates out of its ship vector & checks ship status
		break;
	}
}