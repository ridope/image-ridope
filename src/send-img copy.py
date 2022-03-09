#!/usr/bin/python3

import os
import sys
import traceback
import subprocess
from struct import *

from PIL import Image
import threading, queue
import time

import serial
import numpy as np
from crc import CrcCalculator, Crc8

from enum import Enum

data_type = Enum('Data_Type', 'Img Cmd', start=32)

tx_buffer = queue.Queue()
rx_buffer = queue.Queue()
uart = serial.Serial("/dev/ttyUSB0", 115200)

tx_buffer.put('\n'.encode())

def tx():
    while True:
        data_type = tx_buffer.get()
        counter = tx_buffer.get()
        item = tx_buffer.get()
        uart.write(data_type)
        uart.write(counter)
        uart.write(len(item))
        uart.write(item)
        uart.write('\n'.encode())
        print(item)
        tx_buffer.task_done()

def rx():
    while True:
        if uart.in_waiting != 0:
            item = uart.read_until()

            try:
                item = item.decode()
            except UnicodeDecodeError:
                size = len(item)
                item = unpack('B'*size, item) 

            print(item)
            rx_buffer.put(item)
            rx_buffer.task_done()

def send_img():
    tx_buffer.put("fft".encode())
    # Opening image
    with Image.open("../img/cameraman.png") as im:
        arr_img = np.array(im, dtype="<u1")
        print(arr_img.shape)

        for index,row in enumerate(arr_img[:][:][0]):
            data_send = row.tobytes()
            tx_buffer.put(data_type.Img)
            tx_buffer.put(index)
            tx_buffer.put(data_send)

            ans = tx_buffer.get()

            # Check if received a NACK
            if ans == 5:
                break

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
