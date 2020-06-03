#include "factory.h"


Factory::Factory(int id) {
    list = new LinkedList();
    this->factoryId = id;
}

Factory::~Factory() {
    delete list;
}

/*
* Gets a tile from linked list at i
*
* Parameters:
*           i = index at linked list
*
* Returns the tile at index i in linked list
*
*/
Tile* Factory::getTile(int i) {
    return list->get(i);
}

/*
* Adds a tile to the back of a linked list
*
* Parameters:
*           t = tile to be added
*
*/
void Factory::addTile(Tile* t) {
    list->addBack(t);
}

/*
* Deletes a tile at index i in linked list
*
* Parameters:
*           i = index at linked list
*
*/
void Factory::removeTile(int i) {
    list->deleteAt(i);
}

/*
* Clears a factory according to the linked list
*
*/
void Factory::clearFactory() {
    list->clear();
}

/*
* Returns size of the linked list
*
*/
int Factory::size() {
    return list->getSize();
}

/*
* Returns if this factory is full i.e equal to MAX_TILES_IN_FACTORY
* Normal value of MAX_TILES_IN_FACTORY is 4
* This is always false for a center factory
*
*/
bool Factory::isFull() {
    bool isFull = false;

    if (list->getSize() >= MAX_TILES_IN_FACTORY && isCentre() == false)
    {
        isFull = true;
    }

    return isFull;
}

/*
* Sets the id for this factory
*
* Parameters:
*           id = integer id of factory
*
*/
void Factory::setId(int id) {
    factoryId = id;

}

/*
* Returns the integer of the factory id
*
*/
int Factory::getId() {
    return factoryId;

}

/*
* Returns if a factory is the center factory
* A factory is a center if if the boolean 'centerFactory' is true 
*
*/
bool Factory::isCentre() {
    return this->isCenterFactory;
}

/* 
* Sets a factory as a Center Factory: 
* A factory is a center factory if the boolean 'centerFactory' is true 
*/
void Factory::setCenterFactory(){
    this->isCenterFactory = true; 
}

/*
* Checks if the linked list is empty
* Returns true if empty and false if not
*
*/
bool Factory::isEmpty() {
    bool isEmpty= false;
    if (list->getSize() == 0)
    {
        isEmpty= true;
    }
    return isEmpty;
}

/*
* Adds a tile t to index i
*
* Parameters:
*           t = tile to add at
*           i = index at linked list
*
*/
void Factory::addTileAt(Tile* t, int i) {
    list->addAt(t,i);
}

/*
* Checks if a factory contains a certain tileface
*
* Parameters:
*           face = face to check for
*
* Returns true if a face was found in the factory
*
*/
bool Factory::factoryContainsTile(TileFace face){
    bool tileFound = false;
    for (int i = 0; i < this->size(); i++){
        if(this->getTile(i)->getFace() == face){
            tileFound = true;
        }
    }
    return tileFound;
}

/*
* Deletes First in this factory if found
*
*/
void Factory::deleteFirstPlayerTokenFromCenter(){
    for (int i = 0; i < this->size(); i++){
        if(this->getTile(i)->getFace() == First){
            this->removeTile(i);
        }
    }
}

/*
* Deletes all of a certain face in this factory
*
* Parameters:
*           face = tiles with this face to be deleted for
*
*/
void Factory::deleteAllOf(TileFace face) {
    //Repeat while it contains the face
    while(factoryContainsTile(face)) {
        //Change the size each time we delete
        int size = this->size();
        for (int i = 0; i < size; i++)
        {
            if (getTile(i)->getFace() == face)
            {
                removeTile(i);
                size = this->size();
            }
        }
    }
    bubbleSortFactory();
}

/*
* Uses a bubble sort to sort a factory according to the enums in Tiles
*
*/
void Factory::bubbleSortFactory()
{
    for (int i = 0; i < this->size()-1; i++)
    {
        // Last i elements are already sorted as needed
        for (int j = 0; j < this->size()-i-1; j++)
        {
            int j_face = this->getTile(j)->getFace();
            int j_1_face = this->getTile(j+1)->getFace();

            if(j_face > j_1_face)
            {
                Tile* j_tile = this->getTile(j);

                this->removeTile(j);
                this->addTileAt(j_tile,j+1);
            }
        }
    }
}

/*
* Returns a string of a formmated factory
* This is used for console printing
*
*/
std::string Factory::printFormatted() {
    //Printing Center Factory Data
    std::string toReturn = std::to_string(this->getId()) + ": ";

    for(int i = 0; i < this->size(); i++) {
        toReturn.append(this->getTile(i)->getFaceFormatted(true));
        if(i != this->size() - 1) {
            toReturn.append(" ");
        }
    }

    return toReturn;
}

/*
* Returns a string of a formmated factory
* This is used for save/loading
*
*/
std::string Factory::printSaveFormat() {

    std::string toReturn = "";
    if(this->isCenterFactory){
        toReturn.append("CENTER-");
    }
    toReturn.append("FACTORY_" + std::to_string(this->getId()) + "=");
    for(int i = 0; i < this->size(); i++) {
        Tile * tile = this->getTile(i);
        if(tile != nullptr) {
            toReturn.append(tile->getFaceFormatted(true));
        }
    }

    toReturn.append("\n");

    return toReturn;
}
