#include "board.h"

/* Constructor
*
* No variables need to be initialised
*
*/
Board::Board() {

}

/* Deconstructor
*
* Deletes each contents in array
* when decontructor is called
*
*/

Board::~Board() {

    for (int i = 0; i < WALL_DIM; i++)
    {
        for (int j = 0; j < WALL_DIM; j++)
        {
            delete wall[i][j];
            wall[i][j] = nullptr;
        }
    }

    for (int i = 0; i < FLOOR_DIM; i++)
    {
        delete floor[i];
        floor[i] = nullptr;
    }

    for (int i = 0; i < PATTERN_DIM; i++)
    {
        for (int j = 0; j < PATTERN_DIM; j++)
        {
            delete pattern[i][j];
            pattern[i][j] = nullptr;
        }
    }
}

/*
* Prints console formmated Board for player to see
* Example:
* 1:             L || .  .  .  .  .
* 2:          .  . || .  .  .  .  .
* 3:       .  .  . || .  .  .  .  .
* 4:    .  .  .  . || .  .  .  .  .
* 5: .  .  .  .  . || .  .  .  .  .
* Broken: -1  -1  -2  -2  -2  -3  -3
*        F   .   .   .   .   .   .
*/
void Board::printBoard() {
    //Use either dimension since they both would always have the same dimensions
    for (int row = 0; row < WALL_DIM; row++)
    {
        //Printing 1st row
        cout << row + 1 << ":";

        //Check each column for Pattern
        for (int col = 0; col < PATTERN_DIM; col++)
        {
            cout << " ";
            //Check if array is a place a player is allowed to place at
            //Dimension - column gives right to left count of column so col 0 would be col 5
            //If column is greater than the row (amount of tiles that can be placed) then its a space cout
            if (PATTERN_DIM - col > row + 1)
            {
                cout << " ";
            }
            //If no tile exists then it becomes a .
            else if (pattern[row][col] == nullptr)
            {
                cout << EMPTY_TILE;
            }
            //Otherwise get shorthand format of face
            else
            {
                cout << pattern[row][col]->getFaceFormatted(true);
            }

            cout << " ";
        }

        cout << "||";
        //Check each column for Wall
        //Seperating loop makes it easier to understand as each loop defines which array its printing
        for (int col = 0; col < WALL_DIM; col++)
        {
            cout << " ";
            //Check if tile is empty & print entire array as it uses all parts
            if (wall[row][col] == nullptr)
            {
                cout << EMPTY_TILE;
            }
            else
            {
                cout << wall[row][col]->getFaceFormatted(true);
            }
            cout << " ";
        }
        cout << endl;
    }
    //Printing Broken Floor Tiles
    cout << "Broken: -1  -1  -2  -2  -2  -3  -3";
    cout << endl;
    cout << "      ";

    for (int i = 0; i < FLOOR_DIM; i++)
    {
        cout << "  ";
        if (floor[i] == nullptr)
        {
            cout << EMPTY_TILE;
        }
        else
        {
            cout << floor[i]->getFaceFormatted(true);
        }
        cout << " ";
    }
    cout << endl;
}

/*
* Returns whether a certain color(face) can be placed in a specified row
* Parameters:
*           face: the tileface being checked
*           row: the row of the pattern that is being checked
*
* Example:
*        Placing a black tile in row 1.
*        Checks whether a black tile exists in wall row 1
*        Checks whether a tile of different color exists in pattern row 1
*        If both of these are false then this function returns true
*        Otherwise this function returns false
*
*/
bool Board::canPlaceAtPattern(TileFace face, int row) {

    bool canPlace = false;

    //Assuming array 0 == row 1
    int trueRow = row - 1;

    //If Pattern line is empty and if face doesnt exist in wall then it can be placed
    if (isPatternLineEmpty(trueRow) == true && doesFaceExist(face, trueRow) == false)
    {
        canPlace = true;
    }
    else
    {
        //Start at row 1 until i == 5
        for (int i = 0; i < PATTERN_DIM && !canPlace; i++)
        {
            //If row == i (row)
            if (trueRow == i)
            {
                //Then check each column for a face that equals the tile/s to be placed
                for (int j = 0; j < PATTERN_DIM && !canPlace; j++)
                {
                    if (pattern[i][j] != nullptr)
                    {
                        if (pattern[i][j]->getFace() == face)
                        {
                            canPlace = true;
                        }
                    }
                }
            }
        }
    }
    return canPlace;
}

/*
* Adds a tile to a pattern row, if a row is full, then this tile will be moved
* to the end of the boxLid
* Parameters:
*           t: the tile being placed
*           row: the row of the pattern
*           boxLid: the bag object that holds leftover tiles
*
* Example:
*        Add a black tile to row 1
*        Checks if row 1 is full
*        If full then this black tile is added to the back of the boxLid's vector
*        If not full then the black tile is added to the right-most free array space
*        in the pattern 2D array
*
*/
void Board::addToPattern(Tile* t, int row, Bag* boxLid) {
    //Add right to left from pattern
    //Reverse loop start at pattern[row][4]
    bool placed = false;

    //Assuming array 0 == row 1
    int trueRow = row - 1;
;
    int maxColumn = PATTERN_DIM - row;
    //Row determines many times the loop iterates
    //For example row 4 allows max 4 tiles starting from right [x][y][y][y][y]
    // i == 4
    for (int i = PATTERN_DIM - 1; i >= 0 && !placed; i--)
    {
        if (i >= maxColumn)
        {
            if (pattern[trueRow][i] == nullptr)
            {
                pattern[trueRow][i] = t;
                placed = true;
            }
        }

    }

    //Else if the row is full then place tile to wall
    if (placed == false)
    {
        addToFloor(t, boxLid);
    }

}

/*
* Adds a tile to a floor row, if the floor is full, then this tile will be moved
* to the end of the boxLid. Unless this tile's tileFace is First, then the First
* tile replaces the first index of floor and moves the replaced tile to the boxLid
*
* Parameters:
*           t: the tile being placed
*           boxLid: the bag object that holds leftover tiles
*
* Example:
*        Add t, a Red Tile
*        Checks if the floor array is full
*        If full then this tile is moved to the back of the boxLid's vector
*        If not full then the tile is added at the left-most free array space
*        in the floor array
*
*/
void Board::addToFloor(Tile* t, Bag* boxLid) {
    bool placed = false;

    for (int i = 0; i < FLOOR_DIM && !placed; i++)
    {
        if (floor[i] == nullptr)
        {
            floor[i] = t;
            placed = true;
        }
    }

    if (placed == false)
    {
        if (t->getFace() != First)
        {
            boxLid->addBack(t);
        }
        else
        {
            //If the first player tile is placed in a full floor line
            //Move the first floor tile to boxLid and replace with first player tile
            boxLid->addBack(floor[0]);
            floor[0] = t;
        }

    }

}

/*
* Adds completed pattern lines to wall according to Azul rules.
* The right-most tile in a completed line is moved to the wall.
* Points are then accumulated for each tile added to the wall
* The accumulated points are then returned
*
* Example:
*        Called function with row 1 full with one black tile
*        The black tile from row 1 is moved to its position in row 1 of the wall
*        Points are accumlated from the black tiles positions
*        Repeat for each row
*        Return an integer of the amount of points gained at the end of this round
*
*/
int Board::endOfTurnBoard() {
    //Check all pattern rows if they are completed
    //A row is completed if the amount of columns with tiles equal the row number
    int row = 0;
    int numberOfTiles = 0;
    int pointsToReturn = 0;

    for (int i = 0; i < PATTERN_DIM; i++)
    {
        row = i + 1;
        for (int j = 0; j < PATTERN_DIM; j++)
        {
            if (pattern[i][j] != nullptr)
            {
                numberOfTiles++;
            }
        }
        //When a row is considered complete then add to wall and do points
        if (row == numberOfTiles)
        {
            //Always get the last (5th column) of tile
             addToWall(i, pattern[i][PATTERN_DIM - 1]);

            //Change pattern to nullptr
            pattern[i][PATTERN_DIM - 1] = nullptr;

            //Then calculate the board according to the recently placed tile from addToWall()
            pointsToReturn += calculateBoard();
        }
        row = 0;
        numberOfTiles = 0;
    }

    floorAdded = false;
    return pointsToReturn;

}

/*
* Cleans a board, usually called after endOfTurnBoard()
* Cleaning a board means removing leftover tiles from completed lines
* and Removing tiles from the floor line
*
* Parameters:
*           boxLid: the bag object that holds leftover tiles
*
* Example:
*        Leftover tiles from pattern row 5 and 3 tiles in the floor array
*        Checks if a row has its last tile removed
*        If true, this indicates a completed line, all tiles in this row are moved to the back of the boxLid
*        from left-most order
*        If false, this row is skipped
*        All tiles in the floor are moved to the back of the boxLid from left-most order
*        Exception is the First tile, this tile is directly deleted
*
*/
void Board::cleanBoard(Bag* boxLid) {
    //Remove all tiles that were completed
    //Tiles lines are considered complete when the right-most one is missing and there are tiles in that row
    int numberOfTiles = 0;
    bool lastTile = false;

    for (int i = 0; i < PATTERN_DIM; i++)
    {
        //Check each Row if the last tile exists
        if (pattern[i][PATTERN_DIM - 1] != nullptr)
        {
            lastTile = true;
        }

        for (int j = 0; j < PATTERN_DIM; j++)
        {
            //Count each tile in that row
            if (pattern[i][j] != nullptr)
            {
                numberOfTiles++;
            }
        }

        //If the last tile does not exists and the number of tiles is > 0 then move tiles to box lid
        if (lastTile == false && numberOfTiles > 0)
        {
            for (int j = 0; j < PATTERN_DIM; j++)
            {
                if (pattern[i][j] != nullptr)
                {
                    boxLid->addBack(pattern[i][j]);
                    pattern[i][j] = nullptr;
                }
            }
        }

        numberOfTiles = 0;
        lastTile = false;
    }

    //Cleaning Floor
    //Put all tiles to box lid except playerfirst tile - just delete
    for (int i = 0; i < FLOOR_DIM; i++)
    {
        if (floor[i] != nullptr)
        {
            if (floor[i]->getFace() != First)
            {
                boxLid->addBack(floor[i]);
                floor[i] = nullptr;
            }
            else
            {
                delete floor[i];
                floor[i] = nullptr;
            }
        }
    }
}

/*
* Checks whether a horizontal line in the wall 2D array is complete and returns
* either true if one or more horizontal line was completed
* or false if no line was complete
*
* Example:
*        row 4 in wall tile is complete with tiles Red, Black, Light Blue, Blue and Yellow
*        Checks from row 0 to 4 in wall array
*        Row 4 is considered full and the return value becomes true
*
*/
bool Board::wallLineCompleted() {

    bool lineCompleted = false;
    int counter = 0;

    //Check each row and column and count for each row
    //If count is == 5 then all 5 tiles exist in that row
    for (int i = 0; i < WALL_DIM; i++)
    {
        for (int j = 0; j < WALL_DIM; j++)
        {
            if (wall[i][j] != nullptr)
            {
                counter++;
            }
        }
        if (counter == WALL_DIM)
        {
            lineCompleted = true;
        }
        counter = 0;
    }

    return lineCompleted;
}

/*
* Returns points gained from the end of the game according to Azul
* According to rulebook (https://www.ultraboardgames.com/azul/game-rules.php)
* Gain 2 points for each complete horizontal line of 5 consecutive tiles on your wall.
* Gain 7 points for each complete vertical line of 5 consecutive tiles on your wall.
* Gain 10 points for each color of which you have placed all 5 tiles on your wall.
*
* Example:
*        1 horizontal line and 2 vertical lines completed in the wall
*        Checks and finds 1 horizontal lines, adding H_POINTS to the gained points
*        Checks and finds 2 vertical lines, adding V_POINTS to the gained points
*        No 5 total colors of the same color found,
*        returns total gained points
*
*/
int Board::endOfGameBoard() {
    //Seperating loops so its easier to understand
    int pointsGained = 0;

    int horizontalCounter = 0;
    //Check horizontal lines
    for (int i = 0; i < WALL_DIM; i++)
    {
        for (int j = 0; j < WALL_DIM; j++)
        {
            if (wall[i][j] != nullptr)
            {
                horizontalCounter++;
            }
        }

        if (horizontalCounter == WALL_DIM)
        {
            pointsGained += H_POINTS;
        }
        horizontalCounter = 0;
    }

    int verticalCounter = 0;
    //Check Vertical lines
    for (int i = 0; i < WALL_DIM; i++)
    {
        for (int j = 0; j < WALL_DIM; j++)
        {
            if (wall[j][i] != nullptr)
            {
                verticalCounter++;
            }
        }

        if (verticalCounter == WALL_DIM)
        {
            pointsGained += V_POINTS;
        }
        verticalCounter = 0;
    }

    //Count each color made then just add 10 for each
    // red
    // yellow
    // blue
    // lightBlue
    // black
    // In array
    int colors[TOTAL_COLORS] = {0, 0, 0, 0, 0};

    //Check each element
    for (int i = 0; i < WALL_DIM; i++)
    {
        for (int j = 0; j < WALL_DIM; j++)
        {
            if (wall[i][j] != nullptr)
            {
                if (wall[i][j]->getFace() == Red)
                {
                    colors[0] += 1;
                }
                else if (wall[i][j]->getFace() == Yellow)
                {
                    colors[1] += 1;
                }
                else if (wall[i][j]->getFace()== Blue)
                {
                    colors[2] += 1;
                }
                else if (wall[i][j]->getFace() == Light_Blue)
                {
                    colors[3] += 1;
                }
                else if (wall[i][j]->getFace() == Black)
                {
                    colors[4] += 1;
                }
            }
        }
    }
    //Then just check each array if theyre == 5
    for (int i = 0; i < TOTAL_COLORS; i++)
    {
        if (colors[i] == WALL_DIM)
        {
            pointsGained += C_POINTS;
        }
    }

    return pointsGained;
}

/*
* Returns total amount of horizontal lines completed in the wall array
*
* Example:
*        2 horizontal lines are completed in the wall
*        Cycles through each row, if tiles exists for all columns then add
*        finds 2 lines and returns 2 lines
*
*/
int Board::getHorizontalLines() {

    int numberOfLines = 0;
    int horizontalCounter = 0;
    //Check horizontal lines
    for (int i = 0; i < WALL_DIM; i++)
    {
        for (int j = 0; j < WALL_DIM; j++)
        {
            if (wall[i][j] != nullptr)
            {
                horizontalCounter++;
            }
        }

        if (horizontalCounter == WALL_DIM)
        {
            numberOfLines++;
        }
        horizontalCounter = 0;
    }

    return numberOfLines;
}

/*
* Returns a tile from a pattern array according to row and column specified
* Input takes in true array coordinates
*
* Parameters:
*           col: column specified to look at in pattern array
*           row: row specified to look at in pattern array
*
* Example:
*        Get a tile from row 0, column 4
*        returns a tile or nullptr from pattern[0][4]
*
*/
Tile* Board::getPattern(int col, int row) {
    Tile* tile = nullptr;

    if (col <= PATTERN_DIM && row <= PATTERN_DIM)
    {
        tile = pattern[row][col];
    }

    return tile;
}

/*
* Returns a tile from a wall array according to row and column specified
* Input takes in true array coordinates
*
* Parameters:
*           col: column specified to look at in wall array
*           row: row specified to look at in wall array
*
* Example:
*        Get a tile from row 0, column 4
*        returns a tile or nullptr from wall[0][4]
*
*/
Tile* Board::getWall(int col, int row) {
    Tile* tile = nullptr;

    if (col <= WALL_DIM && row <= WALL_DIM)
    {
        tile = wall[row][col];
    }

    return tile;
}

/* PRIVATE FUNCTIONS */

/*
* Returns points added according to Azul
* Uses the the most recently placed tile x, y position from
* class variable xPos, yPos usually determined from function endOfTurnBoard
* Points are added for adjacent lines of tiles
*
* Example:
*        Checking a tile at (0,0) where a tiles exists in (1,0)
*        Checks nearby tiles and fins a tile in the horizontal position
*        Finds and adds a point for tile in (1,0) and stops after finding a
*        nullptr in (2,0)
*        returns pointsGained
*
*/
int Board::calculateBoard() {
    //Add the initial point
    int pointsGained = 1;
    int x = 0;
    int y = 0;
    bool horizontalFound = false;
    bool verticalFound = false;


    //Checking Vertical Tiles
    //Checking Up
    //Check immediately in direction otherwise it checks the initial point as a point
    x = xPos - 1;
    y = yPos;
    //Keep checking (x - 1, y) till limit
    while (wall[x][y] != nullptr && !(x < 0))
    {
        pointsGained++;
        x--;
        verticalFound = true;
    }

    x = xPos + 1;
    y = yPos;

    //Checking Down
    while (wall[x][y] != nullptr && !(x > WALL_DIM))
    {
        pointsGained++;
        x++;
        verticalFound = true;
    }

    x = xPos;
    y = yPos + 1;

    //Checking Horizontal
    //Checking Right
    while (wall[x][y] != nullptr && !(y > WALL_DIM))
    {
        pointsGained++;
        y++;
        horizontalFound = true;
    }

    x = xPos;
    y = yPos - 1;
    //Checking Left
    while (wall[x][y] != nullptr && !(y < 0))
    {
        pointsGained++;
        y--;
        horizontalFound = true;
    }

    //If tiles are adjacent on each direction then add one
    //This is for the extra point for having another adjacent tile in a different direction
    if (horizontalFound && verticalFound)
    {
        pointsGained++;
    }


    if (floorAdded == false)
    {
        //Now check floor line
        for (int i = 0; i < FLOOR_DIM; i++)
        {
            //Check if floor has a tile
            if (floor[i] != nullptr)
            {
                //Floor 0 to 1 is -1 points
                if (i <= NEG_ONE_LIMIT)
                {
                    pointsGained -= 1;
                }
                //Floor 2 to 4 is -2 points
                else if (i > NEG_ONE_LIMIT && i <= NEG_TWO_LIMIT)
                {
                    pointsGained -= 2;
                }
                //Floor 4 to 6 is -3 Points
                else if (i > NEG_TWO_LIMIT && i <= NEG_THREE_LIMIT)
                {
                    pointsGained -= 3;
                }
            }
        }
        floorAdded = true;
    }
    return pointsGained;

}

/*
* Checks whether a certain tile face (color) exists in a wall row and returns
* true or false
* Input requires true array coordinates
*
* Parameters:
*           face: TileFace to compare to
*           row: row specified to look at in wall array
*
* Example:
*        Checking red tile in wall row 0 that contains only a blue tile
*        Sees only nullptr and blue tile which does not equal red
*        returns false for exists
*
*/
bool Board::doesFaceExist(TileFace face, int row) {
    bool exists = false;
    //Checks each column in row if a tile with the same color has been finished
    for (int i = 0; i < WALL_DIM; i++)
    {
        if (wall[row][i] != nullptr)
        {
            if (wall[row][i]->getFace() == face)
            {
                exists = true;
            }
        }
    }

    return exists;
}

/*
* Adds a tile to wall row in correct formmating according to Azul
* Input requires true array coordinates
*
* Parameters:
*           face: t the tile to be added
*           row: row specified to look at in wall array
*
* Example:
*        Adding yellow to row 0
*        Yellow column position is 1 + row
*        The yellow tile is added to wall[0][1]
*
*/
void Board::addToWall(int row, Tile* t) {
    /* The pattern for a normal Azul wall can be determined by its column
        Blue is located at 0 + row (loop if over 5)
        Yellow is located at 1 + row
        Only if row starts at 0 like arrays */

    int colPosition = 0;
    TileFace face = t->getFace();

    if (face == Blue)
    {
        colPosition = 0 + row;
    }
    else if (face == Yellow)
    {
        colPosition = 1 + row;
    }
    else if (face == Red)
    {
        colPosition = 2 + row;
    }
    else if (face == Black)
    {
        colPosition = 3 + row;
    }
    else if (face == Light_Blue)
    {
        colPosition = 4 + row;
    }
    //Fix Column position if over 5
    if (colPosition >= 5)
    {
        colPosition = colPosition % WALL_DIM;
    }
    //Place tile and record (x,y) position
    wall[row][colPosition] = t;

    xPos = row;
    yPos = colPosition;
}

/*
* Checks whether a pattern line has no tiles in a row
* Input requires true array coordinates
*
* Parameters:
*           row: row specified to look at in pattern array
*
* Example:
*        Checking row 0 which has no tiles
*        Checks each column in row 0
*        No tiles were counted then return true
*
*/
bool Board::isPatternLineEmpty(int row) {

    bool isEmpty = false;
    int counter = 0;

    //Increment counter if nullptr
    for (int i = 0; i < PATTERN_DIM; i++)
    {
        if (pattern[row][i] == nullptr)
        {
            counter++;
        }
    }

    //If all 5 elements of that row is null then it is empty
    if (counter == PATTERN_DIM)
    {
        isEmpty = true;
    }

    return isEmpty;
}

/* Load/Save Related Functions */

/*
* Adds tile t to specified row and column in pattern array
* This replaces anything in that location
*
* Parameters:
*           t: tile to be added
*           col: column specified for pattern array
*           row: row specified for pattern array
*
* Example:
*        Add tile to row 0, col 0
*        pattern[0][0] now points to tile t
*
*/
void Board::loadToPattern(Tile* t, int col, int row) {
    pattern[row][col] = t;
}

/*
* Adds tile t to left-most free space in floor array
* Nothing occurs if floor is full
*
* Parameters:
*           t: tile to be added
*
* Example:
*        Add tile to floor with 2 tiles already added
*        Find free spot at floor[2]
*        floor[2] now points at t
*
*/
void Board::loadToFloor(Tile* t) {
    //Possible duplicate but to seperate loading and actual gameplay this is better
    bool emptyFound = false;
    //Find one that's empty
    for (int i = 0; i < FLOOR_DIM || emptyFound == true; i++)
    {
        if (floor[i] != nullptr)
        {
            floor[i] = t;
            emptyFound = true;
        }
    }
}

/*
* Adds tile t to specified row and column in wall array
* This replaces anything in that location
*
* Parameters:
*           t: tile to be added
*           col: column specified for wall array
*           row: row specified for wall array
*
* Example:
*        Add tile to row 1, col 1
*        wall[1][1] now points to t
*
*/
void Board::loadToWall(Tile* t, int col, int row) {
     wall[row][col] = t;
}

/*
* Returns a std::string save formmated version of floor
* This is used for saving/loading
*
* Example:
*        PLAYER_1_FLOOR_LINE=F
*
*/
std::string Board::printFloor() {
    std::string toReturn;
    Tile * tile = nullptr;

    for(int i=0; i < FLOOR_DIM; i++) {
        tile = floor[i];
        if(tile != nullptr) {
            toReturn.append(tile->getFaceFormatted(true));
        }
    }

    return toReturn;
}

/*
* Returns a std::string save formmated version of either the pattern or wall
* This is used for saving/loading
*
* Parameters:
*           row: row of 2D array
*           printNulls: Whether the string should include no tile as "-"
*           type: Whether to get from
*                 1 for pattern array
*                 2 for wall array
* Example:
*        PLAYER_1_PATTERN_LINE0=Y
*        or
*        PLAYER_1_MOSAIC_0=-----
*
*/
std::string Board::printPattern(int row, bool printNulls, int type) {
    std::string toReturn;
    for(int i = 0; i < 5; i++) {
        Tile * tile = nullptr;

        if (type == 1) {
            tile = getPattern(i, row);
        } else if (type == 2) {
            tile = getWall(i, row);
        }

        if(tile == nullptr) {
            if(printNulls) {
                toReturn.append("-");
            }
        } else {
            toReturn.append(tile->getFaceFormatted(true));
        }
    }

    return toReturn;
}

/*
* Returns a std::string save formmated version of the board's information
* This is used for saving/loading
*
* Parameters:
*           front: String added to front of save formats, usually the player index such as PLAYER_1
* Example:
*        PLAYER_1_PATTERN_LINE0=
*        PLAYER_1_PATTERN_LINE1=
*        PLAYER_1_PATTERN_LINE2=
*        PLAYER_1_PATTERN_LINE3=
*        PLAYER_1_PATTERN_LINE4=
*        PLAYER_1_FLOOR_LINE=
*        PLAYER_1_MOSAIC_0=-----
*        PLAYER_1_MOSAIC_1=-----
*        PLAYER_1_MOSAIC_2=-----
*        PLAYER_1_MOSAIC_3=-----
*        PLAYER_1_MOSAIC_4=-----
*/
std::string Board::printSaveFormat(std::string front) {
    std::string toReturn;

    for(int i = 0; i < 5; i++) {
        toReturn.append(front + "PATTERN_LINE" + std::to_string(i) + "=" + printPattern(i, false, 1) + "\n");
    }

    toReturn.append(front + "FLOOR_LINE=" + printFloor() + "\n");

    for(int i = 0; i < 5; i++) {
        toReturn.append(front + "MOSAIC_" + std::to_string(i) + "=" + printPattern(i, true, 2) + "\n");
    }

    return toReturn;
}




std::string Board::printBoardRow(bool broken , int row) {

    /* 
    when broken == false:
    'row' is between 0-4 

    when broken == true
    'row' is between 0-1
    */

   bool validCommand = false;
   if (!broken){
       if(row >=0 && row < WALL_DIM){
           validCommand=true;
       }
   }
   else{
       if (row >=0 && row <= 1){
           validCommand = true;
       }
   }

    std::string outputRow = "";

    //Use either dimension since they both would always have the same dimensions
    // for (int row = 0; row < WALL_DIM; row++)
    //Printing 1st row
    if (broken == false && row >=0 && row< WALL_DIM && validCommand){
        int trueRow = row+1;
        outputRow.append(std::to_string(trueRow) + ":");


        //Check each column for Pattern
        for (int col = 0; col < PATTERN_DIM; col++)
        {
            outputRow.append(" ");
            //Check if array is a place a player is allowed to place at
            //Dimension - column gives right to left count of column so col 0 would be col 5
            //If column is greater than the row (amount of tiles that can be placed) then its a space cout
            if (PATTERN_DIM - col > row + 1)
            {
                outputRow.append(" ");
            }
            //If no tile exists then it becomes a .
            else if (pattern[row][col] == nullptr)
            {
                outputRow.append(EMPTY_TILE);
            }
            //Otherwise get shorthand format of face
            else
            {
                outputRow.append(pattern[row][col]->getFaceFormatted(true));
            }

            outputRow.append(" ");
        }

        outputRow.append("||");
        //Check each column for Wall
        //Seperating loop makes it easier to understand as each loop defines which array its printing
        for (int col = 0; col < WALL_DIM; col++)
        {
            outputRow.append(" ");
            //Check if tile is empty & print entire array as it uses all parts
            if (wall[row][col] == nullptr)
            {
                outputRow.append(EMPTY_TILE);
            }
            else
            {
                outputRow.append(wall[row][col]->getFaceFormatted(true));
            }
            outputRow.append(" ");
        }
        //outputRow.append("\n");
    }
    else if (broken && validCommand)
    {
        if (broken)
        {
            if(row == 0){
                outputRow.append("Broken: -1  -1  -2  -2  -2  -3  -3");
            }
            else if (row == 1){
                outputRow.append("      ");

                //Printing Broken Floor Tiles
                for (int i = 0; i < FLOOR_DIM; i++)
                {
                    outputRow.append("  ");
                    if (floor[i] == nullptr)
                    {
                        outputRow.append(EMPTY_TILE);
                    }
                    else
                    {
                        outputRow.append(floor[i]->getFaceFormatted(true));
                    }
                    outputRow.append(" ");
                    //cout << endl;
                }
            }
        }
    }
    return outputRow;
}