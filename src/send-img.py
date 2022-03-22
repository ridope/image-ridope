#!/usr/bin/env python3
import serial
import os
import sys
import traceback
import subprocess
import numpy as np
from PIL import Image
import threading, queue
import time
from struct import *
from enum import Enum
import matplotlib.pyplot as plt

cmd = Enum('CMD_TYPE', 'REBOOT TRANS_PHOTO TRANS_FFT TRANS_IFFT PHOTO_SIZE START_TRANS STOP_TRANS NULL_CMD', start=48)
tx_buffer = queue.Queue()
rx_buffer = queue.Queue()
uart = serial.Serial("/dev/ttyUSB0", 115200)

tx_buffer.put("\n".encode())

def tx():
    while True:
        item = tx_buffer.get()
        uart.write(item)
        uart.write('\n'.encode())
        tx_buffer.task_done()

def rx():
    while True:
        if uart.in_waiting != 0:
            item = uart.read_until()

            if(len(item) > 2):

                if(item[1] >= cmd.REBOOT.value and item[1] <= cmd.NULL_CMD.value):
                    format = "<cIffc"

                    while(len(item) < calcsize(format)):
                        item += uart.read_until()

                    item_temp = unpack("<cIffc", item)
                    rx_buffer.put(item_temp)

                else:
                    item = item.decode()
                    print(item)
            
def save_img():
    N = 0
    M = 0
    cont = 0
    img_array = []

    while True:
        item = rx_buffer.get()
        rx_buffer.task_done() 

        if(item[1] == cmd.PHOTO_SIZE.value):
            N = int(item[2])
            M = int(item[3])
            print("pyGot size!")
            img_array = np.zeros(N*M, dtype=float)
 
        elif(item[1] == cmd.START_TRANS.value):
            print("pyGot start flag!")
            flag = item[1]

            while flag != cmd.STOP_TRANS.value:
                item = rx_buffer.get()
                rx_buffer.task_done() 

                flag = item[1]

                if(flag == cmd.TRANS_FFT.value):
                    pixel = abs(complex(item[2], item[3]))

                    img_array[cont] = pixel
                    cont += 1

                    if(cont+1 == N*M):
                        print("pyGot in the end!")
                        break

            print("pyGot stop flag!")
            im = Image.fromarray(np.fft.fftshift(np.reshape(img_array, (N,M))), mode="F")
            im = im.convert("L")
            im.save("../img/fft-image_NB32.png")
            plt.imshow(im)
            plt.show()

            N=0
            M=0
            cont = 0
            img_array = []

                 

def send_img():
    # Opening image
    with Image.open("../img/image_NB32.bmp").convert('L') as im:
        arr_img = np.array(im, dtype="<u1")
        print(arr_img.shape)
    
        data_send = pack("<iff", cmd.PHOTO_SIZE.value, arr_img.shape[0], arr_img.shape[1])
        tx_buffer.put(data_send)

        data_send = pack("<iff", cmd.START_TRANS.value, 0, 0)
        tx_buffer.put(data_send)

        for elem in np.nditer(arr_img,order="C"):
            data_send = pack("<iff", cmd.TRANS_PHOTO.value, elem,0)
            tx_buffer.put(data_send)
            time.sleep(0.005)

        data_send = pack("<iff", cmd.STOP_TRANS.value, 0, 0)
        tx_buffer.put(data_send)

def send_reboot():
    init = pack("<iff", cmd.REBOOT.value, 0, 0)
    tx_buffer.put(init)

# turn-on the tx thread
threading.Thread(target=tx, daemon=True).start()
threading.Thread(target=rx, daemon=True).start()
threading.Thread(target=save_img, daemon=True).start()

try:
    print("Available commands: ")
    print("send             - Sends image to FPGA")
    print("reboot           - Reboots the RISCV")
    while True:
        
        value = input("Please enter a string:  ")

        if(value=="send"):
            send_img()
        elif(value=="reboot"):
            send_reboot()

except KeyboardInterrupt:
        traceback.print_exc(file=sys.stdout)
        tx_buffer.join()    
        rx_buffer.join()      
        uart.close()        
        print("\nGoodbye!\n")
        sys.exit()
except:
    traceback.print_exc(file=sys.stdout)
    tx_buffer.join()    
    rx_buffer.join()     
    uart.close()     
    sys.exit()
