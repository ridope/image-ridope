#!/usr/bin/python3

import serial
import os
import sys
import traceback
import subprocess
import numpy as np
from PIL import Image
import threading, queue
import time

tx_buffer = queue.Queue()
rx_buffer = queue.Queue()
uart = serial.Serial("/dev/ttyUSB0", 115200)

def tx():
    while True:
        item = tx_buffer.get()
        print(f'\nSending {item}')
        uart.write(item)
        uart.write('\n'.encode())
        tx_buffer.task_done()

def rx():
    while True:
        if uart.in_waiting != 0:
            item = uart.read_until()
            print(f'Getting {item}')
            rx_buffer.put(item)
            rx_buffer.task_done()

def send_img():
    # Opening image
    with Image.open("../img/cameraman.png") as im:
        arr_img = np.array(im, dtype="<u1")
        print(arr_img.shape)

        for row in arr_img[:][:][0]:
            for elem in row:
                #print(row)
                data_send = elem.tobytes()
                tx_buffer.put(data_send)
                time.sleep(0.005)

def send_bin():
    bashCmd = ["sudo lxterm /dev/ttyUSB0 --kernel demo.bin"]
    process = subprocess.Popen(bashCmd, stdout=subprocess.PIPE)
    output, error = process.communicate()
    print(error)

# turn-on the tx thread
threading.Thread(target=tx, daemon=True).start()
threading.Thread(target=rx, daemon=True).start()

try:
    print("Available commands: ")
    print("fft              - calls FFT function on FPGA")
    print("send             - Sends image to FPGA")
    print("upload           - Sends program to RISCV")
    while True:
        
        value = input("Please enter a string:  ")
        
        if(value=="fft"):
            tx_buffer.put("fft".encode())
        elif(value=="send"):
            send_img()
        elif(value=="upload"):
            send_bin()

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
