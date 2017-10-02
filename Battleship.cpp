// Battleship.cpp : Defines the entry point for the console application.

/********************************************************************

Program:		Battleship
Author:			Minh Kristein Vuong
Date started:	9/20/2017

Purpose:		Battleship console application that features AI with
				two difficulty settings.

*********************************************************************/

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <iomanip>		//setw
#include <time.h>
#include <map>
#include <algorithm>	//find_if
#include "Ship.h"
#include "Point.h"
#include "Bot.h"
#include "EasyBot.h"
#include "HardBot.h"
#include <stdlib.h>		//srand, rand
#include <stdio.h>		//NULL
//#include <cstdlib>

using namespace std;

//Legend                                                                                |
const int water	= 0;
const int ship	= 1;
const int miss	= 2;
const int hit	= 3;

//Players 1 & 2
const int human = 1;
const int computer = 2;

//grids declared & initialized
int gridOffense[rows][cols] = { water };
int gridDefense[rows][cols] = { water };

//Declarations 
string tokens[2];	//pretested coordinates
int x, x2, y, y2;	//approved coordinates to place ships
int xran, yran;		//random coordinates for generating enemy ship
int pointCounter;	//sum of points per ship
int sizeLimit;		//used in checkAnswer()
int dif;			//distance between 2 coordinates
int compHP = 17;	//computer's healthpoints
int humanHP = 17;	//human's healthpoints
Ship *shipPtr;		//Ship pointer for user's ships
Bot *botPtr;
bool findHit(int);
bool horizontal();
bool vertical();
bool checkAtk(string);
bool attack();
void placeShip();
void convertTokens();
void update();
void enemyAttack();

//Create your ships
Ship d(2, "Destroyer");
Ship s(3, "Submarine");
Ship c(3, "Cruiser");
Ship b(4, "Battleship");
Ship ca(5, "Carrier");

//Create enemy ships
Ship d2(2, "Destroyer");
Ship s2(3, "Submarine");
Ship c2(3, "Cruiser");
Ship b2(4, "Battleship");
Ship ca2(5, "Carrier");

//Check input of attack
bool checkAtk(string ans)
{
	//Checks if answer is correct size
	if (ans.length() != 2)
	{
		cout << "Please input only 2 characters (Ex. 5E):\n";
		return false;
	}

	//Checks for 0-9
	else if (ans.at(0) < '0' || ans.at(0) > '9')
	{
		cout << "'" << ans.at(0) << "' out of 0-9 range. Try again\n";
		return false;
	}

	//Checks for A-J (not caps-sensitive)
	else if (!((ans.at(1) >= 'A' && ans.at(1) <= 'J') ||
		(ans.at(1) >= 'a' && ans.at(1) <= 'j')))
	{
		cout << "'" << ans.at(1) << "' out of A-J range. Try again\n";
		return false;
	}
	return true;
}

//Attack - returns true if game over
bool attack()
{
	//Ask user where to attack
	cout << "\nWhere would you like to bomb?\n";

	//String to hold user's input
	string ans;

	//Retrieve initial user input
	getline(cin, ans);

	//Check atk attempt, retrieve user input again if fail
	while (!checkAtk(ans))
		getline(cin, ans);

	//Convert y portion to int
	y = ans.at(0) - 48;	//-48 because going by ASCII table

	//Convert x portion to int
	if (ans.at(1) >= 'A' && ans.at(1) <= 'J')
		x = ans.at(1) - 65;
	else
		x = ans.at(1) - 97;

	//Declare atk coordinates
	//Point atk(x, y);

	//
	// Check the target area
	//

	//MISS
	if (gridOffense[x][y] == water)
	{
		gridOffense[x][y] = miss;
		update();
		cout << "         MISS\n";
	}

	//HIT
	else if (gridOffense[x][y] == ship)
	{
		gridOffense[x][y] = hit;
		update();
        cout << "         HIT\n";
		//Create function to cycle through all ships' vectors of points to find what ship was hit, 
		//	remove that point and check if vector.empty(), announce sunken ship if so
		findHit(human);
		compHP--;	//subtract 1 health point from computer

		//Checks for Game Over
		if (compHP == 0)
			return true;
	}

	//SHOT ALREADY TAKEN AT THIS LOCATION
	else if (gridOffense[x][y] == miss || gridOffense[x][y] == hit)
	{
		cout << "You just bombed the target area again Captain, what a waste.\n";
	}

	//Point()
	enemyAttack();

	return false;
}

//Parameter is either human or computer
bool findHit(int attacker)
{
	//Initalize array of pointers to computer's ships
	Ship *shipArPtrs[5] = { &d2,&s2,&c2,&b2,&ca2 };

	//Change to human's ships if computer is attacking
	if (attacker == computer)
	{
		shipArPtrs[0] = &d;
		shipArPtrs[1] = &s;
		shipArPtrs[2] = &c;
		shipArPtrs[3] = &b;
		shipArPtrs[4] = &ca;
	}

	//Create 2nd array of pointers for when your ship is hit instead, have function take a int

	//Cycle through all ships for to find the hit
	for (int i = 0; i < 5; i++)
	{
		for (vector<Point>::iterator it = shipArPtrs[i]->coords.begin(); 
			it != shipArPtrs[i]->coords.end(); it++)
		{
			//Compare vector element thats being pointed at, to the hit's x & y
			if (it->x == x && it->y == y)
			{
				*it = shipArPtrs[i]->coords.back();
				shipArPtrs[i]->coords.pop_back();
				//cout << "Ship found: " << shipArPtrs[i]->getName() << ". Current vctr elements remain:\n";

				//Check if that entire ship is destroyed
				if (shipArPtrs[i]->coords.empty())
				{
					cout << "Bot: You sank my " << shipArPtrs[i]->getName() << "!\n";
				}
				//resetStrat = 1;	//only apply this when computer destroys human's ship
				
				//Display remaining vector elements of that ship
				/*for (vector<Point>::iterator it = shipArPtrs[i]->coords.begin(); 
					it != shipArPtrs[i]->coords.end(); it++)
				{
					cout << it->x << ", ";
					cout << (*it).y << endl;
				}*/
				
				return true;
			}
		}
	}
	return false;
}

void enemyAttack()
{
	//EMPTY FOR NOW
}

void populateEnemyGrid()
{
	//Select random coordinate
	srand(time(NULL));	//seeds random time
	xran = rand() % rows;
	yran = rand() % cols;

	//If selected random coordinate is occupied, find another
	while (gridOffense[xran][yran] == ship)
	{
		//Select random coordinate
		srand(time(NULL));	//seeds random time
		xran = rand() % rows;
		yran = rand() % cols;
	}

	//Randomize direction
	srand(time(0));	//seeds random time
	int temp = rand() % 2;
	if (temp == 0)	//if temp is even
	{
		//Cycle through again if placement fails
		while (!horizontal())
		{
			//choose a different coordinate if outer while fails
			while (gridOffense[xran][yran] == ship)
			{
				//Select random coordinate
				srand(time(NULL));	//seeds random time
				xran = rand() % rows;
				yran = rand() % cols;
			}
		}
	}
	else
	{
		//Cycle through again if placement fails
		while (!vertical())
		{
			//choose a different coordinate if outer while fails
			while (gridOffense[xran][yran] == ship)
			{
				//Select random coordinate
				srand(time(NULL));	//seeds random time
				yran = rand() % cols;
				xran = rand() % rows;
			}
		}
	}	
	
	/*cout << "-@-@-@-@-@-@-@-Checks AI ship vectors-@-@-@-@-@-@-@-\n";
	for (vector<Point>::iterator it = shipPtr->coords.begin(); it != shipPtr->coords.end(); it++)
	{
		cout << it->x << ", ";
		cout << (*it).y << endl;
	}	
	cout << "-@-@-@-@-@-@-@-@-@-@-@-@-@-@-\n";*/
	
	update();
}

//AI horizontal ship placement - returns true if ship placement successful
bool horizontal()
{
	int positiveCount = -1;	//start at -1 to ignore the initial placement
	bool collide = false;	//prevents overlapping ships
	for (int i = 0; i < shipPtr->getLength(); i++)
	{
		//Check for restrictions on positive side
		if (xran + i < rows && gridOffense[xran+i][yran] != ship && !collide)
		{
			//assign coords to ship & add point to Ship vector
			gridOffense[xran + i][yran] = ship;
			shipPtr->coords.push_back(Point(xran + i, yran));
			positiveCount++;
			//cout << xran + i << ", " << yran << endl;
		}
		//Check for restrictions on negative side
		else if (xran - i + positiveCount >= 0 && gridOffense[xran - i + positiveCount][yran] != ship)
		{
			//assign coords to ship & add point to Ship vector
			gridOffense[xran - i + positiveCount][yran] = ship;
			shipPtr->coords.push_back(Point(xran - i + positiveCount, yran));
			collide = true;
			//cout << xran - i + positiveCount << ", " << yran << endl;
		}
		else
		{			
			//Reads through all coordinates, removes them while changing coordinates to water
			vector<Point>::iterator it = shipPtr->coords.begin();
			while (shipPtr->coords.size() > 0)
			{
				gridOffense[it->x][it->y] = water;	//change back to water
				*it = shipPtr->coords.back();		//push current coordinate to the back
				shipPtr->coords.pop_back();			//removes that coordinate
			}
			//cout << "test\n";
			srand(time(0));
			yran = rand() % cols;
			xran = rand() % rows;
			return 0;
		}
	}
	//cout << "-------------\n";
	return 1;
}

//AI vertical ship placement - returns true if ship placement successful
bool vertical()
{
	int positiveCount = -1;	//start at -1 to ignore the initial placement
	bool collide = false;	//prevents overlapping ships
	for (int i = 0; i < shipPtr->getLength(); i++)
	{
		//Check for restrictions on positive side
		if (yran + i < cols && gridOffense[xran][yran + i] != ship && !collide)
		{
			//assign coords to ship & add point to Ship vector
			gridOffense[xran][yran+i] = ship;
			shipPtr->coords.push_back(Point(xran, yran+i));
			positiveCount++;
			//cout << xran << ", " << yran + i << endl;
		}
		//Check for restrictions on negative side
		else if (yran - i + positiveCount >= 0 && gridOffense[xran][yran - i + positiveCount] != ship)
		{
			//assign coords to ship & add point to Ship vector
			gridOffense[xran][yran - i + positiveCount] = ship;
			shipPtr->coords.push_back(Point(xran, yran - i + positiveCount));
			collide = true;
			//cout << xran << ", " << yran - i + positiveCount << endl;
		}
		else
		{
			//Reads through all coordinates, removes them while changing coordinates to water
			vector<Point>::iterator it = shipPtr->coords.begin();
			while (shipPtr->coords.size() > 0)
			{
				gridOffense[it->x][it->y] = water;	//change back to water
				*it = shipPtr->coords.back();		//push current coordinate to the back
				shipPtr->coords.pop_back();			//removes that coordinate
			}
			//wipe the vector of points
			//shipPtr->coords.clear();
			//cout << "test2\n";
			srand(time(0));
			xran = rand() % rows;
			yran = rand() % cols;
			return 0;
		}
	}
	//cout << "-------------\n";
	return 1;
}

void update()
{
	system("cls");
	//Print Offense grid
	cout << setw(17) << "Offense Grid" << "              " << setw(17) << "Defense Grid\n";
	cout << " |A|B|C|D|E|F|G|H|I|J|         |A|B|C|D|E|F|G|H|I|J|\n";
	for (int i = 0; i < rows; i++)
	{
		cout << i << "|";
		//Prints OFFENSE grid
		for (int j = 0; j < cols; j++)
		{
			/* PROPER
			if (gridOffense[j][i] == water || gridOffense[j][i] == ship)
				cout << " |";
			else if (gridOffense[j][i] == miss)
				cout << "X|";
			else if (gridOffense[j][i] == hit)
				cout << "@|";
				*/
			//shows offense grid temporarily
			if (gridOffense[j][i] == water)
				cout << " |";
			else if (gridOffense[j][i] == ship)
				cout << "O|";
			else if (gridOffense[j][i] == miss)
				cout << "X|";
			else if (gridOffense[j][i] == hit)
				cout << "@|";
		}
		cout << "        " << i << "|";
		//Prints DEFENSE grid
		for (int j = 0; j < cols; j++)
		{
			if (gridDefense[j][i] == water)
				cout << " |";
			else if (gridDefense[j][i] == ship)
				cout << "O|";
			else if (gridDefense[j][i] == miss)
				cout << "X|";
			else if (gridDefense[j][i] == hit)
				cout << "@|";
		}
		cout << endl;
	}
	cout << endl;
}

bool checkAnswer()
{
	//Holds user's answer
	string ans;

	//Ask question
	cout << "Place your " << shipPtr->getName() << " (Length: " << shipPtr->getLength()
		<< ") Ex: 0a 1a\n";

	//Retrieve user input
	getline(cin, ans);

	//Checks if answer is too large
	if (ans.length() != 5)
	{
		cout << "Please input the correct format, not case sensitive (Ex. 5E 7e):\n";
		return 0;
	}

	//Checks if space was input
	if (ans.find(' ', 0) == string::npos)	//if ' ' isn't found
		return 0;

	//separated string to place user's ships
	tokens[0] = ans.substr(0, 2);
	tokens[1] = ans.substr(3, 2);

	//Check cycle for both tokens
	for (int i = 0; i < 2; i++)
	{
		//Checks for 0-9
		if (tokens[i].at(0) < '0' || tokens[i].at(0) > '9')
		{
			cout << "'" << tokens[i].at(0) << "' out of 0-9 range. Try again\n";
			return 0;
		}
		//Checks for A-J (not caps-sensitive)
		if (!((tokens[i].at(1) >= 'A' && tokens[i].at(1) <= 'J') ||
			(tokens[i].at(1) >= 'a' && tokens[i].at(1) <= 'j')))
		{
			cout << "'" << tokens[i].at(1) << "' out of A-J range. Try again\n";
			return 0;
		}
	}

	convertTokens();

	//Tests if coordinates make ship of length 1
	if(x==x2 && y==y2)
	{
		cout << "Please enter coordinates for a ship of length: " << shipPtr->getLength() << endl;
		return 0;
	}

	//Tests if ship placed horizontally or vertically
	if (!((x == x2 && y != y2) || (x != x2 && y == y2)))
	{
		cout << "Cannot place ship diagonally\n";
		return 0;
	}

	//Retrieves distance between 2 coordinates
	if (x == x2)
		dif = abs(y - y2);
	else if (y == y2)
		dif = abs(x - x2);

	//Checks size of input coordinates
	if (dif + 1 != shipPtr->getLength())
	{
		cout << "Please enter coordinates for a ship of length: " << shipPtr->getLength() << endl;
		return 0;
	}

	//Checks for collision for defense grid
	if (x == x2)
	{
		if (y < y2)
		{
			for (int i = 0; i < dif + 1; i++)
			{
				if (gridDefense[x][y + i] == ship)
				{
					cout << "There already exists a ship within these coordinates, try again.\n";
					return 0;
				}
			}
		}
		else if (y2 < y)
		{
			for (int i = 0; i < dif + 1; i++)
			{
				if (gridDefense[x][y2 + i] == ship)
				{
					cout << "There already exists a ship within these coordinates, try again.\n";
					return 0;
				}
			}
		}
	}
	else if (y == y2)
	{
		if (x < x2)
		{
			for (int i = 0; i < dif + 1; i++)
			{
				if (gridDefense[x + i][y] == ship)
				{
					cout << "There already exists a ship within these coordinates, try again.\n";
					return 0;
				}
			}
		}
		else if (x2 < x)
		{
			for (int i = 0; i < dif + 1; i++)
			{
				if (gridDefense[x2 + i][y] == ship)
				{
					cout << "There already exists a ship within these coordinates, try again.\n";
					return 0;
				}
			}
		}
	}

	//Place the ship and set to true
	placeShip();
	return 1;
}

void placeShip()
{
	//Builds ship into designated coordinates
	if (x == x2)
	{
		if (y < y2)
		{
			for (int i = 0; i < dif + 1; i++)
			{
				gridDefense[x][y + i] = ship;
			}
		}
		else if (y2 < y)
		{
			for (int i = 0; i < dif + 1; i++)
			{
				gridDefense[x][y2 + i] = ship;
			}
		}
	}
	else if (y == y2)
	{
		if (x < x2)
		{
			for (int i = 0; i < dif + 1; i++)
			{
				gridDefense[x+i][y] = ship;
			}
		}
		else if (x2 < x)
		{
			for (int i = 0; i < dif + 1; i++)
			{
				gridDefense[x2+i][y] = ship;
			}
		}
	}

	/*print ship placement coordinates
	cout << "x is: " << x << endl;
	cout << "y is: " << y << endl;
	cout << "x2 is: " << x2 << endl;
	cout << "y2 is: " << y2 << endl;*/
	
	//Update game after ship placement successful
	update();
}

//converts string to separate coordinates
void convertTokens()
{
	//Convert y token to int
	y = tokens[0].at(0) - 48;	//-48 because going by ASCII table

	//Convert y2 token to int
	y2 = tokens[1].at(0) - 48;

	//Convert x token to int
	if (tokens[0].at(1) >= 'A' && tokens[0].at(1) <= 'J')
		x = tokens[0].at(1) - 65;
	else
		x = tokens[0].at(1) - 97;

	//Convert x2 token to int
	if (tokens[1].at(1) >= 'A' && tokens[1].at(1) <= 'J')
		x2 = tokens[1].at(1) - 65;
	else
		x2 = tokens[1].at(1) - 97;
}

int main()
{
	update();


	//HardBot bot;
	//botPtr = &bot;

	//Change ship pointer to the Destroyer
	shipPtr = &d;
	//cout << "Place your Destroyer (2 units) Ex: 0a 1a\n";
	while (!checkAnswer());

	//Direct Ship pointer to all of AI's Ships
	shipPtr = &d2;
	populateEnemyGrid();
	shipPtr = &s2;
	populateEnemyGrid();
	shipPtr = &c2;
	populateEnemyGrid();
	shipPtr = &b2;
	populateEnemyGrid();
	shipPtr = &ca2;
	populateEnemyGrid();

	/////////////////////////
	//
	// Place human's ships
	//
	/////////////////////////

	//Change ship pointer to the Submarine
	shipPtr = &s;
	//while (!checkAnswer());

	//Change ship pointer to the Cruiser
	shipPtr = &c;
	//while (!checkAnswer());

	//Change ship pointer to the Battleship
	shipPtr = &b;
	//while (!checkAnswer());

	//Change ship pointer to the Carrier
	shipPtr = &ca;
	//while (!checkAnswer());

	// Commence Firing!
	while (!attack());

	// GAME OVER
	if (compHP == 0)
		cout << "\nYou are VICTORIOUS! GG.\n";
	else
		cout << "\nYou are DEFEATED, what a loser. GG.\n";

	cout << endl;
	system("pause");
    return 0;
}

