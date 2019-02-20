/*Abedallah Abedrabbah
 *CL
 *
 *Capstone
 *Slave Robot One Software to run
 *
 *Values for line sensors:
 *sensorMiddle >= 85
 *sensorLeft >= 89
 *sensorRight >= 91
 *
 *Ping sensor value:
 *max = 25
 *min = 0
 *echo = 7
 *trig = 6
 *Starttime pulses = 330
 */

//PROGRAM TO DRIVE ROBOT THROUGH COURSE WITH LINE SENSORS AND PING SENSORS
#include <Servo.h>
#include <ArduinoJson.h>
// We'll use SoftwareSerial to communicate with the XBee:
#include <SoftwareSerial.h>
SoftwareSerial Xbee(2, 3); // TX, RX
#define echoPin 6 // Echo Pin
#define trigPin 7 // Trigger Pin
#define LEDPin 13 // Onboard LED
uint8_t encoder = 8;  //is set high or low from  

int MasterControl;
Servo rightServo;
Servo leftServo;
//For Motor
int angle = 0;   // servo position in degrees

//For Ping Sensor
int maximumRange = 15; // Maximum range needed
int minimumRange = 0;  // Minimum range needed
double duration;       // Duration used to calculate distance
int distance;
int sensorMiddle;      //reads the input on analog pin 1
int sensorRight;       //reads the input on analog pin 2
int sensorLeft;        //reads the input on analog pin 1
int count = 0;
int pulses;
char json[200];
 
// Install Pin change interrupt for a pin, can be called multiple times
 
void pciSetup(byte pin) 
{
    *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));  // enable pin
    PCIFR  |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
    PCICR  |= bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group 
}
 
// Use one Routine to handle each group
 
ISR (PCINT0_vect) // handle pin change interrupt for D8 to D13 here
 {    
     count++;
 } 
void getMasterControl(){
  Xbee.readString().toCharArray(json,200);   
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  MasterControl = root["action"];
  pulses = root["pulses"];
  pulses = pulses + 325;
}
void setup()
{
  rightServo.attach(5); //pin 5
  leftServo.attach(4); //pin 4
  Xbee.begin(9600);
  
  pciSetup(encoder);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LEDPin, OUTPUT);
}
void loop()
{     
      getMasterControl();         
top:  distance = ObjectPresent();
      sensorRight = analogRead(A2);  //reads the input on analog pin 2
      sensorLeft = analogRead(A0);   //reads the input on analog pin 1
      
  
  //Robot is on track, drive straight    
              if(MasterControl != 50 && MasterControl != 51){
                  if(distance > maximumRange && distance > minimumRange){   
                     if (sensorRight < 600 && sensorLeft < 600){
                        moveForward();
                 
                     }//LINE SENSORS
                     else if (sensorRight >= 600) //Line sensor middle hit line
                     {   
                        lineSensorRight();
                        delay(70);
                        moveForward();
                     }
                    else if (sensorLeft >= 600) //Line sensor middle hit line
                    {
                        lineSensorLeft();
                        delay(70);
                         moveForward();
                    }
                }else if( distance < maximumRange && distance > minimumRange){
                    stopMotor();
                 
                }
               }else if(MasterControl == 50){     //Master Robot tells slave to turn left
                  if(count >= pulses){
                    leftAngle();
   
                  }else{
                     if(distance > maximumRange && distance > minimumRange){   
                     if (sensorRight < 600 && sensorLeft < 600){
                        moveForward();
                 
                     }//LINE SENSORS
                     else if (sensorRight >= 600) //Line sensor middle hit line
                     {   
                        lineSensorRight();
                        delay(70);
                        moveForward();
                     }
                    else if (sensorLeft >= 600) //Line sensor middle hit line
                    {
                        lineSensorLeft();
                        delay(70);
                         moveForward();
                    }
                }else if( distance < maximumRange && distance > minimumRange){
                    stopMotor();
                 
                }
                    goto top;
                    
                  }
               }
               else if(MasterControl == 51){     //Master Robot tells slave to turn right
                  if(count >= pulses){
                    rightAngle(); //Turn car right
                   
                  }else{
                     if(distance > maximumRange && distance > minimumRange){   
                     if (sensorRight < 600 && sensorLeft < 600){
                        moveForward();
                 
                     }//LINE SENSORS
                     else if (sensorRight >= 600) //Line sensor middle hit line
                     {   
                        lineSensorRight();
                        delay(70);
                        moveForward();
                     }
                    else if (sensorLeft >= 600) //Line sensor middle hit line
                    {
                        lineSensorLeft();
                        delay(70);
                         moveForward();
                    }
                }else if( distance < maximumRange && distance > minimumRange){
                    stopMotor();
                 
                }
                    goto top;
                  }     
               }

               

}
        

//------------------------------------------------------------------------------------------
// Methods to call during loop
//------------------------------------------------------------------------------------------
void moveForward() //Drive forward
{
  rightServo.write(84);
  leftServo.write(84);
  delay(100);
}
void moveBackward() //Drive backwards
{
  rightServo.write(105);
  leftServo.write(105);
  delay(100);
}
void turnRight() //Turn car right
{
  rightServo.write(0);
  leftServo.write(75);
  delay(30);
}
void turnLeft() //Turn car left
{
  rightServo.write(75);
  leftServo.write(0);
  delay(30);
}
void stopMotor() //Stop the car
{
  rightServo.write(0);
  leftServo.write(0);
  delay(30);
}

void lineSensorRight() //Line sensor right turns car left
{
 
  for (angle = 0; angle < 1; angle++)
  {
    turnLeft();
  }
}
void lineSensorLeft() //Line sensor left turns car right
{
 
  for (angle = 0; angle < 1; angle++)
  {
    turnRight();
  }
}
void rightAngle(){
  for (angle = 0; angle <26; angle++)
      {
        turnRight();
      }
      delay(50);
}

void leftAngle(){
  for (angle = 0; angle < 24; angle++)
      {
          rightServo.write(75);
          leftServo.write(105);
          delay(30);
      }
      delay(50);
}

void lineBackup(){
  for (angle = 0; angle < 7; angle++)
      {
        moveBackward();
      }
      delay(50);
}

//Compute distance of object
int ObjectPresent(){  
    analogWrite(trigPin,175);
    duration = pulseIn(echoPin, HIGH);
    return duration / 58.2;
}
