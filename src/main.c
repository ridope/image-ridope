// This file is Copyright (c) 2020 Florent Kermarrec <florent@enjoy-digital.fr>
// License: BSD

#include "main.h"


static void prompt(void)
{
	printf("\e[92;1mridope-app\e[0m> ");
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
	puts("7seg             	 - Seven Segments");
	puts("fft 				- FFT");
}

/*-----------------------------------------------------------------------*/
/* Commands                                                              */
/*-----------------------------------------------------------------------*/

static void reboot_cmd(void)
{
	ctrl_reset_write(1);
}

static void fft_cmd(void)
{
	printf("Start fft\n");

	float complex *sig = read_img();
	int N = 32;
	int M = 32;
		
	printf("fft 2D start\n");

	fft2d(sig, N, M);

	printf("#### fft 2D done ####\n");
	
	for(int i=0; i<N*M; i++) {
		if(i%M == 0){
			printf("####\n");
		}
		printf("%f%+fi\n", crealf(sig[i]), cimagf(sig[i]));
	}
}

float complex * read_img(void)
{	
	static const int N = 32;
	static const int M = 32;

	static float complex img[32][32];

	int i,j = 0;
	int byte;

	// Waits for the img to be sent
	do{
		if(readchar_nonblock()){

			byte = getchar();
		}

	}while(byte!='#');

	printf("Got init flag\n");

	byte=0;
	// Reads img
	printf("Reading img\n");
	while(byte!='#'){
		if(readchar_nonblock()) {
			byte = getchar();

			if(i==N && j==M){
				break;
			}else if(j==M){
				j = 0;
				i++;
			}

			img[i][j++] = byte;

			printf("%d\n", byte);
		}
	}

	printf("Done\n");
	return img[0];
}

int main(void)
{
	#ifdef CONFIG_CPU_HAS_INTERRUPT
		irq_setmask(0);
		irq_setie(1);
	#endif

	uart_init();

	while(1) {
		
	}

	return 0;
}
