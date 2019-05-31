#include <ncurses.h>
#include <stdlib.h>
#include <string>

//대상별 색상인덱스 설정
#define EMPTY_INDEX 0
#define WALL_INDEX 1
#define BOX_INDEX 2
#define DEST_INDEX 3
#define OUTSIDE_INDEX 4
#define DESBOX_INDEX 5
#define CHARACTER_INDEX 6
#define DESCHAR_INDEX 7

std::string char_list = ".^#x~+o*";

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
