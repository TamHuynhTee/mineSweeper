#include<iostream>
#include<conio.h>
#include<Windows.h>
#include<time.h>
#include<stdlib.h>
using namespace std;
// Xử lý bấm số để mở ô
// Xử lý loang
// Thuật toán sinh mìn sau khi bấm lần đầu
#define MAX 50

int col, row, level, x, y, mineNum, cellLeft;
int Matrix[MAX][MAX];
bool Opened[MAX][MAX];// check whether the cell is opened

void refresh()
{
	for (int i = 0; i < row; i++)
		for (int j = 0; j < col; j++)
		{
			Matrix[i][j] = 0;
			Opened[i][j] = false;
		}
}

void gotoxy(int x, int y)
{
	static HANDLE h = NULL;
	if (!h)
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { x, y };
	SetConsoleCursorPosition(h, c);
}

int random(int minN, int maxN) 
{
	return minN + rand() % (maxN + 1 - minN);
}

void matrix_Output()
{
	for (int i = 1; i < row - 1; i++)
	{
		for (int j = 1; j < col - 1; j++)
			if (Matrix[i][j] == 0)
				printf("_ ");
			else
				printf("%d ", Matrix[i][j]);
		printf("\n");
	}
}

void loseSituation()
{
	for (int i = 0; i < row; i++)
		for (int j = 0; j < col; j++)
			if (Matrix[i][j] == 9)
			{
				gotoxy(j * 2 + 45, i + 3);
				printf("x"); gotoxy(j * 2 + 45, i + 3);
			}
	gotoxy(35, 1);
	printf("Oooooohh, You've lost =))! Good luck next time!"); Sleep(2000); getch();
}

void victorySituation()
{
	for (int i = 0; i < row; i++)
		for (int j = 0; j < col; j++)
			if (Matrix[i][j] == 9)
			{
				gotoxy(j * 2 + 45, i + 3);
				printf("*"); gotoxy(j * 2 + 45, i + 3);
			}
	gotoxy(35, 1);
	printf("Wowww, Congrats!! Well done!"); Sleep(2000); getch();
}

bool Control()
{
	int matrixX = 1, matrixY = 1;
	x += 2; y = 4;
	gotoxy(x, y);
	while (1)
	{
		switch (getch())
		{
		case 'W':
		case 'w':	if (y > 4)
		{
			y--; matrixY--; gotoxy(x, y);
		}
					break;
		case 'S':
		case 's':	if (y < row + 1)
		{
			y++; matrixY++; gotoxy(x, y);
		}
					break;
		case 'A':
		case 'a':	if (x > 48)
		{
			x -= 2; matrixX--; gotoxy(x, y);
		}
					break;
		case 'D':
		case 'd':	if (x < col * 2 + 41)
		{
			x += 2; matrixX++; gotoxy(x, y);
		}
					break;
		case 'k':// set Flag
		case 'K':	if (Opened[matrixY][matrixX] == true)
			break;
					else if (Matrix[matrixY][matrixX] == 0)
		{
			Matrix[matrixY][matrixX] = 10;
			printf("F"); gotoxy(x, y);
		}
					else if (Matrix[matrixY][matrixX] == 10)
		{
			Matrix[matrixY][matrixX] = 0;
			printf("%c", 254); gotoxy(x, y);
		}
			else if (Matrix[matrixY][matrixX] > 0)
		{
			Matrix[matrixY][matrixX] *= -1;
			printf("F"); gotoxy(x, y);
		}
					else if (Matrix[matrixY][matrixX] < 0)
		{
			Matrix[matrixY][matrixX] *= -1;
			printf("%c", 254); gotoxy(x, y);
		} break;
		case 32:	if (Matrix[matrixY][matrixX] == 9) // Open cell
		{
			loseSituation();
			return false;
		}
					if (Matrix[matrixY][matrixX] >= 0 && Matrix[matrixY][matrixX] != 10)
		{
			Opened[matrixY][matrixX] = true;
			printf("%d", Matrix[matrixY][matrixX]); cellLeft--;
			gotoxy(x, y);
		}
		default: break;
		}
		if (cellLeft == 0)
		{
			victorySituation();
			return true;
		}
	}
	return true;
}

void setMine()
{
	int randCol, randRow;
	srand((int)time(0));
	while (mineNum > 0)
	{
		randCol = random(1, col - 2);
		randRow = random(1, row - 2);
		if (Matrix[randRow][randCol] != 9)
		{
			Matrix[randRow][randCol] = 9;
			mineNum--;
		}
		else
			continue;
	}
	int X[8] = { -1,0,1,1,1,0,-1,-1 };
	int Y[8] = { -1,-1,-1,0,1,1,1,0 };
	for (int i = 1; i < row - 1; i++)
		for (int j = 1; j < col - 1; j++)
			if (Matrix[i][j] == 9)
				for (int k = 0; k < 8; k++)
					if(Matrix[i + X[k]][j + Y[k]] != 9)
						Matrix[i + X[k]][j + Y[k]]++;
}

void setMap()
{
	x = 45, y = 3;
	gotoxy(x, y); y++;
	printf("%c", 201);
	for (int i = 1; i < col - 1; i++)
		printf("%c%c", 205, 205);
	printf("%c%c\n",205 , 187);
	//-------
	for (int i = 1; i < row - 1; i++)
	{
		gotoxy(x, y);
		printf("%c", 186);
		for (int j = 1; j < col - 1; j++)
			printf(" %c", 254);
		printf(" %c", 186);
		y++;
	}
	//-------
	gotoxy(x, y);
	printf("%c", 200);
	for (int i = 1; i < col - 1; i++)
		printf("%c%c", 205, 205);
	printf("%c%c\n", 205, 188);
}

void setLevel()
{
	switch (level)
	{
	case 1: col = 8; row = 11; mineNum = 10; break;
	case 2: col = 14; row = 20; mineNum = 39; break;
	case 3: col = 20; row = 29; mineNum = 102; break;
	default: col = 26; row = 38; mineNum = 230; break;
	}
	cellLeft = col * row - mineNum;
	col += 2; row += 2;
}

void menu()
{
	gotoxy(30, 3);
	printf("Welcome to minesweeper created by TamTee!!\n\nChoose a level\n\t1. 8x11: Easy\n\t2. 14x20: Normal\n\t3. 20x29: Hard\n\t4. 26x38: Expert\nYou choose: ");
	while (1)
	{
		scanf("%d", &level);
		if (level > 4 || level < 1)
		{
			printf("Wrong level input, choose again: ");
			continue;
		}
		else
			break;
	}
	setLevel();
	setMine();
}

void main()
{
	Playing: while (1)
	{
		refresh();
		system("cls");
		menu();
		system("cls");
		setMap();
		matrix_Output();
		bool play = Control();
		if (play == false)
		{
			system("cls");
			gotoxy(30, 5);
			printf("Do you wish to try again?\n\t\t1. Yes\n\t\t0. Exit ");
			while (1)
			{
				switch (getch())
				{
				case '1': goto Playing; break;
				case '0': return;
				default: break;
				}
			}
		}
		else
		{
			system("cls");
			gotoxy(30, 5);
			printf("Take another victory?\n\t\t1. Yes\n\t\t0. Exit ");
			while (1)
			{
				switch (getch())
				{
				case '1': goto Playing; break;
				case '0': return;
				default: break;
				}
			}
		}
	}
	_getch();
}