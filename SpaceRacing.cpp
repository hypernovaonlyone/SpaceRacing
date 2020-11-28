#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <vector>
using namespace std;

constexpr int height = 25;
constexpr int width = 30;
bool gameOver = false;
int score = 0;
int maxScore = 0;
int point = 10;

typedef struct SCar
{
	int x, y, width, height;
	char symbol;
}TCar;

char map[height][width + 1];
std::vector<TCar> obstacles;

TCar car;

void init_car()
{
	car.width = 2;
	car.height = 3;
	car.x = (width - car.width) / 2;
	car.y = height - car.height - 1;
	car.symbol = '@';
}

void print_car(TCar car)
{
	for (int i = car.y; i < car.y + car.height; i++)
	{
		for (int j = car.x; j < car.x + car.width; j++)
			map[i][j] = car.symbol;
	}
}

void move(int dx)
{
	auto old = car.x;
	car.x += dx;
	if (car.x <= 0 || car.x + car.width >= width)
		car.x = old;
}

void init_map()
{
	for (int i = 0; i < width; i++)
		map[0][i] = '#';
	map[0][width] = '\0';
	strncpy_s(map[1], map[0], width);
	
	for (int i = 1; i < width - 1; i++)
		map[1][i] = ' ';
	
	for (int i = 1; i < height; i++)
		strncpy_s(map[i], map[1], width);
}

void print_map()
{
	for (int i = 0; i < height; i++)
	{
		cout << map[i];
		if (i == 3) cout << "       Score: " << score;
		if (i == 4) cout << "       Record: " << maxScore;
		
		if (i < height - 1)
			cout << '\n';
	}
}

void setCursor(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void init_obstacle()
{
	TCar obstacle;
	obstacle.width = 2;
	obstacle.height = 3;
	obstacle.x = 1 + rand() % (width - obstacle.width - 1);
	obstacle.y = 1;
	obstacle.symbol = '*';
	obstacles.push_back(obstacle);
}

void print_obstacles()
{
	for (int i = 0; i < obstacles.size(); i++)
		print_car(obstacles[i]);
}

void move_obstacles()
{
	for (int i = 0; i < obstacles.size(); i++)
	{
		obstacles[i].y++;

		if (map[obstacles[i].y + obstacles[i].height][obstacles[i].x] == '@' ||
			map[obstacles[i].y + obstacles[i].height][obstacles[i].x + obstacles[i].width] == '@')
			gameOver = true;
		
		if (obstacles[i].y + obstacles[i].height >= height)
		{
			obstacles.erase(obstacles.begin() + i);
			score += point;
		}
	}
}

int main()
{
	init_car();
	int count = 5;
	while (GetKeyState(VK_ESCAPE) >= 0)
	{
		setCursor(0, 0);
		if (count == 10)
		{
			init_obstacle();
			count = 0;
		}
		
		if (gameOver)
		{
			if (score > maxScore)
				maxScore = score;
			
			obstacles.clear();
			init_car();
			init_obstacle();
			count = 0;
			score = 0;
			gameOver = false;
		}
		
		init_map();
		print_car(car);
		print_obstacles();
		print_map();

		if (GetKeyState('A') < 0) move(-1);
		if (GetKeyState('D') < 0) move(1);
		move_obstacles();
		Sleep(50);
		count++;
	}
	
	return 0;
}

