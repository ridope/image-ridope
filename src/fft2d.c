#include "fft2d.h"


uint8_t fft2d(float complex * matrix, const uint32_t *N, const uint32_t *M){

    float complex *sig_temp;
    char *ptr_iterator = (char *) matrix;

    float complex * img_trans = malloc((*N)*(*M)*sizeof(float complex));

	if(!img_trans)
	{
		return -1;
	}
	    
	for(int i=0; i<*N; i++) {
		sig_temp = (float complex *) ptr_iterator;
		fft(sig_temp, *N);
		ptr_iterator += (*M)*sizeof(float complex);
	}

	transpose(matrix, img_trans, N, M);
		
	ptr_iterator = (char *) matrix;

	for(int i=0; i<*N; i++) {
		sig_temp = (float complex *) ptr_iterator;
		fft(sig_temp, *N);
		ptr_iterator += (*M)*sizeof(float complex);
	}

	transpose(matrix, img_trans, N, M);

	free(img_trans);

	return 0;
}

void transpose(float complex * matrix, float complex * trans_sig, const uint32_t *N, const uint32_t *M){
	
	float complex *sig_temp;
	char *ptr_iterator = (char *) matrix;
	
	float complex *trans_sig_temp;
	char *ptr_trans_iterator;

	char * first_addr_sig = (char *) matrix;
	char * last_addr_sig = first_addr_sig + (((*N)*(*M))-1)*sizeof(float complex);

	char * first_addr_trans = (char *) trans_sig;

	int elem_trans = 0;
	int elem_sig = 0;

	for(ptr_iterator = first_addr_sig; ptr_iterator < last_addr_sig; ) {
		sig_temp = (float complex *) ptr_iterator;

		ptr_trans_iterator = first_addr_trans;
		
		for(elem_sig = 0; elem_sig < *N; elem_sig++ ){
			trans_sig_temp = (float complex *) ptr_trans_iterator;
			
			trans_sig_temp[elem_trans] = sig_temp[elem_sig];

			ptr_trans_iterator += (*M)*sizeof(float complex);
		}

		elem_trans++;
		ptr_iterator += (*M)*sizeof(float complex);
	}

	for(int i=0; i < (*N)*(*M); i++){
		matrix[i] = trans_sig[i];
	}
}