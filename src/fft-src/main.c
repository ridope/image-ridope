// This file is Copyright (c) 2020 Florent Kermarrec <florent@enjoy-digital.fr>
// License: BSD

#include "main.h"

/*-----------------------------------------------------------------------*/
/* Commands                                                              */
/*-----------------------------------------------------------------------*/
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

	while(1) {
		printf("Wait for img\n");

		uint32_t N,M = 0;
		float complex *sig = comm_ridope_receive_img(&N,&M);

		printf("fft 2D start\n");

		// Change this to a function millis_ridope
		timer0_en_write(0);
		timer0_load_write(0);
		timer0_reload_write(0xFFFFFFFF);
		timer0_en_write(1);
		timer0_update_value_write(1);

		timer0_update_value_write(1);
		uint32_t time_begin = timer0_value_read();

		uint8_t status = fft2d(sig, &N, &M);

		timer0_update_value_write(1);
		uint32_t time_end = timer0_value_read();

		float time_spent_ms = (time_begin - time_end)/(CONFIG_CLOCK_FREQUENCY/1000.0);

		if(status == -1)
		{
			printf("Error\n");
			free(sig);
			continue;
		}

		printf("#### fft 2D done ####\n");

		COMM_RIDOPE_MSG_t msg;
		msg.msg_data.cmd = OP_TIME;
		msg.msg_data.data = time_spent_ms+0*I;

		comm_ridope_send_cmd(&msg);

		comm_ridope_send_img(sig, TRANS_FFT, N, M);

		free(sig);
	}

	return 0;
}
