#include "game.h"


// Contructor for New Game - 2-3-4 Players and Variable Center Factory Count

Game::Game(int seed, std::vector<std::string> playerNames, int centerFactoryCount) {
    tileBag = new Bag();
    tileBag->fillBag();
    tileBag->randomize(seed);

    boxLid = new Bag();

    for(int i=0; i<playerNames.size(); i++){
        players.push_back(new Player(playerNames.at(i), std::to_string(i+1)));
    }

    currentPlayer = players.at(0);
    this->seed = seed;

    std::cout<<"\n\nEntered Game Class:"<<std::endl;

    std::cout<<"The Center Factory Count is: "<< centerFactoryCount<<std::endl;

    // Initiating the Factory Count
    int normalFactories = 0 ;
    if (players.size() == 2){
        normalFactories = 5;
    }
    else if (players.size() == 3){
        normalFactories = 7;
    }
    else if(players.size() == 4){
        normalFactories = 9;
    }

    // Calculating Total number of Factories 
    int totalFactoryCount = normalFactories + centerFactoryCount;
    std::cout<<"The Total Factory Count is: "<< totalFactoryCount<<std::endl;

    this->centerFactoryCount = centerFactoryCount;
    this->totalFactoryCount = totalFactoryCount;

    

    // TEsting factories center 
    for(int i=0; i<factories.size(); i++){
        if(factories.at(i)->isCentre()){
            std::cout<<"Center Fac ID: "<<factories.at(i)->getId()<<std::endl;;
        }
    }

    std::cout<<"The Players Count is: "<<players.size()<<std::endl;
    std::cout<<"The Players are "<<std::endl;
    for(int i=0; i<players.size();i++){
        std::cout<<"Player Name is: "<<players.at(i)->getName()<< " and ID: "<<players.at(i)->getId()<<std::endl;
    }
    this->gameLoop_Milestone2();
}

void Game::gameLoop_Milestone2() {

    // Initialising factories if not loading from a save game (when loading a save game the factories are already initialised)
    if (!loadedFromFile)
    {
        initFactories_Milestone2();
    }

    // While End Game condition is false (no complete row on wall) 
    // Execute the game loop
    while(checkWallLines() == false) {
        std::cout << "--- New round starting ---\nFilling factories\n";

        if(skipFill) {
            skipFill = false;
        } else {
            fillFactories();
        }

        int playerIndex = nextRoundStartPlayer;
        
        // While End Of Round condition is false (no factory is empty) Execute the round
        while(checkFactoryState() == false) {
            this->displayFactories();

            currentPlayer = players.at(playerIndex % players.size());
            printf("\n%s's board\n", currentPlayer->getName().c_str());
            currentPlayer->getBoard()->printBoard();

            std::vector<std::string> userCommand = getGameInput();

            while(validTurnCommand(userCommand)==false){
                userCommand =  getGameInput();
            }
            if (userCommand.size()==1){
                if(userCommand[0] == "display"){
                    printPlayerBoards(playerIndex % players.size());
                    
                }
            }
            if(userCommand.size()==2){
                this->saveGame(userCommand[1]);
                printf("Game successfully saved to %s", userCommand[1].c_str());
            }
            if(userCommand.size()==4){

                int factoryID = std::stoi(userCommand[1]);
                TileFace tileFace = convertToTileFace(userCommand[2]);
                if(userCommand[3] == "broken") {
                    //index 6 represents the broken tiles.
                    userCommand[3] = "6";
                }
                int rowID = std::stoi(userCommand[3]);

                // IF there ARE 2 Center Factories in Game 
                // AND the user chosen factory is a "Normal" Factory
                // We prompt the user for what Center Factory to add ecess tiles to; 
                // Default is Center Factory '0' when there is only one center factory available
                int centerFacID = 0;
                if(factories[factoryID]->isCentre()==false && this->centerFactoryCount == 2){
                    centerFacID = getCenterFactoryInput();
                }

                if (factories[factoryID]->isCentre()){
                    if(factories[factoryID]->factoryContainsTile(First)){
                        
                        //Iterating over all available Center factories and deleteing the First Player token. 
                        for (int i=0;i<factories.size();i++){
                            if (factories.at(i)->isCentre()){
                                factories.at(i)->deleteFirstPlayerTokenFromCenter();
                            }
                        }
                        // Giving the First player to choose from either Center factory the First player token.
                        currentPlayer->getBoard()->addToFloor(new Tile(First), boxLid);       // Use Lexens New function to add First Player tile to floor
                        nextRoundStartPlayer = playerIndex;
                    }
                }
                // Remaining:
                // Move the Tile from the Factory to the Chosen Pattern Line by calling lexen's function once complete

                // Printing The validated commands and Factories to Check if as expected.
                //printUserCommands(userCommand);
                addTilesToBoard(currentPlayer, factoryID, tileFace, rowID, centerFacID);
                printf("Turn Successful.\n");
                playerIndex++;
            }
        }
        // Computing player points at end of round and transferring the complete pattern lines onto the wall and cleaning boeard appropriately
        for (Player * x : players)
        {
            int points = x->getBoard()->endOfTurnBoard();
            x->addScore(points);
            x->getBoard()->cleanBoard(boxLid);
        }

        std::cout << "--- End Round ---\n";
    }
    
    // At the end of game: computing player points and getting the Final score of each player. Print the board of each player at end of round.

    printf("\n\n-----------------\n");
    printf("    GAME OVER    \n");
    printf("-----------------\n");
    for (Player *x : players)
    {
        printf("\n");
        int points = x->getBoard()->endOfGameBoard();
        x->addScore(points);
        printf("Player %s Final Score = %d\n", x->getName().c_str(), x->getScore());
        x->getBoard()->printBoard();
    }
    // Retrieving the game winner information by checking which player has scored highest according to game rules. 
    Player * winner = getWinnerOject();
    if (winner != nullptr){
        printf("\nPlayer %s won the game with a score of %d", winner->getName().c_str(), winner->getScore());
    }
}


// Contructor for New Game 2 Players
Game::Game(int seed, std::string pOneName, std::string pTwoName) {
    tileBag = new Bag();
    tileBag->fillBag();
    tileBag->randomize(seed);

    boxLid = new Bag();

    players.push_back(new Player(pOneName, "1"));
    players.push_back(new Player(pTwoName, "2"));

    currentPlayer = players.at(0);
    this->seed = seed;
    //this->gameLoop();
}


/* 
Constructor for Loading Game 
The Constructor takes as input the Save File format and then loads in the revelant data.
Type: Save File given as input
*/ 
Game::Game(std::ifstream & inputFile) {
    std::string lineSplit[2];
    std::string line;
    while(!inputFile.eof()) {
        std::getline(inputFile, line);

        if(line.substr(0,1) != "#") {
            splitString(line, lineSplit, "=");
            if(lineSplit[0] == "TOTAL_FACTORIES"){
                this->totalFactoryCount = std::stoi(lineSplit[1]);
                for(int i = 0; i<this->totalFactoryCount; i++){
                factories.push_back(new Factory(i));
                }
            }
            else if(lineSplit[0] == "BAG") {
                tileBag = makeBag(lineSplit[1]);
            } else if(lineSplit[0] == "LID") {
                boxLid = makeBag(lineSplit[1]);
            } else if((lineSplit[0].find("PLAYER") != std::string::npos) && (lineSplit[0].find("NAME") != std::string::npos) ) {
                std::string playerIDString[2];
                splitString(lineSplit[0], playerIDString, "_");
                std::string playerID = playerIDString[1].substr(0,1);
                players.push_back(makePlayer(lineSplit[1], inputFile, playerID));

            } else if(lineSplit[0] == "CURRENT_PLAYER") {
                nextRoundStartPlayer = std::stoi(lineSplit[1]) - 1;
            } else if(lineSplit[0].find("FACTORY") != std::string::npos) {
                std::string factoryIDString[2];
                splitString(lineSplit[0], factoryIDString, "_");
                int factoryID = std::stoi(factoryIDString[1]);

                factories[factoryID] = makeFactory(factoryID,lineSplit[1]);
                if(lineSplit[0].find("CENTER")!= std::string::npos){
                    factories[factoryID]->setCenterFactory();
                    this->centerFactoryCount+=1;
                }
            } else if(lineSplit[0] == "SEED") {
                seed = std::stoi(lineSplit[1]);
            }
        }
    }
    this->skipFill = true;
    this->loadedFromFile = true;

    gameLoop_Milestone2();
}

/* 
Destrucutor
Appropriately deallocates memory when instance of object is deleted. 
*/
Game::~Game() {
    delete tileBag;
    delete boxLid;
    delete currentPlayer;
    players.clear();
    for (int i = 0; i < factories.size(); i++)
    {
        delete factories[i];
        factories[i] = nullptr;
    }
}

/* Game Loop: Simulates the rounds of the game and does nessesary printing for end of Game*/
// void Game::gameLoop() {

//     // Initialising factories if not loading from a save game (when loading a save game the factories are already initialised)
//     if (!loadedFromFile)
//     {
//         initFactories();
//     }

//     // While End Game condition is false (no complete row on wall) 
//     // Execute the game loop
//     while(checkWallLines() == false) {
//         std::cout << "--- New round starting ---\nFilling factories\n";

//         if(skipFill) {
//             skipFill = false;
//         } else {
//             fillFactories();
//         }

//         int playerIndex = nextRoundStartPlayer;
        
//         // While End Of Round condition is false (no factory is empty) Execute the round
//         while(checkFactoryState() == false) {
//             this->displayFactories();

//             currentPlayer = players.at(playerIndex % players.size());
//             printf("\n%s's board\n", currentPlayer->getName().c_str());
//             currentPlayer->getBoard()->printBoard();

//             std::vector<std::string> userCommand = getGameInput();

//             while(validTurnCommand(userCommand)==false){
//                 userCommand =  getGameInput();
//             }
//             if(userCommand.size()==2){
//                 this->saveGame(userCommand[1]);
//                 printf("Game successfully saved to %s", userCommand[1].c_str());
//             }
//             if(userCommand.size()==4){

//                 int factoryID = std::stoi(userCommand[1]);
//                 TileFace tileFace = convertToTileFace(userCommand[2]);
//                 if(userCommand[3] == "broken") {
//                     //index 6 represents the broken tiles.
//                     userCommand[3] = "6";
//                 }
//                 int rowID = std::stoi(userCommand[3]);

//                 if (factoryID == 0){
//                     if(factories[factoryID]->factoryContainsTile(First)){
//                         factories[factoryID]->deleteFirstPlayerTokenFromCenter();
//                         currentPlayer->getBoard()->addToFloor(new Tile(First), boxLid);       // Use Lexens New function to add First Player tile to floor
//                         nextRoundStartPlayer = playerIndex;
//                     }
//                 }
//                 // Remaining:
//                 // Move the Tile from the Factory to the Chosen Pattern Line by calling lexen's function once complete

//                 // Printing The validated commands and Factories to Check if as expected.
//                 //printUserCommands(userCommand);
//                 addTilesToBoard(currentPlayer, factoryID, tileFace, rowID);
//                 printf("Turn Successful.\n");
//                 playerIndex++;
//             }
//         }
//         // Computing player points at end of round and transferring the complete pattern lines onto the wall and cleaning boeard appropriately
//         for (Player * x : players)
//         {
//             int points = x->getBoard()->endOfTurnBoard();
//             x->addScore(points);
//             x->getBoard()->cleanBoard(boxLid);
//         }

//         std::cout << "--- End Round ---\n";
//     }
    
//     // At the end of game: computing player points and getting the Final score of each player. Print the board of each player at end of round.

//     printf("\n\n-----------------\n");
//     printf("    GAME OVER    \n");
//     printf("-----------------\n");
//     for (Player *x : players)
//     {
//         printf("\n");
//         int points = x->getBoard()->endOfGameBoard();
//         x->addScore(points);
//         printf("Player %s Final Score = %d\n", x->getName().c_str(), x->getScore());
//         x->getBoard()->printBoard();
//     }
//     // Retrieving the game winner information by checking which player has scored highest according to game rules. 
//     Player * winner = getWinnerOject();
//     if (winner != nullptr){
//         printf("\nPlayer %s won the game with a score of %d", winner->getName().c_str(), winner->getScore());
//     }
// }


/* GAME LOOP FUNCTIONS */

// Creates Factories At start of New Game.
void Game::initFactories(){
    for(int i = 0; i<6; i++){
        factories[i] = new Factory(i);
    }
}

void Game::initFactories_Milestone2(){
    for(int i = 0; i<this->totalFactoryCount; i++){
        factories.push_back(new Factory(i));
    }

    //setting Factory as Center Factory. 
    if(this->centerFactoryCount == 1){
        factories.at(0)->setCenterFactory();
    }
    if(this->centerFactoryCount == 2){
        factories.at(0)->setCenterFactory();
        factories.at(1)->setCenterFactory();
    }
}

/* 
Function to Fill the factories appropriately at the beggning of a round 
by adding the first player tile to the Center factory and fill the remaining factories by drawinng 
tiles from the top of the bag 
Factories (1-5) will then be sorted based on preset priority (R,Y,B,L,U) 
*/
void Game::fillFactories() {
    //Tile* firstTile = new Tile(First);
    // factories[0]->addTile(firstTile);

    for(int i = 0; i < factories.size(); i++){
        if (factories.at(i)->isCentre()){
            factories.at(i)->addTile(new Tile(First));
        }
        else{
            for (int j = 0; j < MAX_TILES_IN_FACTORY; j++)
            {
                Tile *drawnTile = tileBag->draw();
                factories[i]->addTile(drawnTile);
            }

            factories[i]->bubbleSortFactory();
        }
    }
}

/* 
Function to display factory information to terminal to allow player to make informed choices in the round to fill their pattern rows
*/
void Game::displayFactories()
{
    std::cout << "\nFactories:" << std::endl;

    for(Factory * x: factories) {
        std::cout << x->printFormatted() << std::endl;
    }
}

/* 
Returns a boolean based on whether any of the players has completed a row on their wall, which is the end of Game condition.
Example: Returns True if any one playe has completed a row on their wall 
         Else: returns false.
*/
bool Game::checkWallLines() {
    bool toReturn = false;
    for(int i = 0; i < (int)players.size() && toReturn == false; i++) {
        if(players.at(i)->getBoard()->wallLineCompleted()) {
            toReturn = true;
        }
    }

    return toReturn;
}

/* 
Function to check for End of Round Conditon: All factories are empty
Example: Returns True if all factoris are empty
        Else: returns false.
*/
bool Game::checkFactoryState() {
    bool toReturn = false;
    int counter = 0;
    for(Factory * x: factories) {
        if(x->isEmpty()) {
            counter++;
        }
    }

    if(counter == this->totalFactoryCount) {
        toReturn = true;
    }

    return toReturn;
}


/* 
Function to add all the tiles of a specific color (as chosen by player) from the chosen factory to the chosen pattern row.
Checks the chosen factory number and adds all the tiles in that factory (which match the chosen tile face), into the pattern 
row chosen by the player. 
Type: Player: a pointer to the player making the turn 
      factoryNNumebr = the factory to pull chosen tiles from 
      face = the chosen tile face 
      row = the pattern row into which all the tiles (of chosen tile face) from chosen factory should be added into 
*/
void Game::addTilesToBoard(Player* player, int factoryNumber, TileFace face, int row, int centerFacID)
{
    //Get player board
    Board* board =  player->getBoard();
    Tile* tile = nullptr;
    //Copy of Tile as deleting would delete the tile given to the board
    Tile* copyTile = nullptr;

    //Loop through factory and check each face
    for (int i = 0; i < factories[factoryNumber]->size(); i++)
    {
        tile = factories[factoryNumber]->getTile(i);

        if (tile != nullptr)
        {
            if (tile->getFace() == face)
            {
                //Copy the tile as deleting is neccessary for a linked list
                copyTile = new Tile(tile->getFace());
                //Add tiles to board floor if player wants to
                if (row == 6)
                {
                    board->addToFloor(copyTile, boxLid);
                }
                //Else add to the boards pattern row
                else
                {
                    board->addToPattern(copyTile, row, boxLid);
                }
                /*The copy tiles pointer is stored in the board and
                  the tile from the factory is deleted from the factory itself */
                tile = nullptr;
                copyTile = nullptr;
            }
        }
    }
    //Call delete on all colors the same as the face in this factory
    factories[factoryNumber]->deleteAllOf(face);
    // Afterwords copy all contents from this factory to factory 0
    if (factories[factoryNumber]->isCentre() == false)
    {
        for (int i = 0; i < factories[factoryNumber]->size(); i++)
        {
            tile = factories[factoryNumber]->getTile(i);
            copyTile = new Tile(tile->getFace());
            factories[centerFacID]->addTile(copyTile);

        }
        tile = nullptr;
        copyTile = nullptr;
        //After we copied each value clear the list in the factory
        factories[factoryNumber]->clearFactory();
        factories[centerFacID]->bubbleSortFactory();
    }

}


/* 
Function to Return a pointer to winning player.
If the game is a draw and the scores of the player are equal, we check for which playe has completed the most horizontal wall lines
If the number of horizontal wall lines completed by each player is also equal the Game is  draw between both players.
*/

Player * Game::getWinnerOject() {
    bool draw = false;
    Player * winner = nullptr;
    for(Player * x: players) {
        if(winner == nullptr) {
            winner = x;
        } else {
            int playerScore = x->getScore();
            int winnerScore = winner->getScore();

            if(playerScore > winner->getScore()) {
                winner = x;
            }

            if(playerScore == winnerScore) {
                if(x->getBoard()->getHorizontalLines() > winner->getBoard()->getHorizontalLines()) {
                    winner = x;
                }

                if(x->getBoard()->getHorizontalLines() == winner->getBoard()->getHorizontalLines()) {
                    draw = true;
                }
            }
        }
    }

    if (draw){
        winner = nullptr;
        std::cout << "The two players draw!" << std::endl;
    }
    
    return winner;
}

/* GAME LOAD FUNCTIONS */

/* 
Function to make the Bag object in a load game scenario
Takes a string input 'content' = BYBRUBLBRYBRYUUURLLLRUUYLRYBLYYRULBUBYYYLRBLYBURBBYRRLUUYRURUYYRYBULYBLLLBBRLUUU
which reprents the content of the bag in the save game format.
A new tile is created for each tileFace represented in the string 'content' and tile is added to the back of the bag.
The fully constructed bag is then returned 
*/
Bag * Game::makeBag(std::string content) {
    Bag * temp = new Bag();

    for(char & face: content) {
        if(face != ' ') {
            Tile * tile = new Tile(face);
            temp->addBack(tile);
        }
    }
    
    return temp;
}

/* 
Function to make factories in a load game scenario 
Takes as input the facrory Id (int) and a string which represents the tiles in the factory in the save file format.
Example input: 
id = 1
tiles = "RRYB"
A factory is created with the relevant factory ID 
A new tile is created for each tileFace represented in the string 'tiles' and tile is added to the factory.
The fully constructed factory is then returned 

*/
Factory * Game::makeFactory(int id, std::string tiles) {
    Factory * temp = new Factory(id);

    for(char & face: tiles) {
        if(face != ' ') {
            Tile * tile = new Tile(face);
            temp->addTile(tile);
        }
    }

    return temp;
}

/* 
Function to make pattern row for a given player in a load game scenario 
Takes as imput:
player = a pointer to the player whose patten rows are being initialized
pattern = a string representing the tiles in the specific pattern row as in the save file format
row = the pattern row being initialized  
type = the type of pattern row being initialized (type 1: pattern,  type 2: floor, type 3: wall )
*/
void Game::makePattern(Player * player, std::string pattern, int row, int type) {
    for(int i = 0; i < (int)pattern.length(); i++) {
        char face = pattern.at(i);
        if(face != ' ') {
            if(face != '-') {
                Tile * tile = new Tile(face);

                /*
                *   As adding to the boards are the same process I added a type switch to remove
                *   duplicating the for loop three times.
                *   Type 1 = Adding to the pattern, Type 2 = Adding to the floor, Type 3 = Adding to the Wall
                */
                if(type == 1) {
                    player->getBoard()->loadToPattern(tile, (4-i), row);
                } else if(type == 2) {
                    player->getBoard()->addToFloor(tile, boxLid);
                } else if(type == 3) {
                    player->getBoard()->loadToWall(tile, i, row);
                }
            }
        }
    }
}

/* 
Function to make player objects in a load game scenario 
When the load function sees the 'Player' and it sees 'name' it calls the make player fucntion which 
constructs the player object with player name and player Id, then proceeds to setting player score, and creatinng the 
players board by calling make pattern fucction appropriately by specifying the types (pattern, floor, wall)
Takes as input:
name = string representign the player name 
inputFile = takes in a reference to the save file
playerID = a string for playerID
*/
Player * Game::makePlayer(std::string name, std::ifstream & inputFile, std::string playerId) {
    Player *tmp = new Player(name, playerId);
    std::string line;
    std::string lineSplit[2];

    std::getline(inputFile, line);
    splitString(line, lineSplit, "=");
    tmp->setScore(std::stoi(lineSplit[1]));

    for(int i = 0; i < 5; i++) {
        std::getline(inputFile, line);
        splitString(line, lineSplit, "=");
        makePattern(tmp, lineSplit[1], i, 1);
    }

    std::getline(inputFile, line);
    splitString(line, lineSplit, "=");
    makePattern(tmp, lineSplit[1], 0, 2);

    for (int i = 0; i < 5; i++) {
        std::getline(inputFile, line);
        splitString(line, lineSplit, "=");
        makePattern(tmp, lineSplit[1], i, 3);
    }

    return tmp;
}

/* SAVE GAME FUNCTIONS */
void Game::saveGame(std::string saveFileName) {
    //saveFileName.append(".txt");
    std::ofstream saveFile(saveFileName);
    saveFile << "#Saving game infomation" << std::endl;
    saveFile << "SEED=" << std::to_string(seed) << std::endl;

    saveFile << "#Saving tile bag and box lid infomation" << std::endl;
    saveFile << tileBag->printSaveFormat("BAG");
    saveFile << boxLid->printSaveFormat("LID");

    saveFile << "#Saving Factory Data: " << std::endl;
    saveFile << "TOTAL_FACTORIES="<<this->totalFactoryCount<<std::endl; 
    for(Factory* x: factories) {
        saveFile << x->printSaveFormat();
    }

    for(int i = 0; i < (int)players.size(); i++) {
        saveFile << "#Saving player " << std::to_string(i+1) << "'s infomation" << std::endl;
        saveFile << players.at(i)->printSaveFormat();
    }

    saveFile << "#Saving the current players turn" << std::endl;
    saveFile << "CURRENT_PLAYER=" << currentPlayer->getId();

    saveFile.close();
}

/* USER INPUT FUNCTIONS */
/*
Gets the user input for Turn and returns this as a vector
Does checks to ensure that the given user input meets the specifications as dictated in the Assignnment spec where
FORMAT: turn <factory> <colour> <storage row>
FORMAT: save saveGame
*/

std::vector<std::string> Game::getGameInput() {
    std::cout<<"\nAccepted commands are 'turn' and 'save' \nTURN Commannd Format: turn <factory> <colour> <storage row> "<<std::endl;
    std::cout<<"SAVE Command format: save <saveFileName> "<<std::endl;
    printf(">");

    std::string input;
    std::getline(std::cin, input);

    if(std::cin.eof()) {
        std::cout << "\nGoodbye! \n";
        delete this;
        exit(0);;
    }


    while(isValidInput(input)==0) {

        if (std::cin.eof())
        {
            std::cout << "\nGoodbye!\n";
            delete this;
            exit(0);
        }
        else
        {
            std::cout << ("Invalid input! please try again! \n Only valid 'turn' and 'save' commannds are accepted ") << std::endl;
            printf(">");
            std::getline(std::cin, input);
        }
    }
    return splitByWhiteSpace(input);
}

/*
The function validates the user input to ensure it meets the specified criteria
Returns 0 : if the input command  is Invalid
Returns 1 : if the input command is of FORMAT "turn" and Validated
Returns 2 : if the input command is of FORMAT "save" and validated (check if first word is save, the fileCheck is done elsewhere)
Checks individual inputs to ensure
*/
int Game::isValidInput(std::string input) {
    bool validTurnFormat = false;
    bool validSaveFormat = false;
    bool validDisplayBoardsCommand = false;

    std::vector<std::string> strVec = splitByWhiteSpace(input);

    if(strVec.size()==1){
        if(strVec[0] == "display"){
            validDisplayBoardsCommand = true;
        }
    }

    if(strVec.size() == 4)
    {
        bool turnFound = false;
        bool factoryFound = false;
        bool tileFound = false;
        bool RowFound = false;
        

        

        if (strVec[0] == "turn"){
            //std::cout << strVec[0]<<std::endl;
            turnFound=true;
        }
        if (validInteger(strVec[1])){
            int factoryID = std::stoi(strVec[1]);

            if (factoryID >=0 && factoryID < factories.size())
            {
            //std::cout << strVec[1]<<std::endl;
            factoryFound=true;
            }
        }
        if (strVec[2] == "F" || strVec[2] == "R" || strVec[2] == "Y" || strVec[2] == "B" || strVec[2] == "L" || strVec[2] == "U"){
            //std::cout << strVec[2]<<std::endl;
            tileFound=true;
        }
        if (strVec[3] == "1" || strVec[3] == "2" || strVec[3] == "3" || strVec[3] == "4" || strVec[3] == "5" || strVec[3] == "broken" ){
            //std::cout << strVec[3]<<std::endl;
            RowFound=true;
        }
        if (turnFound && factoryFound && tileFound && RowFound)
        {
            //std::cout<<"Valid 'Turn' Format, all attributes found"<<std::endl;
            validSaveFormat = true;
        }
    }
    else if (strVec.size() == 2){
        // NEED TO ADD CHECKS FOR VALIDATING SAVE COMMAND
        if (strVec[0] == "save"){
            validSaveFormat = true;
            //std::cout << "\nSaved to:" <<strVec[1]<< std::endl;
        }
    }

    /*
    The attribute validatedCommand:
    Stores 0: if the input command is invalid
    Stores 1; if the input command is valid 'turn' Format
    Stores 2; if the input command is valid 'save' Format
    */

    int validatedCommand;

    if (validTurnFormat){
        validatedCommand = 1;
    }
    else if(validSaveFormat){
        validatedCommand = 2;
    }
    else if (validDisplayBoardsCommand){
        validatedCommand = 3;
    }
    else{
        validatedCommand = 0;
    }

    return validatedCommand;
}

TileFace Game::convertToTileFace(std::string str)
{
    TileFace toReturn;

    if (str == "F"){
        toReturn = First;
    }
    if (str=="R"){
        toReturn = Red;
    }
    if(str=="Y"){
        toReturn = Yellow;
    }
    if(str == "B"){
        toReturn = Blue;
    }
    if(str == "L"){
        toReturn = Light_Blue;
    }
    if (str == "U"){
        toReturn = Black;
    }

    return toReturn;
}

bool Game::validTurnCommand(std::vector<std::string> userCommand){
    bool validCommand = false;
    if(userCommand.size()==1){
        if(userCommand[0]=="display"){
            validCommand = true;
        }
    }

    if (userCommand.size()==4){

        if(userCommand[3] == "broken") {
            userCommand[3] = "6";
        }
        // Checking if Entered Tile Face is First_Player_Token,
        // If so command is not valid as the First Player token can never be selected and is only given
        // to first player who selects from the Center Factory
        if (convertToTileFace(userCommand[2])!=First){
            int factoryID = std::stoi(userCommand[1]);
            TileFace tileFace = convertToTileFace(userCommand[2]);
            int rowID = std::stoi(userCommand[3]);

            if(factories[factoryID]->isEmpty() == false ){
                if (factories[factoryID]->factoryContainsTile(tileFace)){
                    if (rowID == 6){
                        // Adding direclty to Broken Tiles
                        validCommand = true;
                    }
                    else {
                        if (currentPlayer->getBoard()->canPlaceAtPattern(tileFace, rowID))
                        {
                            validCommand = true;
                        }
                        else
                        {
                            std::cout << "The Chosen Tile cannnot be added to chosen Pattern Row. Try Again. " << std::endl;
                        }
                    }
                }
                else{
                    std::cout<<"The Chosen Tile is not available in Factory. Try Again. "<<std::endl;
                }
            }
            else{
                std::cout<<"The Chosen Factory is Empty. Try Again. "<<std::endl;
            }
        }
        else{
            std::cout<<"\nINVALID TILE SELECTION:\nThe First_Player_Token cannot be selected, the First Player (in a Round) to pick from the Center Factory recieves the First_Player_Token. \nTry Again" <<std::endl;
        }
    }
    if (userCommand.size()==2){
        // NEED TO ADD CHECKS FOR VALIDATING SAVE COMMAND
        if (userCommand[0] == "save"){
            validCommand = true;
            //std::cout << "\nSaved to:" <<strVec[1]<< std::endl;
        }
    }

    return validCommand;
}

void Game::printUserCommands(std::vector<std::string> userCommand){
    if (userCommand.size() == 4)
    {
        if (validTurnCommand(userCommand))
        {
            //int factoryID = std::stoi(userCommand[1]);
            //TileFace tileFace = convertToTileFace(userCommand[2]);
            //int rowID = std::stoi(userCommand[3]);

            std::cout << "\nThe Input TURN commands are: " << std::endl;
            std::string string;
            for (unsigned int i = 0; i < userCommand.size(); i++)
            {
                string.append(userCommand[i] + " ");
            }
            std::cout << string << std::endl;
        }
    }
    else if (userCommand.size() == 2)
    {
        std::cout << "\nThe Input SAVE command is: " << std::endl;
        std::string string;
        for (unsigned int i = 0; i < userCommand.size(); i++)
        {
            string.append(userCommand[i] + " ");
        }
        std::cout << string << std::endl;
    }
}


int Game::getCenterFactoryInput() {
    std::cout<<"\nEnter Center Factory to add the Excess Tiles (Center Factory: 0 OR 1)"<<std::endl;

    printf(">");

    std::string input;
    std::getline(std::cin, input);

    if(std::cin.eof()) {
        std::cout << "\nGoodbye! \n";
        delete this;
        exit(0);;
    }

    while(!isValidCenterFactoryID(input)) {

        if (std::cin.eof())
        {
            std::cout << "\nGoodbye!\n";
            delete this;
            exit(0);
        }
        else
        {
            std::cout << ("Invalid Center Factory! Please try again! \nOnly 2 Valid Center Factories to add excess tiles (Center Factory: 0 OR 1)") << std::endl;
            printf(">");
            std::getline(std::cin, input);
        }
    }
    return std::stoi(input);
}

bool Game::isValidCenterFactoryID(std::string input) {
    bool toRet = true;
    if(input.length() != 1) {
        toRet = false;
    }
    if(input != "0" && input != "1") {
        toRet = false;
    }
    return toRet;
}



/* UTILITY FUNCTIONS */
void Game::splitString(std::string toSplit, std::string strings[], std::string splitAt)
{
    int splitIndex = toSplit.find(splitAt);

    strings[0] = toSplit.substr(0, splitIndex);
    strings[1] = toSplit.substr(splitIndex + 1);
}

std::vector<std::string> Game::splitByWhiteSpace(std::string str)
{
    std::string bufferStr;
    std::stringstream ss(str);
    std::vector<std::string> arguments;

    while (ss >> bufferStr)
        arguments.push_back(bufferStr);

    return arguments;
}

bool Game::validInteger(std::string str){
    bool toReturn = true;
    for(int i=0; i<str.length(); i++){
        if(!isdigit(str[i])){
            toReturn = false;
        }
    }
    return toReturn;
}




void Game::printPlayerBoards(int currentPlayerIndex){
    std::cout<<"\n\n-------------------------------------------------------------------------------------------------------------------------------------"<<std::endl;
    std::cout<<"------------------------------------------------------- OPPONENTS BOARDS ------------------------------------------------------------\n\n"<<std::endl;
    std::string output = "";

    for(int i=0; i<players.size();i++){
        if (i != currentPlayerIndex){
            output.append(players[i]->getName());
            output.append("\t\t\t\t\t\t");
        }
    }
    output.append("\n");

    for (int i = 0; i<WALL_DIM; i++){
        for(int j=0; j<players.size();j++){
            if (j != currentPlayerIndex){
                output.append(players[j]->getBoard()->printBoardRow(false,i));
                output.append("\t\t");
            }
        }
        output.append("\n");
    }
    output.append("\n");

    for (int i=0; i<2; i++){
        for(int j=0; j<players.size();j++){
            if (j != currentPlayerIndex){
            output.append(players[j]->getBoard()->printBoardRow(true,i));
            output.append("\t\t");
            }
        }
        output.append("\n");
    }
    std::cout<<output<<std::endl;
    std::cout<<"-------------------------------------------------------------------------------------------------------------------------------------"<<std::endl;
    
}


