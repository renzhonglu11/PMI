#!/usr/bin/env python3

# with ubuntu, install python3-serial package

import time
import serial
import threading

ser = serial.Serial()  # open serial port
# ser.port = '/dev/ttyACM0' # check port name in linux
ser.port = 'COM9'        # check port name in windows
ser.baudrate = 115200
ser.timeout = .5       # timeout of .5 seconds 

print("connected to: ", ser.name)         # check which port was really used

ser.open()
ser.read_all()  # clear the input buffer if not empty
while(1) :
    read_data = ser.read(3) # read 3 bytes
    if len(read_data) == 0:
        print('Lost Connection, start Blinking')
    else:
        print('Connected with data:', read_data);
        ser.write(("n\n").encode())
    time.sleep(1)

ser.close()             # close port
