#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <locale>
#include <algorithm>
#include <vector>

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

struct Vector
{
	Point point;
	int dir;
	size_t length;
} VectorEnemy, VectorPlayer;

////////////////////////////////////////////////////////////////////////////////////////////
vector<Point> vec_enemy_turns;
void Init_vec_enemy_turns();
void RandNum(int** field, int size);	

void PrintEnemyTurn();
////////////////////////////////////////////////////////////////////////////////////////////
void DrawPlayerField(int** arr, int size = 10, short x = 0, short y = 0);
void DrawEnemyField(int** arr, int size = 10, short x = 40, short y = 0);  ///в функции поменять Fog на Ship для отображения кораблей
void MoveEnemy(int** field);
void MovePlayer(int** field);
void ToUpperChar(char* ch);
bool ReadUser(char Uchar, int Uint);
bool IsValidNum(int num);
bool IsValidChar(char ch);
void BatleParser(const char* src, char* ch, int* num);
void DrawMenu(short y, short x = 0);
bool IsValidNum(int** field, int col, int row);
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

	while (IsAlivePlayer() && IsAliveEnemy())
	{
		DrawPlayerField(arrPlayer, Size);
		DrawEnemyField(arrEnemy, Size, Enemy_LEFT);
		DrawMenu(Menu_Y, 0);
		if (turn_player)
		{
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
					PrintDrowned();
					CleanAreaOutOfDrawnedShip(arrEnemy, &VectorEnemy);
					Sleep(6500);
				}

			}
		}

		else
		{
			turn_player = true;
			MoveEnemy(arrPlayer);

			PrintEnemyTurn();
			Sleep(6500);

			if (IsDamage(arrPlayer, Enemy.row, Enemy.col))
			{
				PlayerLife--;
				turn_player = false;
				GetBeginOfShip(arrPlayer, Enemy.row, Enemy.col, &VectorPlayer);
				GetSizeOfShip(arrPlayer, &VectorPlayer);
				
				/*PrintVector(&VectorPlayer);
				Sleep(6500);*/

				//#check is drawned
				if (IsDrowned(arrPlayer, &VectorPlayer))
				{
					//PrintDrowned();
					CleanAreaOutOfDrawnedShip(arrPlayer, &VectorPlayer);
					//Sleep(6500);
				}
			}
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

bool IsValidNum(int** field, int col, int row)
{
	if (field[col][row] == SHIP || field[col][row] == FOG)
	{
		return true;
	}
	return false;
}

void InitField(int** field)
{
	int cnt = 0;
	int dir = -1;
	while (cnt < 10)
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
			if (IsAllowedToSet(field, x, y))
			{
				field[y][x] = SHIP;
				++cnt;
			}
		}
	}
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
void PrintEnemyTurn()
{
	SetConsoleCursorPosition(hStdOut, { 0, 26 });
	printf("y=%d x=%d", Enemy.row, Enemy.col);
}

void Init_vec_enemy_turns()
{
	for (int i = 0; i < Size; i++)
	{
		for (int j = 0; j < Size; j++)
		{
			vec_enemy_turns.push_back({ i, j });
		}
	}
	random_shuffle(vec_enemy_turns.begin(), vec_enemy_turns.end());
}

void RandNum(int** field, int size)
{
	do
	{
		Enemy.row = rand() % size;
		Enemy.col = rand() % size;
	} while (!IsValidNum(field, Enemy.col, Enemy.row));
}

void MoveEnemy(int** field) //  Ход ИИ с генерацией рандомных координат
{
	RandNum(field, Size);
	// get Ffirst elem and erase it

	int cell = field[Enemy.row][Enemy.col];
	// Change state of current cell
	if (cell == SHIP)
	{
		field[Enemy.row][Enemy.col] = DESTROYED;

	}
	else
	{
		field[Enemy.row][Enemy.col] = FAILURE;
	}
}
