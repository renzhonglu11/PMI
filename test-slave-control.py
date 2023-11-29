#!/usr/bin/env python3

# with ubuntu, install python3-serial package

import time
import serial


class States():
    "Stores send pattern and duration time of states"
    def __init__(self, pattern, duration):
        self.pattern = pattern
        self.duration = duration
        
        
ser = serial.Serial()  # open serial port
# ser.port = '/dev/ttyACM0' # check port name in linux
ser.port = 'COM9'        # check port name in windows
ser.baudrate = 115200
ser.timeout = .5       # timeout of .5 seconds

print("connected to: ", ser.name)         # check which port was really used

data = []
data.append(States("Gg", 3))
data.append(States("Yg", 1))
data.append(States("RR", 2))
data.append(States("RD", 1))
data.append(States("gG", 10))
data.append(States("RY", 1))
data.append(States("RD", 1))
data.append(States("RR", 1))
data.append(States("DR", 1))
data.append(States("Gg", 3))

ser.open()
ser.read_all()  # clear the input buffer if not empty
for x in data:

    dur = x.duration
    if(dur != 1):
        ser.write((x.pattern + "\n").encode())
        print("send:",x.pattern)
        time.sleep(1)
        read_data = ser.read(2)
        print("recv:", read_data )
        dur -= 1
        while dur > 0:
            dur -= 1
            ser.write((x.pattern + "\n").encode())
            print("send:",x.pattern)
            time.sleep(1)
            read_data = ser.read(2)
            print("recv:", read_data )
    else:
        ser.write((x.pattern + "\n").encode())
        print("send:", x.pattern)
        time.sleep(1)
        read_data = ser.read(2)
        print("recv:", read_data )

ser.close()             # close port
