// This file is Copyright (c) 2020 Florent Kermarrec <florent@enjoy-digital.fr>
// License: BSD

#include "main.h"

/*-----------------------------------------------------------------------*/
/* Commands                                                              */
/*-----------------------------------------------------------------------*/
static void reboot_cmd(void);
static void prompt(void);

	
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
		
		
		float a=1.0;
		float b=1.5;
		float c=1.0;
		printf("%f\t%f\n", a, b);
		

	}


	return 0;
}
