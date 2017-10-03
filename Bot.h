#pragma once
#ifndef BOT_H
#define BOT_H

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include "Point.h"

using namespace std;

class Bot
{
private:
	int health;
	string botName;

public:
	//100 potential places for bot to target
	vector<Point> options;

	Bot();
	~Bot();
	
	//Accessor methods
	int getHealth();
	string getBotName();

	//Mutator methods
	void setHealth(int);
	void setBotName(string);
	
	//Purely virtual method
	virtual void strategy() = 0;
};

#endif BOT_H