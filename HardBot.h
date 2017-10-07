#pragma once
#ifndef HARDBOT_H
#define HARDBOT_H

#include "Bot.h"

class HardBot : public Bot
{
public:
	HardBot();
	~HardBot();
	
	// HardBot Implementation of strategy()
	void strategy(Ship* hS[]);
};

#endif