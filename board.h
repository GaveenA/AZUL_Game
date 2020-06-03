#ifndef BOARD_CLASS
#define BOARD_CLASS
#include "tile.h"
#include "bag.h"
#include <iostream>

#define WALL_DIM 5
#define FLOOR_DIM 7
#define PATTERN_DIM 5
#define NEG_ONE_LIMIT 1
#define NEG_TWO_LIMIT 4
#define NEG_THREE_LIMIT 6

#define EMPTY_TILE "."

#define H_POINTS 2
#define V_POINTS 7
#define C_POINTS 10

using std::cout;
using std::endl;

class Board {

private:
    //Contains pattern of tiles needed to be made
    Tile* wall[WALL_DIM][WALL_DIM];
    /*
    [B] [Y] [R] [U] [L]
    [L] [B] [Y] [R] [U]
    [U] [L] [B] [Y] [R]
    [R] [U] [L] [B] [Y]
    [Y] [R] [U] [L] [B]
    */

    //Contains tiles on floor that deduct points and returns to lid
    Tile* floor[FLOOR_DIM];
    /* [-1] [-1] [-2] [-2] [-2] [-3] [-3] */

    //Contains pattern tiles
    Tile* pattern[PATTERN_DIM][PATTERN_DIM];
    /*
    [N] [N] [N] [N] [Y]
    [N] [N] [N] [Y] [Y]
    [N] [N] [Y] [Y] [Y]
    [N] [Y] [Y] [Y] [Y]
    [Y] [Y] [Y] [Y] [Y]
    */

    //Position of tile that was completed
    int xPos = 0;
    int yPos = 0;

    //bool is floor was already decremented to totalscore
    bool floorAdded = false;

    //Calculates and returns points according to current wall and floor
    int calculateBoard();

    //Checks if in row x that a tile is already completed
    bool doesFaceExist(TileFace face, int x);

    //Add tile to wall and records last placed tile
    void addToWall(int row, Tile* t);

    //Checks if a row is empty
    bool isPatternLineEmpty(int row);


public:
    Board();

    ~Board();

    //Prints Current Board situation
    void printBoard();

    //Determines if a tile or tiles can be placed at a pattern row
    bool canPlaceAtPattern(TileFace face, int row);

    //Handles adding to a pattern row and excess tiles to floor array
    void addToPattern(Tile* t, int row, Bag* boxLid);

    //Directly add to floor
    void addToFloor(Tile* t, Bag* boxLid);

    //Moves finished tiles to wall and other needed moves at pattern-floor phase end
    int endOfTurnBoard();

    //Cleans up board such as removing floor tiles
    void cleanBoard(Bag* boxLid);

    //Returns if a horizontal line has been completed (signals end of game)
    bool wallLineCompleted();

    //Adds points to total score according to end of game rules
    int endOfGameBoard();

    //Returns amount of lines complete in wall
    int getHorizontalLines();

    //Gets tile at pattern (x,y)
    Tile* getPattern(int x, int y);

    //Gets tile at wall (x,y)
    Tile* getWall(int x, int y);

    //SAVING / LOADING FUNCTIONS

    //Loads a tile directly to a x,y pos
    void loadToPattern(Tile* t, int col, int row);

    //Loads a tile to frontmost floor
    void loadToFloor(Tile* t);

    //Loads a tile to x,y pos
    void loadToWall(Tile* t, int col, int row);

    //Prints formmated floor
    std::string printFloor();

    //Prints formmated pattern or wall
    std::string printPattern(int row, bool printNulls, int type);

    //Prints entire board info
    std::string printSaveFormat(std::string front);
    
    // Print a specified row of the board;
    std::string printBoardRow(bool broken , int row);
};



#endif //BOARD_CLASS
