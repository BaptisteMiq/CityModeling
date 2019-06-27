#include "Grove_I2C_Motor_Driver.h"
#include "Car.h"
#include "Pathfinder.h"
#include <VirtualWire.h>
//Emitter
int RF_TX_PIN = 8;
int id = 0;

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
int echoPin = 9;
long lectureEcho;

int distanceEspace = 0;
float ms;
String string;
float metre;
const unsigned long MEASURE_TIMEOUT = 25000UL; // 25ms = ~8m à 340m/s

double prevTimeDist = millis();

void setup() {
  Serial.begin(9600);

  vw_set_tx_pin(RF_TX_PIN); 
  vw_setup(2000); 
      
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

  pathfinding = true;

  int startNode = 0;
  int endNode = 15;

  Pathfinder* p = car->pathfinder;
  p->initNodes();

  if(pathfinding) {
    car->pathfinder->dijkstra(endNode, startNode);
    Serial.println(car->pathfinder->dirs);
    Serial.println("Starting pathfinding");
    car->readDirs();
  }

  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  pinMode(echoPin, INPUT);
}

int prevMil = millis();
void loop() {

  initDirs();

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
  if (millis() - timeold >= 1000) {
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
  }

  if(millis() - prevMil > 100) {
    prevMil = millis();
    espace(getDistance()); 
  }
  
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
  lectureEcho = pulseIn(echoPin, HIGH, MEASURE_TIMEOUT);
  int distCm = lectureEcho / 58;
  return distCm;
}

int onWall = false;
void espace(int distCm){

  if(distCm < 8 && !onWall && millis() - prevTimeDist > 100 && car->canMeasure) {
    onWall = true;
    double intervalTime = millis() - prevTimeDist;
    prevTimeDist = millis();
    //Serial.println(String(ms));
    ms = 3;
    metre = ((ms/10) * (intervalTime/1000))*100;
    if(metre < 15 || metre > 100) return;
    Serial.println(ms);
    metre = ((ms/10) * (intervalTime/1000))*100;
    String distanceStockees = String(id) + " " + String(metre);
    Serial.print("Espace = ");

    String msg = "placeDist";
    msg = msg + ":" + String(metre);
    sendMessage(msg.c_str());
    
    Serial.println(distanceStockees);
    distanceEspace = 0;
    intervalTime = 0;
  }
  if(distCm < 8) {
    prevTimeDist = millis();
    Serial.println("reset");
  }
  if(distCm >= 8) {
    onWall = false;
  }
}

void sendMessage(const char* msg){

    //msg = "Test de la trame !!!!!!!!!!!!!!!";
    int idVehicle = 155;
    char message[strlen(msg)+1];
    int conversion = 0;
    int key = 5;

    message[0] = (char) idVehicle;
    for(int i=0; i < strlen(msg); ++i)
    {
      conversion = (int) msg[i] + key;
      message[i+1] = (char) conversion;
    }
    vw_send((uint8_t *)message, strlen(msg)+1);

    car->stopAll();
    vw_wait_tx();
    car->moveAll(1);
    
}
