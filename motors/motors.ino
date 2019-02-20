/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

Servo leftServo;  // create servo object to control a servo
Servo rightServo;
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
int currentPos;

void setup() {
  leftServo.attach(5);  // attaches the servo on pin 9 to the servo object
  rightServo.attach(4);
}

void loop() {
  for (pos = currentPos; pos <= 270; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    
    leftServo.write(pos);              // tell servo to go to position in variable 'pos'
    rightServo.write(pos);
    delay(5000);                       // waits 15ms for the servo to reach the position
  }
 
}
