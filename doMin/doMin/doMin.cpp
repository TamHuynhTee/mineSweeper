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

int col = 10, row = 10, level, x, y, mineNum = 10, cellLeft = col* row - mineNum;
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

void matrix_Output()// Testing
{
	int newY = 3;
	for (int i = 0; i < row; i++)
	{
		gotoxy(35 + col * 2, newY);
		for (int j = 0; j < col; j++)
			if (Matrix[i][j] == 0)
				printf("- ");
			else
				printf("%d ", Matrix[i][j]);
		newY++;
	}
}

void loseSituation(int corY,int corX)
{
	HANDLE hConsoleColor;
	hConsoleColor = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsoleColor, 12);// Set color red
	for (int i = 1; i < row - 1; i++)
		for (int j = 1; j < col - 1; j++)
			if (Matrix[i][j] == 9)
			{
				gotoxy(j * 2 + 30, i + 3);
				printf("x");
			}
	gotoxy(corX * 2 + 30, corY + 3);
	SetConsoleTextAttribute(hConsoleColor, 28);
	if (Matrix[corY][corX] == 9)
		printf("x");
	else
		printf("%d", Matrix[corY][corX]);
	gotoxy(35, row + 5);
	SetConsoleTextAttribute(hConsoleColor, 15);
	printf("Oooooohh, You've lost =))! Good luck next time!"); Sleep(3000); gotoxy(35, row + 5);
	printf("                   Press any key to continue .."); getch();
}

void victorySituation()
{
	HANDLE hConsoleColor;
	hConsoleColor = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsoleColor, 14);// Set color yellow
	for (int i = 1; i < row - 1; i++)
		for (int j = 1; j < col - 1; j++)
			if (Matrix[i][j] == 9)
			{
				gotoxy(j * 2 + 30, i + 3);
				printf("+"); gotoxy(j * 2 + 30, i + 3);
			}
	gotoxy(35, 1);
	SetConsoleTextAttribute(hConsoleColor, 15);
	printf("Wowww, Congrats!! Well done!"); Sleep(3000); gotoxy(35, row + 5); 
	printf("Press any key to continue .."); getch();
}

bool index_Open(int index_i, int index_j)
{
	int count_Flag = 0;
	int index = Matrix[index_i][index_j];
	int X[8] = { -1,-1,-1,0,1,1,1,0 };
	int Y[8] = { -1,0,1,1,1,0,-1,-1 };
	for (int k = 0; k < 8; k++)// count flags around the index
		if (Matrix[index_i + X[k]][index_j + Y[k]] == 10 || Matrix[index_i + X[k]][index_j + Y[k]] < 0)
			count_Flag++;
	//----------------
	bool hitBomb = false;
	if (index == count_Flag)
	{
		for (int k = 0; k < 8; k++)
			if (Matrix[index_i + X[k]][index_j + Y[k]] >= 0 && Matrix[index_i + X[k]][index_j + Y[k]] != 10)//&& Opened[index_i + X[k]][index_j + Y[k]] == false)
			{
				gotoxy(30 + 2 * (index_j + X[k]), 3 + index_i + Y[k]);//Bug 
				printf("%d", Matrix[index_i + X[k]][index_j + Y[k]]);//Bug Bug Bug
			}
	}
	gotoxy(0, 5);
	printf("index: %d\nind_i: %d\nind_j: %d", index, index_i, index_j);
	gotoxy(x, y);
	return hitBomb;
}

bool Control()
{
	int matrixX = 1, matrixY = 1;// matrixX for j, matrixY for i, used to run in Matrix
	x += 2; y = 4;// Display matrix abstraction
	gotoxy(x, y);
	HANDLE hConsoleColor;
	hConsoleColor = GetStdHandle(STD_OUTPUT_HANDLE);
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
		case 'a':	if (x > 33)
					{
						x -= 2; matrixX--; gotoxy(x, y);
					}
					break;
		case 'D':
		case 'd':	if (x < col * 2 + 26)
					{
						x += 2; matrixX++; gotoxy(x, y);
					}
					break;
					//--------------------------------------------
		case 'k':// set Flag
		case 'K':	if (Opened[matrixY][matrixX] == true)// Can't set flag on opened cell
						break;
					else if (Matrix[matrixY][matrixX] == 0)// set Flag on empty cell
					{
						Matrix[matrixY][matrixX] = 10; SetConsoleTextAttribute(hConsoleColor, 14);
						printf("F"); gotoxy(x, y);
						SetConsoleTextAttribute(hConsoleColor, 15);
					}
					else if (Matrix[matrixY][matrixX] == 10)
					{
						Matrix[matrixY][matrixX] = 0;
						printf("%c", 254); gotoxy(x, y);
					}
					else if (Matrix[matrixY][matrixX] > 0)// set flag on non-empty cell
					{
						Matrix[matrixY][matrixX] *= -1; SetConsoleTextAttribute(hConsoleColor, 14);
						printf("F"); gotoxy(x, y);
						SetConsoleTextAttribute(hConsoleColor, 15);
					}
					else if (Matrix[matrixY][matrixX] < 0)
					{
						Matrix[matrixY][matrixX] *= -1;
						printf("%c", 254); gotoxy(x, y);
					} 
					break;
					//--------------------------------------------
		case 32:	if (Matrix[matrixY][matrixX] == 9) // Hit bomb
					{
						loseSituation(matrixY,matrixX);
						return false;
					}
					if (Opened[matrixY][matrixX] == true && Matrix[matrixY][matrixX] > 0)
					{
						if (index_Open(matrixY, matrixX) == true) // Hit bomb
						{
							loseSituation(matrixY, matrixX);
							return false;
						}
						break;
					}
					else if (Matrix[matrixY][matrixX] >= 0 && Matrix[matrixY][matrixX] != 10 && Opened[matrixY][matrixX] == false)// Open cell
					{
						Opened[matrixY][matrixX] = true;
						if (Matrix[matrixY][matrixX] == 0)
							printf("-");
						else
							printf("%d", Matrix[matrixY][matrixX]); 
						cellLeft--;
						gotoxy(x, y);
					}
		default: break;
		}
		if (cellLeft == 0)// Victory
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
	x = 30, y = 3;
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
	default: col = 20; row = 29; mineNum = 102; break;
	}
	cellLeft = col * row - mineNum;
	col += 2; row += 2;
}

bool menu()
{
	gotoxy(30, 3);
	printf("Welcome to minesweeper created by TamTee!!\n\nChoose a level\n\t1. 8x11: Easy\n\t2. 14x20: Normal\n\t3. 20x29: Hard\n\t0. Exit\n\t\tYou choose: ");
	while (1)
	{
		scanf("%d", &level);
		if (level > 3 || level < 0)
		{
			printf("Wrong level input, choose again: ");
			continue;
		}
		else
			break;
	}
	if (level == 0)
		return true;
	setLevel();
	setMine();
	return false;
}

void main()
{
	/*bool exit;
	Playing: while (1)
	{
		refresh();
		system("cls");
		exit = menu();
		if (exit == true)
			return;
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
	}*/
	setMine();
	setMap();
	matrix_Output();
	Control();
	_getch();
}