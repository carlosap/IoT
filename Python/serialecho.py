import serial
port = "/dev/ttyUSB0"
arduinoSerial = serial.Serial(port,9600)
arduinoSerial.flushInput()
while True:
	if(arduinoSerial.inWaiting() > 0):
		input = arduinoSerial.read(1)
		print(ord(input))
		print(str(ord(input)) + " = ASCII character " + input + ".")
