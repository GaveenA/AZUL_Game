#ifndef BAG_CLASS
#define BAG_CLASS
#include <vector>
#include <algorithm>
#include <iostream>
#include <random>
#include "tile.h"

class Bag {

private:
    std::vector<Tile*> bagContent;
    int bagSize;

    void RemoveAll();
public:
    Bag();

    ~Bag();

    //Returns size of vector
    int size();

    //Returns tile at i
    Tile* get(int i);

    //Adds node at back
    void addBack(Tile*);

    //Delete at point
    void deleteAt(int i);

    //Randomises vector
    void randomize();
    void randomize(int seed);

    //Fills bag with predetermined 100 tiles
    void fillBag();

    //Prints load format
    std::string printSaveFormat(std::string type);

    // Draw a Tile
    Tile* draw();
};


#endif //BAG_CLASS
