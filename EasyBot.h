#pragma once
#ifndef EASYBOT_H
#define EASYBOT_H

#include "Bot.h"


class EasyBot : public Bot
{
public:
	EasyBot();
	~EasyBot();

	//EasyBot Implementation of strategy()
	void strategy();
};

#endif EASYBOT_H