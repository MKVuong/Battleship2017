#pragma once
#ifndef BOT_H
#define BOT_H

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include "Point.h"
#include "Constants.h"
#include "Ship.h"

using namespace std;

class Bot
{
private:
	int health;
	string botName;

public:
	//100 potential places for bot to target
	vector<Point> options;

	int sx, sy;				//x & y belonging to strategy method
	int sx2, sy2;			//x & y used to update options vector - used to avoid messing w/ functionality
	int atkDirection;		//Used for HardBot - Either horizontal, vertical, or 2(neither)
	int displacement;		//Used for HardBot - Finishes off a ship in certain direction
	bool resetStrat;		//Used for HardBot

	Bot();
	~Bot();
	
	//Accessor methods
	int getHealth();
	string getBotName();

	//Mutator methods
	void setHealth(int);
	void setBotName(string);
	
	void updateOptions();	//remove element where attack was made

	//Pure virtual method
	virtual void strategy(Ship* hS[]) = 0;	//Paramter is pointer to array of human Ships
};

#endif 