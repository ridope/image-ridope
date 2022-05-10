#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <irq.h>
#include <libbase/uart.h>
#include <libbase/console.h>
#include <generated/csr.h>

#include "complex.h"
#include "comm_ridope.h"

#include "../lib/cnn_ridope/activation_functions.h"
#include "../lib/cnn_ridope/conv2d_layer.h"
#include "../lib/cnn_ridope/max_pool_layer.h"
#include "../lib/cnn_ridope/dense_layer.h"

#include "../lib/cnn_ridope/data/layer_1_bias.h"
#include "../lib/cnn_ridope/data/layer_3_bias.h"
#include "../lib/cnn_ridope/data/layer_5_bias.h"

#include "../lib/cnn_ridope/data/layer_1_weights.h"
#include "../lib/cnn_ridope/data/layer_3_weights.h"
#include "../lib/cnn_ridope/data/layer_5_weights.h"


#endif