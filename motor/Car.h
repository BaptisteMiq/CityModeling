#ifndef ca
#define ca

class Pathfinder;

class Car {
  public:
    Car();
    int getMotorSpeed();
    void setMotorSpeed(int speed);
    void turnLeft();
    void turnRight();
    void moveAllLittle(int dir);
    void moveAll(int dir);
    void moveLeft(int dir);
    void moveRight(int dir);
    void stopAll();
    void stopLeft();
    void stopRight();
    void moveArround(bool left, bool right, bool middleLeft, bool middleRight);
    void movePattern(bool left, bool right, bool middleLeft, bool middleRight);
    void recalib();
    void readDirs();
    void sendMessage(const char* msg);
    
    int indexNode;
    int carAngle;
    int recCounter;
    bool allowRecLeft;
    bool allowRecRight;
    int pulseTurnLeft;
    int pulseTurnRight;
    int pulseTurnArround;
    bool allowTurnLeft;
    bool allowTurnRight;
    bool allowTurnArround;
    int flashingCounter;
    int flashingSpeed;
    bool flashingLeft;
    bool flashingRight;
    bool canMeasure;

    int dirsCursor;
    bool hasFinishedPattern;

    Pathfinder* pathfinder;

  private:
    int _motorSpeed;
};

#endif
