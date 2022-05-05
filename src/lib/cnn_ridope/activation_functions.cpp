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

void softmax(float *img_fltr,int rows, int cols, float bias)
{
	float * exps = (float *)malloc(cols*sizeof(float));
	
	float m = FLT_MIN;

	for (int i = 0; i < cols; i++) {
		if (img_fltr[i] > m) {
			m = img_fltr[i];
		}
	}

	float sum = 0.0;

	for (int i = 0; i < cols; i++) {
		exps[i] = expf(img_fltr[i] - m);
		sum += exps[i];
	}

	for (int i = 0; i < cols; i++) {
		img_fltr[i] = exps[i]/sum;
	}

	free(exps);
}