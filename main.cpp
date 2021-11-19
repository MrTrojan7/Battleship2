#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <locale>
#include <algorithm>
#include <vector>
#include <stack>

using namespace std;

HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
const int Size = 10;
const int Tale_size_X = 4;
const int Tale_size_Y = 2;
const char Empty = '~';
const char Ship = 'O';
const char Fog = '.';
const char Failure = ' ';
const char Destroyed = 'x';
const char* TopFrame = "RESPUBLICA";
const unsigned PAUSE = 1000;
int Field_LEFT = 2;
int Field_UP = 2;
const short Menu_Y = (Tale_size_Y * Size + Field_UP * 2);
const int Enemy_LEFT = (Field_LEFT + Tale_size_X) * Size;
char letter_of_move;
int Coord_X;
short Coord_Y;
bool TrueShip = false;
const short SizeShoot = 4;
bool ShootDir[SizeShoot];
int DrawShips = 1;
bool Damage = false;

///
short PlayerLife = 20;
short EnemyLife = 20;
///

bool Damaged = 0;

enum Status
{
	EMPTY,
	SHIP,
	FOG,
	FAILURE,
	DESTROYED
};

enum Direction
{
	LEFT,
	UP,
	RIGHT,
	DOWN,
	IMPOSSIBLE = -1
};

struct Point
{
	int row;
	int col;
} User, Enemy;

bool operator==(const Point& a, const Point& b)
{
	return a.row == b.row && a.col == b.col;
}

struct Vector
{
	Point point;
	int dir;
	size_t length;
} VectorEnemy, VectorPlayer;


////////////////////////////////////////////////////////////////////////////////////////////
vector<Point> enemy_shoots;
bool IsVertical();
bool IsHorizontal();
bool HasVerticals();
bool HasHorizontals();
void RestartEnemyShoots();
void AddVerticalShoots(int** field, int row, int col);
void AddHorizontalShoots(int** field, int row, int col);
bool IsSootReachedLine();
////////////////////////////////////////////////////////////////////////////////////////////
void MoveEnemy(int** field);

vector<Point> random_enemy_turns;
void Init_random_enemy_turns();
void RandNum(int** field);
void RandomTurnEnemy(int** field);
void EraseRandomTurnEnemys(int** field);
////////////////////////////////////////////////////////////////////////////////////////////
vector<Point> smart_enemy_turns;
void AddNeigboursToSmart(int** field, int row, int col);
bool IsUnknown(int** filed, int row, int col);
void SmartTurnEnemy(int** field);
void RestartSmart();
bool IsSmartOn();

//void ChangeOrderRandomTurnEnemys(int row, int col);
//bool ChangeRandomTurnEnemy(int row, int col, vector<Point> const& vec);

void PrintRandomTurnEnemy();
////////////////////////////////////////////////////////////////////////////////////////////
void DrawPlayerField(int** arr, int size = 10, short x = 0, short y = 0);
void DrawEnemyField(int** arr, int size = 10, short x = 40, short y = 0);  ///в функции поменять Fog на Ship для отображения кораблей
void MovePlayer(int** field);
void ToUpperChar(char* ch);
bool ReadUser(char Uchar, int Uint);
bool IsValidNum(int num);
bool IsValidChar(char ch);
void BatleParser(const char* src, char* ch, int* num);
void DrawMenu(short y, short x = 0);
bool IsValidCell(int** field, int col, int row);
////////////////////////////////////////////////////////////////////////////////////////////
void InitField(int** field);
bool IsAllowedToSet(int** field, short x, short y);
bool OutOfBounds(int** field, short x, short y);
int GetAllowedDirection(int** field, short x, short y, int size);
void SetShip(int** field, short x, short y, int size, int dir);
////////////////////////////////////////////////////////////////////////////////////////////
bool CheckLeft(int** field, short x, short y, int size);
bool CheckUp(int** field, short x, short y, int size);
bool CheckRight(int** field, short x, short y, int size);
bool CheckDown(int** field, short x, short y, int size);
////////////////////////////////////////////////////////////////////////////////////////////
bool IsDamage(int** field, short x, short y);
bool IsAlivePlayer();
bool IsAliveEnemy();
////////////////////////////////////////////////////////////////////////////////////////////
void CleanAreaOutOfDrawnedShip(int** field, Vector const* vec);
bool IsDrowned(int** field, Vector const* vec);
void GetBeginOfShip(int** field, short y, short x, Vector* vec);
bool IsMonoShip(int** field, short y, short x);
bool IsLeftOfShip(int** field, short y, short x);
bool IsTopOfShip(int** field, short y, short x);
int GetShiftX(int** field, short y, short x);
int GetShiftY(int** field, short y, short x);
bool IsCellShipOrDestroy(int** field, short y, short x);
bool CanItBeLeft(int** field, short y, short x);
void GetSizeOfShip(int** field, Vector* vec);
void PrintVector(Vector const* v);
void PrintDrowned();
bool IsShip(int** field, short x, short y);
////////////////////////////////////////////////////////////////////////////////////////////


int main()
{
	system("mode con cols=105 lines=29");
	setlocale(LC_ALL, "Russian");
	SetConsoleTitle(TEXT("Морской Бой"));
	srand(time(NULL));

	Init_random_enemy_turns();

	int** arrPlayer = new int* [Size];
	for (int i = 0; i < Size; i++)
	{
		arrPlayer[i] = new int[Size];
	}

	int** arrEnemy = new int* [Size];
	for (int i = 0; i < Size; i++)
	{
		arrEnemy[i] = new int[Size];
	}

	for (size_t i = 0; i < Size; i++)
	{
		for (size_t j = 0; j < Size; j++)
		{
			arrPlayer[i][j] = FOG;
			arrEnemy[i][j] = FOG;
		}
	}
	InitField(arrEnemy);
	InitField(arrPlayer);

	//begin main loop

	bool turn_player = true;
	bool win_player = false;

	while (true)
	{
		DrawPlayerField(arrPlayer, Size);
		DrawEnemyField(arrEnemy, Size, Enemy_LEFT);
		DrawMenu(Menu_Y, 0);
		if (turn_player)
		{
			if (!IsAliveEnemy())
			{
				break;
			}
			MovePlayer(arrEnemy);
			turn_player = false;
			if (IsDamage(arrEnemy, User.row, User.col))
			{
				EnemyLife--;
				turn_player = true;
				//#Debug code
				GetBeginOfShip(arrEnemy, User.row, User.col, &VectorEnemy);
				GetSizeOfShip(arrEnemy, &VectorEnemy);
				//#PrintVector(&VectorEnemy);

				//#check is drawned
				if (IsDrowned(arrEnemy, &VectorEnemy))
				{
					//PrintDrowned();
					CleanAreaOutOfDrawnedShip(arrEnemy, &VectorEnemy);
					//Sleep(6500);
				}

			}
		}

		else
		{
			if (!IsAlivePlayer())
			{
				break;
			}
			turn_player = true;
			MoveEnemy(arrPlayer);

			PrintRandomTurnEnemy();
			//Sleep(3000);

			if (IsDamage(arrPlayer, Enemy.row, Enemy.col))
			{
				PlayerLife--;
				turn_player = false;
				//changes_step = 0;
				//ChangeOrderRandomTurnEnemys(Enemy.row, Enemy.col);

				//Smart move
				AddNeigboursToSmart(arrPlayer, Enemy.row, Enemy.col);

				//AddMore();
				
				GetBeginOfShip(arrPlayer, Enemy.row, Enemy.col, &VectorPlayer);
				GetSizeOfShip(arrPlayer, &VectorPlayer);
				
				/*PrintVector(&VectorPlayer);
				Sleep(6500);*/

				//#check is drawned
				if (IsDrowned(arrPlayer, &VectorPlayer))
				{
					//PrintDrowned();
					CleanAreaOutOfDrawnedShip(arrPlayer, &VectorPlayer);
					EraseRandomTurnEnemys(arrPlayer);

					RestartSmart();
					RestartEnemyShoots();
					//Sleep(6500);
				}
			}
			Sleep(2000);
		}
		system("cls");
	}

	////////////////////////////////////////////////////////////////////////

	system("cls");
	win_player = turn_player;
	if (win_player)
	{
		cout << "Congratulations! You won!\n";
	}
	else
	{
		cout << "Game over!\n";
	}

	////////////////////////////////////////////////////////////////////////

	for (int i = 0; i < Size; i++) // удаление из памяти массива поля игрока
	{
		delete[] arrPlayer[i];
	}

	delete[] arrPlayer;

	for (int i = 0; i < Size; i++) // удаление из памяти массива поля ИИ
	{
		delete[] arrEnemy[i];
	}

	delete[] arrEnemy;

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//					Draw field
void DrawPlayerField(int** field, int size, short x, short y) // отрисовка поля игрока
{
	int shift_X = 5;
	for (size_t i = 0; i < size; i++)
	{
		SetConsoleCursorPosition(hStdOut, { short(x + shift_X + i * Tale_size_X), short(y) });
		cout << TopFrame[i];
	}

	for (size_t row = 0; row < size; row++)
	{
		SetConsoleCursorPosition(hStdOut, { short(x), short(y + 2 + row * Tale_size_Y) });
		cout << row + 1;
		for (size_t col = 0; col < size; col++)
		{
			SetConsoleCursorPosition(hStdOut, { short(x + shift_X + col * Tale_size_X), short(y + 2 + row * Tale_size_Y) });
			int cell = field[row][col];
			switch (cell)
			{
			case SHIP:
				cout << Ship;
				break;

			case FOG:
				cout << Fog;
				break;

			case DESTROYED:
				cout << Destroyed;
				break;

			case FAILURE:
				cout << Failure;
				break;

			default:
				break;
			}
		}
	}
}

void DrawEnemyField(int** field, int size, short x, short y) //отрисовка поля ИИ
{
	int shift_X = 5;
	for (size_t i = 0; i < size; i++)
	{
		SetConsoleCursorPosition(hStdOut, { short(x + shift_X + i * Tale_size_X), short(y) });
		cout << TopFrame[i];
	}

	for (size_t row = 0; row < size; row++)
	{
		SetConsoleCursorPosition(hStdOut, { short(x), short(y + 2 + row * Tale_size_Y) });
		cout << row + 1;
		for (size_t col = 0; col < size; col++)
		{
			SetConsoleCursorPosition(hStdOut, { short(x + shift_X + col * Tale_size_X), short(y + 2 + row * Tale_size_Y) });
			int cell = field[row][col];
			switch (cell)
			{
			case SHIP:
				cout << Ship;///////////Поменять на Ship для теста
				//cout << Fog;///////////Поменять на Ship для теста
				break;

			case FOG:
				cout << Fog;
				break;

			case DESTROYED:
				cout << Destroyed;
				break;

			case FAILURE:
				cout << Failure;
				break;

			default:
				break;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//					Read player turn
void ToUpperChar(char* ch)
{
	const char shift = 'A' - 'a';

	if (*ch >= 'a' && *ch <= 'z')
	{
		*ch += shift;
	}
}

bool ReadUser(char Uchar, int Uint)
{
	if (!IsValidNum(Uint) || !IsValidChar(Uchar))
	{
		return false;
	}
	User.row = Uint - 1;
	ToUpperChar(&Uchar);
	User.col = strchr(TopFrame, Uchar) - TopFrame;;
	return true;
}

bool IsValidNum(int num)
{
	if (num < 1 || num > 10)
	{
		return false;
	}
	return true;
}

bool IsValidChar(char ch)
{
	ToUpperChar(&ch);
	return strchr(TopFrame, ch);
}

void BatleParser(const char* src, char* ch, int* num)
{
	*ch = src[0];
	*num = atoi(src + 1);
}

void DrawMenu(short y, short x)
{
	SetConsoleCursorPosition(hStdOut, { x, y });
	cout << "Введите координаты: ";
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//			Set ships on field

void InitField(int** field)
{
	int cnt = 0;
	int dir = -1;
	while (cnt != 10)
	{
		int x = rand() % Size;
		int y = rand() % Size;

		//Set 4x
		if (cnt == 0)
		{
			dir = GetAllowedDirection(field, x, y, 4);
			if (dir != IMPOSSIBLE)
			{
				SetShip(field, x, y, 4, dir);
				++cnt;
			}

		}
		//Set 3x
		else if (cnt < 3)
		{
			dir = GetAllowedDirection(field, x, y, 3);
			if (dir != IMPOSSIBLE)
			{
				SetShip(field, x, y, 3, dir);
				++cnt;
			}
		}
		//Set2x
		else if (cnt < 6)
		{
			dir = GetAllowedDirection(field, x, y, 2);
			if (dir != IMPOSSIBLE)
			{
				SetShip(field, x, y, 2, dir);
				++cnt;
			}
		}
		//Set1x
		else
		{
			if (IsAllowedToSet(field, x, y) && OutOfBounds(field, x, y))
			{
				field[y][x] = SHIP;
				++cnt;
			}
		}

	}
	//debug print
	SetConsoleCursorPosition(hStdOut, { 0, 26 });
	cout << cnt << endl;
	//Sleep(3000);
}

bool IsAllowedToSet(int** field, short x, short y)
{
	if (y == 0)
	{
		if (x == 0)
		{
			if (
				field[y][x + 1] == SHIP ||
				field[y + 1][x] == SHIP || field[y + 1][x + 1] == SHIP
				)
				return false;
		}
		else if (x == Size - 1)
		{
			if (
				field[y][x - 1] == SHIP ||
				field[y + 1][x] == SHIP || field[y + 1][x - 1] == SHIP
				)
				return false;
		}
		else
		{
			if (
				field[y][x - 1] == SHIP || field[y][x + 1] == SHIP ||
				field[y + 1][x] == SHIP || field[y + 1][x - 1] == SHIP || field[y + 1][x + 1] == SHIP
				)
				return false;
		}
	}
	else if (y == Size - 1)
	{
		if (x == 0)
		{
			if (
				field[y][x + 1] == SHIP ||
				field[y - 1][x] == SHIP || field[y - 1][x + 1] == SHIP
				)
				return false;
		}
		else if (x == Size - 1)
		{
			if (
				field[y][x - 1] == SHIP ||
				field[y - 1][x] == SHIP || field[y - 1][x - 1] == SHIP
				)
				return false;
		}
		else
		{
			if (
				field[y][x - 1] == SHIP || field[y][x + 1] == SHIP ||
				field[y - 1][x] == SHIP || field[y - 1][x - 1] == SHIP || field[y - 1][x + 1] == SHIP
				)
				return false;
		}
	}
	else
	{
		if (x == 0)
		{
			if (
				field[y - 1][x] == SHIP || field[y - 1][x + 1] == SHIP ||
				field[y][x + 1] == SHIP ||
				field[y + 1][x] == SHIP || field[y + 1][x + 1] == SHIP
				)
				return false;
		}
		else if (x == Size - 1)
		{
			if (
				field[y - 1][x - 1] == SHIP || field[y - 1][x] == SHIP ||
				field[y][x - 1] == SHIP ||
				field[y + 1][x] == SHIP || field[y + 1][x - 1] == SHIP
				)
				return false;
		}
		else
		{
			if (
				field[y - 1][x - 1] == SHIP || field[y - 1][x] == SHIP || field[y - 1][x + 1] == SHIP ||
				field[y][x - 1] == SHIP || field[y][x + 1] == SHIP ||
				field[y + 1][x - 1] == SHIP || field[y + 1][x] == SHIP || field[y + 1][x + 1] == SHIP
				)
				return false;
		}
	}
	return true;
}

bool OutOfBounds(int** field, short x, short y)
{
	if (
		y < 0 ||
		y >= Size ||
		x < 0 ||
		x >= Size ||
		field[y][x] == SHIP
		)
	{
		return false;
	}
	return true;
}

int GetAllowedDirection(int** field, short x, short y, int size)
{
	int results[4] = { -1, -1, -1, -1 };
	int result = -1;
	int head = 0;
	if (y == 0)
	{
		if (x == 0)
		{
			//check to right
			if (CheckRight(field, x, y, size))
			{
				results[head++] = RIGHT;
			}
			//check to down
			if (CheckDown(field, x, y, size))
			{
				results[head++] = DOWN;
			}
		}
		else if (x == (Size - 1))
		{
			//check to left
			if (CheckLeft(field, x, y, size))
			{
				results[head++] = LEFT;
			}
			//check to down
			if (CheckDown(field, x, y, size))
			{
				results[head++] = DOWN;
			}
		}
		else
		{
			//check to left
			if (CheckLeft(field, x, y, size))
			{
				results[head++] = LEFT;
			}
			//check to right
			if (CheckRight(field, x, y, size))
			{
				results[head++] = RIGHT;
			}
			//check to down
			if (CheckDown(field, x, y, size))
			{
				results[head++] = DOWN;
			}

		}
	}
	else if (y == (Size - 1))
	{
		if (x == 0)
		{
			//check to right
			if (CheckRight(field, x, y, size))
			{
				results[head++] = RIGHT;
			}
			//check to up
			if (CheckUp(field, x, y, size))
			{
				results[head++] = UP;
			}
		}
		else if (x == (Size - 1))
		{
			//check to left
			if (CheckLeft(field, x, y, size))
			{
				results[head++] = LEFT;
			}
			//check to up
			if (CheckUp(field, x, y, size))
			{
				results[head++] = UP;
			}
		}
		else
		{
			//check to left
			if (CheckLeft(field, x, y, size))
			{
				results[head++] = LEFT;
			}
			//check to right
			if (CheckRight(field, x, y, size))
			{
				results[head++] = RIGHT;
			}
			//check to up
			if (CheckUp(field, x, y, size))
			{
				results[head++] = UP;
			}
		}
	}
	else
	{
		if (x == 0)
		{
			//check to right
			if (CheckRight(field, x, y, size))
			{
				results[head++] = RIGHT;
			}
			//check to up
			if (CheckUp(field, x, y, size))
			{
				results[head++] = UP;
			}
			//check to down
			CheckDown(field, x, y, size);
		}
		else if (x == (Size - 1))
		{
			//check to left
			if (CheckLeft(field, x, y, size))
			{
				results[head++] = LEFT;
			}
			//check to up
			if (CheckUp(field, x, y, size))
			{
				results[head++] = UP;
			}
			//check to down
			if (CheckDown(field, x, y, size))
			{
				results[head++] = DOWN;
			}
		}
		else
		{
			//check to left
			if (CheckLeft(field, x, y, size))
			{
				results[head++] = LEFT;
			}
			//check to right
			if (CheckRight(field, x, y, size))
			{
				results[head++] = RIGHT;
			}
			//check to up
			if (CheckUp(field, x, y, size))
			{
				results[head++] = UP;
			}
		}
	}
	if (head != 0)
	{
		int rnd = rand() % head;
		result = results[rnd];
	}
	return result;
}

void SetShip(int** field, short x, short y, int size, int dir)
{
	switch (dir)
	{
	case LEFT:
		for (size_t i = 0; i < size; i++)
		{
			field[y][x - i] = SHIP;
		}
		break;

	case UP:
		for (size_t i = 0; i < size; i++)
		{
			field[y - i][x] = SHIP;
		}
		break;

	case RIGHT:
		for (size_t i = 0; i < size; i++)
		{
			field[y][x + i] = SHIP;
		}
		break;

	case DOWN:
		for (size_t i = 0; i < size; i++)
		{
			field[y + i][x] = SHIP;
		}
		break;

	default:
		break;
	}
}
//////
bool CheckLeft(int** field, short x, short y, int size)
{
	for (size_t i = 0; i < size; i++)
	{
		if (!OutOfBounds(field, x - i, y))
		{
			return false;
		}
		else if (!IsAllowedToSet(field, x - i, y))
		{
			return false;
		}
	}
	return true;
}

bool CheckUp(int** field, short x, short y, int size)
{
	for (size_t i = 0; i < size; i++)
	{
		if (!OutOfBounds(field, x, y - i))
		{
			return false;
		}
		else if (!IsAllowedToSet(field, x, y - i))
		{
			return false;
		}
	}
	return true;
}

bool CheckRight(int** field, short x, short y, int size)
{
	for (size_t i = 0; i < size; i++)
	{
		if (!OutOfBounds(field, x + i, y))
		{
			return false;
		}
		else if (!IsAllowedToSet(field, x + i, y))
		{
			return false;
		}
	}
	return true;
}

bool CheckDown(int** field, short x, short y, int size)
{
	for (size_t i = 0; i < size; i++)
	{
		if (!OutOfBounds(field, x, y + i))
		{
			return false;
		}
		else if (!IsAllowedToSet(field, x, y + i))
		{
			return false;
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//						Check Who is alive
bool IsDamage(int** field, short y, short x)
{
	return field[y][x] == DESTROYED;
}

bool IsAlivePlayer()
{
	return PlayerLife != 0;
}

bool IsAliveEnemy()
{
	return EnemyLife != 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//						Solve Drawing ship and erase draned steps
void CleanAreaOutOfDrawnedShip(int** field, Vector const* vec)
{
	int lefter = vec->point.col - 1;
	int upper = vec->point.row - 1;
	int const iters = 3;
	int rows, cols;
	int longer = vec->length + 2;
	if (vec->dir == LEFT)
	{
		rows = iters;
		cols = longer;
	}
	else
	{
		rows = longer;
		cols = iters;
	}

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (upper + i < 0 || upper + i >= Size
				|| lefter + j < 0 || lefter + j >= Size
				|| field[upper + i][lefter + j] == DESTROYED
				)
			{
				continue;
			}
			field[upper + i][lefter + j] = EMPTY;
		}
	}

}

bool IsDrowned(int** field, Vector const* vec)
{
	int left = vec->point.col;
	int up = vec->point.row;
	int len = vec->length;
	if (vec->dir == LEFT)
	{
		for (size_t i = 0; i < len; i++)
		{
			if (field[up][left + i] != DESTROYED)
			{
				return false;
			}
		}
	}
	else // UP
	{
		for (size_t i = 0; i < len; i++)
		{
			if (field[up + i][left] != DESTROYED)
			{
				return false;
			}
		}
	}

	return true;
}

void GetBeginOfShip(int** field, short y, short x, Vector* vec)
{
	// Check Monoship
	if (IsMonoShip(field, y, x))
	{
		vec->dir = IMPOSSIBLE;
		vec->point.row = y;
		vec->point.col = x;
		return;
	}
	// Check Left
	if (IsLeftOfShip(field, y, x))
	{
		vec->dir = LEFT;
		vec->point.row = y;
		vec->point.col = x;
		return;
	}
	// Check Up
	if (IsTopOfShip(field, y, x))
	{
		vec->dir = UP;
		vec->point.row = y;
		vec->point.col = x;
		return;
	}
	// Try to find left
	if (CanItBeLeft(field, y, x))
	{
		vec->dir = LEFT;
		vec->point.row = y;
		vec->point.col = x - GetShiftX(field, y, x);
		return;
	}
	else// Try to find up
	{
		vec->dir = UP;
		vec->point.row = y - GetShiftY(field, y, x);
		vec->point.col = x;
	}
}

bool IsMonoShip(int** field, short y, short x)
{
	if (x == 0)
	{

		if (y == 0)
		{
			return  !IsCellShipOrDestroy(field, y, x + 1) // check righter
				&&
				!IsCellShipOrDestroy(field, y + 1, x); // check botomer
		}
		else if (y < Size - 1)
		{
			return  !IsCellShipOrDestroy(field, y, x + 1) // check righter
				&&
				!IsCellShipOrDestroy(field, y + 1, x) // check botomer
				&&
				!IsCellShipOrDestroy(field, y - 1, x); // check upper
		}
		else
		{
			return	!IsCellShipOrDestroy(field, y, x + 1) // check righter
				&&
				!IsCellShipOrDestroy(field, y - 1, x); // check upper
		}
	}
	else if (x < Size - 1)
	{

		if (y == 0)
		{
			return  !IsCellShipOrDestroy(field, y, x + 1) // check righter
				&&
				!IsCellShipOrDestroy(field, y, x - 1) // check lefter
				&&
				!IsCellShipOrDestroy(field, y + 1, x); // check botommer
		}
		else if (y < Size - 1)
		{
			return	!IsCellShipOrDestroy(field, y, x + 1) // check righter
				&&
				!IsCellShipOrDestroy(field, y, x - 1) // check lefter
				&&
				!IsCellShipOrDestroy(field, y - 1, x) // check upper
				&&
				!IsCellShipOrDestroy(field, y + 1, x); // check botommer
		}
		else
		{
			return	!IsCellShipOrDestroy(field, y, x + 1) // check righter
				&&
				!IsCellShipOrDestroy(field, y, x - 1) // check lefter
				&&
				!IsCellShipOrDestroy(field, y - 1, x); // check upper
		}
	}
	else
	{

		if (y == 0)
		{
			return	!IsCellShipOrDestroy(field, y, x - 1) // check lefter
				&&
				!IsCellShipOrDestroy(field, y + 1, x); // check botommer
		}
		else if (y < Size - 1)
		{
			return	!IsCellShipOrDestroy(field, y, x - 1) // check lefter
				&&
				!IsCellShipOrDestroy(field, y - 1, x) // check upper
				&&
				!IsCellShipOrDestroy(field, y + 1, x); // check botommer
		}
		else
		{
			return	!IsCellShipOrDestroy(field, y, x - 1) // check lefter
				&&
				!IsCellShipOrDestroy(field, y - 1, x); // check upper
		}
	}
}

bool IsLeftOfShip(int** field, short y, short x)
{
	if (x == 0)
	{
		return	IsCellShipOrDestroy(field, y, x + 1); // check righter
	}
	else if (x < Size - 1)
	{
		return	IsCellShipOrDestroy(field, y, x + 1) // check righter
			&& !IsCellShipOrDestroy(field, y, x - 1);// check lefter
	}
	else
	{
		return false;
	}
}

bool IsTopOfShip(int** field, short y, short x)
{
	if (y == 0)
	{
		return	IsCellShipOrDestroy(field, y + 1, x); // check upper
	}
	else if (y < Size - 1)
	{
		return	IsCellShipOrDestroy(field, y + 1, x) // check upper
			&& !IsCellShipOrDestroy(field, y - 1, x);// check downer
	}
	else
	{
		return false;
	}
}

int GetShiftX(int** field, short y, short x)
{
	int i = 1;
	while (true)
	{
		if (IsLeftOfShip(field, y, x - i))
		{
			break;
		}
		i++;
	}
	return i;
}

int GetShiftY(int** field, short y, short x)
{
	int i = 1;
	while (true)
	{
		if (IsTopOfShip(field, y - i, x))
		{
			break;
		}
		i++;
	}
	return i;
}

bool IsCellShipOrDestroy(int** field, short y, short x)
{
	return field[y][x] == SHIP || field[y][x] == DESTROYED;
}

bool CanItBeLeft(int** field, short y, short x)
{
	return IsCellShipOrDestroy(field, y, x - 1);
}

void GetSizeOfShip(int** field, Vector* left_or_up)
{
	size_t len = 0;
	// Left
	if (left_or_up->dir == LEFT)
	{
		while (true)
		{
			len++;
			if (left_or_up->point.col + len == Size || !IsCellShipOrDestroy(field, left_or_up->point.row, left_or_up->point.col + len))
			{
				break;
			}
		}
	}
	// Up
	else
	{
		while (true)
		{
			len++;
			if (left_or_up->point.row + len == Size || !IsCellShipOrDestroy(field, left_or_up->point.row + len, left_or_up->point.col))
			{
				break;
			}
		}
	}
	//
	//write length 
	left_or_up->length = len;
}

void PrintVector(Vector const* v)
{
	SetConsoleCursorPosition(hStdOut, { 0, 25 });
	printf("{y=%d, x=%d} ", v->point.row, v->point.col);
	switch (v->dir)
	{
	case LEFT:
		printf("left");
		break;

	case UP:
		printf("up");
		break;

	case IMPOSSIBLE:
		printf("MONOSHIP");
		break;

	default:
		break;
	}
	printf("\tlen=%d", v->length);
}

void PrintDrowned()
{
	SetConsoleCursorPosition(hStdOut, { 0, 26 });
	printf("\tDRAWNED!!");
}

bool IsShip(int** field, short y, short x)
{
	SetConsoleCursorPosition(hStdOut, { 0, 25 });
	/*printf("{y=%d, x=%d}", y, x);
	Sleep(6500);*/
	return field[y][x] == SHIP || field[y][x] == DESTROYED;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//			Move Player and Enemy Player
void MovePlayer(int** field) // Ход Игрока с проверкой введенных значений
{
	// Read Player
	char UserCh;
	int UserInt;
	char tmp[4];
	scanf_s("%s", tmp, _countof(tmp));
	BatleParser(tmp, &UserCh, &UserInt);
	ReadUser(UserCh, UserInt);
	int cell = field[User.row][User.col];

	if (cell == SHIP || cell == DESTROYED)
	{
		field[User.row][User.col] = DESTROYED;
	}
	else
	{
		field[User.row][User.col] = FAILURE;
	}
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
void PrintRandomTurnEnemy()
{
	SetConsoleCursorPosition(hStdOut, { 0, 26 });
	printf("y=%d x=%d", Enemy.row, Enemy.col);
}

void Init_random_enemy_turns()
{
	for (int i = 0; i < Size; i++)
	{
		for (int j = 0; j < Size; j++)
		{
			random_enemy_turns.push_back({ i, j });
		}
	}
	random_shuffle(random_enemy_turns.begin(), random_enemy_turns.end());
}

bool IsValidCell(int** field, int row, int col)
{
	return field[row][col] != DESTROYED && field[row][col] != FAILURE;
}

void RandNum(int** field)
{
	do
	{
		Enemy.row = rand() % Size;
		Enemy.col = rand() % Size;
	} while (!IsValidCell(field, Enemy.row, Enemy.col));
}

void RandomTurnEnemy(int** field)
{
	int row = random_enemy_turns[0].row;
	int col = random_enemy_turns[0].col;
	if (random_enemy_turns.size() > 0)
		random_enemy_turns.erase(random_enemy_turns.begin());
	if (IsValidCell(field, row, col))
	{
		Enemy.row = row;
		Enemy.col = col;
	}
}



void MoveEnemy(int** field) //  Ход ИИ с генерацией рандомных координат
{
	//RandNum(field);
	// if it didn't achived an ame read from Random Turn
	if (IsSmartOn())
	{
		/*if (!IsNecessaryToAddMore())
		{
			EraseStupidTurns();
		}*/
		SmartTurnEnemy(field);
		//RandomTurnEnemy(field);
	}
	else
	{
		RandomTurnEnemy(field);
	}
	
	// else read from smar
	int cell = field[Enemy.row][Enemy.col];
	// Change state of current cell if it possible
	if (cell == SHIP)
	{
		field[Enemy.row][Enemy.col] = DESTROYED;
	}
	else if (cell != DESTROYED)
	{
		field[Enemy.row][Enemy.col] = FAILURE;
	}
}

void EraseRandomTurnEnemys(int** field)
{
	/*SetConsoleCursorPosition(hStdOut, { 0, 25 });
	printf("{y=%d, x=%d} ", row, col);
	Sleep(2000);*/

	for (int row = 0; row < Size; row++)
	{
		for (int col = 0; col < Size; col++)
		{
			if (field[row][col] == EMPTY)
			{
				Point tmp{ row, col };
				auto it = find_if(random_enemy_turns.begin(), random_enemy_turns.end(),
					[&tmp](Point const& p)
					{
						return p.row == tmp.row && p.col == tmp.col;
					}
				);
				if (it != random_enemy_turns.end())
				{
					random_enemy_turns.erase(it);
					/*SetConsoleCursorPosition(hStdOut, { 0, 25 });
					printf("{Drowned##y=%d, x=%d} ", row, col);
					Sleep(3000);*/
				}
			}
		}
	}
}

void AddNeigboursToSmart(int** field, int row, int col)
{
	// add Achived point to shots
	enemy_shoots.push_back({ row, col });
	if (IsSootReachedLine()) // if we have min two points - basis
	{
		if (IsVertical())
		{
			// if first time lets clean
			if (HasHorizontals())
			{
				RestartSmart();
			}
			// sort by rows
			sort(enemy_shoots.begin(), enemy_shoots.end(), [](Point const& a, Point const& b)
				{
					return a.row < b.row;
				}
			);
			//try add
			//upper
			if (enemy_shoots[0].row - 1 >= 0 && IsUnknown(field, enemy_shoots[0].row - 1, col))
			{
				smart_enemy_turns.push_back({ enemy_shoots[0].row - 1, col });
			}
			//downer
			if (enemy_shoots[enemy_shoots.size() - 1].row + 1 < Size && IsUnknown(field, enemy_shoots[enemy_shoots.size() - 1].row + 1, col))
			{
				smart_enemy_turns.push_back({ enemy_shoots[enemy_shoots.size() - 1].row + 1, col });
			}
		}
		else
		{
			// if first time lets clean
			if (HasVerticals())
			{
				RestartSmart();
				// sort by cols from 0 to Size - 1
			}
			sort(enemy_shoots.begin(), enemy_shoots.end(), [](Point const& a, Point const& b)
				{
					return a.col < b.col;
				}
			);
			//try add
			//lefter
			if (enemy_shoots[0].col - 1 >= 0 && IsUnknown(field, enemy_shoots[0].row, col - 1))
			{
				smart_enemy_turns.push_back({ enemy_shoots[0].row, col - 1 });
			}
			//righter
			if (enemy_shoots[enemy_shoots.size() - 1].row < Size && IsUnknown(field, enemy_shoots[enemy_shoots.size() - 1].row, col + 1))
			{
				smart_enemy_turns.push_back({ enemy_shoots[enemy_shoots.size() - 1].row, col + 1 });
			}
		}
		return;
	}

	//lefter
	if (col - 1 >= 0 && IsUnknown(field, row, col - 1))
	{
		smart_enemy_turns.push_back({ row, col - 1 });
	}
	//righter
	if (col + 1 < Size && IsUnknown(field, row, col + 1))
	{
		smart_enemy_turns.push_back({ row, col + 1 });
	}

	//upper
	if (row - 1 >= 0 && IsUnknown(field, row - 1, col))
	{
		smart_enemy_turns.push_back({ row - 1, col });
	}
	//downer
	if (row + 1 < Size && IsUnknown(field, row + 1, col))
	{
		smart_enemy_turns.push_back({ row + 1, col });
	}
}

bool IsUnknown(int** field, int row, int col)
{
	return field[row][col] != DESTROYED && field[row][col] != FAILURE && field[row][col] != EMPTY;
	//return field[row][col] != DESTROYED && field[row][col] != FAILURE;
}

void SmartTurnEnemy(int** field)
{
	int row = smart_enemy_turns[0].row;
	int col = smart_enemy_turns[0].col;
	if (smart_enemy_turns.size() > 0)
		smart_enemy_turns.erase(smart_enemy_turns.begin());

	Point tmp{ row, col };
	auto it = find_if(random_enemy_turns.begin(), random_enemy_turns.end(),
		[&tmp](Point const& p)
		{
			return p.row == tmp.row && p.col == tmp.col;
		}
	);
	if (it != random_enemy_turns.end())
	{
		random_enemy_turns.erase(it);
	}

	if (IsValidCell(field, row, col))
	{
		Enemy.row = row;
		Enemy.col = col;
	}
}

void RestartSmart()
{
	smart_enemy_turns.clear();
	smart_enemy_turns.shrink_to_fit();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////// 

bool IsSmartOn()
{
	return smart_enemy_turns.size() >= 1;
}

bool IsSootReachedLine()
{
	return enemy_shoots.size() >= 2;
}

bool IsVertical()
{
	return enemy_shoots[0].col - enemy_shoots[1].col == 0;
}

bool IsHorizontal()
{
	return !IsVertical();
}

bool HasVerticals()
{
	auto it = find_if(smart_enemy_turns.begin(), smart_enemy_turns.end(),
		[](auto const& e)
		{
			return enemy_shoots[0].row - e.row != 0;
		}
	);
	return it != smart_enemy_turns.end();
}

bool HasHorizontals()
{
	auto it = find_if(smart_enemy_turns.begin(), smart_enemy_turns.end(), 
		[](auto const& e) 
		{ 
			return enemy_shoots[0].col - e.col != 0;
		}
	);
	return it != smart_enemy_turns.end();
}

void RestartEnemyShoots()
{
	enemy_shoots.clear();
	enemy_shoots.shrink_to_fit();
}

void AddVerticalShoots(int** field, int row, int col)
{

}

void AddHorizontalShoots(int** field, int row, int col)
{
}


