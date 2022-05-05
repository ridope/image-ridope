#ifndef MAX_POOL_RIDOPE_H
#define MAX_POOL_RIDOPE_H

#include "activation_functions.h"
#include "math.h"
#include "stdint.h"
#include "float.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef  void (*activation_func)(float *img_fltr,int rows, int cols, float bias);

typedef struct MAX_POOL_LAYER_TYPE
{
    uint8_t pool_size;
    uint8_t pad_size;
    uint8_t stride;

    uint8_t input_rows;
    uint8_t input_cols;
    uint8_t input_channel;
}max_pool_layer_t;

void init_max_pool_layer(max_pool_layer_t *layer, uint8_t pool_size, uint8_t pad_size, uint8_t stride, uint8_t input_rows, uint8_t input_cols, uint8_t input_channel);
float * get_max_pool(max_pool_layer_t *layer, float *input_ptr);

#ifdef __cplusplus
}
#endif

#endif

