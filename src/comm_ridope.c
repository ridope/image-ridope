#include "comm_ridope.h"

/**
 * @brief Initializes the RIDOPE communication
 * 
 */
void comm_ridope_init(void)
{
    uart_init();
}

/**
 * @brief Receives the command to be executed
 * 
 * @param msg The pointer to the COMM_RIDOPE_MSG_t struct to save the command received
 */
void comm_ridope_receive_cmd(COMM_RIDOPE_MSG_t *msg)
{   
    if(msg == NULL){
        return;
    }

    uint8_t count = 0;
    char byte;

    do{
        if(uart_read_nonblock())
        {
            byte = uart_read();

            if(byte != '\n')
            {
                msg->buffer[count++] = byte;
            }
        }
    }while(byte != '\n');
    
    if (count == 0){
        msg->cmd_send.cmd = NULL_CMD;
    }
}

/**
 * @brief Sends a command to be executed
 * 
 * @param msg The pointer to the COMM_RIDOPE_MSG_t struct where the command is stored
 */
void comm_ridope_send_cmd(COMM_RIDOPE_MSG_t *msg)
{
    if(msg == NULL){
        return;
    }

    uint8_t buff_len = sizeof(COMM_RIDOPE_MSG_t);

    for(int i = 0; i < buff_len; i++)
    {
        uart_write(msg->buffer[i]);
    }

    uart_write('\n');
}

