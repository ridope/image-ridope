#ifndef CONV2D_RIDOPE_H
#define CONV2D_RIDOPE_H

#include "activation_functions.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef  void (*activation_func)(float *img_fltr,int rows, int cols, float bias);

typedef struct CONV2D_LAYER_TYPE
{
    uint8_t filters;
    uint8_t kernel_size;
    uint8_t pad_size;

    activation_func act_func;

    float * weights;
    float * bias;

    uint8_t input_rows;
    uint8_t input_cols;
    uint8_t input_channel;
}conv2d_layer_t;

void init_conv2d_layer(conv2d_layer_t *layer, uint8_t filters, uint8_t kernel_size, uint8_t pad_size, activation_func  act_func, float * weights, float * bias, uint8_t input_rows, uint8_t input_cols, uint8_t input_channel);
float * forward_propagation(conv2d_layer_t *layer, float *input_ptr);

#ifdef __cplusplus
}
#endif

#endif

