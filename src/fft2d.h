#ifndef FFT2D_H
#define FFT2D_H

#include <stdint.h>
#include <stdlib.h>
#include "complex.h"
#include "lib/ufft/fft.h"
#include "lib/ufft/ift.h"

uint8_t fft2d(float complex * matrix, const uint32_t *N, const uint32_t *M);
void transpose(float complex * matrix, float complex * trans_sig, const uint32_t *N, const uint32_t *M);

#endif
