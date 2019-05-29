#include "GameLogic.h"
using namespace std;
#define EMPTY 0
#define WALL 1
#define BOX 2
#define DESTINATION 3
#define OUTSIDE 4
#define DESBOX 5
#define CHARACTER 6
#define DESCHARACTER 7
#define LEFT 4
#define RIGHT 6
#define UP 8
#define DOWN 2
typedef pair<int, int> pii;
pair<int, int> operator +(pair<int, int> &i, pair<int, int> &j)
{
	pair<int,int> r = { i.first + j.first,i.second + j.second };
	return r;
}

int Convert_dir(int direction)
{
	if (direction == LEFT)return 0;
	if (direction == UP)return 1;
	if (direction == RIGHT)return 2;
	if (direction == DOWN)return 3;
	return -1;
}

GameLogic::GameLogic(int **map, int R, int C) :step(0), push(0)
{
	this->map = map;
	this->R = R;
	this->C = C;
	for (int i = 0; i < R; i++)
		for (int j = 0; j < C; j++)
			if (map[i][j] == DESTINATION)
				destination.push_back({ i,j });
	this->numDestination = destination.size();
}

bool GameLogic::isMap(int i, int j)
{
	if (i < 0 || i >= R || j < 0 || j >= C)return false;
	return true;
}
void GameLogic::Move(int direction)
{
	pii dir[4];
	dir[Convert_dir(LEFT)] = { 0,-1 };
	dir[Convert_dir(UP)] = { -1,0 };
	dir[Convert_dir(RIGHT)] = { 0,1 };
	dir[Convert_dir(DOWN)] = { 1,0 };
	
	pii *myPos=NULL;
	for (int i = 0; i < R; i++)
		for (int j = 0; j < C; j++)
			if (map[i][j] == CHARACTER || map[i][j]==DESCHARACTER)
			{
				myPos = new pii(i,j);
				break;
			}

	if (!myPos)
	{
		//cout << "캐릭터가 맵 안에 없습니다.\n";
		return;
	}

	direction = Convert_dir(direction);
	if (direction == -1)
	{
		//cout << "올바른 방향을 입력하지 않았습니다\n";
		return;
	}
	pii movePos = *myPos + dir[direction];
	if (!isMap(myPos->first, myPos->second))
		return;
	else if (map[movePos.first][movePos.second] == EMPTY || map[movePos.first][movePos.second]==DESTINATION)
	{
		map[myPos->first][myPos->second] = EMPTY;
		map[movePos.first][movePos.second] = CHARACTER;
		++step;
	}
	else if (map[movePos.first][movePos.second] == BOX || map[movePos.first][movePos.second] == DESBOX)
	{
		pii movePos2 = movePos + dir[direction];
		if (!isMap(movePos2.first, movePos2.second))return;
		else if (map[movePos2.first][movePos2.second] == EMPTY || map[movePos2.first][movePos2.second]==DESTINATION)
		{
			map[movePos2.first][movePos2.second] = BOX;
			map[movePos.first][movePos.second] = CHARACTER;
			map[myPos->first][myPos->second] = EMPTY;
			++step;
			++push;
		}
	}
	for (int i = 0; i < numDestination; i++)
	{
		pii pos = destination[i];
		if (map[pos.first][pos.second] == EMPTY)
			map[pos.first][pos.second] = DESTINATION;
		else if (map[pos.first][pos.second] == CHARACTER)
			map[pos.first][pos.second] = DESCHARACTER;
		else if (map[pos.first][pos.second] == BOX)
			map[pos.first][pos.second] = DESBOX;
	}
	return;
}
bool GameLogic::isGameclear()
{
	for (int i = 0; i < destination.size(); i++)
	{
		pii pos = destination[i];
		if (map[pos.first][pos.second] != DESBOX)return false;
	}
	return true;
}
int GameLogic::getStep()
{
	return step;
}
int GameLogic::getPush()
{
	return push;
}
int main(void)
{
	ifstream input("input.txt");
	int r, c;
	pii pos;
	input >> r >> c;
	input >> pos.first >> pos.second;
	if (input.fail())return 0;
	int **map = new int*[r];
	for (int i = 0; i < r; i++)
	{
		map[i] = new int[c];
		for (int j = 0; j < c; j++)
		{
			input >> map[i][j];
		}
	}
	map[pos.first][pos.second] = CHARACTER;
	GameLogic gl(map,r,c);
	while (1)
	{
		for (int i = 0; i < r; i++)
		{
			for (int j = 0; j < c; j++)
				cout << map[i][j];
			cout << "\n";
		}
		int dir;
		cin >> dir;
		gl.Move(dir);
		system("cls");
		if (gl.isGameclear())break;
	}
	return 0;
}