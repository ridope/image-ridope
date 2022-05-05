#include "max_pool_layer.h"

float * get_max_pool(max_pool_layer_t *layer, float *input_ptr)
{

    uint8_t output_shape_rows = floor((layer->input_rows - 1) / layer->stride) + 1;
    uint8_t output_shape_cols = floor((layer->input_cols - 1) / layer->stride) + 1;
    float *output_ptr = (float *)malloc(output_shape_rows * output_shape_cols * sizeof(float));

    uint8_t cols_pad = layer->input_cols + 2 * layer->pad_size; // number of columns in padded image
	uint8_t rows_pad = layer->input_rows + 2 * layer->pad_size;

    int i, j, cnt_pad, k1, k2, output_cnt;
    float max_pool;

    output_cnt = 0;

    for(uint8_t ch = 0; ch < layer->input_channel; ch++)
    {

        //image padding before performing max pooling
	    float *img_pad = (float *)malloc(rows_pad * cols_pad * sizeof(float));
        pad_image(input_ptr+ch, img_pad, layer->input_rows, layer->input_cols, layer->pad_size);

        max_pool = FLT_MIN;

        for (i = layer->pad_size; i < rows_pad - layer->pad_size; i=i+layer->stride)
        for (j = layer->pad_size; j < cols_pad - layer->pad_size; j=j+layer->stride)
        {
            for (k1 = -layer->pad_size; k1 <= layer->pad_size; k1++)
            for (k2 = -layer->pad_size; k2 <= layer->pad_size; k2++)
            {
                cnt_pad = (i + k1)*cols_pad + j + k2; // counter which shows each neighbouring pixel of padded image used for pooling
                max_pool = max(max_pool, (*(img_pad + cnt_pad)));
            }
            *(output_ptr + output_cnt) = max_pool;
            output_cnt++;
        }

        free(img_pad);
        img_pad = NULL;
    }

    return output_ptr; 
}

