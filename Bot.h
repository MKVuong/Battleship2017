/****************************************************************
FILE:      Bot.h
AUTHOR:    Kristein Minh Vuong

PURPOSE:   Abstract Base/Super class that contains the basic
		   attributes and methods for its derived classes:
		   EasyBot and HardBot.
****************************************************************/
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
	Bot();
	~Bot();	
	
	vector<Point> options;	//100 potential places for bot to target
	int sx, sy;			//x & y belonging to strategy method
	int sx2, sy2;		//x & y that updates options vector - avoids functionality interruption
	int atkDirection;	//Either horizontal, vertical, or 2(neither)
	int displacement;	//Finishes off a ship in certain direction
	bool resetStrat;	//dictates whether to proceed with strategy or not
		
	//Accessor methods
	int getHealth();
	string getBotName();

	//Mutator methods
	void setHealth(int);
	void setBotName(string);
	
	void updateOptions();			//remove element from bot's options vector where attack was made
	bool findCpuHit(Ship* hS[]);	//remove element from corresponding ship vector where hit was made

	//Pure virtual method
	virtual void strategy(Ship* hS[]) = 0;	//Parameter is pointer to array of human Ships
};
#endif 