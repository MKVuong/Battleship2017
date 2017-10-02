#pragma once
#ifndef HARDBOT_H
#define HARDBOT_H

#include "Bot.h"

class HardBot : public Bot
{
private:
	bool resetStrat;

public:
	HardBot();
	~HardBot();

	//Accessor Methods
	bool getResetStrat();

	//Mutator Methods
	void setResetStrat(bool);
	
	// HardBot Implementation of strategy()
	void strategy();
};

#endif HARDBOT_H