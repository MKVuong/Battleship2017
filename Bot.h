#pragma once
#ifndef BOT_H
#define BOT_H

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

using namespace std;

class Bot
{
private:
	int health;
	string botName;

public:
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