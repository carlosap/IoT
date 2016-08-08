import serial
from datetime import datetime
from time import sleep
import subprocess
import sys
port = ""
lastrecord = ""
command = ""
filename = "/code/sendpmt.exe"
ctr = -1
if (len(sys.argv) != 2):
        print("setting default port")
        port = "/dev/ttyUSB0"
        command = ""
else:
        port = sys.argv[0]
        command = sys.argv[1]

arduinoSerial = serial.Serial(port,9600)
arduinoSerial.flushInput()
while True:
        ctr += 1
	input = arduinoSerial.readline()
	input = input.strip()
	if(ctr > 0):
                if(len(input) > 0):                
                        if(input != lastrecord):
                                lastrecord = input
                                #print(input)
                                try:
                                        #print(filename)
                                        subprocess.call(["mono", filename, input])
                                except:
                                        pass
                                        print("...")
                        else:                
                                print("waiting for temperature changes")

sleep(5)
