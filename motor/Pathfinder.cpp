#include "Pathfinder.h"
#include "Car.h"
#include "Math.h"

#include <SoftwareSerial.h>

Pathfinder::Pathfinder() {
  dirs = "";
}

void Pathfinder::setCar(Car* c) {
  car = c;
}

void Pathfinder::dijkstra(int startNode, int endNode) {

  // Define start point
  nodes[startNode].setDist(0);

  if(!nodes[startNode].getExists()) {
    return;
  }
  if(!nodes[startNode].getExists()) {
    return;
  }
  if(startNode == endNode) {
    return;
  }

  Node* minNode = findMinNode();

  while(minNode->getExists() && minNode->getDist() < 999) {
    
    // For each neighbours
    for(int i = 0; i < 3; i++) {
      if(minNode->getNeighbour(i)->getExists() && minNode->getNeighbour(i)->isValidNode()) {
        Node* nb = minNode->getNeighbour(i);
        float distance = nodeDist(minNode, nb) + minNode->getDist();
        if(distance < nb->getDist() &! nb->getKnown()) {
          nb->setDist(distance);
          nb->setPrev(minNode);
        }
      }
      minNode->setKnown(true);
    }
    minNode = findMinNode();
  }

  Node* prev = &nodes[endNode];

  while(prev->getPrev()->getExists() && prev->getPrev()->isValidNode() && (prev->getX() != nodes[startNode].getX() || prev->getY() != nodes[startNode].getY())) {
    setRotationFromNode(prev);
    prev = prev->getPrev();
  }

  malloc(sizeof(prev));
  malloc(sizeof(nodes));
}

// Get distance between 2 nodes
float Pathfinder::nodeDist(Node* node1, Node* node2) {
  return sqrt(pow((node2->getX() - node1->getX()), 2) + pow((node2->getY() - node1->getY()), 2));
}

// Find the node with the minimum distance
Node* Pathfinder::findMinNode() {
  int minDist = 9999;
  Node* minNode;
  for(int i = 0; i <= 15; i++) {
    if(nodes[i].getDist() < minDist &! nodes[i].getKnown() && nodes[i].isValidNode()) {
      minDist = nodes[i].getDist();
      minNode = &nodes[i];
    }
  }
  return minNode;
}

void Pathfinder::setRotationFromNode(Node* prev) {
  if(prev->getX() - prev->getPrev()->getX() > 0) {
    dirs = dirs + getRelativeRotation("left");
  }
  if(prev->getX() - prev->getPrev()->getX() < 0) {
    dirs = dirs + getRelativeRotation("right");
  }
  if(prev->getY() - prev->getPrev()->getY() > 0) {
    dirs = dirs + getRelativeRotation("up");
  }
  if(prev->getY() - prev->getPrev()->getY() < 0) {
    dirs = dirs + getRelativeRotation("down");
  }
}

int Pathfinder::getRelativeRotation(String dir) {
  switch(car->carAngle) {
    // Looking right
    case 0:
      if(dir == "down") {
        car->carAngle = 3;
        return 2; // Rotate right
      }
      if(dir == "up") {
        car->carAngle = 1;
        return 1; // Rotate left
      }
      if(dir == "left") {
        car->carAngle = 2;
        return 3; // Rotate back
      }
      break;
    // Looking up
    case 1:
      if(dir == "right") {
        car->carAngle = 0;
        return 2; // Rotate right
      }
      if(dir == "left") {
        car->carAngle = 2;
        return 1; // Rotate left
      }
     if(dir == "down") {
        car->carAngle = 3;
        return 3; // Rotate back
      }
      break;
    // Looking left
    case 2:
      if(dir == "up") {
        car->carAngle = 1;
        return 2; // Rotate right
      }
      if(dir == "down") {
        car->carAngle = 3;
        return 1; // Rotate left
      }
      if(dir == "right") {
        car->carAngle = 0;
        return 3; // Rotate back
      }
      break;
    // Looking down
    case 3:
      if(dir == "left") {
        car->carAngle = 2;
        return 2; // Rotate right
      }
      if(dir == "right") {
        car->carAngle = 0;
        return 1; // Rotate left
      }
      if(dir == "up") {
          car->carAngle = 1;
          return 3; // Rotate back
        }
      break;
  }
  return 0;
}

void Pathfinder::initNodes() {
  nodes[0].setPos(0, 0);
  nodes[1].setPos(1, 0);
  nodes[2].setPos(3, 0);
  nodes[3].setPos(5, 0);
  nodes[4].setPos(6, 0);
  nodes[5].setPos(0, 1);
  nodes[6].setPos(1, 1);
  nodes[7].setPos(3, 1);
  nodes[8].setPos(0, 3);
  nodes[9].setPos(5, 3);
  nodes[10].setPos(6, 3);
  nodes[11].setPos(1, 4);
  nodes[12].setPos(5, 4);
  nodes[13].setPos(0, 5);
  nodes[14].setPos(1, 5);
  nodes[15].setPos(6, 5);

  nodes[0].setNeighbour(&nodes[1], 0);
  nodes[0].setNeighbour(&nodes[5], 1);

  nodes[1].setNeighbour(&nodes[0], 0);
  nodes[1].setNeighbour(&nodes[2], 1);
  nodes[1].setNeighbour(&nodes[6], 2);

  nodes[2].setNeighbour(&nodes[1], 0);
  nodes[2].setNeighbour(&nodes[3], 1);
  nodes[2].setNeighbour(&nodes[7], 2);

  nodes[3].setNeighbour(&nodes[2], 0);
  nodes[3].setNeighbour(&nodes[4], 1);
  nodes[3].setNeighbour(&nodes[9], 2);

  nodes[4].setNeighbour(&nodes[3], 0);
  nodes[4].setNeighbour(&nodes[10], 1);

  nodes[5].setNeighbour(&nodes[0], 0);
  nodes[5].setNeighbour(&nodes[6], 1);
  nodes[5].setNeighbour(&nodes[8], 2);

  nodes[6].setNeighbour(&nodes[1], 0);
  nodes[6].setNeighbour(&nodes[5], 1);
  nodes[6].setNeighbour(&nodes[7], 2);

  nodes[7].setNeighbour(&nodes[2], 0);
  nodes[7].setNeighbour(&nodes[6], 1);

  nodes[8].setNeighbour(&nodes[5], 0);
  nodes[8].setNeighbour(&nodes[9], 1);
  nodes[8].setNeighbour(&nodes[13], 2);
  
  nodes[9].setNeighbour(&nodes[3], 0);
  nodes[9].setNeighbour(&nodes[8], 1);
  nodes[9].setNeighbour(&nodes[10], 2);
  nodes[9].setNeighbour(&nodes[12], 3);

  nodes[10].setNeighbour(&nodes[4], 0);
  nodes[10].setNeighbour(&nodes[9], 1);
  nodes[10].setNeighbour(&nodes[15], 2);

  nodes[11].setNeighbour(&nodes[12], 0);
  nodes[11].setNeighbour(&nodes[14], 1);

  nodes[12].setNeighbour(&nodes[9], 0);
  nodes[12].setNeighbour(&nodes[11], 1);

  nodes[13].setNeighbour(&nodes[8], 0);
  nodes[13].setNeighbour(&nodes[14], 1);

  nodes[14].setNeighbour(&nodes[11], 0);
  nodes[14].setNeighbour(&nodes[13], 1);
  nodes[14].setNeighbour(&nodes[15], 2);

  nodes[15].setNeighbour(&nodes[10], 0);
  nodes[15].setNeighbour(&nodes[14], 1);
}
