#!/usr/bin/python3

import serial
import numpy as np
from PIL import Image
import time

# Opening image
with Image.open("../img/cameraman.png") as im:
    arr_img = np.array(im, dtype="<u1")
    print(arr_img.shape)

    # Opening serial communication
    with serial.Serial("/dev/ttyUSB0") as uart:
        uart.write('#'.encode())
        
        for row in arr_img[:][:][0]:
            for elem in row:
                print(row)
                data_send = elem.tobytes()
                uart.write(data_send)
                uart.write('\n'.encode())
                time.sleep(0.005)

        while True:
            if uart.in_waiting != 0:
                line = uart.read_until()
                print(line)
        
