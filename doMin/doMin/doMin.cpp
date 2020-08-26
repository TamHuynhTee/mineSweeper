﻿#include<iostream>
#include<conio.h>
#include<Windows.h>
#include<time.h>
#include<stdlib.h>
using namespace std;

// Xử lý loang
// Thuật toán sinh mìn sau khi bấm lần đầu
#define MAX 50

int col = 10, row = 10, level, x, y, mineNum = 10, cellLeft = (col - 2) * (row - 2) - mineNum;
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
	int newY = 4;
	for (int i = 1; i < row - 1; i++)
	{
		gotoxy(35 + col * 2, newY);
		for (int j = 1; j < col - 1; j++)
			if (Matrix[i][j] == 0)
				printf("- ");
			else
				printf("%d ", Matrix[i][j]);
		newY++;
	}
}

void changeColor(int k)
{
	HANDLE hConsoleColor;
	hConsoleColor = GetStdHandle(STD_OUTPUT_HANDLE);
	switch (k)
	{
	case 1: SetConsoleTextAttribute(hConsoleColor, 9); break;
	case 2: SetConsoleTextAttribute(hConsoleColor, 2); break;
	case 3: SetConsoleTextAttribute(hConsoleColor, 4); break;
	case 4: SetConsoleTextAttribute(hConsoleColor, 7); break;
	case 5: SetConsoleTextAttribute(hConsoleColor, 5); break;
	case 6: SetConsoleTextAttribute(hConsoleColor, 3); break;
	case 7: SetConsoleTextAttribute(hConsoleColor, 6); break;
	case 8: SetConsoleTextAttribute(hConsoleColor, 8); break;
	default: SetConsoleTextAttribute(hConsoleColor, 15); break;
	}
}

void loseSituation(int corY,int corX)// position u hit the bomb
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
	SetConsoleTextAttribute(hConsoleColor, 28);// Set color red, theme blue
	if (Matrix[corY][corX] == 9)
		printf("x");
	else
		printf("%d", Matrix[corY][corX]);
	gotoxy(20, row + 5);
	SetConsoleTextAttribute(hConsoleColor, 8);
	printf("Oooooohh, You've lost =))! Good luck next time!"); Sleep(3000); gotoxy(20, row + 5);
	SetConsoleTextAttribute(hConsoleColor, 15);
	printf("                   Press any key to continue .."); getch();
}

void victorySituation()
{
	HANDLE hConsoleColor;
	hConsoleColor = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsoleColor, 30);// Set color yellow
	for (int i = 1; i < row - 1; i++)
		for (int j = 1; j < col - 1; j++)
			if (Matrix[i][j] == 9 || Matrix[i][j] == -9)
			{
				gotoxy(j * 2 + 30, i + 3);
				printf("F"); gotoxy(j * 2 + 30, i + 3);
			}
	gotoxy(20, row + 5);
	SetConsoleTextAttribute(hConsoleColor, 14);
	printf("Wowww, Congrats!! Well done!"); Sleep(3000); gotoxy(20, row + 5); 
	SetConsoleTextAttribute(hConsoleColor, 15);
	printf("Press any key to continue .."); getch();
}

bool index_Open(int index_i, int index_j)// Coordinate of hit index
{
	HANDLE hConsoleColor;
	hConsoleColor = GetStdHandle(STD_OUTPUT_HANDLE);
	int count_Flag = 0;
	int index = Matrix[index_i][index_j];
	int X[8] = { -1,-1,-1,0,1,1,1,0 };
	int Y[8] = { -1,0,1,1,1,0,-1,-1 };
	for (int k = 0; k < 8; k++)// count flags around the index
		if (Matrix[index_i + X[k]][index_j + Y[k]] == 10 || Matrix[index_i + X[k]][index_j + Y[k]] < 0)
			count_Flag++;
	if (count_Flag == 0)
		return false;
	//----------------
	bool hitBomb = false;
	int count = 0;
	if (index == count_Flag)
	{
		for (int k = 0; k < 8; k++)
		{
			if (Matrix[index_i + X[k]][index_j + Y[k]] == 9)// hit bomb
				hitBomb = true;
			if ((index_i + X[k] > 0 && index_i + X[k] < row - 1) && (index_j + Y[k] > 0 && index_j + Y[k] < col - 1))
				if (Matrix[index_i + X[k]][index_j + Y[k]] >= 0 && Matrix[index_i + X[k]][index_j + Y[k]] != 10 && Opened[index_i + X[k]][index_j + Y[k]] == false)
				{
					Opened[index_i + X[k]][index_j + Y[k]] = true;
					count++;
				}
		}
		for (int k = 0; k < 8; k++)
			if (Opened[index_i + X[k]][index_j + Y[k]] == true)
			{
				gotoxy(30 + 2 * (index_j + Y[k]), 3 + index_i + X[k]);
				changeColor(Matrix[index_i + X[k]][index_j + Y[k]]);
				if (Matrix[index_i + X[k]][index_j + Y[k]] == 0)
					printf("-");
				else
					printf("%d", Matrix[index_i + X[k]][index_j + Y[k]]);
			}
		SetConsoleTextAttribute(hConsoleColor, 15);
	}
	cellLeft -= count;
	/*gotoxy(0, 5);
	printf("index: %d\nind_i: %d\nind_j: %d\ncount: %d\ncellLeft: %d", index, index_i, index_j, count, cellLeft);
	printf("\n\n");*/
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
					else if (Matrix[matrixY][matrixX] == 0 && mineNum > 0)// set Flag on empty cell
					{
						Matrix[matrixY][matrixX] = 10; SetConsoleTextAttribute(hConsoleColor, 14);
						printf("F"); gotoxy(x, y); mineNum--;
						SetConsoleTextAttribute(hConsoleColor, 15);
					}
					else if (Matrix[matrixY][matrixX] == 10)
					{
						Matrix[matrixY][matrixX] = 0; mineNum++;
						printf("%c", 254); gotoxy(x, y);
					}
					else if (Matrix[matrixY][matrixX] > 0 && mineNum > 0)// set flag on non-empty cell
					{
						Matrix[matrixY][matrixX] *= -1; SetConsoleTextAttribute(hConsoleColor, 14);
						printf("F"); gotoxy(x, y); mineNum--;
						SetConsoleTextAttribute(hConsoleColor, 15);
					}
					else if (Matrix[matrixY][matrixX] < 0)
					{
						Matrix[matrixY][matrixX] *= -1; mineNum++;
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
						changeColor(Matrix[matrixY][matrixX]);
						Opened[matrixY][matrixX] = true;
						if (Matrix[matrixY][matrixX] == 0)
							printf("-");
						else
							printf("%d", Matrix[matrixY][matrixX]);
						SetConsoleTextAttribute(hConsoleColor, 15);
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
		SetConsoleTextAttribute(hConsoleColor, 14);
		gotoxy(20, 5); printf("%3d", cellLeft);
		gotoxy(20, 7); printf("%3d", mineNum);
		gotoxy(10, 10); printf("%2d", matrixX);
		gotoxy(20, 10); printf("%2d", matrixY);
		SetConsoleTextAttribute(hConsoleColor, 15); gotoxy(x, y);
	}
	return true;
}

void setMine()
{
	int randCol, randRow, bombNum = mineNum;
	srand((int)time(0));
	while (bombNum > 0)
	{
		randCol = random(1, col - 2);
		randRow = random(1, row - 2);
		if (Matrix[randRow][randCol] != 9)
		{
			Matrix[randRow][randCol] = 9;
			bombNum--;
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
	//----------------Stats table-------
	gotoxy(3, 3);
	printf(" S T A T S_T A B L E |"); gotoxy(3, 4);
	printf(" ------------------- |"); gotoxy(3, 5);
	printf("Empty cell left: %3d |", cellLeft); gotoxy(3, 6);
	printf("     -----------     |"); gotoxy(3, 7);
	printf("Available flags: %3d |", mineNum); y++; gotoxy(3, 8);
	printf("     -----------     |"); gotoxy(3, 9);
	printf("      COORDINATE     |"); gotoxy(3, 10);
	printf("   X :  1 || Y :  1  |"); gotoxy(3, 11);
	printf("       .--->  X      |"); gotoxy(3, 12);
	printf("       |             |"); gotoxy(3, 13);
	printf("    Y  V             |"); gotoxy(3, 14);
	printf("     -----------     |"); gotoxy(3, 15);
	printf(" MAP SIZE: %3d x %3d |", col - 2, row - 2); gotoxy(3, 16);
	printf(" ------------------- |");
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
	HANDLE hConsoleColor;
	hConsoleColor = GetStdHandle(STD_OUTPUT_HANDLE);
	gotoxy(50, 3);
	SetConsoleTextAttribute(hConsoleColor, 11);
	printf("WELCOME TO MINESWEEPER"); gotoxy(50, 4); SetConsoleTextAttribute(hConsoleColor, 13);
	printf("--Created by TamTee--"); gotoxy(25, 6); SetConsoleTextAttribute(hConsoleColor, 15);
	printf("Choose a level"); gotoxy(32, 7); SetConsoleTextAttribute(hConsoleColor, 10);
	printf("1. 8x11: Easy"); gotoxy(32, 8); SetConsoleTextAttribute(hConsoleColor, 14);
	printf("2. 14x20 : Normal"); gotoxy(32, 9); SetConsoleTextAttribute(hConsoleColor, 12);
	printf("3. 20x29 : Hard"); gotoxy(32, 10); SetConsoleTextAttribute(hConsoleColor, 7);
	printf("0. Exit"); gotoxy(25, 12); SetConsoleTextAttribute(hConsoleColor, 15);
	printf("You choose : ");
	while (1)
	{
		scanf("%d", &level);
		if (level > 3 || level < 0)
		{
			gotoxy(25, 12);
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
	bool exit;
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
	}
	//menu();
	//setMine();
	//setMap();
	//matrix_Output();
	//Control();
	_getch();
}