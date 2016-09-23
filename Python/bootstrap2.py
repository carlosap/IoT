import serial
from datetime import datetime
from time import sleep
import subprocess
import sys
import re
devices = []
port = ""
lastrecord = ""
command = ""
msg = ""
filename = "/code/sendpmt.exe"
ctr = -1
device_re = re.compile("Bus\s+(?P<bus>\d+)\s+Device\s+(?P<device>\d+).+ID\s(?P<id>\w+:\w+)\s(?P<tag>.+)$", re.I)
df = subprocess.check_output("lsusb", shell=True)
for i in df.split('\n'):
    if i:
        info = device_re.match(i)
        if info:
            dinfo = info.groupdict()
            dinfo['device'] = '/dev/bus/usb/%s/%s' % (dinfo.pop('bus'), dinfo.pop('device'))
            if "1b4f" in dinfo['id']: 
                    msg = "Detected Micro Pro"
                    port = "/dev/ttyACM0"
            if "0403" in dinfo['id']:
                    msg = "Ltd FT232 USB-Serial (UART) IC"
                    port = "/dev/ttyUSB0"                 
            devices.append(dinfo)

if (len(sys.argv) != 2):
        print(msg + " : " + port)      
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
                                try:
                                        print(input)
                                        subprocess.call(["mono", filename, input])
                                except:
                                        pass
                                        print("...")
                        else:                
                                print("waiting for temperature changes")

sleep(5)
