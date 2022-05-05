#ifndef UTILS_RIDOPE_H
#define UTILS_RIDOPE_H

#include "stdint.h"
#include "math.h"
#include "float.h"
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif


void imfilter(float *img, float *kernel, float *img_fltr, int rows, int cols, int padsize);
float dot_product(float *v, float *u, int size);
float max(float a, float b);
void pad_image(float *img, float *img_pad, int rows, int cols, int padsize);


#ifdef __cplusplus
}
#endif

#endif

