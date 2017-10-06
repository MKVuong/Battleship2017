#pragma once
#ifndef POINT_H
#define POINT_H

struct Point
{
	//x & y coordinates of a point in grid
	int x;
	int y;

	Point(int parmx, int parmy)
	{
		x = parmx;
		y = parmy;
	}
	//or
	//Point(int parmx, int parmy) : x(parmx), y(parmy) {}
};

#endif 