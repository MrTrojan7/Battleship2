#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <math.h>
#include <time.h>
#include <Windows.h>
#include <iomanip>
#include <string>
#include <stdio.h>
#include <dos.h>

using namespace std;

const int SIZEf = 10;
int field[SIZEf][SIZEf] = {};

HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

char s1[256];

void GotoXY(int X, int Y)
{
    COORD coord = { X, Y };
    SetConsoleCursorPosition(hStdOut, coord);
}

void menu()
{
    int f = 1, k, code;

    cout << "\n\n"; // * ������
    for (int x = 0; x < 13; x++)
    {
        for (int y = 0; y < 13; y++)
        {
            if (x == 0 || x == y && y < 13 / 2 || x + y == 13 - 1 && y > 13 / 2 - 1)
            {
                cout << "    \t* ";
            }
            else
            {
                cout << "    \t  ";
            }
        }
        cout << endl;
    } // *

    GotoXY(27, 12);
    cout << "\t\t\t--> ����� ���� <--";
    GotoXY(30, 13);
    cout << "\t\t\t       �����    ";
    cout << "\n\n\t\t�������� ��� � ���� ��� ���� ����������, � ������� ������ �� ������� �������� ���������� �� ����������� �� ����� ���������. ���� � ��������� �� ���� ����������� ������� ������� (���������� ������), �� ������� ��� ��� ����� ���������, � �������� �������� ����� ������� ��� ���� ���";
    do
    {
        k = 0;
        code = _getch();
        if (code == 224 || code == 0)
        {
            code = _getch();
        }

        if (code == 80)
        {
            f = f * (-1);
            if (f == 1)
            {
                GotoXY(27, 12);
                cout << "\t\t\t--> ����� ���� <--";
                GotoXY(30, 13);
                cout << "\t\t\t       �����    ";
            }
            else if (f == 2)
            {
                GotoXY(27, 12);
                cout << "\t\t\t    ����� ����    ";
                GotoXY(30, 13);
                cout << "\t\t\t       �����    ";
            }
            else
            {
                GotoXY(27, 12);
                cout << "\t\t\t    ����� ����    ";
                GotoXY(30, 13);
                cout << "\t\t\t   --> ����� <--";
            }
        }
        else if (code == 72)
        {
            f = f * (-1);
            if (f == 1)
            {
                GotoXY(27, 12);
                cout << "\t\t\t--> ����� ���� <--";
                GotoXY(30, 13);
                cout << "\t\t\t       �����    ";
            }
            else if (f == 2)
            {
                GotoXY(27, 12);
                cout << "\t\t\t    ����� ����    ";
                GotoXY(30, 13);
                cout << "\t\t\t       �����    ";
            }
            else
            {
                GotoXY(27, 12);
                cout << "\t\t\t    ����� ����    ";
                GotoXY(30, 13);
                cout << "\t\t\t   --> ����� <--";
            }
        }
        else if (code == 13)
        {
            k = 1;
        }
        cout << endl << endl << endl << endl << endl;
    }

    while (k == 0);
    {
        if (f != 1)
        {
            exit(0);
        }
    }
    system("cls");
    cout << endl << endl;
}

void figure1()
{
    system("cls");
    for (int x = 0; x < 15; x++) // * ������� �������
    {
        for (int y = 0; y < 15; y++)
        {
            if (x == 0 || x == 15 - 1 || y == 0 || y == 15 - 1)
            {
                cout << "    \t* ";
            }
            else
            {
                cout << "    \t ";
            }
            if (x == 6) // ** ���������� ����, ����� �������� ����� ������ ��������
            {
                cout << "      \t\t\t\t\t";
                string h = "������� ���!";
                for (int i = 0; i < h.length(); i++)
                {
                    Sleep(60);
                    cout << h[i];
                }
                break;
            } // **
        }
        cout << endl; // ���� ����� ��������� ������ ��� ��������
    }
    Sleep(1500); // ����� // *
    cout << "\a"; // ���� ������������ ����
}

void reminder1()
{
    system("cls");
    cout << "\n\n\n\n\n\n\n\n\n\t\t\t\t\t    �����������:\n";
    cout << "\t\t\t    ������� ������� ������, ����� ����� �� ����\n\t\t\t       ��� ���� ���������� �� �����������!";
    cout << "\n\t\t\t\t";
    string h = "................................."; // ** ������������� �������� � ������� �������� �������� ����������������� �������
    for (int i = 0; i < h.length(); i++)
    {
        Sleep(50);
        cout << h[i];
    } // **
    cout << "\n\n";
    Sleep(1000);
    system("cls");
}

void win()
{
    system("cls");
    for (int x = 0; x <= 2; x++)
    {
        if (x <= 1)
        {
            system("Color 05");
            Sleep(200);
            cout << "\n\n\n\n\n\n\n\t\t\t\t\t\t\t�";
            system("Color 15");
            Sleep(200);
            cout << " �";
            system("Color 25");
            Sleep(200);
            cout << " �";
            system("Color 35");
            Sleep(200);
            cout << " �";
            system("Color 48");
            Sleep(200);
            cout << " �";
            system("Color 65");
            Sleep(200);
            cout << " �";
            system("Color 85");
            Sleep(200);
            cout << " �";
            system("Color 93");
            Sleep(200);
            cout << " �";
            system("Color A5");
            Sleep(200);
            cout << " �";
            system("Color B5");
            Sleep(200);
            cout << " �";
            system("Color C0");
            Sleep(200);
            cout << " �";
            system("Color E5");
            Sleep(200);
            cout << "\n\n\t\t\t\t\t\t\t\t�";
            system("Color F5");
            Sleep(200);
            cout << " �";
            system("Color 75");
            Sleep(200);
            cout << " �";
            Sleep(200);
        }
        else
        {
            system("cls");
            for (int x = 0; x < 15; x++) // * ������� �������
            {
                for (int y = 0; y < 15; y++)
                {
                    if (x == 0 || x == 15 - 1 || y == 0 || y == 15 - 1)
                    {
                        cout << "    \t* ";
                    }
                    else
                    {
                        cout << "    \t ";
                    }
                    if (x == 6) // ** ���������� ����, ����� �������� ����� ������ ��������
                    {
                        cout << "      \t\t\t\t\t";
                        string h = "�� ��������!";
                        for (int i = 0; i < h.length(); i++)
                        {
                            Sleep(60);
                            cout << h[i];
                        }
                        break;
                    } // **
                }
                cout << endl; // ���� ����� ��������� ������ ��� ��������
            } // *
        }
    }
}

void ad() // ��������� ������� 1
{
    system("cls");
    for (int x = 0; x < 15; x++) // * ������� �������
    {
        for (int y = 0; y < 15; y++)
        {
            if (x == 0 || x == 15 - 1 || y == 0 || y == 15 - 1)
            {
                cout << "    \t* ";
            }
            else
            {
                cout << "    \t ";
            }
            if (x == 6) // ** ���������� ����, ����� �������� ����� ������ ��������
            {
                cout << "      \t\t\t\t��������� ��������� ����������:\n\n      \t\t\t\t             ";
                string h = "������ �� ������! ���� ������ �����!\n\n      \t\t\t\t             ������� ��������! ���� ������ �����!\n\n      \t\t\t\t             ������� �������! ���� ������ �����!\n\n      \t\t\t\t             �������� ���������! ���� ������ �����!\n\n      \t\t\t\t               ��������� �� ������ ������ ������\n\n      \t\t\t\t                     ����� ��� �����...\n\n";
                for (int i = 0; i < h.length(); i++)
                {
                    Sleep(40);
                    cout << h[i];
                }
                break;
            } // **
        }
        cout << endl; // ���� ����� ��������� ������ ��� ��������
    } //
    Sleep(3300);
    system("cls");
}

void tableShow()
{
    system("cls");
    cout << endl << endl << "\t\t\t\t";
    int counter = 0;
    for (int i = 0; i < SIZEf; i++)
    {
        counter++;
        cout << counter;
        cout << "\t";
    }
    cout << "\n\t\t\t\t==========================================================================";

    counter = 0;

    cout << endl << endl << "\t\t";
    for (int i = 0; i < SIZEf; i++)
    {
        for (int j = 0; j < SIZEf; j++)
        {
            if (i >= 0 && j == 0)
            {
                counter++;
                cout << "\t" << counter << " |";
            }
            cout << "\t" << field[i][j];
        }
        cout << endl << endl << endl << "\t\t";
    }
}

int main()
{
    setlocale(LC_ALL, "Rus");
    srand(unsigned(time(0)));
    system("Color 75");

    menu();
    figure1();
    reminder1();

    int x, y;

    int enemyShipCounter = 0; // * ������������ � ���������� ��������� ��������
    while (true)
    {
        x = rand() % SIZEf;
        if (x <= 4)
        {
            y = rand() % SIZEf;
            field[x][y] = 1;
            enemyShipCounter++;
            if (enemyShipCounter == 10)
            {
                break;
            }
        }
    } // *


    int myShipCounter = 0; // * ������������ � ���������� ���� ��������
    while (true)
    {
        x = rand() % SIZEf;
        if (x >= 5)
        {
            y = rand() % SIZEf;
            field[x][y] = 1;
            myShipCounter++;
            if (myShipCounter == 10)
            {
                break;
            }
        }
    } // *

    tableShow();

    bool isMyRound = false;
    while (true)
    {
        cout << "\t\t������� ����������: ";
        do
        {
            cin >> x;
            cin >> y;
            if (x >= 6 || x <= 0 || y <= 0)
            {
                cout << "\n\t\t\t\t������� ���������� ������� ���� 6 ������ (6, 5, 4, 3, 2, 1), ����� �������� �� ���������� ����!\n\n";
                cout << "\t\t\t\t������� ����������: ";
            }
        } while (x >= 6 || x <= 0 || y <= 0);
        {
            x--;
            y--;
        }

        if (field[x][y] == 1)
        {
            cout << "\n\t\t\t\t�� ������ �� ���������� �������!\a" << endl;
            field[x][y] = 5;
            Sleep(3300);

            tableShow();

            field[x][y] = 0;

            bool ship_detect = false;
            for (int i = 0; i < SIZEf - 5; i++)
            {
                for (int j = 0; j < SIZEf; j++)
                {
                    if (field[i][j] == 1)
                    {
                        ship_detect = true;
                        break;
                    }
                }
                if (ship_detect == true)
                {
                    break;
                }
            }
            if (ship_detect == false)
            {
                cout << "\t\t�� ��������!!! ��� ������� ��������� ���������!" << endl;
                Sleep(4000);
                win();
                Sleep(2000);
                ad();
                break;
            }
            isMyRound = true;
        }
        else
        {
            cout << "\n\t\t\t\t�� ������������!" << endl;
            field[x][y] = 0;
            isMyRound = false;
            Sleep(3400);
            tableShow();
        }

        while (isMyRound == false) // * ��� ����
        {
            cout << "\t\t��� ����...\n\n";
            x = rand() % 5 + 6;
            y = rand() % 10 + 1;
            cout << "\t\t\t\t��� ���� ����������: x[" << x-- << "] � y[" << y-- << "]\n";

            if (field[x][y] == 1)
            {
                cout << "\n\t\t\t\t��� ����� �� ������ �������!\a" << endl;
                field[x][y] = 5;

                Sleep(5500);

                tableShow();

                field[x][y] = 0;

                bool ship_detect = false;
                for (int i = 6; i < SIZEf; i++)
                {
                    for (int j = 0; j < SIZEf; j++)
                    {
                        if (field[i][j] == 1)
                        {
                            ship_detect = true;
                            break;
                        }
                    }
                    if (ship_detect == true)
                    {
                        break;
                    }
                }
                if (ship_detect == false)
                {
                    cout << "\t\t\t\t��� �������!!! ��� ���� ������� ���������!" << endl;
                    Sleep(5500);
                    ad();
                    break;
                }
            }
            else
            {
                cout << "\n\t\t\t\t��� �����������! ���� �������..." << endl;
                isMyRound = true;
                field[x][y] = 0;
                Sleep(5500);
                tableShow();
            }
        } // *
    }

    cout << endl << endl << endl;

    system("pause");
    return 0;
}