#include "conv2d_layer.h"

float * forward_propagation(conv2d_layer_t *layer, float *input_ptr)
{

    float *output_ptr = (float *)malloc(layer->input_rows * layer->input_cols * layer->filters * sizeof(float));
    float *output_ptr_temp = output_ptr;
    float *kernel = (float *)malloc(layer->filters*sizeof(float));
    int cnt_weight = 0;
	
	float bias_tmp;

    for(int ch = 0; ch < layer->input_channel; ch++)
    {
        for (int i = 0; i < layer->filters; i++)
        {
            // reading corresponding weights to kernel pointer
            for (int cnt_kernel = 0; cnt_kernel < layer->filters; cnt_kernel++)
            {
                *(kernel + cnt_kernel) = layer->weights[cnt_weight + cnt_kernel];
            }
            cnt_weight = cnt_weight + layer->filters;

            imfilter(input_ptr, kernel, output_ptr_temp, layer->input_rows, layer->input_cols, layer->pad_size);

            bias_tmp = layer->bias[i];

            layer->act_func(output_ptr_temp, rows, cols, bias_tmp);

            output_ptr_temp = output_ptr_temp + layer->input_rows*layer->input_cols;
        }
    }
	
    return output_ptr; 
}



