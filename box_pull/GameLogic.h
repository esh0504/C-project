#pragma once
#include<iostream>
#include<fstream>
#include<vector>
#include<cstdlib>
using namespace std;
class GameLogic
{
	int step;
	int push;
	int **map;
	vector< pair<int,int> > destination;
	int R;
	int C;
	int numDestination;
public:
	GameLogic(int **map, int R, int C);
	void Move(int direction);
	bool isGameclear();
	int getStep();
	int getPush();
	bool isMap(int i, int j);
	void printMap();
};
