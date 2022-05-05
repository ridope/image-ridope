#ifndef DENSE_LAYER_RIDOPE_H
#define DENSE_LAYER_RIDOPE_H

#include "utils.h"
#include "math.h"
#include "activation_functions.h"

using namespace std;

template <typename weight_t, typename bias_t>
class DENSE_LAYER{
    private:
        uint8_t units;

        activation_func act_func;

        weight_t * weights;
        bias_t * bias;

        int input_rows;
        int input_cols;
        int input_channel;
        
    public:   
        DENSE_LAYER(uint8_t units, activation_func  act_func, weight_t * weights, bias_t * bias, int input_rows, int input_cols, int input_channel){
                this->units = units;
                this->act_func = act_func;
                this->weights = weights;
                this->bias = bias;
                this->input_rows = input_rows;
                this->input_cols = input_cols;
                this->input_channel = input_channel;
        }

        float * ForwardPropagation(float *input_ptr){
            float *output_ptr = (float *)malloc(this->input_rows * this->units * sizeof(float));
            float *output_ptr_temp = output_ptr;
            float *kernel = (float *)malloc(this->input_cols*sizeof(float));
            int cnt_weight = 0;
            
            for (int i = 0; i < this->units; i++)
            {
                // reading corresponding weights to kernel pointer
                for (int cnt_kernel = 0; cnt_kernel < this->input_cols; cnt_kernel++)
                {
                    *(kernel + cnt_kernel) = this->weights[cnt_weight + cnt_kernel];
                }
                cnt_weight = cnt_weight + this->input_cols;

                output_ptr_temp[i] = dot_product(input_ptr, kernel, this->input_cols) + this->bias[i];
            }

            free(kernel);

            this->act_func(output_ptr_temp, this->input_rows, this->input_cols, 0);

            return output_ptr; 
        }
        
        uint8_t GetUnits() const{
            return this->units;
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
