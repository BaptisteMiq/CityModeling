#include "Grove_I2C_Motor_Driver.h"
#include "Car.h"
#include "Pathfinder.h"

// Motor
#define I2C_ADDRESS 0x0f
int motorSpeed;

// Line reader
int leftPin = 2;
int rightPin = 7;
int middleLeftPin = 6;
int middleRightPin = 5;

bool left = false;
bool right = false;
bool middleLeft = false;
bool middleRight = false;

// Motor speed detector
int speedPin = 3;
int rpm;
volatile byte pulses; // Number of pulses
unsigned long timeold;

// number of pulses per revolution
// based on encoder disc
unsigned int pulsesperturn = 20;

bool pathfinding = false;

int ledLeft = 12;
int ledRight = 4;

Car* car;

int trigPin = 13;
int echoPin = 8;
long lectureEcho;

int distanceEspace = 0;
float ms;
String string;
float metre;

void setup() {
  Serial.begin(9600);
  Motor.begin(I2C_ADDRESS);

  car = new Car();

  Serial.println("Motor started");

  pinMode(ledLeft, OUTPUT);
  pinMode(ledRight, OUTPUT);

  // Motor speed
  pinMode(speedPin, INPUT);
  //Interrupt 0 is digital pin 2
  //Triggers on Falling Edge (change from HIGH to LOW)
  attachInterrupt(1, counter, FALLING);
  // Initialize
  pulses = 0;
  rpm = 0;
  timeold = 0;
  Serial.println("Speed counter started");

  car->stopAll();

  Serial.println("Setup finished");

  pathfinding = false;

  int startNode = 0;
  int endNode = 15;

  Pathfinder* p = car->pathfinder;
  p->initNodes();

  if(pathfinding) {
    car->pathfinder->dijkstra(endNode, startNode);
        Serial.println(car->pathfinder->dirs);

    car->readDirs();
  }

  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  pinMode(echoPin, INPUT);
}

int prevMil = millis();
void loop() {

  initDirs();

  if(millis() - prevMil > 100) {
    prevMil = millis();
    espace(getDistance()); 
  }

  car->flashingCounter++;
  if(car->flashingCounter > car->flashingSpeed) {
    car->flashingCounter = 0;
  }

  if(car->flashingCounter > car->flashingSpeed/2 && car->flashingLeft) {
    digitalWrite(ledLeft, HIGH);
  } else if(car->flashingCounter > car->flashingSpeed/2 && car->flashingRight) {
    digitalWrite(ledRight, HIGH);
  } else {
    digitalWrite(ledRight, LOW);
    digitalWrite(ledLeft, LOW);
  }

  if(!pathfinding) {
    car->moveArround(left, right, middleLeft, middleRight);
  } else {
    car->movePattern(left, right, middleLeft, middleRight);
  }

  // Get Rpm
  //if (millis() - timeold >= 1000) {
    detachInterrupt(1);
    rpm = (60000 / pulsesperturn ) / (millis() - timeold) * pulses;
    ms = ((2*3.14*0.0325)/60) * rpm;
    string = String(ms);
    timeold = millis();
    pulses = 0;
    /*Serial.print(string);
    Serial.print(" : ");
    Serial.println(rpm);*/
    attachInterrupt(1, counter, FALLING);
  //}
  
}

void counter()
{
  pulses++;
}

int getRpm() {
  if (millis() - timeold >= 1000) {
    detachInterrupt(1);
    rpm = (60000 / pulsesperturn ) / (millis() - timeold) * pulses;
    timeold = millis();
    pulses = 0;
    //Serial.println(rpm);
    attachInterrupt(1, counter, FALLING);
  }
  return rpm;
}

void initDirs() {
  left = digitalRead(leftPin) == HIGH;
  right = digitalRead(rightPin) == HIGH;
  middleLeft = digitalRead(middleLeftPin) == HIGH;
  middleRight = digitalRead(middleRightPin) == HIGH;
}

int getDistance() {
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  lectureEcho = pulseIn(echoPin, HIGH);
  int distCm = lectureEcho / 58;
  return distCm;
}

String distancesStockees[1];
void espace(int distCm){
  if(distCm < 8 && distanceEspace > 0){
    ms = 7;
    Serial.print(distanceEspace);
    Serial.print(" : ");
    Serial.println(ms);
    metre = (((ms * distanceEspace) / 10) / 100 + 4);
    distancesStockees[0] = String(metre);
    Serial.print("Espace = ");
    Serial.println(distancesStockees[0]);
    distanceEspace = 0;
    }
  else if(distCm >= 8) {
   distanceEspace += 100;
  } 
}
