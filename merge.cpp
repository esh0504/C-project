#include <ncurses.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <string>

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

using namespace std;

string char_list = ".^#x~+o*";

class UI
{
public:
	UI()
	{
		initscr();
		curs_set(0);		   //실행 시 커서 비활성화
		if (has_colors() == FALSE) //사용자의 터미널(커맨드)에서 색상을 지원하는지 여부확인
		{
			endwin();
			printf("Your terminal does not support color\n");
			exit(1);
		}
		start_color();				      // Color 사용 선언
		if (init_color(COLOR_BLUE, 0, 0, 300) == ERR) //터미널에서 색상이 정상적으로 바뀌는지 여부확인
		{
			printw("Your terminal cannot change the color definitions\n");
			printw("press any key to continue...\n");
			getch();
			move(0, 0);
		}
		init_pair(EMPTY, COLOR_WHITE, COLOR_BLACK);	//0(빈칸):  하양, 검정
		init_pair(WALL, COLOR_BLACK, COLOR_WHITE);	 //1(벽): 검정, 노랑
		init_pair(BOX, COLOR_MAGENTA, COLOR_YELLOW);       //2(상자): 분홍, 노랑
		init_pair(DESTINATION, COLOR_YELLOW, COLOR_BLACK); //3(목표): 노랑, 검정
		init_pair(OUTSIDE, COLOR_CYAN, COLOR_BLUE);	//4(바깥): 형광, 파랑
		init_pair(DESBOX, COLOR_YELLOW, COLOR_RED);	//5(목표+상자): 노랑, 빨강
		init_pair(CHARACTER, COLOR_RED, COLOR_MAGENTA);    //6(캐릭터) 빨강, 분홍
		init_pair(DESCHARACTER, COLOR_CYAN, COLOR_CYAN);   //7(목표+캐릭터):  형광, 형광
	}
	void menu(int s, int p)
	{
		printw("character-step count: %d, box-push count: %d", s, p);
		printw("\n\n");
	}
	void draw(int **map, int step, int push)
	{
		clear();
		menu(step, push);
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				int index = map[i][j];
				attron(COLOR_PAIR(index));
				printw(" %c ", char_list[index]);
				attroff(COLOR_PAIR(index));
			}
			printw("\n");
		}
		refresh();
	}
};

class GameLogic
{
	int step;
	int push;
	int **map;
	vector<pair<int, int>> destination;
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
};

typedef pair<int, int> pii;
pair<int, int> operator+(pair<int, int> &i, pair<int, int> &j)
{
	pair<int, int> r = {i.first + j.first, i.second + j.second};
	return r;
}

int Convert_dir(int direction)
{
	if (direction == LEFT)
		return 0;
	if (direction == UP)
		return 1;
	if (direction == RIGHT)
		return 2;
	if (direction == DOWN)
		return 3;
	return -1;
}

GameLogic::GameLogic(int **map, int R, int C) : step(0), push(0)
{
	this->map = map;
	this->R = R;
	this->C = C;
	for (int i = 0; i < R; i++)
		for (int j = 0; j < C; j++)
			if (map[i][j] == DESTINATION)
				destination.push_back({i, j});
	this->numDestination = destination.size();
}

bool GameLogic::isMap(int i, int j)
{
	if (i < 0 || i >= R || j < 0 || j >= C)
		return false;
	return true;
}

void GameLogic::Move(int direction)
{
	pii dir[4];
	dir[Convert_dir(LEFT)] = {0, -1};
	dir[Convert_dir(UP)] = {-1, 0};
	dir[Convert_dir(RIGHT)] = {0, 1};
	dir[Convert_dir(DOWN)] = {1, 0};

	pii *myPos = NULL;
	for (int i = 0; i < R; i++)
		for (int j = 0; j < C; j++)
			if (map[i][j] == CHARACTER || map[i][j] == DESCHARACTER)
			{
				myPos = new pii(i, j);
				break;
			}

	if (!myPos)
	{
		cout << "캐릭터가 맵 안에 없습니다.\n";
		return;
	}

	direction = Convert_dir(direction);
	if (direction == -1)
	{
		cout << "올바른 방향을 입력하지 않았습니다\n";
		return;
	}

	pii movePos = *myPos + dir[direction];
	if (!isMap(myPos->first, myPos->second))
		return;
	else if (map[movePos.first][movePos.second] == EMPTY || map[movePos.first][movePos.second] == DESTINATION)
	{
		map[myPos->first][myPos->second] = EMPTY;
		map[movePos.first][movePos.second] = CHARACTER;
		++step;
	}
	else if (map[movePos.first][movePos.second] == BOX || map[movePos.first][movePos.second] == DESBOX)
	{
		pii movePos2 = movePos + dir[direction];
		if (!isMap(movePos2.first, movePos2.second))
			return;
		else if (map[movePos2.first][movePos2.second] == EMPTY || map[movePos2.first][movePos2.second] == DESTINATION)
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
		if (map[pos.first][pos.second] != DESBOX)
			return false;
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

int map[5][10][10] = {{{4, 4, 4, 4, 4, 4, 4, 4, 4, 4}, {1, 1, 1, 1, 4, 4, 4, 4, 4, 4}, {1, 3, 0, 1, 1, 4, 4, 4, 4, 4}, {1, 3, 6, 0, 1, 4, 4, 4, 4, 4}, {1, 3, 0, 2, 1, 4, 4, 4, 4, 4}, {1, 1, 2, 0, 1, 1, 1, 4, 4, 4}, {4, 1, 0, 2, 0, 0, 1, 4, 4, 4}, {4, 1, 0, 0, 0, 0, 1, 4, 4, 4}, {4, 1, 0, 0, 1, 1, 1, 4, 4, 4}, {4, 1, 1, 1, 1, 4, 4, 4, 4, 4}},
		      {{4, 4, 4, 4, 4, 4, 4, 4, 4, 4}, {4, 4, 4, 4, 4, 4, 4, 4, 4, 4}, {4, 4, 4, 4, 4, 4, 4, 4, 4, 4}, {4, 4, 1, 1, 1, 1, 1, 4, 4, 4}, {4, 4, 1, 0, 0, 0, 1, 4, 4, 4}, {4, 4, 1, 3, 3, 3, 1, 4, 4, 4}, {4, 4, 1, 2, 2, 2, 1, 1, 4, 4}, {4, 4, 1, 0, 0, 6, 0, 1, 4, 4}, {4, 4, 1, 0, 0, 0, 0, 1, 4, 4}, {4, 4, 1, 1, 1, 1, 1, 1, 4, 4}},
		      {{4, 4, 4, 4, 4, 4, 4, 4, 4, 4}, {4, 4, 4, 4, 4, 4, 4, 4, 4, 4}, {4, 4, 4, 4, 4, 4, 4, 4, 4, 4}, {4, 4, 1, 1, 1, 1, 1, 1, 1, 4}, {4, 4, 1, 0, 0, 0, 0, 0, 1, 4}, {4, 4, 1, 0, 3, 2, 3, 6, 1, 4}, {4, 4, 1, 0, 2, 3, 2, 0, 1, 4}, {4, 4, 1, 0, 3, 2, 3, 0, 1, 4}, {4, 4, 1, 0, 0, 0, 0, 0, 1, 4}, {4, 4, 1, 1, 1, 1, 1, 1, 1, 4}},
		      {{4, 4, 4, 4, 4, 4, 4, 4, 4, 4}, {4, 4, 4, 4, 4, 4, 4, 4, 4, 4}, {4, 4, 4, 4, 4, 4, 4, 4, 4, 4}, {4, 4, 4, 4, 4, 4, 4, 4, 4, 4}, {4, 1, 1, 1, 1, 1, 1, 1, 1, 4}, {4, 1, 3, 0, 0, 0, 0, 0, 1, 4}, {4, 1, 0, 3, 2, 2, 2, 0, 1, 4}, {4, 1, 3, 0, 0, 0, 0, 0, 1, 4}, {4, 1, 1, 1, 1, 1, 6, 0, 1, 4}, {4, 4, 4, 4, 4, 1, 1, 1, 1, 4}},
		      {{4, 4, 4, 4, 4, 4, 4, 4, 4, 4}, {4, 4, 4, 4, 4, 4, 4, 4, 4, 4}, {4, 1, 1, 1, 1, 4, 4, 4, 4, 4}, {4, 1, 0, 0, 1, 1, 1, 1, 4, 4}, {4, 1, 0, 0, 0, 0, 0, 1, 1, 4}, {1, 1, 0, 1, 1, 0, 0, 0, 1, 4}, {1, 3, 0, 3, 1, 0, 0, 2, 1, 1}, {1, 0, 0, 0, 1, 0, 2, 2, 0, 1}, {1, 0, 0, 3, 1, 0, 6, 0, 0, 1}, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}}};
int mapGame[10][10];

void copy(int num)
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			mapGame[i][j] = map[num][i][j];
		}
	}
}

int main(void)
{
	UI view;
	keypad(stdscr, TRUE);
	for (int i = 0; i < 5; i++)
	{
		copy(i);
		int **m = new int *[10];
		for (int i = 0; i < 10; i++)
			m[i] = new int[10];
		for (int j = 0; j < 10; j++)
		{
			m[i][j] = mapGame[i][j];
		}
	}
	GameLogic game = GameLogic(m, 10, 10);
	view.draw(m, game.getStep(), game.getPush());
	while (!game.isGameclear())
	{
		int key = getch();
		if (key == 27) //ESC
		{
			break;
		}
		else if (key == KEY_UP)
		{
			game.Move(8);
		}
		else if (key == KEY_DOWN)
		{
			game.Move(2);
		}
		else if (key == KEY_RIGHT)
		{
			game.Move(6);
		}
		else if (key == KEY_LEFT)
		{
			game.Move(4);
		}
		else if (key == 'r')
		{
			copy(i);
			game = GameLogic(m, 10, 10);
		}
		view.draw(m, game.getStep(), game.getPush());
	}
}
endwin();
return 0;
}
