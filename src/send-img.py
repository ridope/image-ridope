#!/usr/bin/python3

import serial
import numpy as np
from PIL import Image

# Opening image
with Image.open("../img/lion1.png") as im:
    arr_img = np.array(im, dtype="<u1")
    print(arr_img.shape)

    # Opening serial communication
    with serial.Serial("/dev/ttyUSB0") as uart:
        #Start sending row
        uart.write("[")
        for row in arr_img:
            data_send = row.tobytes()
            uart.write(data_send)

        #End sending row
        uart.write("]")
        
