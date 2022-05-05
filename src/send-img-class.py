#!/usr/bin/env python3
import serial
import os, fnmatch
import sys
import traceback
import subprocess
import numpy as np
from scipy.io import savemat
from PIL import Image
import threading, queue
import time
from struct import *
from enum import Enum
import matplotlib.pyplot as plt

cmd = Enum('CMD_TYPE', 'REBOOT TRANS_PHOTO TRANS_FFT TRANS_IFFT PHOTO_SIZE START_TRANS STOP_TRANS OP_TIME NULL_CMD', start=48)
tx_buffer = queue.Queue()
rx_buffer = queue.Queue()
uart = serial.Serial("/dev/ttyUSB0", 115200)

next_img = True
image_name = ""

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

                    item_temp = unpack(format, item)
                    rx_buffer.put(item_temp)

                else:
                    item = item.decode()
                    print(item)
            
def save_img():
    global image_name, next_img
    N = 0
    M = 0
    time = 0
    cont = 0
    img_array = []

    while True:
        item = rx_buffer.get()
        rx_buffer.task_done() 

        if(item[1] == cmd.PHOTO_SIZE.value):
            N = int(item[2])
            M = int(item[3])
            print("pyGot size!")
            img_array = np.zeros(N*M, dtype=np.float32)
        
        elif(item[1] == cmd.OP_TIME.value):
            time = item[2]
            print("FFT time: ", time)
 
        elif(item[1] == cmd.START_TRANS.value):
            print("pyGot start flag!")
            flag = item[1]

            while flag != cmd.STOP_TRANS.value:
                item = rx_buffer.get()
                rx_buffer.task_done() 

                flag = item[1]

                if(flag == cmd.TRANS_FFT.value):
                    #pixel = abs(complex(item[2], item[3]))
                    pixel = complex(item[2], item[3])

                    img_array[cont] = abs(pixel)
                    cont += 1

                    if(cont+1 == N*M):
                        print("pyGot in the end!")
                        break

            print("pyGot stop flag!")
            im_array_reshaped = np.reshape(img_array, (N,M))

            im_array_normalized = im_array_reshaped / im_array_reshaped.max() # normalize the data to 0 - 1
            im_array_normalized = 255 * im_array_normalized # Now scale by 255
            im_array_normalized = im_array_normalized.astype(np.uint8)

            im = Image.fromarray(np.fft.fftshift(im_array_normalized), mode="L")
            #fft_path = "../img/fft/fft-" + image_name
            fft_path = "../img/fft/fft-image_NB32.png"
            print(fft_path)
            im.save(fft_path)
            #plt.imshow(im)
            #plt.show()

            N=0
            M=0
            cont = 0
            img_array = []
            next_img = True

                 

def send_img(img_path):
    # Opening image
    with Image.open(img_path).convert('L') as im:
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
            send_img("../img/img_test.png")
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
