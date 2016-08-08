
// *********************************************************************************************************
// I2C_iAQ Core Bathroom Fan Demo Program
//
// Version 3
// This code is only meant to demonstrate the functionality of the iAQ Core from AMS in turning on a
// bathroom fan when odors are present.  This is not production ready code.
//
// This program reads the CO2 equivalents and TVOC equivalents data from the iAQ Core and displays it on 
// the Arduino monitor as well as the connected 16x2 LCD display (if used).
// 
// This demo uses the Sparkfun Arduino Pro 328, 3.3 volt / 8MHz version (DEV-10914)
// and the Sparkfun FTDI Basic Breakout - 3.3V USB to SPI Board (DEV09873) and the 
// Sparkfun 3.3 Volt White on Black LCD (LCD-09052).
//
// Make sure to only connect this board to 3.3 volts if connecting to Vcc or
// you can connect to 4 to 12 volts if connecting to Vin.
//
// This program displays the CO2 equivalents and TVOC equivalents to the
// LCD as read from the iAQ Core
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
// * 20K potentiometer:
// * ends to +3.3V and ground
// * wiper to LCD VO pin (pin 3)
//
//  The iAQ Core connections to the Arduino baord:
// * iAQ Core Pin 6 to 3.3 volts
// * iAQ Core Pin 2 (SCL)to analog pin A5
// * iAQ Core Pin 4 (SDA) to analog pin A4
// * iAQ Core Pin 3 to ground
//  !!!!!  Caution:  Make sure that the program doesn't write to the iAQ Core or damage could occur !!!!!
//
// This sketch uses the standard 7-bit I2C addresses
//
// 
//
// *********************************************************************************************************


// *********************************************************************************************************
// *                                   Included Libraries                                                  *
// ********************************************************************************************************* 
#include <Wire.h>                                 //Include the Wire library
#include <LiquidCrystal.h>                        //Include the LCD library

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);  // initialize the library with the numbers of the interface pins


// *********************************************************************************************************
// *                                         Defines                                                       *
// ********************************************************************************************************* 
#define iAQ_address     0x5A                      //This is the iAQ Core I2C address


// *********************************************************************************************************
// *                                    Global Variables                                                   *
// ********************************************************************************************************* 
unsigned int CO2_Prediction;                      //Declare the prediction CO2 equivalent in ppm variable
unsigned int TVOC;                                //Declare the predicted TVOC equivalnet in ppb variable
int          RelayControl = 10;                   //Control to turn on the solid state relay for the fan Digital I/O 10
//int          RelayControl = 13;                   //Control to turn on the solid state relay "SIMULATED" for the fan Digital I/O 13 using the on-board LED



// *********************************************************************************************************
// *                               Setup Function - Runs Once                                              *
// ********************************************************************************************************* 
void setup()
{
  pinMode (RelayControl, OUTPUT);                 //Set the RelayControl I/O pin to output
  digitalWrite(RelayControl, LOW);                //Make sure the solid state relay / fan are off
  Wire.begin();                                   //Initialize the wire library for I2C
 
  Serial.begin(9600);                             //Sets the baud rate for the serial monitor
  while (!Serial);                                // If you are running on the Leonardo: wait for serial monitor
  
  Serial.println("\n********  iAQ Core Bathroom Fan Demo  ********");

  lcd.begin(16, 2);                               //Set up the LCD's number of columns and rows
}


// *********************************************************************************************************
// *                                      Main Function                                                    *
// ********************************************************************************************************* 
void loop()
{
  byte  CO2_Byte0 = 0x00;                         //iAQ CO2 Byte 0
  byte  CO2_Byte1 = 0x00;                         //iAQ CO2 Byte 1
  byte  Status_Byte2 = 0x00;                      //iAQ Status Byte 2
  byte  Res_Byte3 = 0x00;                         //iAQ Resistance Byte 3
  byte  Res_Byte4 = 0x00;                         //iAQ Resistance Byte 4
  byte  Res_Byte5 = 0x00;                         //iAQ Resistance Byte 5
  byte  Res_Byte6 = 0x00;                         //iAQ Resistance Byte 6
  byte  TVOC_Byte7 = 0x00;                        //iAQ Core TVOC Byte 7
  byte  TVOC_Byte8 = 0x00;                        //iAQ Core TVOC Byte 8
  unsigned int Resistance = 0x0000;               //Sensor Resistance


//********************  Set up to read the data from the iAQ Core  ********************

                                                  //We read the iAQ Core data into individual bytes so we can 
                                                  //easily handle the math
                                                  
    Wire.requestFrom(iAQ_address, 9);             //Read 9 bytes out of the iAQ Core

    if (Wire.available())                         //Returns the number of bytes to be read
    {
      CO2_Byte0 = Wire.read();                    //Get the first byte of the CO2 reading
    }

    if (Wire.available())                         //Returns the number of bytes to be read
    {
      CO2_Byte1 = Wire.read();                    //Get the second byte of the CO2 reading
    }

    if (Wire.available())                         //Returns the number of bytes to be read
    {
      Status_Byte2 = Wire.read();                 //Get the iAQ status
    }

    if (Wire.available())                         //Returns the number of bytes to be read
    {
      Res_Byte3 = Wire.read();                    //Get the first byte of the resistance
    }

    if (Wire.available())                         //Returns the number of bytes to be read
    {
      Res_Byte4 = Wire.read();                    //Get the second byte of the resistance
    }

    if (Wire.available())                         //Returns the number of bytes to be read
    {
      Res_Byte5 = Wire.read();                    //Get the third byte of the resistance
    }

    if (Wire.available())                         //Returns the number of bytes to be read
    {
      Res_Byte6 = Wire.read();                    //Get the fourth byte of the resistance
    }

    if (Wire.available())                         //Returns the number of bytes to be read
    {
      TVOC_Byte7 = Wire.read();                   //Get the first byte of the TVOC reading
    }

    if (Wire.available())                         //Returns the number of bytes to be read
    {
      TVOC_Byte8 = Wire.read();                   //Get the second byte of the TVOC reading
    }

    Wire.endTransmission();                       //Terminate I2C transmission

 //********************  Calculate the CO2_Prediction, the Resistance and the TVOC level  ********************  

    //Calculate the predicted CO2 level in ppm
    CO2_Prediction = (CO2_Byte0 * 256)+ CO2_Byte1;
        
    //Calculate the resistance value in ohms (Note: Res_Byte3 is always 0)
    Resistance = (Res_Byte4 * 65536) + (Res_Byte5 * 256) + Res_Byte6;

    //Calculate the TVOC level in ppb
    TVOC = (TVOC_Byte7 * 256) + TVOC_Byte8;


    switch (Status_Byte2)
    {
      case 0:                                       //Status is OK
      Serial.print("CO2 Level = ");                 //Print the predicted CO2 level to the monitor
      Serial.print(CO2_Prediction);
      Serial.print("  ppm\n");
      Serial.print("TVOC Level = ");                //Print the predicted TVOC level to the monitor
      Serial.print(TVOC);
      Serial.print("  ppb\n");
      Serial.print("Resistance = ");                //Print the Resistance to the monitor
      Serial.print(Resistance);
      Serial.print("  Ohms");
      Serial.print("\nStatus in HEX = ");           //Print the Status to the monitor
      Serial.print(Status_Byte2, HEX);
      Serial.println("\n");
      
      //********************  Display the Predicted CO2 Level data to the LCD  ***********************************************
      clrScreen();                                  //Clear the Predicted CO2 Level and TVOC display screens
      posCO2Cursor();                               //Sub Routine to correctly locate the display of the CO2 Level on the LCD
      lcd.print(CO2_Prediction);                    //Print the Predicted CO2 Level to the LCD    

      //********************  Display the TVOC Level data to the LCD  ********************************************************  
      posTVOCCursor();                              //Sub Routine to correctly locate the display of the TVOC Level on the LCD
      lcd.print(TVOC);                              //Print the TVOC Level to the LCD

      //************* Turn on the fan if TVOC is greater than 1000 (air starting to get bad) *********************************
      if (CO2_Prediction > 1000)
      {
        digitalWrite(RelayControl, HIGH);           //Turn on the Solid state relay, turning on the fan
      }
      else
      {
        digitalWrite(RelayControl, LOW);            //Turn off the Solid state relay, turning off the fan
      }
      break;

      case 16:
      Serial.print("\nStatus in HEX = ");          //Print the Status to the monitor
      Serial.print(Status_Byte2, HEX);
      Serial.println("  iAQ Core Warming Up\n");
      lcd.clear();                                 //Clear the LCD screen
      lcd.print("    iAQ Core    ");               //Print the warm up message to the LCD
      lcd.setCursor(0,1);                          //Set cursor to position 0, second row
      lcd.print("   Warming Up   ");
      break;

      default:
      Serial.println("Error");
      Serial.print("\nStatus = ");                  //Print the Error Status to the monitor
      Serial.print(Status_Byte2, HEX);
      Serial.println("\n");
      break;
    }
     delay(1000);                                  // wait 1 second for next update
}



/**********************************************************************************************************************
 * Subroutine to clear the display and print the CO2 and TVOCmessages to the screen                                   *
 **********************************************************************************************************************/
void clrScreen(void)
{
  lcd.clear();                                    //Clear the LCD screen
  lcd.print("CO2:         ppm");                  //Print the CO2 message to the LCD
  lcd.setCursor(0,1);                             //Set cursor to position 0, second row
  lcd.print("TVOC:        ppb");                  //Print the TVOC message to the LCD
  return;
}




/**********************************************************************************************************************
 * Subroutine to correctly position the Predicted CO2 Level cursor based on the value to be displayed                 *
 **********************************************************************************************************************/
void posCO2Cursor(void)
{
  if (CO2_Prediction < 10)                        //If the value read is greater than 0, but less than 10
  {
    lcd.setCursor(11,0);                          //Set the cussor to the 11th position on line 0
  }
  else if (CO2_Prediction < 100)                  //If the value read is between 10 and 99
  {
    lcd.setCursor(10,0);                          //Set the cussor to the 10th position on line 0
  }
  else if (CO2_Prediction < 1000)                 //If the value read is between 100 and 999
  {
    lcd.setCursor(9,0);                           //Set the cussor to the 9th position on line 0
  }
  else                                            //Otherwise
  {
    lcd.setCursor(8,0);                           //Set the cussor to the 8th position on line 0
  }
  return;

}




/**********************************************************************************************************************
 * Subroutine to correctly position the TVOC Level cursor based on the value to be displayed                            *
 **********************************************************************************************************************/
void posTVOCCursor(void)
{
  if (TVOC < 10)                                 //If the value read is greater than 0, but less than 10
  {
    lcd.setCursor(11,1);                         //Set the cussor to the 11th position on line 1
  }
  else if (TVOC < 100)                           //If the value read is between 10 and 99
  {
    lcd.setCursor(10,1);                         //Set the cussor to the 10th position on line 1
  }
  else if (TVOC < 1000)                          //If the value read is between 100 and 999
  {
    lcd.setCursor(9,1);                         //Set the cussor to the 9th position on line 1
  }
  else                                           //Otherwise
  {
    lcd.setCursor(8,1);                          //Set the cussor to the 8th position on line 1
  }
  return;
}

