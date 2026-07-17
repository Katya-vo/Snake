#define _CRT_SECURE_NO_WARNINGS
#include "snake_records.h"
#include <iostream>
#include <fstream>
#include <time.h>
#include <windows.h>	//
#include <conio.h>		//

using namespace std;

#define MIN_X 3
#define MAX_X 72
#define MIN_Y 1
#define MAX_Y 21
#define FIELD_WIDTH 72
#define FIELD_HEIGHT 23
#define S_SIZE 500

void showCursor(bool flag);
void setCursorPosition(COORD coord);
enum ConsoleColor;
void SetColor(int text, int background);
void draw_field();

void showCursor(bool flag)
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	GetConsoleCursorInfo(hOut, &info);
	info.bVisible = flag;
	SetConsoleCursorInfo(hOut, &info);
}

void setCursorPosition(COORD coord)
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOut, coord);
}

enum ConsoleColor
{
	Black = 0, Blue, Green, Cyan, Red, Magenta, Brown, LightGray, DarkGray,
	LightBlue, LightGreen, LightCyan, LightRed, LightMagenta, Yellow, White
};

void SetColor(int text, int background)
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hOut, (WORD)((background << 4) | text));
}

void draw_field()
{
	char border_char = '+';
	char inner_char = ' ';
	char field_map[FIELD_HEIGHT][FIELD_WIDTH];
	for (int y = 0; y < FIELD_HEIGHT; y++) {
		for (int x = 0; x < FIELD_WIDTH; x++) {
			if (y == 0 || y == FIELD_HEIGHT - 1 || x == 0 || x == FIELD_WIDTH - 1) {
				field_map[y][x] = border_char;
			}
			else {
				field_map[y][x] = inner_char;
			}
		}
	}
	SetColor(LightCyan, Black);

	short start_x = MIN_X - 1;
	short start_y = MIN_Y - 1;

	for (int y = 0; y < FIELD_HEIGHT; y++) {
		setCursorPosition({ start_x, (SHORT)(start_y + y) });
		for (int x = 0; x < FIELD_WIDTH; x++) {
			cout << field_map[y][x];
		}
	}

	SetColor(White, Black);
}
int main()
{
	char playerName[50];
	int bestScore = 0;
	int score = 0;
	int records_size = 0;
	rec* records = nullptr;
	COORD D[4] = { { 0, -1 },{ 0, 1 },{ -1, 0 },{ 1, 0 } };		// direction
	COORD snake[S_SIZE]{ 0, 0 };
	COORD buks = { 0, 0 };			// []
	short cnt = 0;	// next buks period
	short i{ 0 }, k{ 0 };
	int dir = 0;	// direction 0...3
	int size = 5;	// snake size
	char head = '@', body = 'o';	// !
	bool work = true;

	float waitmSecs = 1.f / 8.f;
	clock_t start = 0;


	setlocale(0, "");
	cout << "Enter ur name ";
	cin >> playerName;
	records = readRecords(records_size);
	for (i = 0; i < records_size; i++)
	{
		if (strcmp(records[i].name, playerName) == 0)
		{
			bestScore = records[i].score;
			break;
		}
	}
	if (records != nullptr)
	{
		delete[] records;
		records = nullptr;
	}

	cout << "Ur current record: " << bestScore << "\n";
	cout << "Press enter to start";
	_getch();

	system("cls");		// clear
	system("mode con cols=75 lines=28");	// console size

	srand((int)time(nullptr));

	showCursor(false);

	for (i = 0; i < S_SIZE; i++)		//
		snake[i] = { 0, 0 };

	k = (MAX_X - MIN_X) / 2;
	for (i = 0; i < size; i++)				// init first snake
		snake[i] = { (short)k, (short)(i + MAX_Y - 7) };

	cnt = rand() % 5 + 5;
	do {
		buks.X = rand() % (MAX_X - MIN_X - 1) + (MIN_X + 1);
		buks.Y = rand() % (MAX_Y - MIN_Y - 1) + (MIN_Y + 1);

		k = 0;
		for (i = 0; i < size; i++)
			if (snake[i].X == buks.X and snake[i].Y == buks.Y)
				k = 1;

	} while (k == 1);
	setCursorPosition(buks);
	cout << '*';
	draw_field();


	do		// main loop
	{
		setCursorPosition({ MIN_X,  MAX_Y + 2 });
		cout << "Time: " << clock() / CLK_TCK; // timer

		setCursorPosition({ MIN_X, MAX_Y + 3 });
		cout << "Score: " << score;

		setCursorPosition({ MIN_X, MAX_Y + 4 });
		cout << "Best: " << bestScore;

		setCursorPosition(buks);
		cout << '*';


		setCursorPosition(snake[0]);	// show snake
		cout << head;
		for (i = 1; i < size; i++)
		{
			setCursorPosition(snake[i]);
			cout << body;
		}

		Sleep(100);

		// erase 
		setCursorPosition(snake[0]);
		cout << ' ';
		for (i = 1; i < size; i++)
		{
			setCursorPosition(snake[i]);
			cout << ' ';
		}

		if (clock() - start > waitmSecs * CLOCKS_PER_SEC)	// CLK_TCK
		{
			start = clock();
			//
			for (i = size - 1; i > 0; i--)
				snake[i] = snake[i - 1];	// move snake
			snake[0].X += D[dir].X;			// move head
			snake[0].Y += D[dir].Y;			//
			if (snake[0].X <= MIN_X or snake[0].X >= MAX_X or
				snake[0].Y <= MIN_Y or snake[0].Y >= MAX_Y) // если голова бумс об поле то все пока-пока
			{
				work = false;
			}
			for (i = 1; i < size; i++)
			{
				if (snake[0].X == snake[i].X and snake[0].Y == snake[i].Y)
					work = false; // если об себя стукаемся то тоже история не длинная
			}
			if (snake[0].X == buks.X and snake[0].Y == buks.Y) // щоб кушала и не голодала
			{
				size++;  //жирнеем 
				score++; // и считаем сколько сьели
				do {
					buks.X = rand() % (MAX_X - MIN_X - 1) + (MIN_X + 1);
					buks.Y = rand() % (MAX_Y - MIN_Y - 1) + (MIN_Y + 1); // случайно создаем коорды еды

					k = 0;
					for (i = 0; i < size; i++)
						if (snake[i].X == buks.X and snake[i].Y == buks.Y)
							k = 1; // если коорда еды совпала с тельцем то заново ищем куда еду поставиттьБ читтеров не любим

				} while (k == 1);
				setCursorPosition(buks);
				cout << '*';
			}

		}

		// if (boom)		// +++ 
		//	break;

		if (_kbhit())
		{
			k = (int)_getch();
			switch (k)
			{
			case (27):		// ESC		// +++
			case ('q'):
			case ('Q'):
				work = false;
				break;
			case (72):	 // ^
				dir = 0;
				break;
			case (80):	 // v
				dir = 1;
				break;
			case (75):	 // <-
				dir = 2;
				break;
			case (77):	 // ->
				dir = 3;
				break;
			}
		}
		// if (dir > 3) dir = 0;
		// if (dir < 0) dir = 3;
	} while (work);


	// show snake 
	setCursorPosition(snake[0]);
	cout << head;
	for (i = 1; i < size; i++)
	{
		setCursorPosition(snake[i]);
		cout << body;
	}

	setCursorPosition({ MAX_X / 2, MAX_Y + 3 });
	cout << "BUMS!\n\n";


	// clearIntputBuffer  +-
	HANDLE hOut = GetStdHandle(STD_INPUT_HANDLE);
	FlushConsoleInputBuffer(hOut);

	_getch();
	// system("pause");
	showCursor(true);

	if (score > bestScore)
	{
		updateRecord(playerName, score);
	}
	return 0;
}