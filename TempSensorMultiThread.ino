#NOTE: this firmware is for testing puproses and won't work with ThermoApp. use the other firmware

#include <Arduino_FreeRTOS.h>
#include <Wire.h> // I2C library, required for MLX90614
#include <SparkFunMLX90614.h> // SparkFunMLX90614 Arduino library

// define two tasks for Blink & AnalogRead
void TaskSendTemp( void *pvParameters );
void TaskReadSerial( void *pvParameters );


IRTherm therm; // Create an IRTherm object to interface with the temp sensor
const int Laser_Pin=8;  //Laser Pin
String incomeMessage; //serial output
float newEmissivity; //serial output emissivity

// the setup function runs once when you press reset or power the board
void setup() {
  
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.setTimeout(100);
  therm.begin(); // Initialize thermal IR sensor
  therm.setUnit(TEMP_C);
  therm.setEmissivity(1); //human skin
  pinMode(Laser_Pin,OUTPUT); //laser pin as an output
  digitalWrite(Laser_Pin, LOW); //laser turned off by defualt
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB, on LEONARDO, MICRO, YUN, and other 32u4 based boards.
  }

  // Now set up two tasks to run independently.
  xTaskCreate(
    TaskSendTemp
    ,  "TaskSendTemp"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  0  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

  xTaskCreate(
    TaskReadSerial
    ,  "TaskReadSerial"
    ,  128  // Stack size
    ,  NULL
    ,  2  // Priority
    ,  NULL );

  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
}

void loop()
{
  // Empty. Things are done in Tasks.
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskSendTemp(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  for (;;) // A Task shall never return or exit.
  {
    // Call therm.read() to read object and ambient temperatures from the sensor.
    if (therm.read()) // On success, read() will return 1, on fail 0.ite(LED_BUILTIN, HIGH);   
    // turn the LED on (HIGH is the voltage level)
    {
        Serial.print("Obj:," + String(therm.object(), 2) + ","); //display the object temp with 2 decimal places
        Serial.print("Amb:," + String(therm.ambient(), 2)); //display the ambient temp with 2 decimal places
        Serial.print('\n');
    }//if
    
    vTaskDelay( 250 / portTICK_PERIOD_MS ); // delay 250 ms
  }//for loop
}//task

void TaskReadSerial(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  
  for (;;)
  {
    if(Serial.available()) //incoming serial data
    {
      incomeMessage = Serial.readString(); //read the incoming serial data into a string
      newEmissivity = incomeMessage.toFloat(); //convert the incoming serial data to float if its convertable
      if (incomeMessage.startsWith("ON")) 
      { 
    
        digitalWrite(Laser_Pin, HIGH); //turn on laser
        Serial.println("Laser on");
        Serial.println(therm.readEmissivity());

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
    
  }//main if
   
   vTaskDelay(1);  // one tick delay (15ms) in between reads for stability
    
  } //for loop
} //task
