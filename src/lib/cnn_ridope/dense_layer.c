#include "dense_layer.h"
#include "math.h"

float * forward_propagation_dense(dense_layer_t *layer, float *input_ptr)
{
    float *output_ptr = (float *)malloc(layer->input_rows * layer->units * sizeof(float));
    float *output_ptr_temp = output_ptr;
    float *kernel = (float *)malloc(layer->input_cols*sizeof(float));
    int cnt_weight = 0;
	
    for (int i = 0; i < layer->units; i++)
    {
        // reading corresponding weights to kernel pointer
        for (int cnt_kernel = 0; cnt_kernel < layer->input_cols; cnt_kernel++)
        {
            *(kernel + cnt_kernel) = layer->weights[cnt_weight + cnt_kernel];
        }
        cnt_weight = cnt_weight + layer->input_cols;

        output_ptr_temp[i] = dot_product(input_ptr, kernel, layer->input_cols) + layer->bias[i];
    }

    free(kernel);

    layer->act_func(output_ptr_temp, layer->input_rows, layer->input_cols, 0);

    return output_ptr; 
}