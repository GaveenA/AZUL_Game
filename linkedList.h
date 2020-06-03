#ifndef LINKEDLIST_CLASS
#define LINKEDLIST_CLASS

#include "node.h"
#include <iostream>

class LinkedList {

private:
    Node* head;
    Node* tail;
    int size;

public:
    LinkedList();

    ~LinkedList();

    //Returns size of list
    int getSize();

    //Returns tile at i
    Tile* get(int i);

    //Add tile to front
    void addFront(Tile* t);

    //Add tile to back
    void addBack(Tile* t);

    //Delete at i
    void deleteAt(int i);

    //Delete at front
    void deleteFront();

    //Delete at the Back
    void deleteBack();

    //Clear Linked List elements unless nullptr
    void clear();

    // Add a tile at a desired place
    void addAt(Tile* tile, int i);

};



#endif //LINKEDLIST_CLASS
