#ifndef t1
#define t1

class Node {
  public:
    // Constructor
    Node();

    // Methods
    int getX();
    int getY();
    void setPos(int x, int y);
    Node* getNeighbour(int i);
    void setNeighbour(Node* n, int i);
    int getDist();
    void setDist(int d);
    bool getKnown();
    void setKnown(bool k);
    void setPrev(Node* n);
    Node* getPrev();
    bool getExists();
    bool isValidNode();

  private:
    int _x;
    int _y;
    int _dist;
    Node* _neighbours[4];
    Node* _prev;
    bool _known;
    bool _exists;
};

#endif
