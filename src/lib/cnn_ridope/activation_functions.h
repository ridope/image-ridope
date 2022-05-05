#ifndef ACTIVATION_FUNC_RIDOPE_H
#define ACTIVATION_FUNC_RIDOPE_H

#include "utils.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef  void (*activation_func)(float *img_fltr, int rows, int cols, float bias);

void relu(float *img_fltr,int rows, int cols, float bias);
void softmax(float *img_fltr,int rows, int cols, float bias);

#ifdef __cplusplus
}
#endif

#endif
