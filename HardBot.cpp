#include "stdafx.h"
#include "HardBot.h"



HardBot::HardBot()
{
	//setHealth(17);	//Not needed, health set in super class
	setBotName("Challenger Bot");
}


HardBot::~HardBot()
{
}

bool HardBot::getResetStrat()
{
	return resetStrat;
}

void HardBot::setResetStrat(bool newBool)
{
	resetStrat = newBool;
}

void HardBot::strategy()
{
	cout << "hello\n";
}
