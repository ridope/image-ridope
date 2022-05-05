#include "conv2d_layer.h"

void init_conv2d_layer(conv2d_layer_t *layer, uint8_t filters, uint8_t kernel_size, uint8_t pad_size, activation_func  act_func, float * weights, float * bias, uint8_t input_rows, uint8_t input_cols, uint8_t input_channel)
{
    layer->filters = filters;
    layer->kernel_size = kernel_size;
    layer->pad_size = pad_size;
    layer->act_func = act_func;
    layer->weights = weights;
    layer->bias = bias;
    layer->input_rows = input_rows;
    layer->input_cols = input_cols;
    layer->input_channel = input_channel;
}


float * forward_propagation_conv2d(conv2d_layer_t *layer, float *input_ptr)
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

            layer->act_func(output_ptr_temp, layer->input_rows, layer->input_cols, bias_tmp);

            output_ptr_temp = output_ptr_temp + layer->input_rows*layer->input_cols;
        }
    }
	
    free(kernel);
    return output_ptr; 
}



