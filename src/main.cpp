// This file is Copyright (c) 2020 Florent Kermarrec <florent@enjoy-digital.fr>
// License: BSD

#include "main.h"

/*-----------------------------------------------------------------------*/
/* Commands                                                              */
/*-----------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
static void reboot_cmd(void);
static void prompt(void);
#ifdef __cplusplus
}
#endif


	
static void reboot_cmd(void)
{
	ctrl_reset_write(1);
}

static void prompt(void)
{
	printf("\e[92;1mlitex-demo-app\e[0m> ");
}

int main(void)
{
	#ifdef CONFIG_CPU_HAS_INTERRUPT
		irq_setmask(0);
		irq_setie(1);
	#endif
	
	comm_ridope_init();
	prompt();

	CONV2D_LAYER<float,float> layer1 = CONV2D_LAYER<float,float>(32, 9, 1, relu, (float *) &layer_1_weights[0] , (float *) &layer_1_bias[0], 14, 14, 1);
	MAXPOOL_LAYER layer2 = MAXPOOL_LAYER(4, 0, 2, layer1.GetInputRows(), layer1.GetInputCols(), layer1.GetInputChannel());
	CONV2D_LAYER<int8_t,int8_t> layer3 = CONV2D_LAYER<int8_t,int8_t>(64, 9, 1, relu, (int8_t *) &layer_3_weights[0] , (int8_t *) &layer_3_bias[0], 7, 7, 32);
	MAXPOOL_LAYER layer4 = MAXPOOL_LAYER(4, 0, 2, layer3.GetInputRows(), layer3.GetInputCols(), layer3.GetInputChannel());
	DENSE_LAYER<int8_t,int8_t> layer5 = DENSE_LAYER<int8_t,int8_t>(10, softmax, (int8_t *) &layer_5_weights[0] , (int8_t *) &layer_5_bias[0], 1, 1024, 1);

	while(1) {
		printf("Wait for img\n");

		uint32_t N,M = 0;
		float *sig = comm_ridope_receive_img(&N,&M);
		
		printf("Got image\n");

		float *output_layer_1 = layer1.ForwardPropagation(sig);
		free(sig);

		float *output_layer_2 = layer2.ForwardPropagation(output_layer_1);
		free(output_layer_1);

		float *output_layer_3 = layer3.ForwardPropagation(output_layer_2);
		free(output_layer_2);

		float *output_layer_4 = layer4.ForwardPropagation(output_layer_3);
		free(output_layer_3);

		float *output_layer_5 = layer5.ForwardPropagation(output_layer_4);
		free(output_layer_4);

		for(int i=0; i < layer5.GetUnits(); i++)
		{
			printf("%f\n", output_layer_5[i]);
		}

		free(output_layer_5);
	}


	return 0;
}
