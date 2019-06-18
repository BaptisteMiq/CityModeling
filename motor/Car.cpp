#include "Car.h"
#include "Pathfinder.h"
#include "Grove_I2C_Motor_Driver.h"
#define I2C_ADDRESS 0x0f

Car::Car() {
  _motorSpeed = 70;
  pulseTurnLeft = 0;
  pulseTurnRight = 0;
  pulseTurnArround = 0;
  allowTurnLeft = false;
  allowTurnRight = false;
  allowTurnArround = false;
  recCounter = 0;
  allowRecLeft = false;
  allowRecRight = false;
  flashingCounter = 0;
  flashingSpeed = 20;
  flashingLeft = false;
  flashingRight = false;
  dirsCursor = 0;
  hasFinishedPattern = false;
  carAngle = 0;

  pathfinder = new Pathfinder();
  pathfinder->setCar(this);
}

int Car::getMotorSpeed() {
  return _motorSpeed;
}

void Car::setMotorSpeed(int speed) {
  _motorSpeed = speed;
}

void Car::turnLeft() {
  setMotorSpeed(100);
  moveLeft(-1);
  moveRight(1);
  setMotorSpeed(70);
}

void Car::turnRight() {
  setMotorSpeed(100);
  moveRight(-1);
  moveLeft(1);
  setMotorSpeed(70);
}

void Car::moveAllLittle(int dir) {
  moveAll(dir);
  delay(160);
  stopAll();
  delay(100);
}

void Car::moveAll(int dir) {
  Motor.speed(MOTOR1, getMotorSpeed() * dir * -1);
  Motor.speed(MOTOR2, getMotorSpeed() * dir * -1);
}

void Car::moveLeft(int dir) {
  Motor.speed(MOTOR2, getMotorSpeed() * dir * -1);
}

void Car::moveRight(int dir) {
  Motor.speed(MOTOR1, getMotorSpeed() * dir * -1);
}

void Car::stopAll() {
  Motor.stop(MOTOR1);
  Motor.stop(MOTOR2);
}

void Car::stopLeft() {
  Motor.stop(MOTOR2);
}

void Car::stopRight() {
  Motor.stop(MOTOR1);
}


void Car::moveArround(bool left, bool right, bool middleLeft, bool middleRight) {

  if (left && right  && middleLeft && middleRight) {
    stopAll();
    return;
  }

  if ((allowTurnLeft) & ! (allowTurnRight)) {
    turnLeft();
  } else if ((allowTurnRight) & ! (allowTurnLeft)) {
    turnRight();
  } else
  if (left & ! (allowTurnRight || allowTurnLeft)) {
    moveAllLittle(1);
    allowTurnRight = false;
    allowTurnLeft = true;
  } else
  if (right & ! (allowTurnRight || allowTurnLeft)) {
    moveAllLittle(1);
    allowTurnLeft = false;
    allowTurnRight = true;
  } else
  if (allowRecLeft) {
    turnLeft();
  } else
  if (allowRecRight) {
    turnRight();
  }
  else
  if (middleLeft) {
    allowRecLeft = true;
  } else
  if (middleRight) {
    allowRecRight = true;
  }
  else {
    moveAll(1);
  }

  recalib();

  if (allowTurnLeft) {
    flashingLeft = true;
    pulseTurnLeft++;
    if (pulseTurnLeft > 10) {
      if(middleLeft) {
        pulseTurnLeft = 0;
        allowTurnLeft = false;
        flashingLeft = false;
      }
    }
  }

  if (allowTurnRight) {
    flashingRight = true;
    pulseTurnRight++;
    if (pulseTurnRight > 10) {
      if(middleRight) {
        pulseTurnRight = 0;
        allowTurnRight = false;
        flashingRight = false;
      }
    }
  }
}

void Car::recalib() {
  if (allowRecLeft || allowRecRight) {
    recCounter++;
    if (recCounter > 1) {
      recCounter = 0;
      allowRecLeft = false;
      allowRecRight = false;
    }
  }
}

void Car::movePattern(bool left, bool right, bool middleLeft, bool middleRight) {

  if(hasFinishedPattern) {
    return;
  }

  if ((allowTurnLeft) & ! (allowTurnRight)) {
    turnLeft();
  } else if ((allowTurnRight) & ! (allowTurnLeft)) {
    turnRight();
  } else if ((allowTurnArround) & ! (allowTurnLeft || allowTurnRight)) {
    turnRight();
  } else
  if ((left || right) & ! (allowTurnRight || allowTurnLeft)) {
    readDirs();
  } else
  if (allowRecLeft) {
    turnLeft();
  } else
  if (allowRecRight) {
    turnRight();
  }
  else
  if (middleLeft) {
    allowRecLeft = true;
  } else
  if (middleRight) {
    allowRecRight = true;
  }
  else {
    moveAll(1);
  }

  recalib();

  if (allowTurnLeft) {
    pulseTurnLeft++;
    if (pulseTurnLeft > 10) {
      if(middleLeft) {
        Serial.println("Gauche arret");
        pulseTurnLeft = 0;
        allowTurnLeft = false;
      }
    }
  }

  if (allowTurnRight) {
    pulseTurnRight++;
    if (pulseTurnRight > 10) {
      if(middleRight) {
        Serial.println("Droite arret");
        pulseTurnRight = 0;
        allowTurnRight = false;
      }
    }
  }  

  if (allowTurnArround) {
    pulseTurnArround++;
    if (pulseTurnArround > 50) {
      if(middleRight) {
        pulseTurnArround = 0;
        allowTurnArround = false;
      }
    }
  }  
}

void Car::readDirs() {
    char dir = pathfinder->dirs.charAt(dirsCursor);
    Serial.print(dirsCursor);
    Serial.print(" : ");
    Serial.println(dir);

    flashingLeft = false;
    flashingRight = false;
      
    if(pathfinder->dirs.charAt(dirsCursor + 1) == '1') {
      flashingLeft = true;
    } else if(pathfinder->dirs.charAt(dirsCursor + 1) == '2') {
      flashingRight = true;
    }

    if(dir == '0') {
      dirsCursor++;
      moveAll(1);
      delay(100);
    } else if(dir == '1') {
      moveAllLittle(1);
      allowTurnRight = false;
      allowTurnLeft = true;
      turnLeft();
      dirsCursor++;
    } else if(dir == '2') {
      moveAllLittle(1);
      allowTurnRight = true;
      allowTurnLeft = false;
      dirsCursor++;
    } else if(dir == '3') {
      allowTurnArround = true;
      allowTurnRight = false;
      allowTurnLeft = false;
      dirsCursor++;
    } else {
      moveAllLittle(1);
      stopAll();
      hasFinishedPattern = true;
      return;
    }
}
