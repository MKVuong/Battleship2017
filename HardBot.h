/****************************************************************
FILE:		HardBot.h
AUTHOR:		Kristein Minh Vuong

PURPOSE:	A derived class of Bot.h, containing extra
			methods and attributes for deeper strategy() 
			implementation.
****************************************************************/
#ifndef HARDBOT_H
#define HARDBOT_H

#include "Bot.h"

class HardBot : public Bot
{
public:
	HardBot();
	~HardBot();

	//variable used to change firing direction within HardBot's strategy()
	int changeDirection = nDirect;
	
	//Clarified direction conditions
	int current;
	int right;
	int left;
	int down;
	int up;
	
	// HardBot Implementation of strategy()
	void strategy(Ship* hS[]);

	void verticalProcess(Ship* hS[]);
	void horizontalProcess(Ship* hs[]);
};
#endif