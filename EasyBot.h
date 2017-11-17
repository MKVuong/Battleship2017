/****************************************************************
FILE:		EasyBot.h
AUTHOR:		Kristein Minh Vuong

PURPOSE:	A derived class of Bot.h
****************************************************************/
#ifndef EASYBOT_H
#define EASYBOT_H

#include "Bot.h"

class EasyBot : public Bot
{
public:
	EasyBot();
	~EasyBot();

	//EasyBot Implementation of strategy()
	void strategy(Ship* hS[]);
};
#endif 