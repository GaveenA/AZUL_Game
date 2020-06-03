#ifndef FACTORY_CLASS
#define FACTORY_CLASS
#include "linkedList.h"
#include "tile.h"

#define MAX_TILES_IN_FACTORY 4

class Factory {

private:
    LinkedList* list;

    //Factory Id for saving 0-6 factories, 0 being the center factory
    int factoryId = -1;

    bool isCenterFactory = false;

public:
    Factory(int id);

    ~Factory();

    //gets tile at i
    Tile* getTile(int i);

    //Adds tile to linked list
    void addTile(Tile* t);

    //Remove tile at i
    void removeTile(int i);

    //Clears and deletes all information in linked list
    void clearFactory();

    //returns lists size
    int size();

    //returns if normal factory is full aka at 4 UNLESS it is center
    bool isFull();

    //Sets id
    void setId(int id);

    //Gets id
    int getId();

    //Gets center
    bool isCentre();

    // Returns if a Factory is Empty
    bool isEmpty();

    //Adds tile at i in list
    void addTileAt(Tile* t, int i);

    //Checks if factory has this face
    bool factoryContainsTile(TileFace face);

    //Removes first token
    void deleteFirstPlayerTokenFromCenter();

    //Deletes all occurances of this face and formats
    void deleteAllOf(TileFace face);

    //Sorts factory by bubble sort, sorts by azul rules
    void bubbleSortFactory();

    //Prints formatted factory
    std::string printFormatted();

    //Prints load format
    std::string printSaveFormat();

    void setCenterFactory();
};



#endif //FACTORY_CLASS
