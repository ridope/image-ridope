#ifndef ACTIVATION_FUNC_RIDOPE_H
#define ACTIVATION_FUNC_RIDOPE_H

#include "utils.h"

#ifdef __cplusplus
extern "C" {
#endif

void relu(float *img_fltr,int rows, int cols, float bias);
void softmax(float *img_fltr,int rows, int cols, float bias);

#ifdef __cplusplus
}
#endif

#endif
