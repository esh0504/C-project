#include "GameLogic.h"
#include "UI.cpp"
using namespace std;

int map[5][10][10] ={ { { 4,4,4,4,4,4,4,4,4,4 },{ 1,1,1,1,4,4,4,4,4,4 },{ 1,3,0,1,1,4,4,4,4,4 },{ 1,3,6,0,1,4,4,4,4,4 },{ 1,3,0,2,1,4,4,4,4,4 },{ 1,1,2,0,1,1,1,4,4,4 },{ 4,1,0,2,0,0,1,4,4,4 },{ 4,1,0,0,0,0,1,4,4,4 },{ 4,1,0,0,1,1,1,4,4,4 },{ 4,1,1,1,1,4,4,4,4,4 } }
,{ { 4,4,4,4,4,4,4,4,4,4 },{ 4,4,4,4,4,4,4,4,4,4 },{ 4,4,4,4,4,4,4,4,4,4 },{ 4,4,1,1,1,1,1,4,4,4 },{ 4,4,1,0,0,0,1,4,4,4 },{ 4,4,1,3,3,3,1,4,4,4 },{ 4,4,1,2,2,2,1,1,4,4 },{ 4,4,1,0,0,0,0,1,4,4 },{ 4,4,1,0,6,0,0,1,4,4 },{ 4,4,1,1,1,1,1,1,4,4 } }
,{ { 4,4,4,4,4,4,4,4,4,4 },{ 4,4,4,4,4,4,4,4,4,4 },{ 4,4,1,1,1,1,1,1,1,4 },{ 4,4,1,0,0,0,0,0,1,4 },{ 4,4,1,0,3,2,3,0,1,4 },{ 4,4,1,0,2,3,2,0,1,4 },{ 4,4,1,0,3,2,3,0,1,4 },{ 4,4,1,0,2,3,2,0,1,4 },{ 4,4,1,0,0,6,0,0,1,4 },{4,4,1,1,1,1,1,1,1,4} }
,{ { 4,4,4,4,4,4,4,4,4,4 },{ 4,4,4,4,4,4,4,4,4,4 },{ 4,4,4,4,4,4,4,4,4,4 },{ 4,4,4,4,4,4,4,4,4,4 },{ 4,1,1,1,1,1,1,1,1,4 },{ 4,1,3,0,0,0,0,0,1,4 },{ 4,1,0,3,2,2,2,6,1,4 },{ 4,1,3,0,0,0,0,0,1,4 },{ 4,1,1,1,1,1,0,0,1,4 },{ 4,4,4,4,4,1,1,1,1,4 } }
,{ { 4,4,4,4,4,4,4,4,4,4 },{ 4,4,4,4,4,4,4,4,4,4 },{ 4,1,1,1,1,4,4,4,4,4 },{ 4,1,0,0,1,1,1,1,4,4 },{ 4,1,0,0,0,0,0,1,1,4 },{ 1,1,0,1,1,0,0,0,1,4 },{ 1,3,0,3,1,0,6,2,1,1 },{ 1,0,0,0,1,0,2,2,0,1 },{ 1,0,0,3,1,0,0,0,0,1 },{ 1,1,1,1,1,1,1,1,1,1 } }
};
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
	keypad(stdscr, TRUE);
	for (int i = 0; i < 5; i++)
	{
		UI view = UI();
		copy(i);
		int **m = new int*[10];
		for (int k = 0; k < 10; k++)
		{
			m[k] =new int[10];
			for (int j = 0; j < 10; j++)
			{
				m[k][j] = mapGame[k][j];
			}
		}
		view.draw(m,0,0);
		GameLogic game = GameLogic(m, 10, 10);
		while (!game.isGameclear()) {
			char key = getch();
			if (key == 'n')
			{
				break;
			}
			if (key=='q')
			{
				i=5;
				break;
			}
			if (key == 'w')
			{
				game.Move(8);
			}
			else if (key == 's')
			{
				game.Move(2);
			}
			else if (key == 'd')
			{
				game.Move(6);
			}
			else if (key == 'a')
			{
				game.Move(4);
			}
			else if (key == 'r')
			{
				copy(i);
				for (int k = 0; k< 10; k++)
				{
					m[k] = new int[10];
					for (int j = 0; j < 10; j++)
					{
						m[k][j] = mapGame[k][j];

					}
				}
				game = GameLogic(m, 10, 10);
			}
			int step=game.getStep();
			int getPush=game.getPush();
			view.draw(m, step, getPush);
		}
	}
	endwin();
	return 0;
}