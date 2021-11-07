#include <iostream>
#include <stdio.h>

void delete_field()
{
	for (int i = 0; i < size; i++)
	{
		delete[] &arr[i];
	}

	delete[] arr;
}