#include "field.h"

void field()
{
	int size = 10;
	int** arr = new int* [size];
	for (int i = 0; i < size; i++)
	{
		arr[i] = new int[size];
	}
	for (size_t i = 0; i < size; i++)
	{
		for (size_t j = 0; j < size; j++)
		{
			std::cout << (arr[i][j] = 0) << "\t";
		}
		std::cout << "\n\n";
	}
}