#include "activation_functions.h"

void relu(float *img_fltr,int rows, int cols, float bias)
{
	int cnt = 0;
	for (int i = 0; i < rows;i++)
	for (int j = 0; j < cols; j++)
	{
		cnt = i*cols + j;
		*(img_fltr + cnt) = max(*(img_fltr + cnt) + bias, 0);
	}
}