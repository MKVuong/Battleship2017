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
