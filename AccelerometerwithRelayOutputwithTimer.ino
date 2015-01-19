/*####################################################################
Tilt To Unlock
Alex Glover
February 2013
Combination to unlock box is Y+ (left), X+ (forward), Y+ (left), X- (back), Y+ (forward)
Based on autoCalibration.ino, provided by virtuabotix
Modified by Stephen O'Gara to include a power relay and a winning latch movement 
Adding in timer to switch off the circuit if the puzzle is not solved within a selectable time
Last opened on August 8th
Thinking about modifying program to use a solenoid for the latch
and a red and green led to indicate the latch opening 

#######################################################################*/



#include "Accelerometer.h"
#include <SoftwareSerial.h>
// import the timer library - includes Event and Timer
#include <Event.h>
#include <Timer.h>



Accelerometer myAccelerometer = Accelerometer();
int yPosPin = 11;
int yNegPin = 13;
int xPosPin = 10;
int xNegPin = 9;
int zPin = 12;
int currentMove = 1;
int resetTrigger = 0;
const int coil = 7; // the pin number for the solenoid coil

// constants won't change. They're used here to set pin numbers:
const int powerOnPin =  2;    // green ring led on momentary button LED
const int relaypin = 6;  // power relay coil , pull LOW to activate, HIGH to shut off relay 
const int winPin = 8; // win pin to light up after winning game to indicate lock is open

// timer stuff
Timer t;

void setup()
{
Serial.begin(9600);

 // initialize the LED pin as an output:
   pinMode(relaypin, OUTPUT);  // power on relay is an output
   pinMode(powerOnPin, OUTPUT); // green ring is an output
  pinMode(winPin, OUTPUT); // light to indicate box is open

 //Power up the processor 
digitalWrite(powerOnPin, HIGH); // immediately turn on green ring around button  

//Power Timer for Time Out
//timer stuff goes here
  t.pulse(relaypin, 10 * 60L * 300, LOW); // 3 minutes with power relay coil on
   // immediately pull down relaypin, turning ON relay

//Connect up the following pins and your power rail
// SL GS 0G X Y Z
myAccelerometer.begin(3, 4, 5, A0, A1, A2);

pinMode(yPosPin, OUTPUT);
pinMode(yNegPin, OUTPUT);
pinMode(xPosPin, OUTPUT);
pinMode(xNegPin, OUTPUT);
pinMode(zPin, OUTPUT);
pinMode(coil,OUTPUT); // make pin 7 an output pin

//calibration performed below
Serial.println("Please place the Accelerometer on a flat n level surface");
delay(2000);//Give user 2 seconds to comply
myAccelerometer.calibrate();

}

void loop()
{
    //timer update for next loop
  t.update();
  
delay(20);//delay for readability
resetTrigger++; //increment the reset trigger
myAccelerometer.read();
if(myAccelerometer._Zgs <= -0.9){ digitalWrite(zPin, HIGH); } else if(myAccelerometer._Zgs > -0.9){
digitalWrite(zPin, LOW);
}
if(myAccelerometer._Xgs >= 0.5){
digitalWrite(xPosPin, HIGH);
if(currentMove==2){
Serial.println("Second move successful");

currentMove++;
delay(1000);
}
else if(currentMove==5){
Serial.println("Fifth move successful - opening");

//winning series of events

 digitalWrite(coil,HIGH); // unlock box
Serial.println("pull in coil");

 //blink Winning led quickly to draw attention
  Serial.println("blink winning light quickly");

   digitalWrite(winPin, HIGH);//blink fast to indicate winning
   delay (50);
  digitalWrite(winPin,LOW);
 delay(80); 
 digitalWrite(winPin, HIGH);
   delay (50);
  digitalWrite(winPin,LOW);
 delay(80); 
 digitalWrite(winPin, HIGH);
   delay (50);
  digitalWrite(winPin,LOW);
 delay(80); 
 digitalWrite(winPin, HIGH);
   delay (50);
  digitalWrite(winPin,LOW);
 delay(80);
 digitalWrite(winPin, HIGH);
   delay (50);
  digitalWrite(winPin,LOW);
 delay(80);
 digitalWrite(winPin, HIGH);
   delay (50);
  digitalWrite(winPin,LOW);
 delay(80); 
 digitalWrite(winPin, HIGH);
   delay (50);
  digitalWrite(winPin,LOW);
 delay(80); 
 digitalWrite(winPin, HIGH);
   delay (50);
  digitalWrite(winPin,LOW);
 delay(80);
 digitalWrite(winPin, HIGH);
   delay (50);
  digitalWrite(winPin,LOW);
 delay(80);
  digitalWrite(winPin, HIGH);
   delay (50);
  digitalWrite(winPin,LOW);
 delay(80); 
 digitalWrite(winPin, HIGH);
   delay (50);
  digitalWrite(winPin,LOW);
 delay(80);
 digitalWrite(winPin, HIGH);
   Serial.println("hold coil on for a couple seconds");

   //delay (2000);

 // add a slight pause
 delay(5000);
 
// turn off solenoid to lock box
digitalWrite(coil,LOW);
Serial.println("lock box");

//then time out or continue to this shut down series
    
       // POWER SHUT OFF SERIES
    // blink the green ring around power button
    Serial.println("winning shutdown timer cycle starts");

    delay(500);
    digitalWrite(powerOnPin, LOW); // turn off green ring around button
    delay(300);
    digitalWrite(powerOnPin, HIGH); // turn on green ring around button
    delay(500);
    digitalWrite(powerOnPin, LOW); // turn off green ring around button
    delay(300);
    digitalWrite(powerOnPin, HIGH); // turn on green ring around button
    delay(500);
   digitalWrite(powerOnPin, LOW); // turn off green ring around button
    delay(300);
   Serial.println("shut off box after winning");

    //turn off power relay - pulled low to activate
    digitalWrite(relaypin, HIGH); // turns off the power on relay
    

currentMove++;
delay(1000);
}


else{
  
  
Serial.println("WRONG MOVE!");

currentMove=1;
}
}

else if(myAccelerometer._Xgs < 0.5){
digitalWrite(xPosPin, LOW);
}

if(myAccelerometer._Xgs <= -0.5)
{digitalWrite(xNegPin, HIGH); 

if(currentMove==4) {Serial.println("Fourth move successful"); 
currentMove++; delay(1000); } 

else{ Serial.println("WRONG MOVE!"); currentMove=1; } } 

else if(myAccelerometer._Xgs > -0.5){digitalWrite(xNegPin, LOW);
}

if(myAccelerometer._Ygs >= 0.5){
digitalWrite(yPosPin, HIGH);

if(currentMove==1){
Serial.println("First move successful");

currentMove++;
delay(1000);
}
else if(currentMove==3){
Serial.println("Third move successful");

currentMove++;
delay(1000);
}
else{
Serial.println("WRONG MOVE!");

currentMove=1;
}
}
else if(myAccelerometer._Ygs < 0.5){
digitalWrite(yPosPin, LOW);
}
if(myAccelerometer._Ygs <= -0.5){digitalWrite(yNegPin, HIGH);
Serial.println("WRONG MOVE!"); currentMove=1; }
else if(myAccelerometer._Ygs> -0.5){
  
digitalWrite(yNegPin, LOW);
}

if(resetTrigger==3000){ //do this every minute (3000 iterations * 0.02 seconds = 60 seconds)
currentMove=1; //reset currentMove back to 1, so user can try to do the sequence again
resetTrigger=0;//set the resetTrigger back to 0 so it can start another iteration to 3000
Serial.println("reset current move to try again");

// blink start again light
digitalWrite(winPin, HIGH);
   delay (500);
  digitalWrite(winPin,LOW);
 
}

}
