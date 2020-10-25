/******************************************************************************
MLX90614 temp sensor Arduino code


This code reads from the MLX90614 and prints out ambient and object
temperatures every 250 ms to the serial output.


Using Sparksfun library:
https://github.com/sparkfun/SparkFun_MLX90614_Arduino_Library
******************************************************************************/

#include <Wire.h> // I2C library, required for MLX90614
#include <SparkFunMLX90614.h> // SparkFunMLX90614 Arduino library

IRTherm therm; // Create an IRTherm object to interface with the temp sensor

String incomeMessage; //serial output
float newEmissivity; //serial output emissivity
const int Laser_Pin=8;  //Laser Pin

void setup()
{
  Serial.begin(9600); // Initialize Serial with 9600 bps to transmit temp and recieve commands
  therm.begin(); // Initialize thermal IR sensor
  therm.setUnit(TEMP_K); // Set the library's units Kelvin, which is the sensor output tempreture unit
  // use TEMP_F for fehrehite
  // TEMP_C for C.
  pinMode(Laser_Pin,OUTPUT); //laser pin as an output
  digitalWrite(Laser_Pin, LOW); //laser turned off by defualt
}


//main method
void loop()
{
  if(Serial.available()) //if there are incoming serial data from the Android app
  {
    SerialRecieved(); //parse incoming serial data
  }

  // Call therm.read() to read object and ambient temperatures from the sensor.
  if (therm.read()) // On success, read() will return 1, on fail 0.
  {
    // Use the object() and ambient() functions to grab the object and ambient
  // temperatures.
    Serial.print("Obj:," + String(therm.object(), 2) + ","); //display the object temp with 2 decimal places
    Serial.print("Amb:," + String(therm.ambient(), 2)); //display the ambient temp with 2 decimal places
    Serial.print('\n');
  }
  delay(250); //delay 250ms
}


//function to parse incoming serial data from the Android app
void SerialRecieved()
{
  incomeMessage = Serial.readString(); //read the incoming serial data into a string
  newEmissivity = incomeMessage.toFloat(); //convert the incoming serial data to float if its convertable
  
  if (incomeMessage.startsWith("ON")) 
  {
    
    digitalWrite(Laser_Pin, HIGH); //turn on laser
    Serial.println("Laser on");

  }

  else if (incomeMessage.startsWith("OFF"))
  {
    
    digitalWrite(Laser_Pin, LOW); //turn on laser
    Serial.println("Laser off");

  }

  else if(newEmissivity >= 0 || newEmissivity <= 1)
  {
    therm.setEmissivity(newEmissivity);
    Serial.println("Emissivity cahnged to " + String(newEmissivity));
  }

}
