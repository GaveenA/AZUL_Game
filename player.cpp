#include "player.h"

Player::Player(string name, std::string id) {
    this->playerID = id;
    this->name = name;
    this->board = new Board();
}

Player::~Player() {
    delete board;
}

string Player::getName() {
    return this->name;
}

string Player::getId() {
    return this->playerID;
}

void Player::setName(string name) {
    this->name = name;
}

int Player::getScore() {
    return this->score;
}

void Player::setScore(int score) {
    this->score = score;
}

/*
    Contract: A players score cannot be (-) as per azul rules.
    If a turn happens where a players score may go (-) then reset the score back to 0
*/
void Player::addScore(int score) {
    this->score+=score;

    if(this->score < 0) {
        this->score = 0;
    }
}

Board* Player::getBoard() {
    return this->board;
}

/*
    Prints the players infomation in the format for the save file. 
    Also calls the other printFormat methods on all the player specific objects. 
    Example: 
        PLAYER_1_NAME=Jordan
        PLAYER_1_SCORE=0
        PLAYER_1_PATTERN_LINE0=
        PLAYER_1_PATTERN_LINE1=
        PLAYER_1_PATTERN_LINE2=LLL
        PLAYER_1_PATTERN_LINE3=
        PLAYER_1_PATTERN_LINE4=
        PLAYER_1_FLOOR_LINE=L
        PLAYER_1_MOSAIC_0=-----
        PLAYER_1_MOSAIC_1=-----
        PLAYER_1_MOSAIC_2=-----
        PLAYER_1_MOSAIC_3=-----
        PLAYER_1_MOSAIC_4=-----
*/
std::string Player::printSaveFormat() {
    std::string toReturn;

    //Print Name
    toReturn.append("PLAYER_" + playerID + "_NAME=" + this->getName() + "\n");

    //Print Score
    toReturn.append("PLAYER_" + playerID + "_SCORE=" + std::to_string(this->getScore()) + "\n");

    //Prints to boards toString
    toReturn.append(this->board->printSaveFormat("PLAYER_" + playerID + "_"));

    return toReturn;
}
