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
 * @brief Receives images from the UART
 * 
 * @param N         The number of lines
 * @param M         The number of columns
 * @return float*   Retruns the pointer of the Img
 */
float complex* comm_ridope_receive_img(uint32_t *N, uint32_t *M){
    COMM_RIDOPE_MSG_t rx;

    // Gets Img size to be transfered
    do{
        comm_ridope_receive_cmd(&rx);

        if(rx.msg_data.cmd == PHOTO_SIZE)
        {
            *N = crealf(rx.msg_data.data);
            *M = cimagf(rx.msg_data.data);

        }else if(rx.msg_data.cmd == REBOOT)
        {
            ctrl_reset_write(1);
        }

    }while(rx.msg_data.cmd != PHOTO_SIZE);

    uint32_t img_pos = (*N)*(*M);
    float complex* img = malloc(img_pos*sizeof(float complex));

    printf("N: %d F: %d\n", *N, *M);

    // Checks mem allocation
    if(!img)
    {
        printf("Can't do malloc\n");
        return NULL;
    }

    // Waits for Start flag
    do{
        comm_ridope_receive_cmd(&rx);

        if(rx.msg_data.cmd == REBOOT)
        {
            ctrl_reset_write(1);
        }

    }while(rx.msg_data.cmd != START_TRANS);

    printf("Got start flag!\n");
    // Gets Img
    uint32_t counter = 0;
    do{
        comm_ridope_receive_cmd(&rx);

        if(rx.msg_data.cmd == TRANS_PHOTO)
        {
            img[counter++] = rx.msg_data.data;
        }
        else if(rx.msg_data.cmd == REBOOT)
        {
            ctrl_reset_write(1);
        }

    }while(rx.msg_data.cmd != STOP_TRANS && counter < img_pos);

    printf("Counter: %d, Img_size: %d\n", counter, img_pos);
    printf("Got stop flag!\n");

    return img;
}

/**
 * @brief Sends an Img throught the UART
 * 
 * @param img       The pointer to the img
 * @param img_type  The type of image being sent
 * @param N         The number of lines
 * @param M         The number of columns
 */
void comm_ridope_send_img(float complex* img, CMD_TYPE_t img_type,uint32_t N, uint32_t M)
{
    COMM_RIDOPE_MSG_t msg;

    msg.msg_data.cmd = img_type;
    msg.msg_data.data = 10+0I;

    comm_ridope_send_cmd(&msg);
       /*  data_send = pack("<iff", cmd.PHOTO_SIZE.value, arr_img.shape[0], arr_img.shape[1])
        tx_buffer.put(data_send)

        data_send = pack("<iff", cmd.START_TRANS.value, 0, 0)
        tx_buffer.put(data_send)

        for elem in np.nditer(arr_img,order="C"):
            data_send = pack("<iff", cmd.TRANS_PHOTO.value, elem,0)
            tx_buffer.put(data_send)
            time.sleep(0.005)

        data_send = pack("<iff", cmd.STOP_TRANS.value, 0, 0)
        tx_buffer.put(data_send) */
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
    char byte = 0;

    do{
        if(readchar_nonblock())
        {
            byte = getchar();
            
            if(byte != '\n')
            {
                msg->buffer[count++] = byte;
            }
        }
    }while(byte != '\n');
    
    if (count == 0){
        msg->msg_data.cmd = NULL_CMD;
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

    printf("buff_len: %d\n", buff_len);

    for(int i = 0; i < buff_len; i++)
    {
        uart_write(msg->buffer[i]);
    }

    uart_write('\n');
}

