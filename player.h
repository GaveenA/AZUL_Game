#ifndef PLAYER_CLASS
#define PLAYER_CLASS

#include <string>
#include "board.h"

using std::string;

class Player {
private:
    int score = 0;
    Board* board = nullptr;
    string name;
    std::string playerID;

public:
    Player(string name, std::string id);

    ~Player();

    //Gets players name
    string getName();

    //Gets player id
    string getId();

    //Sets players name
    void setName(string name);

    //Gets players score
    int getScore();

    //Sets player score
    void setScore(int score);

    //Add to players score
    void addScore(int score);

    //Gets players board
    Board* getBoard();

    //Prints save format
    std::string printSaveFormat();
};

#endif //PLAYER_CLASS
