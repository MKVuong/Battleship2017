/********************************************************************
Program:		Battleship
Author:			Kristein Minh Vuong
Dates:			Around 9/20/2017 through 10/9/2017

Purpose:		Battleship console application that features bots with
				two difficulty settings. 

Notes:			Battleship.cpp defines entry point for console application.

*********************************************************************/
#include <iostream>
#include <vector>
#include <iomanip>		//setw
#include <time.h>
#include <algorithm>	//find_if
#include <stdlib.h>		//srand, rand
#include <stdio.h>		//NULL
#include "Constants.h"
#include "Ship.h"
#include "Point.h"
#include "Bot.h"
#include "EasyBot.h"
#include "HardBot.h"

using namespace std;

//grids declared & initialized
int gridOffense[rows][cols] = { water };
int gridDefense[rows][cols] = { water };

//Hit message notifications
string humanMsg = "";
string botMsg = "";
string debugMsg = "no bug";

//Declarations
string tokens[2];	//pretested coordinates
string ans;			//holds user's answer
int botChoice;		//Choose difficulty
int x, x2, y, y2;	//approved coordinates to place ships
int xran, yran;		//random coordinates for generating enemy ship
int pointCounter;	//sum of points per ship
int sizeLimit;		//used in checkAnswer()
int dif;			//distance between 2 coordinates
int hpHuman = 17;	//human's healthpoints
Ship *shipPtr;		//Ship pointer for user's ships
Bot *botPtr;		//Bot pointer for desired bot

//Function declarations
bool findHit();
bool horizontal();
bool vertical();
bool checkAtk(string);
bool attack();
void placeShip();
void convertTokens();
void update();

//Create your ships
Ship d(2, "Destroyer");
Ship s(3, "Submarine");
Ship c(3, "Cruiser");
Ship b(4, "Battleship");
Ship ca(5, "Carrier");

//Initalize array of pointers to human's ships
Ship *hS[5] = { &d,&s,&c,&b,&ca };

//Create enemy ships
Ship d2(2, "Destroyer");
Ship s2(3, "Submarine");
Ship c2(3, "Cruiser");
Ship b2(4, "Battleship");
Ship ca2(5, "Carrier");

//Initalize array of pointers to computer's ships
Ship *shipArPtrs[5] = { &d2,&s2,&c2,&b2,&ca2 };

//Create potential bot
HardBot hb;
EasyBot eb;

//Check input of Human's attack request
/****************************************************************
FUNCTION:   checkAtk()

ARGUMENTS:  None

RETURNS:	bool

NOTES:		Checks if your attack command has correct input in
			order to process. Returns true if correct.
****************************************************************/
bool checkAtk(string ans)
{
	//Checks if answer is correct size
	if (ans.length() != 2)
	{
		cout << " Please input only 2 characters (Ex. 5E):\n";
		return false;
	}

	//Checks for 0-9
	else if (ans.at(0) < '0' || ans.at(0) > '9')
	{
		cout << " '" << ans.at(0) << "' out of 0-9 range. Try again\n";
		return false;
	}

	//Checks for A-J (not caps-sensitive)
	else if (!((ans.at(1) >= 'A' && ans.at(1) <= 'J') ||
		(ans.at(1) >= 'a' && ans.at(1) <= 'j')))
	{
		cout << " '" << ans.at(1) << "' out of A-J range. Try again\n";
		return false;
	}
	return true;
}

/****************************************************************
FUNCTION:   attack()

ARGUMENTS:  None

RETURNS:	bool

NOTES:		Requests your command to attack at your specified grid
			location. After your turn, computer executes his attack.
			If this function returns true at all, someone has lost.
****************************************************************/
bool attack()
{
	//Ask user where to attack
	cout << "\n Where would you like to bomb?\n\n";

	//String to hold user's input
	string ans;

	//Retrieve initial user input
	getline(cin, ans);

	//Check atk attempt, retrieve user input again if fail
	while (!checkAtk(ans))
		getline(cin, ans);

	//Convert y portion to int
	y = ans.at(0) - 48;	//-48 because ASCII table

	//Convert x portion to int
	if (ans.at(1) >= 'A' && ans.at(1) <= 'J')
		x = ans.at(1) - 65;
	else
		x = ans.at(1) - 97;

	//////////////////////////////
	//							//
	//  Check the target area	//
	//							//
	//////////////////////////////

	//Miss
	if (gridOffense[x][y] == water)
	{
		gridOffense[x][y] = miss;
		humanMsg = "You missed your shot";
	}

	//Hit
	else if (gridOffense[x][y] == ship)
	{
		gridOffense[x][y] = hit;
		humanMsg = "Direct hit";

		//Create function to cycle through all ships' vectors of points to find what ship was hit, 
		//	remove that point and check if vector.empty(), announce sunken ship if so
		findHit();

		botPtr->setHealth(botPtr->getHealth()-1);	//subtract 1 health point from computer

		//Checks if computer lost for Game Over
		if (botPtr->getHealth() == 0)
			return true;
	}

	//Human shot already taken at this grid location
	else if (gridOffense[x][y] == miss || gridOffense[x][y] == hit)
	{
		humanMsg = "You wasted your shot";	//Human already fired at target location
	}

	//Bot's turn to attack
	botPtr->strategy(hS);
	update();	// have 2 message strings connect and cout right after update, encasing hit/miss 
			//from both parties

	/*
	//For debugging purposes
	cout << botPtr->sx << " " << botPtr->sy << endl;
	cout << botPtr->resetStrat << endl;
	cout << botPtr->atkDirection << endl;
	*/

	//Checks if human lost for Game Over
	if (hpHuman == 0)
		return true;

	//Not game over yet
	return false;
}

/****************************************************************
FUNCTION:   findHit()

ARGUMENTS:  None

RETURNS:	bool

NOTES:		Halts searching for hit once a match has been found
****************************************************************/
bool findHit()
{
	//Cycle through all 5 computer ships to find the hit
	for (int i = 0; i < 5; i++)
	{
		//Cycle through all coordinates of each computer ship
		for (vector<Point>::iterator it = shipArPtrs[i]->coords.begin(); 
			it != shipArPtrs[i]->coords.end(); it++)
		{
			//Compare vector element thats being pointed at, to the hit's x & y
			if (it->x == x && it->y == y)
			{
				*it = shipArPtrs[i]->coords.back();	//assign to last element
				shipArPtrs[i]->coords.pop_back();
				//cout << "Ship found: " << shipArPtrs[i]->getName() << ". Current vctr elements remain:\n";

				//Check if that entire ship is destroyed
				if (shipArPtrs[i]->coords.empty())
				{
					humanMsg = "You destroyed their " + shipArPtrs[i]->getName() + "!";
				}
				
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

/****************************************************************
FUNCTION:   populateEnemyGrid()

ARGUMENTS:  None

RETURNS:	None

NOTES:		Initiates enemy ship placement process. Randomly selects
			a location to begin placement, and randomizes direction
			between either horizontal or vertical.
****************************************************************/
void populateEnemyGrid()
{
	//Select random coordinate
	srand(time(NULL));	//seeds random time
	xran = rand() % rows;
	yran = rand() % cols;

	//If selected random coordinates is occupied, find another
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
	/*
	cout << "-@-@-@-@-@-@-@-Checks bot ship vectors-@-@-@-@-@-@-@-\n";
	for (vector<Point>::iterator it = shipPtr->coords.begin(); it != shipPtr->coords.end(); it++)
	{
		cout << it->x << ", ";
		cout << (*it).y << endl;
	}	
	cout << "-@-@-@-@-@-@-@-@-@-@-@-@-@-@-\n";
	*/
}

/****************************************************************
FUNCTION:   horizontal()

ARGUMENTS:  None

RETURNS:	bool

NOTES:		Bot horizontal ship placement. Checks for any collisions.
			Returns true if ship placement was successful.
****************************************************************/
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
			return false;
		}
	}
	//cout << "-------------\n";
	return true;
}

/****************************************************************
FUNCTION:   vertical()

ARGUMENTS:  None

RETURNS:	bool

NOTES:		Bot vertical ship placement. Checks for any collisions.
			Returns true if ship placement was successful.
****************************************************************/
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
			return false;
		}
	}
	//cout << "-------------\n";
	return true;
}

/****************************************************************
FUNCTION:   update()

ARGUMENTS:  None

RETURNS:	None

NOTES:		Used to refresh the arenas and display attack message
			notifications from both parties. May also display debug
			messages if need be.
****************************************************************/
void update()
{
	system("cls");
	//Print Offense grid
	cout << setw(25) << "Offense Grid" << setw(32) << "Defense Grid\n";
	cout << "         |A|B|C|D|E|F|G|H|I|J|          |A|B|C|D|E|F|G|H|I|J|\n";
	for (int i = 0; i < rows; i++)
	{
		cout << "        " << i << "|";
		//Prints OFFENSE grid
		for (int j = 0; j < cols; j++)
		{
			// PROPER
			if (gridOffense[j][i] == water || gridOffense[j][i] == ship)
				cout << " |";
			else if (gridOffense[j][i] == miss)
				cout << "X|";
			else if (gridOffense[j][i] == hit)
				cout << "@|";
				
			/*//shows everything in offense grid - temporary
			if (gridOffense[j][i] == water)
				cout << " |";
			else if (gridOffense[j][i] == ship)
				cout << "O|";
			else if (gridOffense[j][i] == miss)
				cout << "X|";
			else if (gridOffense[j][i] == hit)
				cout << "@|";*/
		}
		cout << "         " << i << "|";
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

	//Hit notification message line
	cout << setw(30) << humanMsg << "         " << setw(22) << botMsg << endl;

	/*//Temporary - helps with what's going on in Bot's strategy()
	cout << debugMsg << endl;
	cout << botPtr->sx << " " << botPtr->sy << endl;
	cout << botPtr->resetStrat << endl;
	cout << botPtr->atkDirection << endl;
	*/
}

/****************************************************************
FUNCTION:   checkAnswer()

ARGUMENTS:  None

RETURNS:    bool

NOTES:      Checks for correct input when attempting to place your
			human ship. If correct, parses accordingly. Also checks 
			for collision with any already existent human ships. 
			If all tests pass, will return true and proceed to 
			call another function for actual placement of ship.
****************************************************************/
bool checkAnswer()
{
	//Ask question
	cout << " Place your " << shipPtr->getName() << " (Length: " << shipPtr->getLength()
		<< ") Ex: 0a 1a\n";

	//Retrieve user input
	getline(cin, ans);

	//Checks if answer is too large
	if (ans.length() != 5)
	{
		cout << " Please input the correct format, not case sensitive (Ex. 5E 7e):\n";
		return false;
	}

	//Checks if space was input
	if (ans.find(' ', 0) == string::npos)	//if ' ' isn't found
		return false;

	//separated string to place user's ships
	tokens[0] = ans.substr(0, 2);
	tokens[1] = ans.substr(3, 2);

	//Check cycle for both tokens
	for (int i = 0; i < 2; i++)
	{
		//Checks for 0-9
		if (tokens[i].at(0) < '0' || tokens[i].at(0) > '9')
		{
			cout << " '" << tokens[i].at(0) << "' out of 0-9 range. Try again\n";
			return false;
		}
		//Checks for A-J (not caps-sensitive)
		if (!((tokens[i].at(1) >= 'A' && tokens[i].at(1) <= 'J') ||
			(tokens[i].at(1) >= 'a' && tokens[i].at(1) <= 'j')))
		{
			cout << " '" << tokens[i].at(1) << "' out of A-J range. Try again\n";
			return false;
		}
	}

	//Called to convert parsed tokens into simpler int values
	convertTokens();

	//Tests if coordinates make ship of length 1
	if(x==x2 && y==y2)
	{
		cout << " Please enter coordinates for a ship of length: " << shipPtr->getLength() << endl;
		return false;
	}

	//Tests if ship placed horizontally or vertically
	if (!((x == x2 && y != y2) || (x != x2 && y == y2)))
	{
		cout << " Cannot place ship diagonally\n";
		return false;
	}

	//Retrieves distance between 2 coordinates
	if (x == x2)
		dif = abs(y - y2);
	else if (y == y2)
		dif = abs(x - x2);

	//Checks size of input coordinates
	if (dif + 1 != shipPtr->getLength())
	{
		cout << " Please enter coordinates for a ship of length: " << shipPtr->getLength() << endl;
		return false;
	}

	//////////////////////////////////////////////
	//											//
	//  Checks for collision for defense grid	//
	//											//
	//////////////////////////////////////////////

	if (x == x2)
	{
		if (y < y2)
		{
			for (int i = 0; i < dif + 1; i++)
			{
				if (gridDefense[x][y + i] == ship)
				{
					cout << " There already exists a ship within these coordinates, try again.\n";
					return false;
				}
			}
		}
		else if (y2 < y)
		{
			for (int i = 0; i < dif + 1; i++)
			{
				if (gridDefense[x][y2 + i] == ship)
				{
					cout << " There already exists a ship within these coordinates, try again.\n";
					return false;
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
					cout << " There already exists a ship within these coordinates, try again.\n";
					return false;
				}
			}
		}
		else if (x2 < x)
		{
			for (int i = 0; i < dif + 1; i++)
			{
				if (gridDefense[x2 + i][y] == ship)
				{
					cout << " There already exists a ship within these coordinates, try again.\n";
					return false;
				}
			}
		}
	}

	//Place the ship and set to true
	placeShip();
	return true;
}

/****************************************************************
FUNCTION:   placeShip()

ARGUMENTS:  None

RETURNS:	None

NOTES:		Processes approved ship placement request.
****************************************************************/
void placeShip()
{
	//Builds ship into designated coordinates
	if (x == x2)
	{
		//if 1st coord < 2nd coord, increment from first coord
		if (y < y2)
		{
			for (int i = 0; i < dif + 1; i++)
			{
				gridDefense[x][y + i] = ship;
				shipPtr->coords.push_back(Point(x, y + i));
			}
		}
		else if (y2 < y)
		{
			for (int i = 0; i < dif + 1; i++)
			{
				gridDefense[x][y2 + i] = ship;
				shipPtr->coords.push_back(Point(x, y2 + i));
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
				shipPtr->coords.push_back(Point(x + i,y));
			}
		}
		else if (x2 < x)
		{
			for (int i = 0; i < dif + 1; i++)
			{
				gridDefense[x2+i][y] = ship;
				shipPtr->coords.push_back(Point(x2 + i, y));
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

/****************************************************************
FUNCTION:   convertTokens()

ARGUMENTS:  None

RETURNS:	None

NOTES:		Converts the char value of input into actual integers
			by subtracting the difference between the values on the
			ASCII table. Used for easier processing of coordinates.
****************************************************************/
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
	//Intro to game & ask user for difficulty
	cout << " Welcome to Battleship\n\nChoose difficulty (Input number)"
		<< "\n 1. Challenger Bot\n 2. Bronze Bot\n\n";

	cin >> botChoice;

	//Must retrieve correct input
	while (botChoice < 1 || botChoice > 2)
	{
		cout << " Please enter a valid choice: ";
		cin >> botChoice;
	}

	//Sets the bot pointer to the address of either the HardBot or EasyBot object
	switch (botChoice)
	{
	case 1: 
		botPtr = &hb;	//set bot pointer to Hard Bot object
		update();
		cout << " Challenger Bot chosen.\n\n";
		break;
	case 2: 
		botPtr = &eb;	//set bot pointer to Easy Bot object
		update();
		cout << " Bronze Bot chosen.\n\n";
		break;
	}
	cout << " Populating enemy ships...\n\n";

	//Populate vector of 100 points - Bot's potential target locations
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			botPtr->options.push_back(Point(i, j));
		}
	}
	
	//clears cin before getline cin
	cin.ignore();

	//////////////////////////////
	//							//
	//  Place computer's ships	//
	//							//
	//////////////////////////////

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
	update();

	//////////////////////////
	//						//
	// Place human's ships	//
	//						//
	//////////////////////////

	//Change ship pointer to the Destroyer
	shipPtr = &d;
	//cout << "Place your Destroyer (2 units) Ex: 0a 1a\n";
	while (!checkAnswer());

	//Change ship pointer to the Submarine
	shipPtr = &s;
	while (!checkAnswer());

	//Change ship pointer to the Cruiser
	shipPtr = &c;
	while (!checkAnswer());

	//Change ship pointer to the Battleship
	shipPtr = &b;
	while (!checkAnswer());

	//Change ship pointer to the Carrier
	shipPtr = &ca;
	while (!checkAnswer());

	// Commence Firing! (Until someone's HP drains to 0)
	while (!attack());

	//Final update before victory/defeat message
	update();

	// GAME OVER
	if (botPtr->getHealth() == 0)
		cout << "\n\n You are VICTORIOUS! GG.\n\n\n\n\n\n\n\n";
	else if(hpHuman == 0)
		cout << "\n\n You've been DEFEATED, what a loser. GG.\n\n\n\n\n\n\n\n";

	system("pause");
    return 0;
}