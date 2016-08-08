
// *********************************************************************************************************
// I2C_ENS210 Temperature & Humidity Demo Program
//
// Version 1
// This code is only meant to demonstrate the functionality of the ENS210 Temperature and Humidity Sensor
// from AMS.  This is not production ready code and does not use the checksum feature or check the ready 
// flag.
//
// This program reads the temperature and humidity data from the ENS210 and displays it on the Arduino
// Monitor as well as the connected 16x2 LCD display.
// 
// This demo uses the Sparkfun Arduino Pro 328, 3.3 volt / 8MHz version (DEV-10914)
// and the Sparkfun FTDI Basic Breakout - 3.3V USB to SPI Board (DEV09873) and the 
// Sparkfun 3.3 Volt White on Black LCD (LCD-09052).
//
// Make sure to only connect this board to 3.3 volts if connecting to Vcc or
// you can connect to 4 to 12 volts if connecting to Vin.
//
// This program displays the current temperature and humidity to the
// LCD as read from the ENS-210
//
//  The LCD Connections to the Arduino board:
// * LCD RS pin to digital pin 12
// * LCD Enable pin to digital pin 11
// * LCD D4 pin to digital pin 5
// * LCD D5 pin to digital pin 4
// * LCD D6 pin to digital pin 3
// * LCD D7 pin to digital pin 2
// * LCD R/W pin to ground
// * LCD VSS pin to ground
// * LCD VCC pin to 3.3V
// * 10K resistor:
// * ends to +3.3V and ground
// * wiper to LCD VO pin (pin 3)
//
//  The ENS210 connections to the Arduino baord:
// * ENS210 Pin 1 to 3.3 volts
// * ENS210 Pin 2 (SCL)to analog pin A5
// * ENS210 Pin 3 (SDA) to analog pin A4
// * ENS210 Pin 4 and 5 to ground
//
// This sketch uses the standard 7-bit I2C addresses
//
// The temperature displayed on the LCD is Fahrenheit, even though Kelvin and Celsius is also calculated.
// A feature could easily be added to allow the display to show any of the three measurements by adding
// a switch to one of the unused port lines.
// 
//
// *********************************************************************************************************


// *********************************************************************************************************
// *                                   Included Libraries                                                  *
// ********************************************************************************************************* 
#include <Wire.h>                                 //Include the Wire library
#include <LiquidCrystal.h>                        //Include the LCD library
#include <Firmata.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);  // initialize the library with the numbers of the interface pins


// *********************************************************************************************************
// *                                         Defines                                                       *
// ********************************************************************************************************* 
#define ENS210_address  0x43                      //This is the ENS210 I2C address
#define SENS_START      0x22                      //Register SENS_START address - Starts T or H measurement
#define Both_T_H        0x03                      //Starts both temperature & humidity measurements
#define T_VAL           0x30                      //Start address for temperature result register
#define H_VAL           0x33                      //Start address for humidity result register


// *********************************************************************************************************
// *                                    Global Variables                                                   *
// ********************************************************************************************************* 
float Humidity;                                   //Declare the Humidity variable
float Temperature;                                //Declare the Temperature variable


// *********************************************************************************************************
// *                               Setup Function - Runs Once                                              *
// ********************************************************************************************************* 
void setup()
{
  
  
  //Firmata.setFirmwareVersion(FIRMATA_FIRMWARE_MAJOR_VERSION, FIRMATA_FIRMWARE_MINOR_VERSION);
  Firmata.attach(STRING_DATA, stringCallback);
  Firmata.attach(START_SYSEX, sysexCallback);
  Firmata.begin(57600);

  
  Wire.begin();                                   //Initialize the wire library
 
  Serial.begin(9600);                             //Sets the baud rate for the serial monitor
  while (!Serial);                                // If you are running on the Leonardo: wait for serial monitor
  
  //Serial.println("\n********  ENS-210 Demo  ********");

  lcd.begin(16, 2);                   //Set up the LCD's number of columns and rows
  lcd.print("Temp           F");      //Print the Temperature message to the LCD
  lcd.setCursor(0,1);                 //Set cursor to position 0, second row
  lcd.print("Humidity       %");      //Print the Humidity message to the LCD
  Humidity = 1;                       //Initialize the humidity variable
  Temperature = -40;                  //Initialize the humidity variable

}
 

// *********************************************************************************************************
// *                                      Main Function                                                    *
// ********************************************************************************************************* 
void loop()
{
  word  T_Data = 0x0000;                          //Temperature data word
  byte  T_Data1 = 0x00;                           //Temperature byte 1, address 0x30
  byte  T_Data2 = 0x00;                           //Temperature byte 2, address 0x31
  byte  T_Data3 = 0x00;                           //Temperature byte 3, address 0x32
  byte  T_Flag = 0x00;                            //Temperature valid flag
  float TinK;                                     //Temperature in Kelvin
  float TinC;                                     //Temperature in Celsius
  float TinF;                                     //Temperature in Fahrenheit
  word  H_Data = 0x0000;                          //Humidity Data Word
  byte  H_Data1 = 0x00;                           //Humidity byte 1, address 0x33
  byte  H_Data2 = 0x00;                           //Humidity byte 2, address 0x34
  byte  H_Data3 = 0x00;                           //Humidity byte 3, address 0x35
  byte  H_Flag = 0x00;                            //Humidity valid flag 


 
   while (Firmata.available()) {
    Firmata.processInput();
  }
 
//********************  Set up to read the temperature data from the ENS210  ********************

    Wire.beginTransmission(ENS210_address);       //Send the slave address
    Wire.write(SENS_START);                       //Write the SENS_START register address
    Wire.write(Both_T_H);                         //Starts both temperature & humidity measurements
    Wire.endTransmission();                       //Terminate I2C transmission

    delay (200);                                  //Delay to allow data to be valid

    Wire.beginTransmission(ENS210_address);       //Send the slave address
    Wire.write(T_VAL);                            //Write the T_VAL register to set the location to be read
    Wire.endTransmission();                       //Terminate I2C transmission


                                                  //We read the ENS210 temp data into individual bytes so we can re-order
                                                  //them easily and the process can be easliy followed.  This code
                                                  //can easily be simplified, but it would be harder to follow.
                                                  //The data comes out little endian and we aren't using 
                                                  //the CRC check for this simple demo.
                                                  
    Wire.requestFrom(ENS210_address, 3);          //Read 3 bytes out of the ENS210 for the temperature

    if (Wire.available())                         //Returns the number of bytes to be read
    {
      T_Data1 = Wire.read();                      //Get the first byte of data - Least significant byte of the temp
    }

    if (Wire.available())                         //Returns the number of bytes to be read
    {
      T_Data2 = Wire.read();                      //Get the second byte of data - Most significant byte of the temp
    }

    if (Wire.available())                         //Returns the number of bytes to be read
    {
      T_Data3 = Wire.read();                      //Get the third byte of data - CRC and valid flag data
    }

    //Assemble the temperature bytes in the correct order to generate the temperature data
    T_Data = T_Data2;                             //Put the second byte into the T_Data word
    T_Data = T_Data << 8;                         //Shift it over 8 bits to the left
    T_Data |= T_Data1;                            //Put in the first byte into the T_Data word

    //Look for the temperature valid flag
    T_Flag = T_Data3 && 0x01;                     //Mask for the temperature valid flag.  It is displayed in the monitor
    Wire.endTransmission();                       //Terminate I2C transmission

    //Convert the T_Data to Kelvin, Celsius, and Fahrenheit
    TinK = float(T_Data) / 64;                    //Temperature in Kelvin
    TinC = TinK - 273.15;                         //Temperature in Celsius
    TinF = TinC * 1.8 + 32;                       //Temperature in Fahrenheit

    Temperature = TinF;                           //Display the temperature in Fahrenheit to the LCD Display

    //Display the Fahrenheit, Celsius, and Kelvin temperature to the Arduino monitor along with the valid flag
    Serial.print("Fahrenheit=");               //Print temperature in Fahrenheit to the monitor
    Serial.print(TinF);
    Serial.print("F");
    Serial.print("|Celsius=");               //Print temperature in Celsius to the monitor
    Serial.print(TinC);
    Serial.print("C");
    Serial.print("|Kelvin=");               //Print temperature in Kelvin to the monitor
    Serial.print(TinK);
    Serial.print("K");
    //Serial.print("\nValid flag = ");
    //Serial.print(T_Flag, HEX);                    //Print the valid flag to the monitor

    //Serial.println("\n");



//********************  Set up to read the humidity data from the ENS210  ********************

    Wire.beginTransmission(ENS210_address);       //Send the slave address
    Wire.write(H_VAL);                            //Write the H_VAL register to set the location to be read
    Wire.endTransmission();                       //Terminate I2C transmission

                                                  //We read the ENS210 humidity data into individual bytes so we can
                                                  //re-order them easily and the process can be easliy followed.  
                                                  //This code can easily be simplified, but it would be harder to follow.
                                                  //The data comes out little endian and we aren't using 
                                                  //the CRC check for this simple demo.
 
    Wire.requestFrom(ENS210_address, 3);          //Read 3 bytes out of the ENS210 for the humidity

    if (Wire.available())                         //Returns the number of bytes to be read
    {
      H_Data1 = Wire.read();                      //Get the first byte of data - Least significant byte of the humidity
    }

    if (Wire.available())                         //Returns the number of bytes to be read
    {
      H_Data2 = Wire.read();                      //Get the second byte of data - Most significant byte of the temp
    }

    if (Wire.available())                         //Returns the number of bytes to be read
    {
      H_Data3 = Wire.read();                      //Get the third byte of data - CRC and valid flag data
    }

    //Assemble the humidity bytes in the correct order to generate the humidity data
    
    H_Data = H_Data2;                             //Put the second byte into the H_Data word
    H_Data = H_Data << 8;                         //Shift it over 8 bits to the left
    H_Data |= H_Data1;                            //Put in the first byte into the H_Data word

    //Look for the temperature valid flag
    H_Flag = H_Data3 && 0x01;                     //Mask for the humidity valid flag
    Wire.endTransmission();                       //Terminate I2C transmission

    //Convert the H_Data to Humidity in %
    Humidity = float(H_Data) / 512;               //Himidity in %

    //Display the Humidity to the Arduino monitor along with the valid flag
    Serial.print("|Humidity=");                //Print temperature in Fahrenheit to the monitor
    Serial.print(Humidity);
    Serial.print("%");
    //Serial.print("\nValid flag = ");
    //Serial.print(H_Flag, HEX);                    //Print the valid flag to the monitor

    Serial.println("\n");                         //Print a line feed
    

//********************  Display the temperature data to the LCD  ********************

    clrScreen();                                  //Clear the temperature and humidity display screens

    posTempCursor();                              //Sub Routine to correctly locate the display of the temperature on the LCD

    lcd.print(Temperature);                       //Print the temperature to the LCD    

//********************  Display the humidity data to the LCD  ********************  

     posHumidCursor();                            //Sub Routine to correctly locate the display of the humidity on the LCD

     lcd.print(Humidity);                         //Print the humidity to the LCD
     delay(2000);                                  // wait 2 seconds for next update
  
}



/**********************************************************************************************************************
 * Subroutine to clear the temperature and humidity values from the screen                                            *
 **********************************************************************************************************************/
void clrScreen(void)
{
  lcd.setCursor(8,0);                             //Set cursor to line 0, 8th position
  lcd.print ("       ");                          //Clear the 7 positions before writing the new temperature value
  lcd.setCursor(8,1);                             //Set cursor to line 1, 8th position
  lcd.print ("       ");                          //Clear the 7 positions before writing the new humidity value
  return;
}




/**********************************************************************************************************************
 * Subroutine to correctly position the temperature cursor based on the value to be displayed                         *
 **********************************************************************************************************************/
void posTempCursor(void)
{
  if (Temperature <-9)                            //If the temperature is less than -9 but greater than -100
  {
    lcd.setCursor(8,0);                           //Set the cussor to the 8th position on line 0
  }
  else if (Temperature < 0)                       //If the temperature is less than 0
  {
    lcd.setCursor(10,0);                          //Set the cussor to the 10th position on line 0
  }
  else if (Temperature < 10)                      //If the value read is greater than 0, but less than 10
  {
    lcd.setCursor(11,0);                          //Set the cussor to the 11th position on line 0
  }
  else if (Temperature < 100)                     //If the value read is between 10 and 99
  {
    lcd.setCursor(10,0);                          //Set the cussor to the 10th position on line 0
  }
  else                                            //Otherwise
  {
    lcd.setCursor(9,0);                           //Set the cussor to the 9th position on line 0
  }
  return;

}




/**********************************************************************************************************************
 * Subroutine to correctly position the humidity cursor based on the value to be displayed                            *
 **********************************************************************************************************************/
void posHumidCursor(void)
{
  if (Humidity < 10)                             //If the value read is greater than 0, but less than 10
  {
    lcd.setCursor(11,1);                         //Set the cussor to the 11th position on line 1
  }
  else if (Humidity < 100)                       //If the value read is between 10 and 99
  {
    lcd.setCursor(10,1);                         //Set the cussor to the 10th position on line 1
  }
  else                                           //Otherwise
  {
    lcd.setCursor(9,1);                          //Set the cussor to the 9th position on line 1
  }
  return;
}
/**********************************************************************************************************************
 * Send Msg to host computer                           *
 **********************************************************************************************************************/
void stringCallback(char *myString)
{
  Firmata.sendString(myString);
}
/**********************************************************************************************************************
 * generic, arbitrary length messages (via MIDI SysEx protocol), uses sysexCallbackFunction for the                   *
 **********************************************************************************************************************/
void sysexCallback(byte command, byte argc, byte *argv)
{
  Firmata.sendSysex(command, argc, argv);
}
