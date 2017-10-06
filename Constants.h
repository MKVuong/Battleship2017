#pragma once
#ifndef CONSTANTS_H
#define CONSTANTS_H

using namespace std;	//string usage

//Legend                                                                                |
const int water = 0;
const int ship = 1;
const int miss = 2;
const int hit = 3;

//Players 1 & 2
const int human = 1;
const int computer = 2;

//Bot attack direction
const int hDirect = 0;	//Horizontal direction
const int vDirect = 1;	//Vertical direction
const int nDirect = 2;	//No direction

//Dimensions of grid
static const int rows = 10;
static const int cols = 10;

//grids declared
extern int gridOffense[rows][cols];
extern int gridDefense[rows][cols];

//Hit message notifications
extern string humanMsg;
extern string botMsg;

#endif