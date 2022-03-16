#ifndef COMM_RIDOPE_H
#define COMM_RIDOPE_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <irq.h>
#include <libbase/uart.h>
#include <libbase/console.h>
#include <generated/csr.h>

#include "complex.h"

/**
 * @brief Enum with the allowed commands in the RIDOPE project UART comunication 
 * 
 */
typedef enum CMD_TYPE
{
    REBOOT = 48,
    TRANS_PHOTO,
    TRANS_FFT,
    TRANS_IFFT,
    PHOTO_SIZE,
    START_TRANS,
    STOP_TRANS,
    NULL_CMD,
}CMD_TYPE_t;

/**
 * @brief Struct with the message format of the RIDOPE project UART comunication
 *  
 */
typedef struct COMM_RIDOPE_CMD_TYPE
{
    CMD_TYPE_t cmd;
    float complex data;
} COMM_RIDOPE_CMD_TYPE_t;

/**
 * @brief Union with the message and the byte buffer ready to be sent
 * 
 */
typedef union
{
    COMM_RIDOPE_CMD_TYPE_t msg_data;
    char buffer[sizeof(COMM_RIDOPE_CMD_TYPE_t)];

} COMM_RIDOPE_MSG_t;

void comm_ridope_init(void);
float complex* comm_ridope_receive_img(uint32_t *N, uint32_t *M);
void comm_ridope_send_img(float complex* img, CMD_TYPE_t img_type, uint32_t N, uint32_t M);
void comm_ridope_receive_cmd(COMM_RIDOPE_MSG_t *msg);
void comm_ridope_send_cmd(COMM_RIDOPE_MSG_t *msg);

#endif