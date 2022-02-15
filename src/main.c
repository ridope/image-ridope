// This file is Copyright (c) 2020 Florent Kermarrec <florent@enjoy-digital.fr>
// License: BSD

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <irq.h>
#include <libbase/uart.h>
#include <libbase/console.h>
#include <generated/csr.h>

#include "complex.h"
#include "hello.h"
#include "fft.h"

/*-----------------------------------------------------------------------*/
/* Uart                                                                  */
/*-----------------------------------------------------------------------*/

static char *readstr(void)
{
	char c[2];
	static char s[64];
	static int ptr = 0;

	if(readchar_nonblock()) {
		c[0] = getchar();
		c[1] = 0;
		switch(c[0]) {
			case 0x7f:
			case 0x08:
				if(ptr > 0) {
					ptr--;
					fputs("\x08 \x08", stdout);
				}
				break;
			case 0x07:
				break;
			case '\r':
			case '\n':
				s[ptr] = 0x00;
				fputs("\n", stdout);
				ptr = 0;
				return s;
			default:
				if(ptr >= (sizeof(s) - 1))
					break;
				fputs(c, stdout);
				s[ptr] = c[0];
				ptr++;
				break;
		}
	}

	return NULL;
}

static char *get_token(char **str)
{
	char *c, *d;

	c = (char *)strchr(*str, ' ');
	if(c == NULL) {
		d = *str;
		*str = *str+strlen(*str);
		return d;
	}
	*c = 0;
	d = *str;
	*str = c+1;
	return d;
}

static void prompt(void)
{
	printf("\e[92;1mlitex-demo-app\e[0m> ");
}

/*-----------------------------------------------------------------------*/
/* Help                                                                  */
/*-----------------------------------------------------------------------*/

static void help(void)
{
	puts("\nLiteX minimal demo app built "__DATE__" "__TIME__"\n");
	puts("Available commands:");
	puts("help               - Show this command");
	puts("reboot             - Reboot CPU");
#ifdef CSR_LEDS_BASE
	puts("led                - Led demo");
#endif
	puts("donut              - Spinning Donut demo");
	puts("7seg             	 - Seven Segments");
	puts("helloc             - Hello C");
	puts("fft 				- FFT");
#ifdef WITH_CXX
	puts("hellocpp           - Hello C++");
#endif
}

/*-----------------------------------------------------------------------*/
/* Commands                                                              */
/*-----------------------------------------------------------------------*/

static void reboot_cmd(void)
{
	ctrl_reset_write(1);
}

#ifdef CSR_LEDS_BASE
static void led_cmd(void)
{
	int i;
	printf("Led demo...\n");

	printf("Counter mode...\n");
	for(i=0; i<32; i++) {
		leds_out_write(i);
		busy_wait(100);
	}

	printf("Shift mode...\n");
	for(i=0; i<4; i++) {
		leds_out_write(1<<i);
		busy_wait(200);
	}
	for(i=0; i<4; i++) {
		leds_out_write(1<<(3-i));
		busy_wait(200);
	}

	printf("Dance mode...\n");
	for(i=0; i<4; i++) {
		leds_out_write(0x55);
		busy_wait(200);
		leds_out_write(0xaa);
		busy_wait(200);
	}
}
#endif

//extern void donut(void);

/*static void donut_cmd(void)
{
	printf("Donut demo...\n");
	donut();
}*/

extern void helloc(void);

static void helloc_cmd(void)
{
	
	int teste = oi();
	printf("Hello C demo...%d\n", teste);
}

extern void gpioc(void);

static void gpioc_cmd(void)
{
	for(int i=0; i<10; i++) {
		leds_out_write(1<<(9-i));
		busy_wait(200);
	}
}

static void fft_cmd(void)
{
	float complex sig[64] = {1,0.951056516295154,0.809016994374948,0.587785252292473,0.309016994374947,6.12323399573677e-17,-0.309016994374948,-0.587785252292473,-0.809016994374947,-0.951056516295154,-1,-0.951056516295154,-0.809016994374947,-0.587785252292473,-0.309016994374948,-1.83697019872103e-16,0.309016994374947,0.587785252292474,0.809016994374948,0.951056516295154,1,0.951056516295153,0.809016994374948,0.587785252292473,0.309016994374947,-5.82016719913287e-16,-0.309016994374949,-0.587785252292473,-0.809016994374947,-0.951056516295153,-1,-0.951056516295154,-0.809016994374948,-0.587785252292472,-0.309016994374946,1.34773045969868e-15,0.309016994374949,0.587785252292473,0.809016994374947,0.951056516295153,1,0.951056516295153,0.809016994374947,0.587785252292472,0.309016994374948,5.51091059616309e-16,-0.309016994374947,-0.587785252292473,-0.809016994374948,-0.951056516295154,-1,-0.951056516295153,-0.809016994374946,-0.587785252292472,-0.309016994374948,1.10280109986921e-15,0.309016994374947,0.587785252292474,0.809016994374947,0.951056516295154,1,0.951056516295153,0.809016994374948,0.587785252292472};
	
	int N = sizeof(sig)/sizeof(sig[0]);

	double complex z = 1.0 + 2.0*I;
    printf("\n%f%+fi\n", creal(z), cimag(z));

	// for(int i=0; i<N; i++) {
	// 	printf("%g + i%g\n", crealf(sig[i]), cimagf(sig[i]));
	// }
	
	// float mag[64];

	// int result = fft(&sig[0], N);

	// for(int i=0; i<N; i++) {
	// 	printf("%f + i%f\n", crealf(sig[i]), cimagf(sig[i]));
	// }
}

#ifdef WITH_CXX
extern void hellocpp(void);

static void hellocpp_cmd(void)
{
	printf("Hello C++ demo...\n");
	hellocpp();
}
#endif

/*-----------------------------------------------------------------------*/
/* Console service / Main                                                */
/*-----------------------------------------------------------------------*/

static void console_service(void)
{
	char *str;
	char *token;

	str = readstr();
	if(str == NULL) return;
	token = get_token(&str);
	if(strcmp(token, "help") == 0)
		help();
	else if(strcmp(token, "reboot") == 0)
		reboot_cmd();
	else if(strcmp(token, "fft") == 0)
		fft_cmd();
#ifdef CSR_LEDS_BASE
	else if(strcmp(token, "led") == 0)
		led_cmd();
#endif
	//else if(strcmp(token, "donut") == 0)
	//	donut_cmd();
	else if(strcmp(token, "helloc") == 0)
		helloc_cmd();
#ifdef WITH_CXX
	else if(strcmp(token, "hellocpp") == 0)
		hellocpp_cmd();
#endif
	else if(strcmp(token, "gpioc") == 0)
		gpioc_cmd();
	
	else if(strcmp(token, "7seg") == 0)
		sevenseg_cmd();
	prompt();	
}

int main(void)
{
#ifdef CONFIG_CPU_HAS_INTERRUPT
	irq_setmask(0);
	irq_setie(1);
#endif
	uart_init();

	help();
	prompt();

	while(1) {
		console_service();
	}

	return 0;
}
