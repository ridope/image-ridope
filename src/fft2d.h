#ifndef FFT2D_H
#define FFT2D_H

#include "complex.h"
#include "lib/ufft/fft.h"
#include "lib/ufft/ift.h"

void fft2d(float complex * matrix, size_t N, size_t M);
void transpose(float complex * matrix, float complex * trans_sig, size_t N, size_t M);

#endif
