#include "menu.h"

Menu::Menu(int argc, char ** argv) {
    argCount = argc;
    argValue = argv;
}

Menu::~Menu() {
    delete game;
}

/*
    displays the menu to the user, 
    gets input and process appropriate menu options
    If ^D pressed at any time exit
*/
void Menu::mainMenu() {
    displayMenu(); 
    std::string option = getInput();

    while(!std::cin.eof() && option != "4") {
        if(option == "1") {
            newGame_Milestone2();
        } else if(option == "2") {
            loadGame();
        } else if(option =="3") {
            printCredits();
        }
        
        displayMenu(); 
        option = getInput();
    }

    exitMenu(); 
}

/*
    Displays the main menu as per spec.
*/
void Menu::displayMenu() {
    printf("Welcome to Azul!\n");
    printf("----------------\n\n");
    printf("Main Menu\n");
    printf("1.  New Game\n");
    printf("2.  Load Game\n");
    printf("3.  Credits (Show Student Information)\n");
    printf("4.  Quit\n\n");
}

/*
    Gets the users input, if not valid show appropriate message.
    If ^D the exit the application
    Example: "1";
*/
std::string Menu::getInput() {
    printf(">");

    std::string input;
    std::getline(std::cin, input);

    if(std::cin.eof()) {
        exitMenu();
    }
    while(!isValidInput(input) && !std::cin.eof()) {
        std::cout << ("Invalid input! please try again! Only options displayed are valid") << std::endl;
        printf(">");
        std::getline(std::cin, input);
    }

    return input;
}

/*
    Checks if the users input if a valid option
    input: The users cmd line text
    Example: True if valid
             False if not valid
*/
bool Menu::isValidInput(std::string input) {
    bool toRet = true;

    if(input.length() != 1) {
        toRet = false;
    }

    if(input != "1" && input != "2" && input != "3" && input != "4") {
        toRet = false;
    }
    return toRet;
}

/*
    Gets the user to enter the player names then creates the new game 
*/
void Menu::newGame() {
    std::string pOneName, pTwoName;
    int seed = getSeed();
    printf("Game seed to be used: %d\n", seed);

    pOneName = getPlayerNames("1");
    pTwoName = getPlayerNames("2");

    game = new Game(seed, pOneName, pTwoName);
}

/*
    Checks if the user entered a filename that exists, if not show appropriate error message
    Fetches the file and passes it to the overloaded Game constructor which takes an 
    ifstream as a parameter
*/
void Menu::loadGame() {
    std::string fileName;

    printf("Enter saved game file name\n>");
    std::getline(std::cin, fileName);
    std::ifstream inputFile;

    if(std::cin.eof()) {
        exitMenu();
    }

    inputFile.open(fileName);

    while(!inputFile.good()) {
        std::cout << "File " << fileName << " not found, please try again\n>";
        std::getline(std::cin, fileName);

        if(fileName == "\0") {
            exitMenu();
        }

        inputFile.open(fileName);
    }

    game = new Game(inputFile);
}

/*
    Prints the student info for our group
*/
void Menu::printCredits() {
    printf("-----------------------------------------\n");
    printf("%26s\n", "Student One");
    printf("Name: Lexen Perez\n");
    printf("StudentID: s3784281\n");
    printf("Email: s3784281@student.rmit.edu.au\n\n");
    printf("%26s\n", "Student Two");
    printf("Name: Hewa Gaveen Amarapala\n");
    printf("StudentID: s3766914\n");
    printf("Email: s3766914@student.rmit.edu.au\n\n");
    printf("%26s\n", "Student Three");
    printf("Name: Jordan Hacking\n");
    printf("StudentID: s3723766\n");
    printf("Email: s3723766@student.rmit.edu.au\n");
    printf("-----------------------------------------\n\n");
}

void Menu::exitMenu() {
    std::cout << "\nGoodbye!\n";
    delete this;
    exit(0);
}

/*
    Gets the players name from console.
    If invalid prompt to try again showing appropriate message
*/
std::string Menu::getPlayerNames(std::string num) {
    std::string name;
    std::cout << "Please enter player " << num << "'s name\n>";

    std::getline(std::cin, name);

    while(name == "" && !std::cin.eof()) {
        printf("Name cannot be empty, please try again\n>");
        std::getline(std::cin, name);
    }

    if(name == "\0") {
        exitMenu();
    }

    return name;
}

/*
    Gets the seed from the execution parms. 
    If a valid seed is presented then convert it to int form. 

    Exception: If a user tries to enter a non integer seed display appropriate message and generate random seed
               If user tries to enter a integar to large to convert display "   "       "     "        "     "
*/
int Menu::getSeed() {
    srand (time(NULL));
    int seed = rand();

    if(argCount >= 2) {
        try
        {
            seed = std::stoi(argValue[2]);
        }
        catch (std::invalid_argument const &e)
        {
            std::cout << "ERROR: Seed passed was type String, type int only allowed, generating a random seed\n";
        }
        catch (std::out_of_range const &e)
        {
            std::cout << "ERROR: Seed passed was to large, generating a random seed\n";
        }
    }

    return seed;
}


void Menu::newGame_Milestone2() {
    std::string pOneName, pTwoName;
    int seed = getSeed();
    printf("Game seed to be used: %d\n", seed);

    int factoryCount = std::stoi(getInput_noFactories());
    std::cout<<"Num of Factories "<<factoryCount<<std::endl;

    printf("Enter Number of Players to Play the Game (Must be 2, 3 or 4 Players)\n");
    int playerCount = std::stoi(getInput_noPlayers());

    std::cout<<"Player Count = " <<playerCount<<std::endl;

    int i=1;
    std::vector<std::string> playerNames; 

    while(i<playerCount+1){
        std::string name = getPlayerNames(std::to_string(i));
        playerNames.push_back(name);
        i++;
    }
    
    game = new Game(seed, playerNames, factoryCount);
    
}

// Function to Get input from user for Number of Players in Game 
std::string Menu::getInput_noPlayers() {
    printf(">");

    std::string input;
    std::getline(std::cin, input);

    if(std::cin.eof()) {
        exitMenu();
    }
    while(!isValidPlayerCount(input)) {
        if(std::cin.eof()) {
        exitMenu();
        }
        else{
        std::cout << ("Invalid input! Game allows 2/3/4 Players only. Try Again! ") << std::endl;
        printf(">");
        std::getline(std::cin, input);
        }
    }

    return input;
}

// Function to Validate Player Count Entered by User. 
bool Menu::isValidPlayerCount(std::string input) {
    bool toRet = true;
    if(input.length() != 1) {
        toRet = false;
    }
    if(input != "2" && input != "3" && input != "4") {
        toRet = false;
    }
    return toRet;
}



// Fuction to Get the user input for Number of Factories 
std::string Menu::getInput_noFactories() {
    std::cout<< "\nEnter the Number of Center Factories to play the Game \tEither 1 OR 2"<<std::endl;  
    printf(">");

    std::string input;
    std::getline(std::cin, input);

    if(std::cin.eof()) {
        exitMenu();
    }
    while(!isValidFactoryCount(input)) {
        if(std::cin.eof()) {
        exitMenu();
        }
        else{
        std::cout << ("Invalid input! Game allows 1 OR 2 Center Factories only. Try Again! ") << std::endl;
        printf(">");
        std::getline(std::cin, input);
        }
    }

    return input;
}

// Function to Validate the Number of Center Factories given by user..
bool Menu::isValidFactoryCount(std::string input) {
    bool toRet = true;
    if(input.length() != 1) {
        toRet = false;
    }
    if(input != "1" && input != "2") {
        toRet = false;
    }
    return toRet;
}
