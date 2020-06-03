#ifndef NODE_CLASS
#define NODE_CLASS

#include "tile.h"

class Node {
private:

public:

   Node(Tile* tile, Node* next, Node* prev);
   Node(Node& other);
   
   //Gets data
   Tile* data();

   //Next Node
   Node* getNext();

   //Previous Node
   Node* getPrev();


   Tile*    tile;
   Node*    next;
   Node*    prev;
};

#endif
