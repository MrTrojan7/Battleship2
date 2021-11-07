//goto 80))
#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <locale>
#include <algorithm>

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
const int Enemy_LEFT = (Field_LEFT + Tale_size_X)* Size ;
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
} User;

struct PointEnemy
{
	int row;
	int col;
} Enemy;

struct Vector
{
	Point point;
	int dir;
} VectorPlayer, VectorEnemy;

struct FreePoints
{
	Point points[14];
	int length;
} FreePointsPlayer, FreePointsEnemy;

//struct FinishIt
//{
//	Point cells[14];
//	int LeftUp;
//}Finish;

void DrawPlayerField(int** arr, int size = 10, short x = 0, short y = 0);

void DrawEnemyField(int** arr, int size = 10, short x = 40, short y = 0);  ///в функции поменять Fog на Ship для отображения кораблей

void MoveEnemy(int** field, int size);

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
////////
bool CheckLeft(int** field, short x, short y, int size);
bool CheckUp(int** field, short x, short y, int size);
bool CheckRight(int** field, short x, short y, int size);
bool CheckDown(int** field, short x, short y, int size);
////////
int RandNum(int** field, int size);

void ShootDirTrue();
void ShootDirFalse();
void CheckDirField(short x, short y);
bool CheckShipField(int** field, short x, short y);
short SumShootDir(bool* arr, short size);
int CheckToFailure(int** field, short x, short y);
int DrawShipsFailure(int** field, short x, short y);
void ShootDamage(int** field, short x, short y);
////////////////////////////////////////////////////////////////////////////////////////////
bool IsDamage(int** field, short x, short y);
bool IsAlivePlayer();
bool IsAliveEnemy();
////////////////////////////////////////////////////////////////////////////////////////////
//bool IsFinish(int** field, short x, short y);
//bool IsUno(int** field, short x, short y);
//bool IsDuo(int** field, short x, short y);
//bool IsTre(int** field, short x, short y);
//bool IsQuadro(int** field, short x, short y);
////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////
bool IsDrowned(int** field, short x, short y);
void GetBeginOfShip(int** field, short y, short x, Vector* vec);
size_t GetSizeOfShip(int** field);
void SetFreepoints(int** field, int length, FreePoints* fp);

void PrintVector(Vector const* v);
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
				PrintVector(&VectorEnemy);
				Sleep(6500);
			}
		}

		else
		{
			turn_player = true;
			MoveEnemy(arrPlayer, Size);
			if (IsDamage(arrPlayer, Enemy.row, Enemy.col))
			{
				PlayerLife--;
				turn_player = false;
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
int RandNum(int** field, int size)
{
	do
	{
		Enemy.row = rand() % size;
		Enemy.col = rand() % size;
	} while (!IsValidNum(field, Enemy.col, Enemy.row));
	return 0;
}

void MoveEnemy(int** field, int size) //  Ход ИИ с генерацией рандомных координат
{
	/*if (TrueShip)
	{
		if (SumShootDir(ShootDir, SizeShoot))
		{
			CheckToFailure(field, Enemy.row, Enemy.col);
			CheckDirField(Enemy.row, Enemy.col);
			ShootDamage(field, Enemy.row, Enemy.col);
		}
		else
		{
			TrueShip = false;
		}

	}*/
	RandNum(field, size);
	

	int cell = field[Enemy.row][Enemy.col];

	if (cell == SHIP)
	{
		field[Enemy.row][Enemy.col] = DESTROYED;
		/*Damage = true;
		if (!TrueShip)
		{
			if (!CheckShipField(field, Enemy.row, Enemy.col))
			{
				TrueShip = false;
				DrawShipsFailure(field, Enemy.col, Enemy.row);
				ShootDirFalse();
			}
		}*/
	}
	else
	{
		field[Enemy.row][Enemy.col] = FAILURE;
		//Damage = false;
	}
}

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
	/*cout << "OK!\n";*/
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
	//cout << "IsvalidNum " << num << endl;
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
			if(CheckDown(field, x, y, size))
			{
				results[head++] = DOWN;
			}
		}
		else if (x == (Size - 1))
		{
			//check to left
			if(CheckLeft(field, x, y, size))
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



///////
// "shoot on wound" functions
///////

short SumShootDir(bool* arr, short size)
{
    if (size > 0)
        return arr[size - 1] + SumShootDir(arr, size - 1);
    else
        return 0;
}

int CheckToFailure(int** field, short x, short y)
{
	if ((x - 1) >= 0)
	{
		if ((field[y][x - 1]) == FAILURE)
		{
			ShootDir[LEFT] = false;
		}
	}
	if ((y - 1) >= 0)
	{
		if ((field[y - 1][x]) == FAILURE)
		{
			ShootDir[UP] = false;
		}
	}
	if ((x + 1) < Size)
	{
		if ((field[y][x + 1]) == FAILURE)
		{
			ShootDir[RIGHT] = false;
		}
	}
	if ((y + 1) < Size)
	{
		if ((field[y][x - 1]) == FAILURE)
		{
			ShootDir[DOWN] = false;
		}
	}
	return 0;
}

void ShootDirFalse()
{
	for (size_t i = 0; i < SizeShoot; i++)
	{
		ShootDir[i] = false;
	}
}

void CheckDirField(short x, short y)
{
	if (x == 0)
	{
		ShootDir[LEFT] = false;
	}
	if (x == (Size - 1))
	{
		ShootDir[RIGHT] = false;
	}
	if (y == 0)
	{
		ShootDir[UP] = false;
	}
	if (y == (Size - 1))
	{
		ShootDir[DOWN] = false;
	}
}

bool CheckShipField(int** field, short x, short y)
{
	if (field[y][x] == SHIP)
	{
		switch (DrawShips % 4)
		{

		case UP:
		{
			if ((y - 1) >= 0)
			{
				if ((field[y - 1][x]) == SHIP)
				{
					return true;
				}
				if ((y - 2) >= 0)
				{
					if ((field[y - 2][x]) == SHIP && (field[y - 1][x]) == DESTROYED)
					{
						return true;
					}
				}
				if ((y - 3) >= 0)
				{
					if ((field[y - 3][x]) == SHIP && (field[y - 2][x]) == DESTROYED && (field[y - 1][x]) == DESTROYED)
					{
						return true;
					}
				}
				else
				{
					DrawShips++;
					return CheckShipField(field, x, y);
				}
			}
			else
			{
				DrawShips++;
				return CheckShipField(field, x, y);
			}
		}
		case RIGHT:
		{
			if ((x + 1) < Size)
			{
				if ((field[y][x + 1]) == SHIP)
				{
					return true;
				}
				if ((x + 2) < Size)
				{
					if ((field[y][x + 2]) == SHIP && (field[y][x + 1]) == DESTROYED)
					{
						return true;
					}
				}
				if ((x + 3) < Size)
				{
					if ((field[y][x + 3]) == SHIP && (field[y][x + 2]) == DESTROYED && (field[y][x + 1]) == DESTROYED)
					{
						return true;
					}
				}
				else
				{
					DrawShips++;
					return CheckShipField(field, x, y);
				}
			}
			else
			{
				DrawShips++;
				return CheckShipField(field, x, y);
			}
		}
		case DOWN:
		{
			if ((y + 1) < Size)
			{
				if ((field[y + 1][x]) == SHIP)
				{
					return true;
				}
				if ((y + 2) < Size)
				{
					if ((field[y + 2][x]) == SHIP && (field[y + 1][x]) == DESTROYED)
					{
						return true;
					}
				}
				if ((y + 3) < Size)
				{
					if ((field[y + 3][x]) == SHIP && (field[y + 2][x]) == DESTROYED && (field[y + 1][x]) == DESTROYED)
					{
						return true;
					}
				}
				else
				{
					DrawShips++;
					return CheckShipField(field, x, y);
				}
			}
			else
			{
				DrawShips++;
				return CheckShipField(field, x, y);
			}
		}
		case LEFT:
		{
			if ((x - 1) >= 0)
			{
				if ((field[y][x - 1]) == SHIP)
				{
					return true;
				}
				if ((x - 2) >= 0)
				{
					if ((field[y][x - 2]) == SHIP && (field[y][x - 1]) == DESTROYED)
					{
						return true;
					}
				}
				if ((x - 3) >= 0)
				{
					if ((field[y][x - 3]) == SHIP && (field[y][x - 2]) == DESTROYED && (field[y][x - 1]) == DESTROYED)
					{
						return true;
					}
				}
				else
				{
					DrawShips = 1;
					ShootDirFalse();
					return false;
				}
			}
			else
			{
				DrawShips = 1;
				ShootDirFalse();
				return false;
			}
		}
		return false;
		ShootDirFalse();
		}
	}
	else
	{
		return false;
	}
}

void ShootDirTrue()
{
	for (size_t i = 0; i < SizeShoot; i++)
	{
		ShootDir[i] = true;
	}
}

int DrawShipsFailure(int** field, short x, short y)
{
	switch (DrawShips % 4)
	{

	case UP:
	{
		if ((y - 1) >= 0)
		{
			if ((field[y - 1][x]) == DESTROYED)
			{
				if ((x - 1) >= 0)
				{
					field[y - 1][x - 1] = FAILURE;
					/*field[y][x - 1] = FAILURE;*/
					return DrawShipsFailure(field, x, y - 1);
				}
				else
				{
					return DrawShipsFailure(field, x, y - 1);
				}
			}
			else if ((field[y - 1][x]) == FOG || (field[y - 1][x]) == FAILURE)
			{
				if ((x - 1) >= 0)
				{
					field[y - 1][x - 1] = FAILURE;
					
				}
				field[y - 1][x] = FAILURE;
				if ((x + 1) < Size)
				{
					field[y - 1][x + 1] = FAILURE;
				}
				DrawShips++;
				return DrawShipsFailure(field, x, y);
			}
		}
		else
		{
			DrawShips++;
			return DrawShipsFailure(field, x, y);
		}
	}
	case RIGHT:
	{
		if ((x + 1) < Size)
		{
			if ((field[y][x + 1]) == DESTROYED)
			{
				if ((y - 1) >= 0)
				{
					field[y - 1][x + 1] = FAILURE;
					field[y - 1][x] = FAILURE;
					DrawShips++;
					return DrawShipsFailure(field, x + 1, y);
				}
				else
				{
					DrawShips++;
					return DrawShipsFailure(field, x + 1, y);
				}
			}
			else if ((field[y][x + 1]) == FOG || (field[y][x + 1]) == FAILURE)
			{
				if ((y - 1) >= 0)
				{
					field[y - 1][x + 1] = FAILURE;
									
				}
				field[y][x + 1] = FAILURE;
				if ((y + 1) < Size)
				{
					field[y + 1][x + 1] = FAILURE;
				}
				DrawShips++;
				return DrawShipsFailure(field, x, y);
			}
		}
		else
		{
			DrawShips++;
			return DrawShipsFailure(field, x, y);
		}
	}
	case DOWN:
	{
		if ((y + 1) < Size)
		{
			if ((field[y + 1][x]) == DESTROYED)
			{
				if ((x + 1) < Size)
				{
					field[y + 1][x + 1] = FAILURE;
					field[y][x + 1] = FAILURE;
					DrawShips++;
					return DrawShipsFailure(field, x, y + 1);
				}
				else
				{
					DrawShips++;
					return DrawShipsFailure(field, x, y + 1);
				}
			}
			else if ((field[y + 1][x]) == FOG || (field[y + 1][x]) == FAILURE)
			{
				if ((x + 1) < Size)
				{
					field[y + 1][x + 1] = FAILURE;
										
				}
				field[y + 1][x] = FAILURE;
				if ((x - 1) >= 0)
				{
					field[y + 1][x - 1] = FAILURE;
				}
				DrawShips++;
				return DrawShipsFailure(field, x, y);
			}
		}
		else
		{
			DrawShips++;
			return DrawShipsFailure(field, x, y);
		}
	}
	case LEFT:
	{
		if ((x - 1) >= 0)
		{
			if ((field[y][x - 1]) == DESTROYED)
			{
				if ((y - 1) >= 0)
				{
					field[y - 1][x - 1] = FAILURE;
					field[y + 1][x - 1] = FAILURE;
					DrawShips++;
					return DrawShipsFailure(field, x - 1, y);
				}
				else
				{
					DrawShips++;
					return DrawShipsFailure(field, x - 1, y);
				}
			}
			else if ((field[y][x - 1]) == FOG || (field[y][x - 1]) == FAILURE)
			{
				if ((y - 1) >= 0)
				{
					field[y - 1][x - 1] = FAILURE;
					
				}
				field[y][x - 1] = FAILURE;
				if ((y + 1) < Size)
				{
					field[y + 1][x - 1] = FAILURE;
				}
				DrawShips = 1;
				return 0;
			}
		}
		else
		{
			DrawShips = 1;
			return 0;
		}
	}

	}
}

void ShootDamage(int** field, short x, short y)
{
	short d;
	do
	{
		d = rand() % SizeShoot;

	} while (ShootDir[d]);

	if (d == LEFT)
	{
		Enemy.col--;
	}
	if (d == UP)
	{
		Enemy.row--;
	}
	if (d == RIGHT)
	{
		Enemy.col++;
	}
	if (d == DOWN)
	{
		Enemy.row++;
	}
}

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

void GetBeginOfShip(int** field, short y, short x, Vector* vec)
{
	if (x == 0 && y == 0)
	{
		// Check to right
		if (field[y][x + 1] == SHIP || field[y][x + 1] == DESTROYED)
		{
			// Get left tip
			vec->dir = RIGHT;
			vec->point.row = y;
			vec->point.col = x;
			return;
		}
		// Check to down
		else if (field[y - 1][x] == SHIP || field[y - 1][x] == DESTROYED)
		{
			// Get Top tip
			for (size_t i = 2; i < 4; i++)
			{
				if (field[y - i][x] != SHIP && field[y - i][x] != DESTROYED)
				{
					vec->dir = DOWN;
					vec->point.row = y - i + 1;
					vec->point.col = x;
					return;
					break;
				}
			}
		}
		// MonoShip!!
		else
		{
			vec->dir = IMPOSSIBLE;
			vec->point.row = y;
			vec->point.col = x;
		}
	}
	
	if (x == 0 && y == Size - 1)
	{
		// Check to right
		if (field[y][x + 1] == SHIP || field[y][x + 1] == DESTROYED)
		{
			// Get left tip
			vec->dir = RIGHT;
			vec->point.row = y;
			vec->point.col = x;
			return;
		}
		// Check to Up
		else if (field[y - 1][x] == SHIP || field[y - 1][x] == DESTROYED)
		{
			// Get Top tip
			vec->dir = DOWN;
			vec->point.row = y;
			vec->point.col = x;
			return;
		}
		// MonoShip!!
		else
		{
			vec->dir = IMPOSSIBLE;
			vec->point.row = y;
			vec->point.col = x;
		}
	}

	if (x == Size - 1 && y == 0)
	{
		// Check to left
		if (field[y][x - 1] == SHIP || field[y][x - 1] == DESTROYED)
		{
			// Get left tip
			for (size_t i = 2; i < 4; i++)
			{
				if (field[y][x - i] != SHIP && field[y][x - i] != DESTROYED)
				{
					vec->dir = DOWN;
					vec->point.row = y;
					vec->point.col = x - i + 1;
					return;
					break;
				}
			}
		}
		// Check to Up
		else if (field[y + 1][x] == SHIP || field[y + 1][x] == DESTROYED)
		{
			// Get Top tip
			vec->dir = DOWN;
			vec->point.row = y;
			vec->point.col = x;
			return;
		}
		// MonoShip!!
		else
		{
			vec->dir = IMPOSSIBLE;
			vec->point.row = y;
			vec->point.col = x;
		}
	}

	if (x == Size - 1 && y == Size - 1)
	{
		// Check to left
		if (field[y][x - 1] == SHIP || field[y][x - 1] == DESTROYED)
		{
			// Get left tip
			for (size_t i = 2; i < 4; i++)
			{
				if (field[y][x - i] != SHIP && field[y][x - i] != DESTROYED)
				{
					vec->dir = DOWN;
					vec->point.row = y;
					vec->point.col = x - i + 1;
					return;
					break;
				}
			}
		}
		// Check to Up
		else if (field[y + 1][x] == SHIP || field[y + 1][x] == DESTROYED)
		{
			// Get Top tip
			for (size_t i = 2; i < 4; i++)
			{
				if (field[y - i][x] != SHIP && field[y - i][x] != DESTROYED)
				{
					vec->dir = DOWN;
					vec->point.row = y - i + 1;
					vec->point.col = x;
					return;
					break;
				}
			}
			return;
		}
		// MonoShip!!
		else
		{ 
			vec->dir = IMPOSSIBLE;
			vec->point.row = y;
			vec->point.col = x;
		}
	}

	if (y == 0)
	{
		// Check to left
		if (field[y][x - 1] == SHIP || field[y][x - 1] == DESTROYED)
		{
			// Get left tip
			for (size_t i = 2; i < 4 ; i++)
			{
				if (x - i >= 0 && field[y][x - i] != SHIP && field[y][x - i] != DESTROYED)
				{
					vec->dir = LEFT;
					vec->point.row = y;
					vec->point.col = x - i + 1;
					return;
					break;
				}
			}
		}
		else if (field[y][x + 1] != SHIP && field[y][x + 1] != DESTROYED)
		{
			// MonoShip!!
			if (field[y + 1][x] != SHIP && field[y + 1][x] != DESTROYED)
			{
				vec->dir = IMPOSSIBLE;
				vec->point.row = y;
				vec->point.col = x;
				return;
			}
			// Get Top tip
			vec->dir = DOWN;
			vec->point.row = y;
			vec->point.col = x;
			return;
		}
		else
		{
			vec->dir = LEFT;
			vec->point.row = y;
			vec->point.col = x;
		}
	}

	if (y == Size - 1)
	{
		// Check to left
		if (field[y][x - 1] == SHIP || field[y][x - 1] == DESTROYED)
		{
			// Get left tip
			for (size_t i = 2; i < 4; i++)
			{
				if (x - i >= 0 && field[y][x - i] != SHIP && field[y][x - i] != DESTROYED)
				{
					vec->dir = LEFT;
					vec->point.row = y;
					vec->point.col = x - i + 1;
					return;
					break;
				}
			}
		}
		else if (field[y][x + 1] != SHIP && field[y][x + 1] != DESTROYED)
		{
			// MonoShip!!
			if (field[y - 1][x] != SHIP && field[y - 1][x] != DESTROYED)
			{
				vec->dir = IMPOSSIBLE;
				vec->point.row = y;
				vec->point.col = x;
				return;
			}
			// Get Top tip
			for (size_t i = 2; i < 4; i++)
			{
				if (field[y - i][x] != SHIP && field[y - i][x] != DESTROYED)
				{
					vec->dir = LEFT;
					vec->point.row = y - i + 1;
					vec->point.col = x;
					return;
					break;
				}
			}
			return;
		}
	}

	if (x == 0)
	{
		if (field[y][x + 1] == SHIP || field[y][x + 1] == DESTROYED)
		{
			vec->dir = LEFT;
			vec->point.row = y;
			vec->point.col = x;
			return;
		}
		else if (field[y - 1][x] != SHIP && field[y - 1][x] != DESTROYED)
		{
			// MonoShip
			if (field[y + 1][x] != SHIP && field[y + 1][x] != DESTROYED)
			{
				vec->dir = IMPOSSIBLE;
				vec->point.row = y;
				vec->point.col = x;
				return;
			}

		}
		else
		{
			// Move to Up
			for (size_t i = 2; i < 4; i++)
			{
				if (field[y - i][x] != SHIP && field[y - i][x] != DESTROYED)
				{
					vec->dir = UP;
					vec->point.row = y - i + 1;
					vec->point.col = x;
					return;
					break;
				}
			}
		}
	}

	if (x == Size - 1)
	{
		// go to Left
		if (field[y][x - 1] == SHIP || field[y][x - 1] == DESTROYED)
		{
			for (size_t i = 2; i < 4; i++)
			{
				if (field[y][x - i] != SHIP && field[y][x - i] != DESTROYED)
				{
					vec->dir = LEFT;
					vec->point.row = y;
					vec->point.col = x - i + 1;
					return;
					break;
				}
			}
		}
		else if (field[y - 1][x] != SHIP && field[y - 1][x] != DESTROYED)
		{
			// MonoShip
			if (field[y + 1][x] != SHIP && field[y + 1][x] != DESTROYED)
			{
				vec->dir = IMPOSSIBLE;
				vec->point.row = y;
				vec->point.col = x;
				return;
			}
			vec->dir = UP;
			vec->point.row = y;
			vec->point.col = x;
			return;
		}
		else
		{
			for (size_t i = 2; i < 4; i++)
			{
				if (field[y - i][x] != SHIP && field[y - i][x] != DESTROYED)
				{
					vec->dir = UP;
					vec->point.row = y - i + 1;
					vec->point.col = x;
					return;
					break;
				}
			}
		}
	}

	//Isn't border
	// check lefter
	if (field[y][x - 1] != SHIP && field[y][x - 1] != DESTROYED)
	{
		// check righter
		// bad situation it's have to check vertical
		if (field[y][x + 1] != SHIP && field[y][x + 1] != DESTROYED)
		{
			//Vertical
			if (field[y - 1][x] != SHIP && field[y - 1][x] != DESTROYED)
			{
				//MonoShip
				if (field[y + 1][x] != SHIP && field[y + 1][x] != DESTROYED)
				{
					vec->dir = IMPOSSIBLE;
					vec->point.row = y;
					vec->point.col = x;
					return;
				}
				vec->dir = UP;
				vec->point.row = y;
				vec->point.col = x;
				return;
			}
			else // go to UP
			{
				for (size_t i = 2; i < 4; i++)
				{
					if (y - i >= 0 && field[y - i][x] != SHIP && field[y - i][x] != DESTROYED)
					{
						vec->dir = UP;
						vec->point.row = y - i + 1;
						vec->point.col = x;
						return;
						break;
					}
				}
			}
		}
		// horizontal!!
		else
		{
			vec->dir = LEFT;
			vec->point.row = y;
			vec->point.col = x;
			return;
		}
	}
	else // we have to move to lefter
	{
		for (size_t i = 2; i < 4; i++)
		{
			if (x - i >= 0 && field[y][x - i] != SHIP && field[y][x - i] != DESTROYED)
			{
				vec->dir = LEFT;
				vec->point.row = y;
				vec->point.col = x - i + 1;
				return;
				break;
			}
		}
	}
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
}

//bool IsFinish(int** field, short x, short y)
//{
//	if (IsUno(field, x, y))
//	{
//		return true;
//	}
//	else if(IsDuo(field, x, y))
//	{
//		return true;
//	}
//	else if (IsTre(field, x, y))
//	{
//		return true;
//	}
//	else if (IsQuadro(field, x, y))
//	{
//		return true;
//	}
//	return false;
//}
//
//bool IsUno(int** field, short x, short y)
//{
//
//	return false;
//}
//
//bool IsDuo(int** field, short x, short y)
//{
//	return false;
//}
//
//bool IsTre(int** field, short x, short y)
//{
//	return false;
//}
//
//bool IsQuadro(int** field, short x, short y)
//{
//	return false;
//}
