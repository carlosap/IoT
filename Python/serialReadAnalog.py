import serial
port = "/dev/ttyUSB0"
arduinoSerial = serial.Serial(port,9600)
arduinoSerial.flushInput()
while True:
	input = arduinoSerial.readline()
	inputAsInt = int(input)
	print(inputAsInt * 10)
