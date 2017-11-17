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

/*********************************************************************
FUNCTION:   strategy()

ARGUMENTS:  Ship* hS[] - array of pointers to human ships

RETURNS:    None

NOTES:		HardBot's implementation of strategy(). Fires at random
			locations from options vector which contain available 
			points to attack. Once a human ship has been hit, bot will
			fire at adjacent points to that hit location. When human
			ship alignment has been established, will proceed to fire
			in that direction until ship is destroyed. If ship wasn't
			finished off and ends up firing at water or out of bounds,
			it will	redirect firing direction.
**********************************************************************/
void HardBot::strategy(Ship* hS[])
{
	///Update direction conditions
	current = gridDefense[sx][sy];
	right = gridDefense[sx + 1][sy];
	left = gridDefense[sx - 1][sy];
	down = gridDefense[sx][sy + 1];
	up = gridDefense[sx][sy - 1];

	debugMsg = "no bug";	//Only displayed when applied to update()

	//////////////////////////////////////////////////////////////
	//															//
	// Bot searches for your ship - resetStrat = initially TRUE	//
	//															//
	//////////////////////////////////////////////////////////////
	if (resetStrat)
	{
		//Displays testing of random element selecting
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
				sx2 = sx + 1;		//update sx to be new coordinate to process
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
			//reset shot entirely
			else if (atkDirection == nDirect)
			{
				resetStrat = true;
				displacement = 0;
				atkDirection = nDirect;
				return strategy(hS);
			}
			return;//leave strategy() once atkDirection found
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
					debugMsg = "left == ship";
					gridDefense[sx - 1][sy] = hit;
					botMsg = getBotName() + " hit you";
					sx--;			//update sx to be newly hit sx coordinate
					sx2 = sx;
					sy2 = sy;
					findCpuHit(hS);	//pops those coordinates out of its ship vector & checks ship status
					updateOptions();//Update bot's vector of available points to attack
				}
				//If reached the edge of the grid, change direction
				else if (sx == 0)
				{
					debugMsg = "sx == 0";
					displacement = 0;
					atkDirection = vDirect;	//change direction
					return verticalProcess(hS);	//run immediately
				}
				//SHOULDN'T REACH THIS, SHIP SHOULD HAVE SUNKEN
				else if (left == water)
				{
					displacement = 0;
					//fired at multiple ships in different alignment, change alignment after this miss
					gridDefense[sx - 1][sy] = miss;
					botMsg = getBotName() + " missed";
					sx2 = sx - 1;
					sy2 = sy;
					updateOptions();		//Update bot's vector of available points to attack
					atkDirection = vDirect;	//change the direction at the end of strategy()
				}
				else  //if left has been shot at before, reset entirely
				{
					resetStrat = true;
					displacement = 0;
					atkDirection = nDirect;
					return strategy(hS);
				}
			}
		}
		else if (atkDirection == vDirect)
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
				else if (up == water)
				{
					displacement = 0;
					//fired at multiple ships in different alignment, change alignment after this miss
					gridDefense[sx][sy - 1] = miss;
					botMsg = getBotName() + " missed";
					sx2 = sx;
					sy2 = sy - 1;
					updateOptions();		//Update bot's vector of available points to attack
					atkDirection = hDirect;	//change the direction at the end of strategy()
				}
				else if (sy == 0)
				{
					atkDirection = hDirect;
					return horizontalProcess(hS);		//change direction and run again immediately
				}
				//if up has been shot at before, reset entirely and shoot random location
				else if (up == hit || up == miss)
				{
					resetStrat = true;
					displacement = 0;
					atkDirection = nDirect;
					return strategy(hS);
				}
			}
		}
	}
}

/****************************************************************
FUNCTION:   verticalProcess()

ARGUMENTS:  Ship* hS[] - array of pointers to human ships

RETURNS:    None

NOTES:		Accessed when a change in firing direction occurs,
			fires vertically.
****************************************************************/
void HardBot::verticalProcess(Ship* hS[])
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
		//If up ends up being water
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
				updateOptions();		//Update bot's vector of available points to attack
				atkDirection = hDirect;	//change the direction at the end of strategy()
			}
			else
			{
				atkDirection = hDirect;
				horizontalProcess(hS);		//change direction and run again immediately
			}
		}
		else  //if up has been shot at before, reset entirely
		{
			resetStrat = true;
			displacement = 0;
			atkDirection = nDirect;
			return strategy(hS);
		}
	}
}

/****************************************************************
FUNCTION:   horizontalProcess()

ARGUMENTS:  Ship* hS[] - array of pointers to human ships

RETURNS:    None

NOTES:		Accessed when a change in firing direction occurs,
			fires horizontally.
****************************************************************/
void HardBot::horizontalProcess(Ship* hS[])
{
	if (right == water && sx + 1 < rows)
	{
		gridDefense[sx + 1][sy] = miss;
		botMsg = getBotName() + " missed";
		sx2 = sx + 1;
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
			debugMsg = "left == ship";
			gridDefense[sx - 1][sy] = hit;
			botMsg = getBotName() + " hit you";
			sx--;			//update sx to be newly hit sx coordinate
			sx2 = sx;
			sy2 = sy;
			findCpuHit(hS);	//pops those coordinates out of its ship vector & checks ship status
			updateOptions();//Update bot's vector of available points to attack
		}
		//If reached the edge of the grid, change direction
		else if (sx == 0)
		{
			debugMsg = "sx == 0";
			displacement = 0;
			atkDirection = vDirect;	//change direction
			verticalProcess(hS);	//run immediately
		}
		//If left ends up being water
		else if (left == water)
		{
			displacement = 0;
			//fired at multiple ships in different alignment, change alignment after this miss
			gridDefense[sx - 1][sy] = miss;
			botMsg = getBotName() + " missed";
			sx2 = sx - 1;
			sy2 = sy;
			updateOptions();		//Update bot's vector of available points to attack
			atkDirection = vDirect;	//change the direction at the end of strategy()
		}
		else  //if left has already been shot at before, reset entirely
		{
			resetStrat = true;
			displacement = 0;
			atkDirection = nDirect;
			return strategy(hS);
		}
	}
}