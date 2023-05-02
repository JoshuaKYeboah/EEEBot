#include <Arduino_BuiltIn.h>
#include "Wire.h"
#include "math.h"
#include <Servo.h>  //include the servo library

//********************************************************//
//*  University of Nottingham                            *//
//*  Department of Electrical and Electronic Engineering *//
//*  Nat Dacombe & Alex Ottway                           *//
//*  UoN EEEBot 2022                                     *//
//*  Motor & Servo Basic Test Code                       *//
//********************************************************//

// ASSUMPTION: Channel A is LEFT, Channel B is RIGHT

// Use this code to correctly assign the four pins to move the car forwards, backwards, clockwise & counter-clockwise
// You first need to change the pin numbers for the four motor 'IN' pins below and then decide which go HIGH and LOW in 
// each of the movements, stopMotors has been done for you
// ** marks where you need to insert the pin number or state

// feel free to modify this code to test existing or new functions

#include <Servo.h>    //include the servo library
#define servoPin 4
Servo myservo;        // create servo object to control a servo
float steeringAngle;  // variable to store the servo position

#define enA 5   //EnableA command line - should be a PWM pin
#define enB 6   //EnableB command line - should be a PWM pin

//name the motor control pins - replace the CHANGEME with your pin number, digital pins do not need the 'D' prefix whereas analogue pins need the 'A' prefix
#define INa A0  //Channel A direction 
#define INb A1  //Channel A direction 
#define INc A2  //Channel B direction 
#define INd A3  //Channel B direction 

// encoder two input pins

//////////////////////////////////////////////////////////////////////////////////////
int distance_ultrasonic;
// int raw_angle_MPU;
/////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// Maze navigation
String inputs = ""; 

#define pinA 2  // Our first hardware interrupt pin is digital pin 2
#define pinB 3 // Our second hardware interrupt pin is digital pin 3
 byte aFlag = 0; // let's us know when we're expecting a rising edge on pinA to signal that the encoder has arrived at a detent
 byte bFlag = 0; // let's us know when we're expecting a rising edge on pinB to signal that the encoder has arrived at a detent (opposite direction to when aFlag is set)
//volatile byte encoderPos = 0; //this variable stores our current value of encoder position. Change to int or uin16_t instead of byte if you want to record a larger range than 0-255
int encoderPos = 0;
//volatile byte oldEncPos = 0; //stores the last encoder position value so we can compare to the current reading and see if it has changed (so we know when to print to the serial monitor)
int oldEncPos = 0;
byte reading = 0; //somewhere to store the direct values we read from our interrupt pins before checking to see if we have moved a whole detent
float angular = 0;

int count=0 ; // for each 6 count one complete cycle
float displacement =0; //in cm 

int distance_count;
float distance ;

 void PinA(){
  cli(); //stop interrupts happening before we read pin values
  reading = PIND & 0xC; // read all eight pin values then strip away all but pinA and pinB's values
  if(reading == B00001100 && aFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    encoderPos --; //decrement the encoder's position count
    count --;
    distance_count ++;
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
  }
  else if (reading == B00000100) bFlag = 1; //signal that we're expecting pinB to signal the transition to detent from free rotation
  sei(); //restart interrupts
 }

 void PinB(){
  cli(); //stop interrupts happening before we read pin values
  reading = PIND & 0xC; //read all eight pin values then strip away all but pinA and pinB's values
  if (reading == B00001100 && bFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    encoderPos ++; //increment the encoder's position count
    count ++;
    distance_count ++;    
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
  }
  else if (reading == B00001000) aFlag = 1; //signal that we're expecting pinA to signal the transition to detent from free rotation
  sei(); //restart interrupts
 }
 //////////////////////////////////////////////////////////////////////////
 
byte speedSetting = 0;  //initial speed = 0
bool code_has_run = false;


void setup() {
  // put your setup code here, to run once:

  //I2C comuncation
  Wire.begin(8);                 // join I2C bus with address #8
  Wire.onReceive(receiveEvent);  // register event

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  pinMode(pinA, INPUT_PULLUP); // set pinA as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  pinMode(pinB, INPUT_PULLUP); // set pinB as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  attachInterrupt(0,PinA,RISING); // set an interrupt on PinA, looking for a rising edge signal and executing the "PinA" Interrupt Service Routine (below)
  attachInterrupt(1,PinB,RISING); // set an interrupt on PinB, looking for a rising edge signal and executing the "PinB" Interrupt Service Routine (below

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  myservo.attach(servoPin);  //attach our servo object to pin D4
  //the Servo library takes care of defining the PinMode declaration (libraries/Servo/src/avr/Servo.cpp line 240)

  //configure the motor control pins as outputs
  pinMode(INa, OUTPUT);
  pinMode(INb, OUTPUT);
  pinMode(INc, OUTPUT);
  pinMode(INd, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);   

  //initialise serial communication
  Serial.begin(9600);
  Serial.println("Arduino Nano is Running"); //sanity check

  speedSetting = 150;
  motors(speedSetting, speedSetting); //make a call to the "motors" function and provide it with a value for each of the 2 motors - can be different for each motor - using same value here for expedience
  Serial.print("Motor Speeds: ");
  Serial.println(speedSetting); 
}


void loop() {


  for (int i = 0; i <= inputs.length(); i++) {
    if (inputs[i] == '2') {

      char cm_char = inputs[i+1];
      int cm_int = atoi(&cm_char);
      int cm_toltal = cm_int*10;

      goForwards_for_dispacement_with_turn(cm_toltal, 84);  // go stright for 10 cm
    }

    else if (inputs[i] == '6') {
      
      turn_lift(2000);
    }

    else if (inputs[i] == '4') {

      
      turn_right(2000);
    }

   else if(inputs[i]=='0'){
       
         stopMotors();
         delay(10000);
     
   }
   else{
     stopMotors();
   }
  }
  

  
}








void motors(int leftSpeed, int rightSpeed) {
  //set individual motor speed
  //direction is set separately

  analogWrite(enA, leftSpeed);
  analogWrite(enB, rightSpeed);
}

void moveSteering(int servo_angle) {
   //you may need to change the maximum and minimum servo angle to have the largest steering motion
   int maxAngle = 110;
   int minAngle = 70;
   int midAngle = 84;
   myservo.write(servo_angle);    

   if ( minAngle > servo_angle> maxAngle) 
   {
     Serial.print("servo error");
   }
   
   
  //  for (steeringAngle = minAngle; steeringAngle <= maxAngle; steeringAngle += 1) { //goes from minAngle to maxAngle (degrees)
  //    //in steps of 1 degree
  //    myservo.write(steeringAngle);   //tell servo to go to position in variable 'steeringAngle'
  //    delay(15);                      //waits 15ms for the servo to reach the position
  //  }
  //  for (steeringAngle = maxAngle; steeringAngle >= minAngle; steeringAngle -= 1) { // goes from maxAngle to minAngle (degrees)
  //  /     myservo.write(steeringAngle);   //tell servo to go to position in variable 'steeringAngle'
  //    delay(15);                      //waits 15 ms for the servo to reach the position
  //  }
 }


//for each of the below function, two of the 'IN' variables must be HIGH, and two LOW in order to move the wheels - use a trial and error approach to determine the correct combination for your EEEBot
void goForwards() {
  digitalWrite(INa, HIGH);
  digitalWrite(INb, LOW);
  digitalWrite(INc, HIGH);
  digitalWrite(INd, LOW);
}

void goBackwards() {
  digitalWrite(INa, LOW);
  digitalWrite(INb, HIGH);
  digitalWrite(INc, LOW);
  digitalWrite(INd, HIGH);
}

// void goClockwise() {
//   digitalWrite(INa, CHANGEME);
//   digitalWrite(INb, CHANGEME);
//   digitalWrite(INc, CHANGEME);
//   digitalWrite(INd, CHANGEME);
// }

// void goAntiClockwise() {
//   digitalWrite(INa, CHANGEME);
//   digitalWrite(INb, CHANGEME);
//   digitalWrite(INc, CHANGEME);
//   digitalWrite(INd, CHANGEME);
// }

void stopMotors() {
  digitalWrite(INa, LOW);
  digitalWrite(INb, LOW);
  digitalWrite(INc, LOW);
  digitalWrite(INd, LOW);
}




/*******Interrupt-based Rotary Encoder Sketch*******
by Simon Merrett, based on insight from Oleg Mazurov, Nick Gammon, rt, Steve Spence
*/
void encoder()
{


 
  if(oldEncPos != encoderPos) {
    
    angular = 1.05*count;
    displacement = 3*angular;
    distance = 3*(1.05*distance_count);    
    Serial.print("angule: ");
    Serial.println(angular);
    Serial.print("count: ");
    Serial.println(count);
    Serial.print("dispalcement: ");
    Serial.println(displacement);
    Serial.print("distance: ");
    Serial.println(distance);
    oldEncPos = encoderPos;

   //return displacement;

        
  }
}

void goForwards_for_dispacement_with_turn(int d, int servo_angle)
{
  
  reset_encoder();
  while ( d> displacement)
    {
  encoder();
  moveSteering(servo_angle);
  digitalWrite(INa, HIGH);
  digitalWrite(INb, LOW);
  digitalWrite(INc, HIGH);
  digitalWrite(INd, LOW);

 
  
    } 
    
  // stopMotors();
  // delay(500);
    
   

}


void goBackwards_for_dispacement_with_turn(int d, int servo_angle)
{
    
    reset_encoder();
  while( d < displacement)  //the d will be in nigative becouse the displacement will be negative
    {
  encoder();
  moveSteering(servo_angle);
  digitalWrite(INa, LOW);
  digitalWrite(INb, HIGH);
  digitalWrite(INc, LOW);
  digitalWrite(INd, HIGH);
  
  
    } 

    

    
    
}
void turn_right( int time)
{
  moveSteering(100);
  motors(200, 200);
  digitalWrite(INa, HIGH);
  digitalWrite(INb, LOW);
  digitalWrite(INc, LOW);
  digitalWrite(INd, HIGH);
  delay(time);
}

void turn_lift( int time)
{
  moveSteering(80);
  motors(200, 200);
  digitalWrite(INa, LOW);
  digitalWrite(INb, HIGH);
  digitalWrite(INc, HIGH);
  digitalWrite(INd, LOW);
  delay(time);
}

void reset_encoder()
{
 aFlag = 0; // let's us know when we're expecting a rising edge on pinA to signal that the encoder has arrived at a detent
 bFlag = 0; // let's us know when we're expecting a rising edge on pinB to signal that the encoder has arrived at a detent (opposite direction to when aFlag is set)
//volatile byte encoderPos = 0; //this variable stores our current value of encoder position. Change to int or uin16_t instead of byte if you want to record a larger range than 0-255
 encoderPos = 0;
//volatile byte oldEncPos = 0; //stores the last encoder position value so we can compare to the current reading and see if it has changed (so we know when to print to the serial monitor)
oldEncPos = 0;
 reading = 0; //somewhere to store the direct values we read from our interrupt pins before checking to see if we have moved a whole detent
 angular = 0;

 count=0 ; // for each 6 count one complete cycle
 displacement =0; //in cm 

 distance_count;
 distance ;
}


// void receiveEvent(int howMany) {
//   char c;
//   while (1 < Wire.available()) { // loop through all but the last
//    c = Wire.read(); // receive byte as a character
//   Serial.print(c);         // print the character
//   }
//   if(c=='d')
//   {
//   distance_ultrasonic = Wire.read();
//   Serial.print("distance is :");
//   Serial.println(distance_ultrasonic);
//   Serial.print("\n");
  
//   }

// }

void receiveEvent(int howMany)
{
  inputs= "";
  while(Wire.available()){
  inputs +=(char) Wire.read();
  }

}
