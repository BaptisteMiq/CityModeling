#ifndef pathfi
#define pathfi

#include "Node.h"
#include <WString.h>

class Car;

class Pathfinder {
  public:
    Pathfinder();
    void dijkstra(int startNode, int endNode);
    float nodeDist(Node* node1, Node* node2);
    Node* findMinNode();
    void initNodes();
    void setRotationFromNode(Node* prev);
    int getRelativeRotation(String dir);
    void setCar(Car* c);

    Car* car;
    Node nodes[16];
    String dirs;
};

#endif
