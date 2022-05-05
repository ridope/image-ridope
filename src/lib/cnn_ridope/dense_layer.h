#ifndef DENSE_LAYER_RIDOPE_H
#define DENSE_LAYER_RIDOPE_H

#include "utils.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct DENSE_LAYER_TYPE
{
    uint8_t units;

    activation_func act_func;

    float * weights;
    float * bias;

    uint8_t input_rows;
    uint8_t input_cols;
    uint8_t input_channel;
}dense_layer_t;

void init_dense_layer(dense_layer_t *layer, uint8_t units, activation_func  act_func, float * weights, float * bias, uint8_t input_rows, uint8_t input_cols, uint8_t input_channel);
float * forward_propagation(dense_layer_t *layer, float *input_ptr);

#ifdef __cplusplus
}
#endif

#endif
