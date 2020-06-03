#ifndef TILE_CLASS
#define TILE_CLASS
#include <string>

#define TOTAL_COLORS 5

enum TileFace {Red = 0, Yellow = 1, Blue = 2, Light_Blue = 3, Black = 4, First = 5};

class Tile {

private:
    TileFace tileFace;
public:
    Tile(TileFace tileFace);
    Tile(char face);
    ~Tile();

    TileFace getFace();
    std::string getFaceFormatted();
    std::string getFaceFormatted(bool shortHand);
};

#endif //TILE_CLASS
