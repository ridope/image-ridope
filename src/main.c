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
#include "ift.h"

static float complex * read_img(void);

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
	puts("7seg             	 - Seven Segments");
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

static void fft_cmd(void)
{
	float complex *sig = read_img();

	int N = 64;

	printf("fft start");

	int result = fft(sig, N);

	printf("fft done");

	result = ift(sig, N);

	printf("ifft");

	for(int i=0; i<N; i++) {
		printf("%.4f%+.4fi\n", crealf(sig[i]), cimagf(sig[i]));
	}
}

extern void gpioc(void);

static void gpioc_cmd(void)
{
  for(int i=0; i<10; i++) {
    leds_out_write(1<<(9-i));
    busy_wait(200);
  }
}

extern void sevenseg(void);

static void sevenseg_cmd(void)
{
  char display_seg[] = {0x3F, 0x06, 0x5B,0x4F};
  for(int i=0; i<sizeof(display_seg); i++) {
    
    seven_out_write(~display_seg[i]);
    busy_wait(500);
  }
  seven_out_write(0xFF);
}

static float complex * read_img(void)
{	
	static const int N = 64;
	static const int M = 64;

	static float complex img[64][64];

	int i,j = 0;
	char byte;

	// Waits for the img to be sent
	do{
		if(readchar_nonblock()){
			byte = getchar();
			printf("%c", byte);
		}

	}while(byte!='#');
	printf("Got init flag\n");

	byte=0;
	// Reads img
	printf("Reading img\n");
	while(byte!='#'){
		if(readchar_nonblock()) {
			byte = getchar();

			printf("%c", byte);

			if(i==N && j==M){
				break;
			}else if(j==M){
				j = 0;
				i++;
			}

			img[i][j++] = byte;
		}
	}

	printf("Done\n");
	return img[0];
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
#ifdef WITH_CXX
	else if(strcmp(token, "hellocpp") == 0)
		hellocpp_cmd();
#endif
	else if(strcmp(token, "gpioc") == 0)
		gpioc_cmd();
	
	else if(strcmp(token, "[") == 0)
		read_img();
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
