// *********************************************************************************************************
// *                                   Included Libraries                                                  *
// ********************************************************************************************************* 
#include <Wire.h>                           //Include the Wire library
#include <LiquidCrystal.h>                  //Include the LCD library
LiquidCrystal lcd(14, 16, 4, 5, 6, 7);      // initialize the library with the numbers of the interface pins
// *********************************************************************************************************
// *                                         Defines                                                       *
// ********************************************************************************************************* 
#define ENS210_address  0x43                      //This is the ENS210 I2C address
#define SENS_START      0x22                      //Register SENS_START address - Starts T or H measurement
#define Both_T_H        0x03                      //Starts both temperature & humidity measurements
#define T_VAL           0x30                      //Start address for temperature result register
#define H_VAL           0x33                      //Start address for humidity result register
#define iAQ_address     0x5A                      //This is the iAQ Core I2C address

// *********************************************************************************************************
// *                                    Global Variables                                                   *
// ********************************************************************************************************* 
float Humidity;                                   //Declare the Humidity variable
float Temperature;                                //Declare the Temperature variable
unsigned int CO2_Prediction;                      //Declare the prediction CO2 equivalent in ppm variable
unsigned int TVOC;                                //Declare the predicted TVOC equivalnet in ppb variable
//Temp:
word  T_Data = 0x0000;                          //Temperature data word
byte  T_Data1 = 0x00;                           //Temperature byte 1, address 0x30
byte  T_Data2 = 0x00;                           //Temperature byte 2, address 0x31
byte  T_Data3 = 0x00;                           //Temperature byte 3, address 0x32
byte  T_Flag = 0x00;                            //Temperature valid flag
float TinK;                                     //Temperature in Kelvin
float TinC;                                     //Temperature in Celsius
float TinF;                                     //Temperature in Fahrenheit
//Humidity:
word  H_Data = 0x0000;                          //Humidity Data Word
byte  H_Data1 = 0x00;                           //Humidity byte 1, address 0x33
byte  H_Data2 = 0x00;                           //Humidity byte 2, address 0x34
byte  H_Data3 = 0x00;                           //Humidity byte 3, address 0x35
byte  H_Flag = 0x00;                            //Humidity valid flag
//iAQ:
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
// *********************************************************************************************************
// *                               Setup Function - Runs Once                                              *
// ********************************************************************************************************* 
void setup()
{
  
  Wire.begin();                       //Initialize the wire library
  Serial.begin(9600);                 //Sets the baud rate for the serial monitor
  while (!Serial);                    // If you are running on the Leonardo: wait for serial monitor
  lcd.begin(16, 2);                   //Set up the LCD's number of columns and rows
  Humidity = 1;                       //Initialize the humidity variable
  Temperature = -40;                  //Initialize the humidity variable
  
}
 
// *********************************************************************************************************
// *                                      Main Function                                                    *
// ********************************************************************************************************* 
void loop()
{

  clrTempAndHumidity_Screen();                    //Clear the temperature and humidity display screens
  getTemp();
  getHumidity();  
  printResults();
  displayTempAndHumidity();  
  delay(3000);                                    // wait 2 seconds for next update 
  getVOC_Prediction();
  printResults();
  delay(3000);                                    // wait 2 seconds for next update 
  
}

/**********************************************************************************************************************
 * Set up to read the temperature from the ENS210                                                                     *
 **********************************************************************************************************************/
void getTemp(void)
{

    Wire.beginTransmission(ENS210_address);       //Send the slave address
    Wire.write(SENS_START);                       //Write the SENS_START register address
    Wire.write(Both_T_H);                         //Starts both temperature & humidity measurements
    Wire.endTransmission();                       //Terminate I2C transmission
    delay (200);                                  //Delay to allow data to be valid
    Wire.beginTransmission(ENS210_address);       //Send the slave address
    Wire.write(T_VAL);                            //Write the T_VAL register to set the location to be read
    Wire.endTransmission();                       //Terminate I2C transmission                                                  
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
    T_Data = T_Data2;                             //Put the second byte into the T_Data word
    T_Data = T_Data << 8;                         //Shift it over 8 bits to the left
    T_Data |= T_Data1;                            //Put in the first byte into the T_Data word
    T_Flag = T_Data3 && 0x01;                     //Mask for the temperature valid flag.  It is displayed in the monitor
    Wire.endTransmission();                       //Terminate I2C transmission
    TinK = float(T_Data) / 64;                    //Temperature in Kelvin
    TinC = TinK - 273.15;                         //Temperature in Celsius
    TinF = TinC * 1.8 + 32;                       //Temperature in Fahrenheit
    Temperature = TinF;                           //Display the temperature in Fahrenheit to the LCD Display
    return;

}

/**********************************************************************************************************************
 * Set up to read the humidity from the ENS210                                                                     *
 **********************************************************************************************************************/
void getHumidity(void)
{
 
    Wire.beginTransmission(ENS210_address);       //Send the slave address
    Wire.write(H_VAL);                            //Write the H_VAL register to set the location to be read
    Wire.endTransmission();                       //Terminate I2C transmission
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
    Humidity = float(H_Data) / 512;               //Himidity in %
    return;
}
/**********************************************************************************************************************
 * Set up to read the data from the iAQ Core                                                                           *
 * Calculate the CO2_Prediction, the Resistance and the TVOC level                                                     *
 **********************************************************************************************************************/
void getVOC_Prediction(void)
{

    
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
    CO2_Prediction = (CO2_Byte0 * 256)+ CO2_Byte1;                        //Calculate the predicted CO2 level in ppm       
    Resistance = (Res_Byte4 * 65536) + (Res_Byte5 * 256) + Res_Byte6;     //Calculate the resistance value in ohms (Note: Res_Byte3 is always 0) 
    TVOC = (TVOC_Byte7 * 256) + TVOC_Byte8;                              //Calculate the TVOC level in ppb

    switch (Status_Byte2)
    {
      case 0:                                     
          clrScreenAsVCO();                             
          posCO2Cursor();                             
          lcd.print(CO2_Prediction);                   
          posTVOCCursor();                             
          lcd.print(TVOC);                          
          break;
      case 16:
          lcd.clear();                                
          lcd.print("    iAQ Core    ");              
          lcd.setCursor(0,1);                         
          lcd.print("   Warming Up   ");
          break;

      default:
          Serial.println("Error");
          Serial.print("\nStatus = ");            
          Serial.print(Status_Byte2, HEX);
          Serial.println("\n");
          break;
    }  
    return;
}

/**********************************************************************************************************************
 * Subroutine to clear the temperature and humidity values from the screen                                            *
 **********************************************************************************************************************/
void clrScreenAsVCO(void)
{
  lcd.clear();                                    //Clear the LCD screen
  lcd.print("CO2:         ppm");                  //Print the CO2 message to the LCD
  lcd.setCursor(0,1);                             //Set cursor to position 0, second row
  lcd.print("TVOC:        ppb");                  //Print the TVOC message to the LCD
  return;
}

void clrScreenTempHumidityMeasurements(void)
{
  lcd.setCursor(8,0);                             //Set cursor to line 0, 8th position
  lcd.print ("       ");                          //Clear the 7 positions before writing the new temperature value
  lcd.setCursor(8,1);                             //Set cursor to line 1, 8th position
  lcd.print ("       ");                          //Clear the 7 positions before writing the new humidity value
  return;
}

void clrTempAndHumidity_Screen(void)
{
  lcd.clear(); 
  lcd.print("Temp           F");      //Print the Temperature message to the LCD
  lcd.setCursor(0,1);                 //Set cursor to position 0, second row
  lcd.print("Humidity       %");      //Print the Humidity message to the LCD
  clrScreenTempHumidityMeasurements();
  
}

void displayTempAndHumidity(void)
{
    posTempCursor();                                //Sub Routine to correctly locate the display of the temperature on the LCD
    lcd.print(Temperature);                         //Print the temperature to the LCD 
    posHumidCursor();                               //Sub Routine to correctly locate the display of the humidity on the LCD
    lcd.print(Humidity);                            //Print the humidity to the LCD
  return;
}

void printResults(void)
{
    Serial.print("Fahrenheit=");                   //Print temperature in Fahrenheit to the monitor
    Serial.print(TinF);
    Serial.print("|Celsius=");                     //Print temperature in Celsius to the monitor
    Serial.print(TinC);
    Serial.print("|Kelvin=");                       //Print temperature in Kelvin to the monitor
    Serial.print(TinK);
    Serial.print("|Humidity=");                //Print temperature in Fahrenheit to the monitor
    Serial.print(Humidity);
    Serial.print("|PPM=");                 
    Serial.print(CO2_Prediction);
    Serial.print("|PPB=");
    Serial.print(TVOC);          
    Serial.println("\n");
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
