#include "WProgram.h"
int main(){ init(); setup(); for(;;) {loop();} }
const int start      = 6;    // switch input
const int ledPinR    = 2;
const int ledPinO    = A0;
const int ledPinY    = A1;
const int ledPinG    = A2;
const int ledPinB    = A3;
const int wink       = 13;

const int motor1Pin  = 8;    // H-bridge leg 1 (pin 2, 1A)
const int motor2Pin  = 7;    // H-bridge leg 2 (pin 7, 2A)
const int enablePin  = 9;    // H-bridge enable pin
const int ledPin     = 13;      // LED

const int motor1PinB = 11;    // H-bridge leg 2 (pin 15, 4A)
const int motor2PinB = 12;    // H-bridge leg 1 (pin 10, 3A)
const int enablePinB = 10;    // H-bridge enable pin
                              //PWM = 11,10,9,6,5,3
void setup();
void loop();
void forward();
void reverse();
void left();
void right();
void stopmotors();
void my_delay(int);

int dir;
int dur;
int counter;
boolean eyes;

void my_delay(int duration){
  duration /= 100;
  for (int j=0; j<duration; j++){
    digitalWrite(wink, eyes);
    eyes = ! eyes;
    delay(100);
  }
}

void setup() {
  // set the switch as an input:
  pinMode(start, INPUT);

  // set all the other pins you're using as outputs:
  pinMode(motor1Pin, OUTPUT);
  pinMode(motor2Pin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(motor1PinB, OUTPUT);
  pinMode(motor2PinB, OUTPUT);
  pinMode(enablePinB, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(ledPinR, OUTPUT);
  pinMode(ledPinO, OUTPUT);
  pinMode(ledPinY, OUTPUT);
  pinMode(ledPinG, OUTPUT);
  pinMode(ledPinB, OUTPUT);
  pinMode(wink, OUTPUT);

  // set enablePin high so that motor can turn on:
  digitalWrite(enablePin, HIGH);
  digitalWrite(enablePinB, HIGH);

  counter = 0;

  my_delay(2000);
  digitalWrite(wink,HIGH);
}



void loop() {
  counter += 5;

  // wait for start button to be pressed
  dir = random(1,7);
  dur = random(2,10);

  switch(dir){
    case 5:
    case 1: forward();
      dur *= 10 + counter;
    break;
    case 2: left();
      dur *= 100;
    break;
    case 3: right();
      dur *= 100;
    break;
    case 6:
    case 4: reverse();
      dur *= 10 + counter;
    break;
  }

  my_delay(dur);
  stopmotors();
  my_delay(100);
}

void forward(){
  digitalWrite(motor1Pin, LOW);   // set leg 1 of the H-bridge low
  digitalWrite(motor2Pin, HIGH);  // set leg 2 of the H-bridge high
  digitalWrite(motor1PinB, LOW);   // set leg 1 of the H-bridge low
  digitalWrite(motor2PinB, HIGH);  // set leg 2 of the H-bridge high

  digitalWrite(ledPinB, HIGH);
  analogWrite(ledPinR, 0);
  analogWrite(ledPinO, 0);
  analogWrite(ledPinY, 0);
  analogWrite(ledPinG, 0);
}

void reverse(){
  digitalWrite(motor2Pin, LOW);   // set leg 1 of the H-bridge low
  digitalWrite(motor1Pin, HIGH);  // set leg 2 of the H-bridge high
  digitalWrite(motor2PinB, LOW);   // set leg 1 of the H-bridge low
  digitalWrite(motor1PinB, HIGH);  // set leg 2 of the H-bridge high
  analogWrite(ledPinR, 255);
  analogWrite(ledPinO, 0);
  analogWrite(ledPinY, 0);
  analogWrite(ledPinG, 0);
  digitalWrite(ledPinB, LOW);
}


void left(){
  digitalWrite(motor2Pin, LOW);   // set leg 1 of the H-bridge low
  digitalWrite(motor1Pin, HIGH);  // set leg 2 of the H-bridge high
  digitalWrite(motor1PinB, LOW);   // set leg 1 of the H-bridge low
  digitalWrite(motor2PinB, HIGH);  // set leg 2 of the H-bridge high
  analogWrite(ledPinY, 255);
  analogWrite(ledPinR, 0);
  analogWrite(ledPinO, 0);
  analogWrite(ledPinG, 0);
  digitalWrite(ledPinB, LOW);
}


void right(){
  digitalWrite(motor1Pin, LOW);   // set leg 1 of the H-bridge low
  digitalWrite(motor2Pin, HIGH);  // set leg 2 of the H-bridge high
  digitalWrite(motor2PinB, LOW);   // set leg 1 of the H-bridge low
  digitalWrite(motor1PinB, HIGH);  // set leg 2 of the H-bridge high
  analogWrite(ledPinG, 255);
  analogWrite(ledPinR, 0);
  analogWrite(ledPinO, 0);
  analogWrite(ledPinY, 0);
  digitalWrite(ledPinB, LOW);
}

void stopmotors(){
  digitalWrite(motor1Pin, LOW);   // set leg 1 of the H-bridge low
  digitalWrite(motor2Pin, LOW);  // set leg 2 of the H-bridge high
  digitalWrite(motor2PinB, LOW);   // set leg 1 of the H-bridge low
  digitalWrite(motor1PinB, LOW);  // set leg 2 of the H-bridge high
  analogWrite(ledPinR, 255);
  analogWrite(ledPinO, 255);
  analogWrite(ledPinY, 255);
  analogWrite(ledPinG, 255);
  digitalWrite(ledPinB, HIGH);
}
