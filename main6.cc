//Minesweeper Program
//This is a program designed to play a game of Minesweeper. It generates a board and allows the user to play a fully functioning game of Minesweeper.
//Made by Mark Dreitzler. Finished 5/15/20.

#include <iostream>
#include <cstdlib>
#include <ctime> //needed for RNG

using namespace std;

/*Definition for boardSpace structure
A boardSpace object is created at every space on the board, and stores all of the data needed to play Minesweeper. Each space has 4 variables of data.
isMine - used to determine if this space is a mine. If a mine is selected, the game ends.
adjacentMines - stores how many spaces adjacent to this space on the board are mines. When a space on the board is flipped up, it displays the number of mines adjacent to that space if there are any.
flipped - determines if a space has been flipped up. Spaces that have been flipped cannot be selected and their adjacentMines value is displayed.
flagged - determines if a space has been flagged. Spaces that have not been flipped up can be flagged, and will display a special symbol showing they have been flagged. This is used if you think you have found a mine.*/
struct boardSpace{
  bool isMine;
  int adjacentMines;
  bool flipped;
  bool flagged;
};

int SPACESFLIPPED = 0;

//Adds all mines to the board
void addMines(boardSpace board [][20]){
  int numMines = 40; //edit this if you want more mines
  while (numMines != 0){
    int randLength = rand() % 20; //generate random value for a column in the board
    int randWidth = rand() % 20; //generate random value for a row in the board
    if (board[randLength][randWidth].isMine == false){ //make sure the space chosen is not already a mine
      board[randLength][randWidth].isMine = true; //set the board space to be a mine
      //cout << "Generated mine at position " << randLength << ", " << randWidth << endl;
      numMines--; //decrement numMines by 1
    }
  }
  return;
}

//gets number of nearby mines for each board position
void checkMines(boardSpace board [][20]){
  for (int i = 0; i < 20; i++){ //check every board position
    for (int j = 0; j < 20; j++){
      if (board[i][j].isMine == false){ //if the board position is not a mine
        /*Do a whole ton of test cases
        We need a test case for every possible direction to make sure that all of them are valid and grab the total adjacent mines for each position
        Each space that isn't a mine will have to check 8 different positions
        Remember that i is the row, j is the column, and the larger i is, the lower it is on the board*/
        //Case 1 - Immediate Right
        if ((j+1 < 20) && board[i][j+1].isMine == true){ //this part changes each case
          board[i][j].adjacentMines = board[i][j].adjacentMines + 1; //this part does not
        }
        //Case 2 - Right-Down
        if ((j+1 < 20) && (i+1 < 20) && board[i+1][j+1].isMine == true){
          board[i][j].adjacentMines = board[i][j].adjacentMines + 1;
        }
        //Case 3 - Immediate Down
        if ((i+1 < 20) && board[i+1][j].isMine == true){
          board[i][j].adjacentMines = board[i][j].adjacentMines + 1;
        }
        //Case 4 - Left-Down
        if ((j-1 >= 0) && (i+1 < 20) && board[i+1][j-1].isMine == true){
          board[i][j].adjacentMines = board[i][j].adjacentMines + 1;
        }
        //Case 5 - Immediate Left
        if ((j-1 >= 0) && board[i][j-1].isMine == true){
          board[i][j].adjacentMines = board[i][j].adjacentMines + 1;
        }
        //Case 6 - Left-Up
        if ((j-1 >= 0) && (i-1 >= 0) && board[i-1][j-1].isMine == true){
          board[i][j].adjacentMines = board[i][j].adjacentMines + 1;
        }
        //Case 7 - Immediate Up
        if ((i-1 >= 0) && board[i-1][j].isMine == true){
          board[i][j].adjacentMines = board[i][j].adjacentMines + 1;
        }
        //Case 8 - Right-Up
        if ((j+1 < 20) && (i-1 >= 0) && board[i-1][j+1].isMine == true){
          board[i][j].adjacentMines = board[i][j].adjacentMines + 1;
        }
      }
    }
  }
  return;
}


//function to actually display the board
void displayBoard(boardSpace board [][20]){
  for (int i = 0; i < 20; i++){
    for  (int j = 0; j < 20; j++){
      if ((board[i][j].flagged == true) && (board[i][j].flipped == false)){ //if a board position is flagged and not flipped up
        cout << "F  "; //print F
      }
      else if (board[i][j].flipped == true){ //if a board position is flipped up
        cout << board[i][j].adjacentMines << "  "; //display the number of adjacent mines
      }
      else{
        cout << "[] "; //display an X to signify not being flipped yet
      }
    }
    cout << '\n';
  }
  cout << '\n';
  return;
}




//function to display a menu
void displayMenu(){
  cout << "Select a piece - s" << endl;
  cout << "Flag a piece - f" << endl;
  cout << "Quit - q" << endl;
  cout << "What is your selection? ";
}

void flipSpace(int xCoordinate, int yCoordinate, boardSpace board [][20], bool isGameOver){
  if (board[xCoordinate][yCoordinate].flipped == false){ //if the space has not been flipped yet and is selectable
    //cout << "Space is valid." << endl;
    if (board[xCoordinate][yCoordinate].isMine == true){ //if the checked space is a mine
      cout << "That was a mine. Game over. You flipped up " << SPACESFLIPPED << " spaces in total." << endl; //display total number of spaces flipped
      exit(1); //exit the program
    }
    else if ((board[xCoordinate][yCoordinate].isMine == false) && (board[xCoordinate][yCoordinate].adjacentMines != 0)){ //if the checked space is not a mine but is adjacent to at least one another mine
      //cout << "This space is not a mine, but is adjacent to a mine." << endl;
      SPACESFLIPPED++; //add 1 more space flipped
      board[xCoordinate][yCoordinate].flipped = true; //set board space to flipped so it can't be selected again
      return;
    }
    else{ //recursive case - if there are no adjacent mines, and the piece selected isn't a mine, flip all spaces adjacent to it that haven't been flipped yet.
      SPACESFLIPPED++;
      board[xCoordinate][yCoordinate].flipped = true;
      /*Holy crap, another shit ton of test cases!
      Once again, every single possible adjacent mine needs to be checked to make sure it actually exists.
      This time, we don't need to check for mines, since we already know there are no adjacent mines.
      Then, for each one that exists, make a recursive call to this function with those coordinates.
      Our base case is when the space is not a mine but has an adjacent mine. This means the program will continue to flip up adjacent mines until it encounters nothing but spaces adjacent to mines.
      Change 5/11/20 - Added check if space is not flipped to prevent seg fault*/
      //Case 1 - Immediate right
      if ((yCoordinate+1 < 20)){ //this part changes each case
        flipSpace(xCoordinate, yCoordinate + 1, board, isGameOver);
      }
      //Case 2 - Right-Down
      if ((yCoordinate+1 < 20) && (xCoordinate+1 < 20)){
        flipSpace(xCoordinate + 1, yCoordinate + 1, board, isGameOver);
      }
      //Case 3 - Immediate Down
      if ((xCoordinate+1 < 20)){
        flipSpace(xCoordinate + 1, yCoordinate, board, isGameOver);
      }
      //Case 4 - Left-Down
      if ((yCoordinate-1 >= 0) && (xCoordinate+1 < 20)){
        flipSpace(xCoordinate + 1, yCoordinate - 1, board, isGameOver);
      }
      //Case 5 - Immediate Left
      if ((yCoordinate-1 >= 0)){
        flipSpace(xCoordinate, yCoordinate - 1, board, isGameOver);
      }
      //Case 6 - Left-Up
      if ((yCoordinate-1 >= 0) && (xCoordinate-1 >= 0)){
        flipSpace(xCoordinate - 1, yCoordinate - 1, board, isGameOver);
      }
      //Case 7 - Immediate Up
      if ((xCoordinate-1 >= 0)){
        flipSpace(xCoordinate - 1, yCoordinate, board, isGameOver);
      }
      //Case 8 - Right-Up
      if ((yCoordinate+1 < 20) && (xCoordinate-1 >= 0)){
        flipSpace(xCoordinate - 1, yCoordinate + 1, board, isGameOver);
      }
      return;
    }
  }
  return;
}



int main(){
  srand(time(NULL)); //generates seed for RNG
  boardSpace board [20][20]; //create 2d array to store the board
  for (int i = 0; i < 20; i++){ //code to populate the board with initial values for the board (no mines added in yet)
    for (int j = 0; j < 20; j++){
      boardSpace space; //create new boardSpace object
      space.isMine = false; //set all initial values to not being a mine, no adjacent mines, not flipped, not flagged
      space.adjacentMines = 0;
      space.flipped = false;
      space.flagged = false;
      board[i][j] = space; //add space to the board
    }
  }
  addMines(board); //add all mines to the board by passing the array by reference
  checkMines(board); //set up the board properly so all the spaces adjacent to bombs are accounted for
  bool isGameOver = false; //initialize variable to determine if the game is over
  char optionInput; //variable to determine what the player wants to do
  int xCoordinate; //variable to determine the x coordinate of the player's choice
  int yCoordinate; //variable to determine the y coordinate of the player's choice
  while (!isGameOver == true){
    displayBoard(board); //show the current board
    displayMenu(); //show the menu
    cin >> optionInput; //input player option for what they want to do
    switch(optionInput) //create switch statement to determine what to do
    {
      case 's':
      cout << "Enter the coordinate data in the following way: y x" << endl; //enter coordinates to be flipped
      cin >> xCoordinate >> yCoordinate;
      flipSpace(xCoordinate, yCoordinate, board, isGameOver); //flip the space and check for a game over
      break;
      case 'f':
      cout << "Enter the coordinate data in the following way: y x" << endl; //enter coordinates to be flipped
      cin >> xCoordinate >> yCoordinate;
      board[xCoordinate][yCoordinate].flagged = true; //flag the target space
      break;
      case 'q':
      cout << "Thank you for playing." << endl;
      return 0; //exit the program
      break;
      default: //used if input is not s, f, or q
      cout << "Not a valid input." << endl;
    }
    if (SPACESFLIPPED == 360){
      cout << "CONGRATULATIONS! YOU FLIPPED UP ALL THE SPACES THAT WEREN'T MINES! A WINNER IS YOU!!" << endl;
      return 0;
    }
  }
  return 0; //end the program
}
