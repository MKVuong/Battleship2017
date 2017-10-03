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
