#ifndef MAX_POOL_RIDOPE_H
#define MAX_POOL_RIDOPE_H

#include "activation_functions.h"

using namespace std;

#include "math.h"
#include "stdint.h"

class MAXPOOL_LAYER{
    private:
        uint8_t pool_size;
        uint8_t pad_size;
        uint8_t stride;

        int input_rows;
        int input_cols;
        int input_channel;
        
    public:   
        MAXPOOL_LAYER(uint8_t pool_size, uint8_t pad_size, uint8_t stride, int input_rows, int input_cols, int input_channel){
                this->pool_size = pool_size;
                this->pad_size = pad_size;
                this->stride = stride;
                this->input_rows = input_rows;
                this->input_cols = input_cols;
                this->input_channel = input_channel;
        }

        float * ForwardPropagation(float *input_ptr){
            uint8_t output_shape_rows = floor((this->input_rows - 1) / this->stride) + 1;
            uint8_t output_shape_cols = floor((this->input_cols - 1) / this->stride) + 1;
            float *output_ptr = (float *)malloc(output_shape_rows * output_shape_cols * sizeof(float));

            uint8_t cols_pad = this->input_cols + 2 * this->pad_size; // number of columns in padded image
            uint8_t rows_pad = this->input_rows + 2 * this->pad_size;

            int i, j, cnt_pad, k1, k2, output_cnt;
            float max_pool;

            output_cnt = 0;

            for(uint8_t ch = 0; ch < this->input_channel; ch++)
            {

                //image padding before performing max pooling
                float *img_pad = (float *)malloc(rows_pad * cols_pad * sizeof(float));
                pad_image(input_ptr+ch, img_pad, this->input_rows, this->input_cols, this->pad_size);

                max_pool = FLT_MIN;

                for (i = this->pad_size; i < rows_pad - this->pad_size; i=i+this->stride)
                for (j = this->pad_size; j < cols_pad - this->pad_size; j=j+this->stride)
                {
                    for (k1 = -this->pad_size; k1 <= this->pad_size; k1++)
                    for (k2 = -this->pad_size; k2 <= this->pad_size; k2++)
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
        
        uint8_t GetPoolSize() const{
            return this->pool_size;
        }
    
        uint8_t GetPadSize() const{
            return this->pad_size;
        }

        uint8_t GetStride() const{
            return this->stride;
        }

        uint8_t GetInputRows() const{
            return this->input_rows;
        }

        uint8_t GetInputCols() const{
            return this->input_cols;
        }

        uint8_t GetInputChannel() const{
            return this->input_channel;
        }
};

#endif

