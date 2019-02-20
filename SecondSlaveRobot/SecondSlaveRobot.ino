
/*Abedallah Abedrabbah
 *
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
 */

//PROGRAM TO DRIVE ROBOT THROUGH COURSE WITH LINE SENSORS AND PING SENSORS
#include <PWMServo.h>
// We'll use SoftwareSerial to communicate with the XBee:
#include <AltSoftSerial.h>
#include <EnableInterrupt.h>
AltSoftSerial XBee(2, 3); // TX, RX
#define echoPin 7 // Echo Pin
#define trigPin 6 // Trigger Pin
#define LEDPin 13 // Onboard LED
uint8_t encoder = 8;  //is set high or low from  
char MasterControl;
PWMServo rightServo;
PWMServo leftServo;
//For Motor
int angle = 0;   // servo position in degrees
//For Ping Sensor
int maximumRange = 25; // Maximum range needed
int minimumRange = 0;  // Minimum range needed
double duration;       // Duration used to calculate distance
int distance;
int sensorMiddle;      //reads the input on analog pin 1
int sensorRight;       //reads the input on analog pin 2
int sensorLeft;        //reads the input on analog pin 1
int counter;
int count = 0;
void setup()
{
  rightServo.attach(5); //pin 5
  leftServo.attach(4); //pin 4
  XBee.begin(9600);
  Serial.begin(9600);
  
  counter = 0;
  pinMode(encoder, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LEDPin, OUTPUT);
  enableInterrupt(encoder, increment, CHANGE); // interrupt connected t
}
void loop()
{ 
  sensorRight = analogRead(A2);  //reads the input on analog pin 2
  sensorLeft = analogRead(A0);   //reads the input on analog pin 1
  //Robot is on track, drive straight
   
      
        MasterControl = XBee.read();  
        
        if(MasterControl == '0'){
          if (sensorRight > 83 && sensorLeft > 83)
          {
            delay(70);
            moveForward();
          
          }//LINE SENSORS
          else if (sensorRight <= 83) //Line sensor middle hit line
          {
            
            lineSensorRight();
            delay(70);
            moveForward();
            
          }
          else if (sensorLeft <= 83) //Line sensor middle hit line
          {
            
            lineSensorLeft();
            delay(70);
            moveForward();
          
          }
        }
        else if(MasterControl == '1'){
          stopMotor();
        }
        else if(MasterControl == '2'){     //Master Robot tells slave to turn left
          if(counter == 0){
            delay(60);
            counter++;
          }
          else if(counter == 1){
            counter = 0;
            delay(55);
          }
          
          leftAngle();
        }
        else if(MasterControl == '3'){     //Master Robot tells slave to turn right
          delay(80);
          rightAngle(); //Turn car right
        }
        
      
 
}
//------------------------------------------------------------------------------------------
// Methods to call during loop
//------------------------------------------------------------------------------------------
void moveForward() //Drive forward
{
  rightServo.write(86);
  leftServo.write(84.5);
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
  for (angle = 0; angle < 32; angle++)
      {
        turnRight();
      }
      delay(50);
}

void leftAngle(){
  for (angle = 0; angle < 29; angle++)
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
void increment(){
  count++;
  Serial.println(count);
}
/*left just incase we found a use for a line sensor and ping sensor
 * void lineSensorMiddle() //Line sensor middle stops the car
{
  for (angle = 0; angle < 7; angle++)
  {
    stopMotor();
  }
}
//Compute distance of object
int ObjectPresent(){  
    analogWrite(trigPin,175);
    duration = pulseIn(echoPin, HIGH);
    return duration / 58.2;
}*/


