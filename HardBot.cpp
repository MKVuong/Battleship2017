#include "stdafx.h"
#include "HardBot.h"

HardBot::HardBot()
{
	//setHealth(17);				//Not needed, health set in super class
	resetStrat = 1;					//Initially reset strategy
	atkDirection = nDirect;			//Initially no direction
	displacement = 0;				//Initial displacement of 0 used when change direction 
	setBotName("Challenger Bot");
}


HardBot::~HardBot()
{
}

void HardBot::strategy(Ship* hS[])
{
	/////////////////////////////////////////////////
	//
	// Bot searches for your ship - resetStrat = TRUE
	//
	/////////////////////////////////////////////////
	if (resetStrat)
	{
		//cout << options.size() << " is the size of options vector\n";
		//cout << o << " is the random chosen element\n";
		//cout << options[o].x << ", " << options[o].y << "\n";

		srand(time(0));	//seed random time
		int o = rand() % options.size();

		//Assign strategy()'s x & y
		sx = options[o].x;
		sy = options[o].y;

		//Remove point from vector
		options[o] = options.back();
		options.pop_back();

		switch (gridDefense[sx][sy])
		{
		case water: botMsg = getBotName() + " missed";
			gridDefense[sx][sy] = miss;
			break;
		case ship: botMsg = getBotName() + " hit you";
			gridDefense[sx][sy] = hit;
			resetStrat = false;
			sx2 = sx;
			sy2 = sy;
			findCpuHit(hS);	//pops those coordinates out of its ship vector & checks ship status
			break;
		}
	}
	else if (!resetStrat)
	{
		//cout << "inside !resetStrat\n";
		//If direction of targeted ship is unknown, proceed to find direction
		if (atkDirection != hDirect && atkDirection != vDirect)
		{
			cout << "inside first if section of !resetStrat\n";
			//test horizontal
			if (gridDefense[sx + 1][sy] == water && sx < 9)
			{
				gridDefense[sx + 1][sy] = miss;
				botMsg = getBotName() + " missed";
				sx2 = sx+1;		//update sx to be new coordinate to process
				sy2 = sy;		//update sy to be new coordinate to process
				updateOptions();//Update bot's vector of available points to attack

			}
			else if (gridDefense[sx + 1][sy] == ship && sx < 9)
			{
				gridDefense[sx + 1][sy] = hit;
				botMsg = getBotName() + " hit you";
				//CHECK FOR SUNKEN 2-LENGTH SHIP
				atkDirection = hDirect;
				sx++;			//update sx to be new hit coordinate
				sx2 = sx;
				sy2 = sy;		//update sy to be new coordinate to process
				displacement++;	//increment displacement
				findCpuHit(hS);	//pops those coordinates out of its ship vector & checks ship status
				updateOptions();//Update bot's vector of available points to attack
			}
			else if (gridDefense[sx - 1][sy] == water && sx > 0)
			{
				gridDefense[sx - 1][sy] = miss;
				botMsg = getBotName() + " missed";
				sx2 = sx - 1;		//update sx to be new hit coordinate
				sy2 = sy;
				updateOptions();	//Update bot's vector of available points to attack

			}
			else if (gridDefense[sx - 1][sy] == ship && sx > 0)
			{
				gridDefense[sx - 1][sy] = hit;
				botMsg = getBotName() + " hit you";
				//CHECK FOR SUNKEN 2-LENGTH SHIP
				atkDirection = hDirect;
				sx--;			//update sx to be new hit coordinate
				sx2 = sx;
				sy2 = sy;		//update sy to be new coordinate to process
				//Notice: Does not need to decrement displacement because it will only be going left
				findCpuHit(hS);	//pops those coordinates out of its ship vector & checks ship status
				updateOptions();//Update bot's vector of available points to attack
			}
			//test vertical
			else if (gridDefense[sx][sy + 1] == water && sy < 9)
			{
				gridDefense[sx][sy + 1] = miss;
				botMsg = getBotName() + " missed";
				sx2 = sx;
				sy2 = sy + 1;		//update sx to be new hit coordinate
				updateOptions();	//Update bot's vector of available points to attack
			}
			else if (gridDefense[sx][sy + 1] == ship && sy < 9)
			{
				gridDefense[sx][sy + 1] = hit;
				botMsg = getBotName() + " hit you";
				//CHECK FOR SUNKEN 2-LENGTH SHIP
				atkDirection = vDirect;
				sy++;			//update sx to be new hit coordinate
				sy2 = sy;
				sx2 = sx;
				displacement++;	//increment displacement
				findCpuHit(hS);	//pops those coordinates out of its ship vector & checks ship status
				updateOptions();//Update bot's vector of available points to attack
			}
			else if (gridDefense[sx][sy - 1] == water && sy > 0)
			{
				gridDefense[sx][sy - 1] = miss;
				botMsg = getBotName() + " missed";
				sy2 = sy - 1;		//update sx to be new hit coordinate
				sx2 = sx;
				updateOptions();	//Update bot's vector of available points to attack
			}
			else if (gridDefense[sx][sy - 1] == ship && sy > 0)
			{
				gridDefense[sx][sy - 1] = hit;
				botMsg = getBotName() + " hit you";
				//CHECK FOR SUNKEN 2-LENGTH SHIP
				atkDirection = vDirect;
				sy--;			//update sx to be new hit coordinate. Displacement update unnecessary
				sy2 = sy;
				sx2 = sx;
				findCpuHit(hS);	//pops those coordinates out of its ship vector & checks ship status
				updateOptions();//Update bot's vector of available points to attack

			}
			return;	//leave this method once atkDirection found
		}//end of whether atkDirection was found

		//TESTTTTTTTTTTTTT, UPDATE SX EVERY TIME A HIT IS FOUND

		//Continue firing at horizontal ship
		if (atkDirection == hDirect)
		{
			if (gridDefense[sx + 1][sy] == water && sx + 1 < rows)
			{
				gridDefense[sx + 1][sy] = miss;
				botMsg = getBotName() + " missed";
				sx2 = sx+1;
				sy2 = sy;
				updateOptions();//Update bot's vector of available points to attack
			}
			else if (gridDefense[sx + 1][sy] == ship && sx + 1 < rows)
			{
				gridDefense[sx + 1][sy] = hit;
				botMsg = getBotName() + " hit you";
				sx++;			//update sx to be the newly hit sx coordinate
				sx2 = sx;
				sy2 = sy;
				displacement++;	//update displacement accordingly
				findCpuHit(hS);	//pops those coordinates out of its ship vector & checks ship status
				updateOptions();//Update bot's vector of available points to attack
			}
			//CHANGE DIRECTION
			else if (gridDefense[sx + 1][sy] == miss || sx == 9 ||
				gridDefense[sx + 1][sy] == hit)
			{
				sx = sx - displacement;	//set sx to its original position
				displacement = 0;		//reset displacement
				if (gridDefense[sx - 1][sy] == ship)
				{
					gridDefense[sx - 1][sy] = hit;
					botMsg = getBotName() + " hit you";
					sx--;			//update sx to be newly hit sx coordinate
					sx2 = sx;
					sy2 = sy;
					findCpuHit(hS);	//pops those coordinates out of its ship vector & checks ship status
					updateOptions();//Update bot's vector of available points to attack
				}
				//SHOULDN'T REACH THIS, SHIP SHOULD HAVE SUNKEN
				else if (gridDefense[sx - 1][sy] == water || sx == 0)
				{
					botMsg = "WTF, SHIP IS UNKILLABLE???\n";
				}
			}
		}
		if (atkDirection == vDirect)
		{
			if (gridDefense[sx][sy + 1] == water && sy + 1 < cols)
			{
				gridDefense[sx][sy + 1] = miss;
				botMsg = getBotName() + " missed";
				sx2 = sx;
				sy2 = sy + 1;
				updateOptions();//Update bot's vector of available points to attack
			}
			else if (gridDefense[sx][sy + 1] == ship && sy + 1 < cols)
			{
				gridDefense[sx][sy + 1] = hit;
				botMsg = getBotName() + " hit you";
				sy++;			//update sx to be the newly hit sx coordinate
				sx2 = sx;
				sy2 = sy;
				displacement++;	//update displacement accordingly
				findCpuHit(hS);	//pops those coordinates out of its ship vector & checks ship status
				updateOptions();//Update bot's vector of available points to attack
			}
			//CHANGE DIRECTION
			else if (gridDefense[sx][sy + 1] == miss || sy == 9 ||
				gridDefense[sx][sy + 1] == hit)
			{
				sy = sy - displacement;	//set sx to its original position
				displacement = 0;		//reset displacement
				if (gridDefense[sx][sy - 1] == ship)
				{
					gridDefense[sx][sy - 1] = hit;
					botMsg = getBotName() + " hit you";
					sy--;			//update sx to be newly hit sx coordinate
					sx2 = sx;
					sy2 = sy;
					findCpuHit(hS);	//pops those coordinates out of its ship vector & checks ship status
					updateOptions();//Update bot's vector of available points to attack
				}
				//SHOULDN'T REACH THIS, SHIP SHOULD HAVE SUNKEN
				else if (gridDefense[sx][sy - 1] == water || sy == 0)
				{
					botMsg = "WTF, SHIP IS UNKILLABLE???\n";
				}
			}
		}
	}
}

bool HardBot::findCpuHit(Ship* hS[])
{
	//Cycle through all ships for to find the hit
	for (int i = 0; i < 5; i++)
	{
		/*cout << "is ship empty? " << hS[i]->coords.empty() << endl;
		if (hS[i]->coords.empty())
			continue;
			*/
		for (vector<Point>::iterator it = hS[i]->coords.begin();
			it != hS[i]->coords.end(); it++)
		{
			//Compare vector element thats being pointed at, to the hit's x & y
			if (it->x == sx2 && it->y == sy2)
			{
				//botMsg = passed coordinates match found
				*it = hS[i]->coords.back();	//assign to last element
				hS[i]->coords.pop_back();
				//cout << "Ship found: " << shipArPtrs[i]->getName() << ". Current vctr elements remain:\n";

				//Check if that entire ship is destroyed
				if (hS[i]->coords.empty())
				{
					//Ship is destroyed
					resetStrat = true;		//reactivate resetStrat
					atkDirection = nDirect;	//reset to atk direction to neutral
					displacement = 0;		//reset displacement to 0
					botMsg = "Your " + hS[i]->getName() + " has been destroyed!";
				}
				hpHuman--;
				//Display remaining vector elements of that ship
				/*for (vector<Point>::iterator it = hS[i]->coords.begin();
				it != hS[i]->coords.end(); it++)
				{
				cout << it->x << ", ";
				cout << (*it).y << endl;
				}*/
				return true;	//hit found
			}
		}
	}
	return false;
}
