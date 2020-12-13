#include <Adafruit_MLX90614.h>
#include <Wire.h> // I2C library, required for MLX90614

Adafruit_MLX90614 therm = Adafruit_MLX90614(); //temp sensor object
const int Laser_Pin=6;  //Laser Pin with PWM
String incomeMessage; //serial output
float newEmissivity; //serial output emissivity

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // Initialize Serial with 9600 bps to transmit temp and recieve commands
  Serial.setTimeout(100);
  therm.begin(); // Initialize thermal IR sensor
  pinMode(Laser_Pin,OUTPUT); //laser pin as an output
  analogWrite(Laser_Pin, 0); //laser turned off by defualt

}

void loop() {

  if(Serial.available()) //if there are incoming serial data from the Android app
  {
    SerialRecieved(); //parse incoming serial data
  }

  // Use the object() and ambient() functions to grab the object and ambient
  // temperatures.
  Serial.print("Obj:," + String(therm.readObjectTempK()) + ","); //display the object temp 
  Serial.print("Amb:," + String(therm.readAmbientTempK()) + ","); //display the ambient temp
  Serial.print("EMS:," + String(therm.readEmissivity())); //read the emmissivity
  Serial.print('\n');


  delay(250);

}


//function to parse incoming serial data from the Android app
void SerialRecieved()
{
  incomeMessage = Serial.readString(); //read the incoming serial data into a string
  newEmissivity = incomeMessage.toFloat(); //convert the incoming serial data to float if its convertable
  Serial.print(String(newEmissivity)+ '\n');
  
  if (incomeMessage.startsWith("ON")) 
  {
    
    analogWrite(Laser_Pin, 10); //turn on laser

  }

  else if (incomeMessage.startsWith("OFF"))
  {
    
    analogWrite(Laser_Pin, 0); //turn on laser

  }

  else if(newEmissivity > 0 && newEmissivity <= 1)
  {
    therm.writeEmissivity(newEmissivity);
  }

}
