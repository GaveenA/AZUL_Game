#include "tile.h"

/*
    The two constructors allow for creation of a Tile using the enum or 
    the char representation
*/
Tile::Tile(TileFace face) {
    this->tileFace = face;
}

Tile::Tile(char face) {
    if (face == 'R')
    {
        this->tileFace = TileFace::Red;
    }

    if (face == 'Y')
    {
        this->tileFace = TileFace::Yellow;
    }

    if (face == 'B')
    {
        this->tileFace = TileFace::Blue;
    }

    if (face == 'L')
    {
        this->tileFace = TileFace::Light_Blue;
    }
    if (face == 'U')
    {
        this->tileFace = TileFace::Black;
    }

    if (face == 'F')
    {
        this->tileFace = TileFace::First;
    }
}

Tile::~Tile() {
}

TileFace Tile::getFace() {
    return this->tileFace;
}

std::string Tile::getFaceFormatted() {
    return getFaceFormatted(false);
}

/*
    Gets the appropriate string for each TileFace enum
    shortHand: true or false
    Example: TileFace.Black returns "Black" when shortHand is false
             TileFace.Black returns "U" when shortHand is true
*/
std::string Tile::getFaceFormatted(bool shortHand) {
    std::string toReturn;
    if(this->tileFace == 0) {
        toReturn = "Red";
    } else if(this->tileFace == 1) {
        toReturn = "Yellow";
    } else if(this->tileFace == 2) {
        toReturn = "Blue";
    } else if(this->tileFace == 3) {
        toReturn = "Light Blue";
    } else if(this->tileFace == 4){
        toReturn = "Black";
    } else if(this->tileFace == 5){
        toReturn = "First Token";
    }

    if(shortHand == true) {
        if(tileFace == 4) {
            toReturn = "U";
        }
        toReturn = toReturn.substr(0,1);
    }

    return toReturn;
}
