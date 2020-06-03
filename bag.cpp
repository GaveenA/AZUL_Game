#include "bag.h"

Bag::Bag() {
    bagSize = 0; 
}

Bag::~Bag() {
    RemoveAll(); 
    bagContent.clear(); 
}

int Bag::size() {
    return bagSize; 
}

Tile* Bag::get(int pos) {
    Tile* toReturn = nullptr; 

    if(pos >= 0 && pos < bagSize) {
        toReturn = bagContent.at(pos);
    }

    return toReturn;
}

void Bag::addBack(Tile* toAdd) {
    bagContent.push_back(toAdd);
    bagSize++;
}

void Bag::deleteAt(int i) {
    if(i >= 0 && i < bagSize) { 
        bagContent.erase(bagContent.begin() + i); 
        bagSize--; 
    }
}

void Bag::randomize() {
    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(bagContent), std::end(bagContent), rng);
}

void Bag::randomize(int seed) {
    auto rng = std::default_random_engine { static_cast<unsigned int>(seed) };
    std::shuffle(std::begin(bagContent), std::end(bagContent), rng);
}

void Bag::RemoveAll() {
    for (int i = 0; i < bagSize; i++)
    {
        delete bagContent.at(i); 
    }
    bagSize = 0; 
}

/*
    Fills the bag with 20 of each tile fave
    The bag will be randomised after filled
*/
void Bag::fillBag() {
    for (int i = 0; i < 100; i++)
    {
        TileFace face = (TileFace)(i % 5); 
        Tile * tmp = new Tile(face);
        this->addBack(tmp);
    }
}

/*
    Print the save format for the bag
    type: BAG or LID 
    Example: 
        BAG=BYULG
*/
std::string Bag::printSaveFormat(std::string type) {
    std::string toReturn = (type + "="); 
    if(this->bagSize != 0) {
        for(Tile * tile: bagContent) {
            if(tile != nullptr) {
                toReturn.append(tile->getFaceFormatted(true));
            }
        }
    }
    
    toReturn.append("\n");

    return toReturn; 
}

/*
    As the bag is randomised upon creation
    draw a Tile* from the bag
*/
Tile* Bag::draw(){

    Tile* toReturn = nullptr; 
    int lastIndex = this->bagSize-1;
    toReturn = this->get(lastIndex);

    if(toReturn != nullptr) {
        this->deleteAt(lastIndex);
    }
    
    return toReturn;
}