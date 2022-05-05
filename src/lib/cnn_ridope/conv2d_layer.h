#ifndef CONV2D_RIDOPE_H
#define CONV2D_RIDOPE_H

#include "activation_functions.h"
#include <stdio.h>

using namespace std;

template <typename weight_t, typename bias_t>
class CONV2D_LAYER{
    private:
        uint8_t filters;
        uint8_t kernel_size;
        uint8_t pad_size;

        activation_func act_func;

        weight_t * weights;
        bias_t * bias;

        int input_rows;
        int input_cols;
        int input_channel;
        
    public:   
        CONV2D_LAYER(uint8_t filters, uint8_t kernel_size, uint8_t pad_size, activation_func  act_func, weight_t * weights, bias_t * bias, int input_rows, int input_cols, int input_channel){
                this->filters = filters;
                this->kernel_size = kernel_size;
                this->pad_size = pad_size;
                this->act_func = act_func;
                this->weights = weights;
                this->bias = bias;
                this->input_rows = input_rows;
                this->input_cols = input_cols;
                this->input_channel = input_channel;
        }

        float * ForwardPropagation(float *input_ptr){
            
            float *output_ptr = (float *)malloc(this->input_rows * this->input_cols * this->filters * sizeof(float));

            printf("Output allocation done\n");

            float *output_ptr_temp = output_ptr;
            float *kernel = (float *)malloc(this->filters*sizeof(float));

            printf("Kernel allocation done\n");

            int cnt_weight = 0;
            
            float bias_tmp;

            for(int ch = 0; ch < this->input_channel; ch++)
            {
                for (int i = 0; i < this->filters; i++)
                {
                    // reading corresponding weights to kernel pointer
                    for (int cnt_kernel = 0; cnt_kernel < this->filters; cnt_kernel++)
                    {
                        *(kernel + cnt_kernel) = this->weights[cnt_weight + cnt_kernel];
                    }
                    cnt_weight = cnt_weight + this->filters;

                    imfilter(input_ptr, kernel, output_ptr_temp, this->input_rows, this->input_cols, this->pad_size);
                    printf("Conv kernel done done\n");

                    bias_tmp = this->bias[i];
                    printf("Access bias done\n");

                    printf("Calling Activation function\n");
                    this->act_func(output_ptr_temp, this->input_rows, this->input_cols, bias_tmp);
                    printf("Activation function done\n");

                    output_ptr_temp = output_ptr_temp + this->input_rows*this->input_cols;

                    printf("filter: %d, channel: %d\n", i, ch);
                }
            }
            
            free(kernel);
            return output_ptr; 
        }
        
        uint8_t GetFilters() const{
            return this->filters;
        }
    
        uint8_t GetKernelSize() const{
            return this->kernel_size;
        }

        uint8_t GetPadSize() const{
            return this->pad_size;
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

