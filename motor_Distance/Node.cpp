#include "Node.h"

Node::Node() {
  _dist = 999;
  _known = false;
  _exists = false;
  _x = 0;
  _y = 0;
}

int Node::getX() {
  return _x;
}

int Node::getY() {
  return _y;
}

void Node::setPos(int x, int y) {
  _x = x;
  _y = y;
  _exists = true; // If the position is set, the node is defined
}

Node* Node::getNeighbour(int i) {
  return _neighbours[i];
}

void Node::setNeighbour(Node* n, int i) {
  _neighbours[i] = n;
}

int Node::getDist() {
  return _dist;
}

void Node::setDist(int d) {
  _dist = d;
}

bool Node::getKnown() {
  return _known;
}

void Node::setKnown(bool k) {
  _known = k;
}

void Node::setPrev(Node* n) {
  _prev = n;
}

Node* Node::getPrev() {
  return _prev;
}

bool Node::getExists() {
  return _exists;
}

bool Node::isValidNode() {
  return (_x >= 0 && _x <= 6 && _y >= 0 && _y <= 5);
}
