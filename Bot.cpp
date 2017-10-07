#include "stdafx.h"
#include "Bot.h"


Bot::Bot()
{
	health = 17;
	botName = "Nameless Bot";
}

Bot::~Bot()
{
}

int Bot::getHealth()
{
	return health;
}

string Bot::getBotName()
{
	return botName;
}

void Bot::setHealth(int newHealth)
{
	health = newHealth;
}

void Bot::setBotName(string newName)
{
	botName = newName;
}

void Bot::updateOptions()
{
	for (vector<Point>::iterator it = options.begin();
		it != options.end(); it++)
	{
		//Compare vector element thats being pointed at, to the hit's x & y
		if (it->x == sx2 && it->y == sy2)
		{
			//botMsg = "PASSED OPTIONS FOUND";
			*it = options.back();	//assign to last element
			options.pop_back();		//remove element
		}
	}
}

//Effectively halts aiming for specific ship once destroyed, resets direction, strat, & displacement
bool Bot::findCpuHit(Ship* hS[])
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
				hpHuman--;	//decrement human health points
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
