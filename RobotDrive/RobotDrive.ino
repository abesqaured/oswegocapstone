//PROGRAM TO DRIVE ROBOT THROUGH COURSE WITH LINE SENSORS AND PING SENSORS
#include <Servo.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
// XBee's DOUT (TX) is connected to pin 2 (Arduino's Software RX)
// XBee's DIN (RX) is connected to pin 3 (Arduino's Software TX)
SoftwareSerial Xbee(2, 3); // TX, RX

#define echoPin 7  // Echo Pin
#define trigPin 6  // Trigger 
#define encoder 8  //is set high or low from  
#define LEDPin 13  // Onboard LED
Servo rightServo;  //Initialize the right motor
Servo leftServo;   //Initialize the left motot
//to get information from Xbee
char received;
//For Motor
int angle = 0;                            // Servo position in degrees
int count = 0;
//For Ping Sensor
int maximumRange = 20;                    // Maximum range needed
int minimumRange = 0;                     // Minimum range needed
double duration;                          // Duration used to calculate distance
int distance;                             // Distance used for ping sensor
int sensorMiddle;                         //reads the input on analog pin 1
int sensorRight;                          //reads the input on analog pin 2
int sensorLeft;                           //reads the input on analog pin 3
// Install Pin change interrupt for a pin, can be called multiple times
int start = 0;
int finish = 0;
int difference = 0;

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
void sendToSlave(char command){
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["action"] = command;
  root["pulses"] = count;
  root.printTo(Xbee);
}
void setup()
{
  rightServo.attach(5);                    //pin 5
  leftServo.attach(4);                     //pin 4
  
  Xbee.begin(9600);
  pciSetup(encoder);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LEDPin, OUTPUT);
}
void loop()
{  
   distance = ObjectPresent();             //Gets value of ultrasonic sensor
   
   sensorMiddle = analogRead(A1);          //reads the input on analog pin 1
   sensorRight = analogRead(A2);           //reads the input on analog pin 2
   sensorLeft = analogRead(A0);            //reads the input on analog pin 3
   
  if(distance > maximumRange && distance > minimumRange ){
      if(sensorMiddle < 600 && sensorLeft < 600 &&  sensorRight < 600){
        
        moveForward();
      }  //Line Sensors detected
      else if (sensorMiddle > 600)            //Line sensor middle hit line, stop car
      {
        lineSensorMiddle();
      }
      else if (sensorRight > 600)               //Line sensor right hit line, turn left
      {
        lineSensorRight();
      } 
      else if (sensorLeft > 600)                //Line sensor left hit line, turn right
      {
        lineSensorLeft();
      }
    }else if( distance < maximumRange && distance > minimumRange){
      rightAngle();
    }
}
//------------------------------------------------------------------------------------------
// Methods to call during loop
//------------------------------------------------------------------------------------------
void moveForward() //Drive forward
{ 
  rightServo.write(83.9);
  leftServo.write(84.7);
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
  
  rightServo.write(105);
  leftServo.write(70);
  delay(50);
}
void lineRight()
{
  rightServo.write(75);
  leftServo.write(0);
  delay(30);
}
void lineLeft()
{
  rightServo.write(0);
  leftServo.write(75);
  delay(30);
}
void turnLeft() //Turn car left
{
  rightServo.write(65);
  leftServo.write(0);
  delay(50);
}
void stopMotor() //Stop the car
{
  // sendToSlave('1');
  rightServo.write(0);
  leftServo.write(0);
  delay(30);

}
void lineSensorMiddle() //Line sensor middle stops the car
{
   sendToSlave('2');
   start = count;
   for (angle = 0; angle < 8; angle++)
      {
        moveBackward();
      }
   finish = count;
   difference = start - finish;
   count = count - difference;
      leftAngle();
      delay(50);
}
void lineSensorRight() //Line sensor right turns car left
{
  for (angle = 0; angle < 1; angle++)
  {
    lineRight();
  }
  
}
void lineSensorLeft() //Line sensor left turns car right
{
  for (angle = 0; angle < 1; angle++)
  {
    lineLeft();
  }
  
}
void rightAngle(){    //PING SENSOR TURN
 
  for (angle = 0; angle < 14; angle++)
      {
        turnRight();
      }
      delay(50);
       sendToSlave('3');
}
void leftAngle(){   //MIDDLE LINE SENSOR TURN LEFT
  
  for (angle = 0; angle < 10.5; angle++)
      {
        rightServo.write(70);
        leftServo.write(105);
        delay(50);
      }
      delay(50);
       
}
int ObjectPresent()
 {  
    analogWrite(trigPin,175);
    duration = pulseIn(echoPin, HIGH);
    return duration / 58.2;
    
  }

