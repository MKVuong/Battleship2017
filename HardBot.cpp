#include "stdafx.h"
#include "HardBot.h"

HardBot::HardBot()
{
	//setHealth(17);				//Not needed, health set in super class
	resetStrat = true;					//Initially reset strategy
	atkDirection = nDirect;			//Initially no direction
	displacement = 0;				//Initial displacement of 0, used when change direction 
	setBotName("Challenger Bot");
}


HardBot::~HardBot()
{
}

void HardBot::strategy(Ship* hS[])
{
	///Update direction conditions
	current = gridDefense[sx][sy];
	right = gridDefense[sx + 1][sy];
	left = gridDefense[sx - 1][sy];
	down = gridDefense[sx][sy + 1];
	up = gridDefense[sx][sy - 1];

	/////////////////////////////////////////////////
	//
	// Bot searches for your ship - resetStrat = initially TRUE
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

		//Check if shot hit or missed
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
			//test horizontal
			if (right == water && sx + 1 < rows)
			{
				gridDefense[sx + 1][sy] = miss;
				botMsg = getBotName() + " missed";
				sx2 = sx+1;		//update sx to be new coordinate to process
				sy2 = sy;		//update sy to be new coordinate to process
				updateOptions();//Update bot's vector of available points to attack

			}
			else if (right == ship && sx + 1 < rows)
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
			else if (left == water && sx > 0)
			{
				gridDefense[sx - 1][sy] = miss;
				botMsg = getBotName() + " missed";
				sx2 = sx - 1;		//update sx to be new hit coordinate
				sy2 = sy;
				updateOptions();	//Update bot's vector of available points to attack

			}
			else if (left == ship && sx > 0)
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
			else if (down == water && sy + 1 < cols)
			{
				gridDefense[sx][sy + 1] = miss;
				botMsg = getBotName() + " missed";
				sx2 = sx;
				sy2 = sy + 1;		//update sx to be new hit coordinate
				updateOptions();	//Update bot's vector of available points to attack
			}
			else if (down == ship && sy + 1 < cols)
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
			else if (up == water && sy > 0)
			{
				gridDefense[sx][sy - 1] = miss;
				botMsg = getBotName() + " missed";
				sy2 = sy - 1;		//update sx to be new hit coordinate
				sx2 = sx;
				updateOptions();	//Update bot's vector of available points to attack
			}
			else if (up == ship && sy > 0)
			{
				gridDefense[sx][sy - 1] = hit;
				botMsg = getBotName() + " hit you";
				//CHECK FOR SUNKEN 2-LENGTH SHIP
				atkDirection = vDirect;
				sy--;			//update sx to be new hit coordinate. Displacement update	 unnecessary
				sy2 = sy;
				sx2 = sx;
				findCpuHit(hS);	//pops those coordinates out of its ship vector & checks ship status
				updateOptions();//Update bot's vector of available points to attack

			}
			return;  //leave method after hit
			//leave strategy() once atkDirection found
			//if (atkDirection != nDirect)
				//return;	

			//NOTICE
			//if atkDirection NOT found, must check for 2 spaces away, if that fails, then resetStrat=1
			//

		}//end of whether atkDirection was found

		//Continue firing at horizontal ship
		if (atkDirection == hDirect)
		{
			if (right == water && sx + 1 < rows)
			{
				gridDefense[sx + 1][sy] = miss;
				botMsg = getBotName() + " missed";
				sx2 = sx+1;
				sy2 = sy;
				updateOptions();//Update bot's vector of available points to attack
			}
			else if (right == ship && sx + 1 < rows)
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
			else if (right == miss || sx + 1 == rows || right == hit)
			{
				sx = sx - displacement;	//set sx to its original position
				left = gridDefense[sx - 1][sy];
				displacement = 0;		//reset displacement
				if (left == ship)
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
				else if (left == water || sx == 0)
				{
					displacement = 0;
					//fired at multiple ships in different alignment, change alignment after this miss
					if (sx > 0)
					{
						gridDefense[sx - 1][sy] = miss;
						botMsg = getBotName() + " missed";
						sx2 = sx - 1;
						sy2 = sy;
						updateOptions();		//Update bot's vector of available points to attack
						changeDirection = 1;	//change the direction at the end of strategy()
					}
					else
						atkDirection = vDirect;	//change direction immediately to do vertical same run
				}
				else  //if left has been shot at before, reset entirely
				{
					resetStrat = true;
					displacement = 0;
					atkDirection = nDirect;
					changeDirection = nDirect;
				}
			}
		}
		if (atkDirection == vDirect)
		{
			if (down == water && sy + 1 < cols)
			{
				gridDefense[sx][sy + 1] = miss;
				botMsg = getBotName() + " missed";
				sx2 = sx;
				sy2 = sy + 1;
				updateOptions();//Update bot's vector of available points to attack
			}
			else if (down == ship && sy + 1 < cols)
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
			else if (down == miss || sy + 1 == cols || down == hit)
			{
				sy = sy - displacement;			//set sx to its original position
				up = gridDefense[sx][sy - 1];	//update direction value
				displacement = 0;				//reset displacement
				if (up == ship)
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
				else if (up == water || sy == 0)
				{
					displacement = 0;
					//fired at multiple ships in different alignment, change alignment after this miss
					if (sy > 0)
					{
						gridDefense[sx][sy - 1] = miss;
						botMsg = getBotName() + " missed";
						sx2 = sx;
						sy2 = sy - 1;
						updateOptions();			//Update bot's vector of available points to attack
						changeDirection = hDirect;	//change the direction at the end of strategy()
					}
					else
					{
						atkDirection = hDirect;
						strategy(hS);		//change direction and run again immediately
					}
				}
				else  //if up has been shot at before, reset entirely
				{
					resetStrat = true;
					displacement = 0;
					atkDirection = nDirect;
					changeDirection = nDirect;
				}
			}
		}
	}
	//if theres a changeDirection requested
	if (changeDirection != nDirect)
	{
		atkDirection = changeDirection;
		changeDirection = nDirect;		//reset changeDirection
	}
}
