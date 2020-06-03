#ifndef MENU_CLASS
#define MENU_CLASS

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "game.h"
using std::string;

class Menu {

private:
    Game * game;
    int argCount;
    char ** argValue;

    //Displays the menu and options to the user.
    void displayMenu();

    //Gets input from user, does basic error handling, reccurisive method.
    std::string getInput();

    //Checks if inputs valid
    bool isValidInput(std::string input);

    //Initialises a Game
    void newGame();

    //Load a game from file - can change string
    void loadGame();

    //Prints Credits
    void printCredits();

    //Quits
    void exitMenu();

    //Gets input for names
    std::string getPlayerNames(std::string num);

    //Gets seed for randomizer
    int getSeed();

    // new Game initialization for Milestone 2
    void newGame_Milestone2();

    // Get input for Number of Players 
    std::string getInput_noPlayers();

    // Validate Player Count
    bool isValidPlayerCount(std::string input);


   // Get User Input for Factory Count 
    std::string getInput_noFactories();


    // Validate Factory Count 
    bool isValidFactoryCount(std::string input);

 



public:
    Menu(int argc, char ** argv);

    ~Menu();

    //Put User in menu loop
    void mainMenu();
};



#endif //MENU_CLASS
