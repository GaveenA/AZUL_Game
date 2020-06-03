#ifndef GAME_CLASS
#define GAME_CLASS
#include <fstream>
#include "bag.h"
#include "factory.h"
#include "player.h"
#include <sstream>
#define NO_TILES_PER_ROUND 20
#define FACTORIES 6

class Game {

private:

    Bag * tileBag = nullptr;
    Bag * boxLid = nullptr;
    std::vector<Player *> players;
    int seed = 0;

    //Factory * factories[FACTORIES];
    std::vector<Factory *> factories;

    Player * currentPlayer = nullptr;
    int nextRoundStartPlayer = 0;

    bool loadedFromFile = false;
    bool skipFill = false;

    int centerFactoryCount = 0;
    int totalFactoryCount = 0;

    //Creates new factories
    void initFactories();
    void initFactories_Milestone2();

    //Fills factories from tilebag
    void fillFactories();

    //Prints formatted factories
    void displayFactories();

    //Check if a players wall is complete
    bool checkWallLines();

    //Checks if all factories are empty
    bool checkFactoryState();

    //Adds tiles to boards pattern or floor
    void addTilesToBoard(Player* player, int factoryNumber, TileFace face, int row, int centerFacID);

    //Gets winner
    Player * getWinnerOject();

    //Loader functions
    Bag *  makeBag(std::string content);

    Factory * makeFactory(int id, std::string tiles);

    void makePattern(Player * player, std::string pattern, int row, int type);

    Player * makePlayer(std::string name, std::ifstream & inputFile, std::string playerId);

    //Saves current state of game
    void saveGame(std::string saveFileName);

    //Gets input from user
    std::vector<std::string> getGameInput();

    //Checks if valid input
    int isValidInput(std::string input);

    //Converts input to tface
    TileFace convertToTileFace(std::string);

    //Returns if valid turn
    bool validTurnCommand(std::vector<std::string> userCommand);

    //Prints users commands
    void printUserCommands(std::vector<std::string> userCommand);

    //Splits string
    void splitString(std::string toSplit, std::string strings[], std::string splitAt);

    //Splits by whitespace
    std::vector<std::string> splitByWhiteSpace(std::string str);

    // utility Function which returns true if string is a valid integer.
    bool validInteger(std::string str);

    // Function to get the Center factory to Add to
    int getCenterFactoryInput();

    // Function to Validate Center Factory ID input by user to add the excess Tiles.
    bool isValidCenterFactoryID(std::string input);


    // Functions to print oponents boards.
    void printPlayerBoards(int CurrentPlayerIndex);


public:
    //Start game off new game
    Game(int seed, std::string pOneName, std::string pTwoName);

    //Start game off load
    Game(std::ifstream &inputFile);

    //start Game with input as vector.
    Game(int seed, std::vector<std::string> playerNames, int factoryCount);

    //Deconstructor
    ~Game();

    //Loops Game turns
    //void gameLoop();

    //Loops Game turns for 2-3-4 Player Game;
    void gameLoop_Milestone2();

};

#endif //GAME_CLASS
