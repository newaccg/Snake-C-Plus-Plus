
#include <iostream>
#include <queue>
#include <random>
#include <cstdlib>

using std::cout;
using std::endl;
using std::deque;
using std::string;

// X_SIZE / Y_SIZE ~~ 3 recommended and pls dont set too small X_SIZE or Y_SIZE
const unsigned X_SIZE = 40;
const unsigned Y_SIZE = 15;

const char SNAKE_HORIZONTAL_SPRITE = '_';
const char SNAKE_VERTICAL_SPRITE = '|';
const char WALL_HORIZONTAL_SPRITE = '-';
const char WALL_VERTICAL_SPRITE = '|';
const char FILL_GRID_SPRITE = '.';
const char SNAKE_HEAD_SPRITE = '#';
const char APPLE_SPRITE = '!';


string GRID[X_SIZE];

class Point
{
public:
	int x;
	int y;

	Point(int X, int Y)
	{
		x = X; y = Y;
	}

	bool operator ==(Point other)
	{
		if (x == other.x && y == other.y)
			return true;
		return false;
	}
};

class Snake
{
public:
	size_t length;
	deque<Point> path;

	Snake(Point* points, Point& Head, size_t size)
	{
		length = size;

		for (int i = 0; i < size; i++)
		{
			path.push_back(points[i]);
		}
	}

	void EatApple()
	{
		length++;
	}

	void Move(Point& where)
	{
		if (path.size() < length)
		{
			path.push_front(where);
		}
		else
		{
			path.pop_back();
			path.push_front(where);
		}
	}
};

void ShowGrid()
{
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif

	for (int y = 0; y < Y_SIZE; y++)
	{
		for (int x = 0; x < X_SIZE; x++)
		{
			cout << GRID[x][y];
		}
		cout << endl;
	}
}

void FillGrid()
{
	// filling with columns, not rows
	string fillList{""};

	for (int i = 0; i < Y_SIZE; i++)
	{
		fillList += WALL_VERTICAL_SPRITE;
	}
	GRID[0] = fillList;
	GRID[X_SIZE - 1] = fillList;


	fillList = WALL_HORIZONTAL_SPRITE;
	for (int i = 1; i < Y_SIZE; i++)
	{
		fillList += FILL_GRID_SPRITE;
	}
	fillList[Y_SIZE - 1] = WALL_HORIZONTAL_SPRITE;


	for (int i = 1; i < X_SIZE - 1; i++)
	{
		GRID[i] = fillList;
	}
}

Point GetAxis(Point& head)
{
	while (1)
	{
		string in;
		std::cin >> in;
		switch (in[0])
		{
		case 'w':
			return Point(head.x, head.y - 1);
		case 'a':
			return Point(head.x - 1, head.y);
		case 's':
			return Point(head.x, head.y + 1);
		case 'd':
			return Point(head.x + 1, head.y);
		}
		cout << "r u stupid??" << endl;
		std::cin >> in;
	}
}

Point GenerateApple()
{
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<> distribX(1, X_SIZE - 2);
	std::uniform_int_distribution<> distribY(1, Y_SIZE - 2);

	Point where = Point(distribX(gen), distribY(gen));
	while (GRID[where.x][where.y] != '.')
	{
		where = Point(distribX(gen), distribY(gen));
	}

	return where;
}

void DrawApple(Point& where)
{
	GRID[where.x][where.y] = APPLE_SPRITE;
}

void DrawSnake(deque<Point>& points, Point& head)
{
	for (int i = 1; i < points.size(); i++)
	{
		if (points[i].x - points[i - 1].x == 0)
		{
			GRID[points[i].x][points[i].y] = SNAKE_VERTICAL_SPRITE;
		}
		else
		{
			GRID[points[i].x][points[i].y] = SNAKE_HORIZONTAL_SPRITE;
		}
	}
	GRID[head.x][head.y] = SNAKE_HEAD_SPRITE;
}

int main()
{
	FillGrid();

	Point startPoints[]
	{Point(X_SIZE / 2, Y_SIZE / 2), Point(X_SIZE / 2 + 1, Y_SIZE / 2), Point(X_SIZE / 2 + 1, Y_SIZE / 2 - 1)};
	Point apple = GenerateApple();

	Snake snake = Snake(startPoints, startPoints[0], 3);
	Point head = snake.path[0];

	DrawSnake(snake.path, head);
	DrawApple(apple);
	ShowGrid();

	while (1)
	{
		Point axis = GetAxis(head);
		while (axis == snake.path[1])
		{
			cout << "cant do it sry :(" << endl;
			axis = GetAxis(head);
		}

		snake.Move(axis);
		head = snake.path[0];

		if (head == apple)
		{
			snake.EatApple();
			apple = GenerateApple();
		}
		else if (std::find_if(snake.path.begin() + 1,
				 snake.path.end(),
				 [&head](Point& point) { return point == head; }) != snake.path.end()
				 || head.x == X_SIZE - 1 || head.y == Y_SIZE - 1 || head.y == 0 || head.x == 0)
		{
			break;
		}

		FillGrid();
		DrawApple(apple);
		DrawSnake(snake.path, head);
		ShowGrid();
	}
	if (snake.length == (X_SIZE - 2) * (Y_SIZE - 2))
	{
		cout << "u won!" << endl;
	}
	else
	{
		cout << '\n' << "u died lol :)" << endl;
	}
}