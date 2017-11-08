#include <stdio.h> //import the standard c libraries
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> //import the boolean type and functions
#include <time.h> //import the time function
//declare structures here
//declare the structureof the player
struct PlayerBoard {
  char cells[7][7];
  int ships[3][5];
  int ShipHits;
  int orientation;
  int row;
  int col;
};
//declare the structure of the computer
struct ComputerBoard {
  char cells[7][7];
  int ShipHits;
  int row;
  int col;
  int orientation;
};
//declare the structure to deal with the computer's guesses
struct ComputerGuessDisplay {
  char cells[7][7];
  bool ShipInArea;
  int guessArea;
  int guessesInArea;
  int rowHit;
  int colHit;
  int shipDirection;
  int guessesInArea0;
  int guessesInArea1;
  int guessesInArea2;
  int guessesInArea3;
};
//declare a structure to deal with the player's guesses
struct PlayerGuessDisplay {
  char cells[7][7];
  int ships[3][5];
  int row;
  int col;
};
//synonyms for easier typing
typedef struct PlayerBoard PlayerBoard;
typedef struct ComputerBoard ComputerBoard;
typedef struct PlayerGuessDisplay PlayerGuess;
typedef struct ComputerGuessDisplay ComputerGuess;
//declare constants for misses (G), Unknown (?), Hit (H), Blank(.)
enum {G = 'm', X = '?', S = 'S', H = 'H', B = '.'};
//inialise blank boards (? for player guesses), as well as all variables
//for the computer guessing
void InitialiseBoards(ComputerBoard *com, PlayerBoard *plyr, PlayerGuess *guess, ComputerGuess *comGuess) {
  for (int i = 0; i < 7; i++) {
    for (int j = 0; j < 7; j++) {
      plyr->cells[i][j] = B;
      com->cells[i][j] = B;
      guess->cells[i][j] = X;
      comGuess->cells[i][j] = B;
    }
  }
  comGuess->guessArea = 0;
  comGuess->rowHit = 0;
  comGuess->colHit = 0;
  comGuess->guessesInArea = 0;
  comGuess->ShipInArea = false;
  comGuess->shipDirection = 0;
  comGuess->guessesInArea0 = 12;
  comGuess->guessesInArea1 = 16;
  comGuess->guessesInArea2 = 12;
  comGuess->guessesInArea3 = 9;
  com->ShipHits = 0;
  plyr->ShipHits = 0;
}
//function to add a ship to the board for the computer
//also stores where the computers ships are stored in an array for guessing
void moveComputer(ComputerBoard *com, PlayerGuess *guessBoard, int counter) {
  com->cells[com->row][com->col] = S;
  guessBoard->ships[counter][4] = 0;
  if (com->orientation == 0) {
    com->cells[(com->row)-1][com->col] = S;
    com->cells[(com->row)-2][com->col] = S;
    guessBoard->ships[counter][0] = com->col;
    guessBoard->ships[counter][1] = com->row;
    guessBoard->ships[counter][2] = (com->row)-1;
    guessBoard->ships[counter][3] = (com->row)-2;
  }
  else {
    com->cells[com->row][(com->col)+1] = S;
    com->cells[com->row][(com->col)+2] = S;
    guessBoard->ships[counter][0] = com->row;
    guessBoard->ships[counter][1] = com->col;
    guessBoard->ships[counter][2] = (com->col)+1;
    guessBoard->ships[counter][3] = (com->col)+2;
  }
}
//draw the player's guessing board (keeps track of guesses)
void drawBoardGuesses(PlayerGuess guess, PlayerBoard plyr) {
  printf("\n");
  char letters[7] = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};
  printf("    1  2  3  4  5  6  7                1  2  3  4  5  6  7\n");
  printf("   ----------------------             ----------------------\n");
  for (int i = 0; i < 7; i++) {
    printf("%c  |%c |%c |%c |%c |%c |%c |%c |          %c  |%c |%c |%c |%c |%c |%c |%c |\n",
    letters[i], guess.cells[i][0], guess.cells[i][1], guess.cells[i][2],
    guess.cells[i][3], guess.cells[i][4], guess.cells[i][5], guess.cells[i][6],
    letters[i], plyr.cells[i][0], plyr.cells[i][1], plyr.cells[i][2],
    plyr.cells[i][3], plyr.cells[i][4], plyr.cells[i][5], plyr.cells[i][6]);
    printf("   ----------------------             ----------------------\n");
  }
  printf("\n");
}
//function to draw the player's guesses in a board
bool ValidComputer(int orientation, int row, int column, ComputerBoard *com) {
  if (orientation == 0) { //check vertical placements
    if (row <= 1) {return false;}
    if (com->cells[row][column] != B) {return false;}
    if (com->cells[row-1][column] != B) {return false;}
    if (com->cells[row-2][column] != B) {return false;}
  }
  else { //placement must be horizontal
    if (column >= 5) {return false;}
    if (com->cells[row][column] != B) {return false;}
    if (com->cells[row][column+1] != B)
    if (com->cells[row][column+2] != B) {return false;}
  }
  com->orientation = orientation;
  com->row = row;
  com->col = column;
  return true;
}
//function to add a ship that the player has input
//also stores where the ships are stored for guessing by computer later
void movePlayer(PlayerBoard *plyr, int counter) {
  plyr->cells[plyr->row][plyr->col] = S;
  plyr->ships[counter][4] = 0;
  if (plyr->orientation == 0) {
    plyr->cells[(plyr->row)-1][plyr->col] = S;
    plyr->cells[(plyr->row)-2][plyr->col] = S;
    plyr->ships[counter][0] = plyr->col;
    plyr->ships[counter][1] = plyr->row;
    plyr->ships[counter][2] = (plyr->row)-1;
    plyr->ships[counter][3] = (plyr->row)-2;
  }
  else {
    plyr->cells[plyr->row][(plyr->col)+1] = S;
    plyr->cells[plyr->row][(plyr->col)+2] = S;
    plyr->ships[counter][0] = plyr->row;
    plyr->ships[counter][1] = plyr->col;
    plyr->ships[counter][2] = (plyr->col)+1;
    plyr->ships[counter][3] = (plyr->col)+2;
  }
}
//function that sets us a game for the computer
int InitComputer(ComputerBoard *com, PlayerGuess *guessBoard) {
  int FirstRow = (rand() % 5) + 2;//generate the first ship (vertical ship)
  int firstCol = (rand() % 7);
  int Oriented = 0; //orientation = vertical (0), horizontal (1)
  int counter = 2;
  bool shipNotPlaced = true;

  while (shipNotPlaced) {
    if(ValidComputer(Oriented, FirstRow, firstCol, com)) { //check validity
      moveComputer(com, guessBoard, counter); //add first ship
      shipNotPlaced = false;
    }
    else {
      FirstRow = (rand() % 5) + 2; //generate the first ship (vertical ship)
      firstCol = (rand() % 7);
    }
  }

  while (counter > 0) { //generate the other 2 ships in the same way
    Oriented = 1;
    FirstRow = (rand() % 7);
    firstCol = (rand() % 5) + 2;
    if (ValidComputer(Oriented, FirstRow, firstCol, com)) {
      counter = counter - 1;
      moveComputer(com, guessBoard, counter);
    }
    else {
      FirstRow = (rand() % 7);
      firstCol = (rand() % 5) + 2;
    }
  }
  return 0;
}
//draw the player's board
void drawBoardInitial(PlayerBoard plyr) {
  printf("\n");
  char letters[7] = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};
  printf("    1  2  3  4  5  6  7   \n");
  printf("   ----------------------\n");
  for (int i = 0; i < 7; i++) {
    printf("%c  |%c |%c |%c |%c |%c |%c |%c |\n",
    letters[i], plyr.cells[i][0], plyr.cells[i][1], plyr.cells[i][2],
    plyr.cells[i][3], plyr.cells[i][4], plyr.cells[i][5], plyr.cells[i][6]);
    printf("   ----------------------\n");
  }
  printf("\n");
}
//print game rules for user
int help() {
  //print game rules
  printf("The aim of the game here is to sink the computer's battleships before "
  "it sinks yours. At the start, place your 3 ships on the board by typing a "
  "direction - either h or v for horizontal or vertical, and a row letter, "
  "a-g, and then a column number 1-7. Your ship will then appear as 3 'S' characters "
  "on the board, starting from your guess position, and either branching right "
  "(if you went for horizontal) or branching upwards (if you guessed vertical). "
  "An example of a valid input would be hf5, va4 etc. To guess where a computer "
  "ship is hiding, simply type a row letter and column number, e.g. a1. You and "
  "the computer take it in turns to guess where each others ships are. If you "
  "hit a ship, you get another go. Characters on the board are: H (hit), S(ship), m(miss), .(empty), ?(unknown). "
  "First player to sink all enemy ships wins. Good luck!\n");
  //check that the user is happy to continue playing
  char onwards[10];
  printf("Ready to play on? Enter 'y' for yes, and anything else for no\n");
  fgets(onwards, 10, stdin);
  if (strcmp(onwards, "y\n") == 0) { //check that the user has entered yes
    return 0; //return back to the main function to start game
  }
  else {
    help(); //if unhappy, the user can print the rules again if they wish
    return 0;
  }
  return 0;
}
//function to check common validity errors
bool GeneralValid(char *input, int firstChar, int secondChar, int length) {
  if (strlen(input) != length) {return false;}
  if (input[firstChar] > 103 || input[firstChar] < 97) {return false;}
  if (input[secondChar] > 55 || input[secondChar] < 49) {return false;}
  return true;
}
//function to convert the string input of row and column to numeric
void SetRowCol(char *input, int *row, int *column, int firstChar, int secondChar) {
  if (input[firstChar] == 'a') {*row = 0;}
  else if (input[firstChar] == 'b') {*row = 1;}
  else if (input[firstChar] == 'c') {*row = 2;}
  else if (input[firstChar] == 'd') {*row = 3;}
  else if (input[firstChar] == 'e') {*row = 4;}
  else if (input[firstChar] == 'f') {*row = 5;}
  else if (input[firstChar] == 'g') {*row = 6;}
  else {*row = 0;}

  if (input[secondChar] == '1') {*column = 0;}
  else if (input[secondChar] == '2') {*column = 1;}
  else if (input[secondChar] == '3') {*column = 2;}
  else if (input[secondChar] == '4') {*column = 3;}
  else if (input[secondChar] == '5') {*column = 4;}
  else if (input[secondChar] == '6') {*column = 5;}
  else if (input[secondChar] == '7') {*column = 6;}
  else {*column = 0;}
}
//function to check validity of player ship input
bool valid(char *input, PlayerBoard *plyr) {
  int orientation = 0;
  int row = 0;
  int column = 0;

  if(GeneralValid(input, 1, 2, 4) == false) {return false;}
  if (input[0] != 'h' && input[0] != 'v') {return false;}

  if (input[0] == 'v') {orientation = 0;} //set orientation
   else {orientation = 1;}

  SetRowCol(input, &row, &column, 1, 2);

  if (orientation == 0) {
    if (row <= 1) {return false;}
    if (plyr->cells[row][column] != B) {return false;}
    if (plyr->cells[row-1][column] != B) {return false;}
    if (plyr->cells[row-2][column] != B) {return false;}
  }
  else {
    if (column >= 5) {return false;}
    if (plyr->cells[row][column] != B) {return false;}
    if (plyr->cells[row][column+1] != B)
    if (plyr->cells[row][column+2] != B) {return false;}
  }
  plyr->orientation = orientation;
  plyr->row = row;
  plyr->col = column;
  return true;
}
//function to check validity of player guesses
bool validGuess(PlayerGuess *guessBoard, char *input) {
  int row = 0;
  int column = 0;

  if(GeneralValid(input, 0, 1, 3) == false) {return false;}
  SetRowCol(input, &row, &column, 0, 1);
  if (guessBoard->cells[row][column] != X) {return false;}
  guessBoard->row = row;
  guessBoard->col = column;
  return true;
}
//function that captures and deals with player input
void PlayerInput(PlayerBoard *plyr) {
  char ShipInput[10];
  int counter = 3;
  printf("Enter where you want to place your 3 ships. Enter a bearing (h or v) followed by a co-ordinate e.g. hf5\n");
  while (counter > 0) { //keep going until 3 ships are on the board
    drawBoardInitial(*plyr);
    fgets(ShipInput, 10, stdin); //get user input
    if (valid(ShipInput, plyr) == true) {
      movePlayer(plyr, counter-1);
      counter = counter - 1;
    }
    else {
      printf("Invalid addition of ship. Enter again.\n");
    }
  }
  drawBoardInitial(*plyr);
}
//function to determine which board area the computer's guess was in
//and then reduce the number of shots remaining in that area
void checkArea(ComputerGuess *comGuess) {
  if (comGuess->rowHit <= 3 && comGuess->colHit <= 2) {
    comGuess->guessesInArea0 = comGuess->guessesInArea0 - 1;
  }
  else if (comGuess->rowHit <= 3 && comGuess->colHit <= 6) {
    comGuess->guessesInArea1 = comGuess->guessesInArea1 - 1;
  }
  else if (comGuess->rowHit <= 6 && comGuess->colHit <= 3) {
    comGuess->guessesInArea2 = comGuess->guessesInArea2 - 1;
  }
  else if (comGuess->rowHit <= 6 && comGuess->colHit <= 6) {
    comGuess->guessesInArea3 = comGuess->guessesInArea3 - 1;
  }
}
//function to reduce the number of remaining shots in the current area
void CountShots(ComputerGuess *comGuess) {
  if (comGuess->guessArea == 0) {
    comGuess->guessesInArea0 = comGuess->guessesInArea0 - 1;
  }
  else if (comGuess->guessArea == 1) {
    comGuess->guessesInArea1 = comGuess->guessesInArea1 - 1;
  }
  else if (comGuess->guessArea == 2) {
    comGuess->guessesInArea2 = comGuess->guessesInArea2 - 1;
  }
  else if (comGuess->guessArea == 3) {
    comGuess->guessesInArea3 = comGuess->guessesInArea3 - 1;
  }
}
//function that deals with a player ship being hit by the computer
void ShipHit(PlayerBoard *plyr, ComputerGuess *comGuess, int row, int col) {
  plyr->cells[row][col] = H;
  plyr->ShipHits = plyr->ShipHits + 1;
  comGuess->cells[row][col] = G;
  checkArea(comGuess);
  printf("Computer hit your ship at (%d, %d)\n", row+1, col+1);
}
//function that deals with the computer hitting a blank space
void BlankHit(PlayerBoard *plyr, ComputerGuess *comGuess, int row, int col) {
  plyr->cells[row][col] = G;
  comGuess->cells[row][col] = G;
  comGuess->shipDirection = comGuess->shipDirection + 1;
}
//function that deals with the edge cases for the first column (col 0)
bool Column0EdgeCase(ComputerGuess *comGuess, bool *guessInvalid) {
  if (comGuess->colHit == 0 && comGuess->shipDirection == 3) {
    comGuess->ShipInArea = false;
    comGuess->shipDirection = 0;
    *guessInvalid = true;
    return true;
  }
  return false;
}
//function that deals with the edge cases for the last column (col 6)
bool Column6EdgeCase(ComputerGuess *comGuess) {
  if (comGuess->colHit == 6 && comGuess->shipDirection == 1) {
    comGuess->shipDirection = 2;
    return true;
  }
  return false;
}
//function that deals with the edge cases for the first row (row 0)
bool Row0EdgeCase(ComputerGuess *comGuess) {
  if (comGuess->rowHit == 0 && comGuess->shipDirection == 0) {
    comGuess->shipDirection = 1;
    return true;
  }
  return false;
}
//function that deals with the edge cases for the last row (row 6)
bool Row6EdgeCase(ComputerGuess *comGuess) {
  if (comGuess->rowHit == 6 && comGuess->shipDirection == 2) {
    comGuess->shipDirection = 3;
    return true;
  }
  return false;
}
//function that fires North (shipDirection '0') for the computer
int fireNorth(PlayerBoard *plyr, ComputerGuess *comGuess) {
  //need to check each case of board state (S,H,G,B)
  char state = plyr->cells[(comGuess->rowHit)-1][(comGuess->colHit)];

  if (state == S) {
    ShipHit(plyr, comGuess, (comGuess->rowHit)-1, comGuess->colHit);
    comGuess->rowHit = comGuess->rowHit - 1;
    return 1;
  }
  else if (state == H || state == G) {
    comGuess->shipDirection = comGuess->shipDirection + 1;
  }
  else { //case is blank
    BlankHit(plyr, comGuess, (comGuess->rowHit)-1, comGuess->colHit);
    comGuess->rowHit = comGuess->rowHit - 1;
    checkArea(comGuess);
    comGuess->rowHit = comGuess->rowHit + 1;
    return 0;
  }
  return 2;
}
//function that fires East (shipDirection '1') for the computer
int fireEast(PlayerBoard *plyr, ComputerGuess *comGuess) {
  //need to check each case of board state (S,H,G,B)
  char state = plyr->cells[(comGuess->rowHit)][(comGuess->colHit)+1];

  if (state == S) {
    ShipHit(plyr, comGuess, comGuess->rowHit, (comGuess->colHit)+1);
    comGuess->colHit = comGuess->colHit + 1;
    return 1;
  }
  else if (state == H || state == G) {
    comGuess->shipDirection = comGuess->shipDirection + 1;
  }
  else { //blank state
    BlankHit(plyr, comGuess, comGuess->rowHit, (comGuess->colHit)+1);
    comGuess->colHit = comGuess->colHit + 1;
    checkArea(comGuess);
    comGuess->colHit = comGuess->colHit - 1;
    return 0;
  }
  return 2;
}
//function that fires South (shipDirection '2') for the computer
int fireSouth(PlayerBoard *plyr, ComputerGuess *comGuess) {
  //need to check each case of board state (S,H,G,B)
  char state = plyr->cells[(comGuess->rowHit)+1][(comGuess->colHit)];

  if (state == S) {
    ShipHit(plyr, comGuess, (comGuess->rowHit)+1, comGuess->colHit);
    comGuess->rowHit = comGuess->rowHit + 1;
    return 1;
  }
  else if (state == H || state == G) {
    comGuess->shipDirection = comGuess->shipDirection + 1;
  }
  else { //blank state
    BlankHit(plyr, comGuess, (comGuess->rowHit)+1, comGuess->colHit);
    comGuess->rowHit = comGuess->rowHit + 1;
    checkArea(comGuess);
    comGuess->rowHit = comGuess->rowHit - 1;
    return 0;
  }
  return 2;
}
//function that fires West (shipDirection '3') for the computer
int fireWest(PlayerBoard *plyr, ComputerGuess *comGuess, bool *guessInvalid) {
  //need to check each case of board state (S,H,G,B)
  char state = plyr->cells[(comGuess->rowHit)][(comGuess->colHit)-1];

  if (state == S) {
    ShipHit(plyr, comGuess, (comGuess->rowHit), (comGuess->colHit)-1);
    comGuess->colHit = comGuess->colHit - 1;
    return 1;
  }
  //if the state is H,G,or B in the west direction, the computer needs to move on and guess elsewhere
  else if (state == H || state == G) {
    comGuess->ShipInArea = false;
    comGuess->shipDirection = 0;
    *guessInvalid = true;
    return 0;
  }
  else { //blank state
    BlankHit(plyr, comGuess, (comGuess->rowHit), (comGuess->colHit)-1);
    comGuess->ShipInArea = false;
    comGuess->colHit = comGuess->colHit - 1;
    checkArea(comGuess);
    comGuess->colHit = comGuess->colHit + 1;
    return 1;
  }
  return 0;
}
//function that manages another shot for the computer and decision of where to fire
bool HitAgain(PlayerBoard *plyr, ComputerGuess *comGuess, bool *guessInvalid) {
  int status = 0;
  //function checks a (current) direction and fires into it. Returns whether shot was hit or miss
  Row0EdgeCase(&*comGuess);
  if (comGuess->shipDirection == 0) {
    status = fireNorth(plyr, comGuess);
    if (status == 1) {return true;}
    else if (status == 0) {return false;}
  }
  Column6EdgeCase(&*comGuess);
  if (comGuess->shipDirection == 1) {
    status = fireEast(plyr, comGuess);
    if (status == 1) {return true;}
    else if (status == 0) {return false;}
  }
  Row6EdgeCase(&*comGuess);
  if (comGuess->shipDirection == 2) {
    status = fireSouth(plyr, comGuess);
    if (status == 1) {return true;}
    else if (status == 0) {return false;}
  }
  Column0EdgeCase(comGuess, guessInvalid);
  if (comGuess->shipDirection == 3) {
    status = fireWest(plyr, comGuess, guessInvalid);
    if (status == 1) {return true;}
    else if (status == 0) {return false;}
  }
  return false;
}
//function that checks whether the computer should guess in another area
void setGuessArea(ComputerGuess *comGuess) {
  if (comGuess->guessesInArea == 2) {
    comGuess->guessesInArea = 0;
    if(comGuess->guessArea == 3) {
      comGuess->guessArea = 0;
    }
    else {
      comGuess->guessArea = comGuess->guessArea + 1;
    }
  }
}
//function that deals with managing a shot for the computer
void BlankFound(PlayerBoard *plyr, ComputerGuess *comGuess, int rowGuess, int colGuess, bool *guessInvalid, bool *targetHit) {
  *guessInvalid = false;
  comGuess->guessesInArea = comGuess->guessesInArea + 1;
  comGuess->cells[rowGuess][colGuess] = G;
  if (plyr->cells[rowGuess][colGuess] == S) { //check if the unguessed blank is a ship
    printf("Computer hit your battleship at (%d, %d)!\n", rowGuess+1, colGuess+1);
    plyr->cells[rowGuess][colGuess] = H;
    plyr->ShipHits = plyr->ShipHits + 1;
    comGuess->ShipInArea = true;
    comGuess->rowHit = rowGuess;
    comGuess->colHit = colGuess;
    comGuess->shipDirection = 0;
    *targetHit = true;
    CountShots(comGuess);
    while(*targetHit) {
      if (HitAgain(plyr, comGuess, guessInvalid) == false) {
        *targetHit = false;
      }
    }

  } else {plyr->cells[rowGuess][colGuess] = G; CountShots(&*comGuess);}
}
//function that generates a guess for the computer
void GenerateGuess(PlayerBoard *plyr, ComputerGuess *comGuess, int *rowGuess, int *colGuess, bool *guessInvalid, bool *targetHit) {
  while (*guessInvalid) { //keep generating until a valid guess
    if (comGuess->guessArea == 0) {
      if (comGuess->guessesInArea0 > 0) {
        *rowGuess = (rand() % 4);
        *colGuess = (rand() % 3);
      } else {comGuess->guessArea = 1;}
    }
    else if (comGuess->guessArea == 1) {
      if (comGuess->guessesInArea1 > 0) {
        *rowGuess = (rand() % 4);
        *colGuess = (rand() % 4) + 3;
      } else {comGuess->guessArea = 2;}
    }
    else if (comGuess->guessArea == 2) {
      if (comGuess->guessesInArea2 > 0) {
        *rowGuess = (rand() % 3) + 4;
        *colGuess = (rand() % 4);
      } else {comGuess->guessArea = 3;}
    }
    else if (comGuess->guessArea == 3) {
      if (comGuess->guessesInArea3 > 0) {
        *rowGuess = (rand() % 3) + 4;
        *colGuess = (rand() % 3) + 4;
      }
      else {comGuess->guessArea = 0;}
    }
    if (comGuess->cells[*rowGuess][*colGuess] == B) { //check guess is valid
      BlankFound(plyr, comGuess, *rowGuess, *colGuess, guessInvalid, targetHit);
    }
  }
}
//function that checks whether the computer has won
bool CheckComputerWin(PlayerBoard *plyr, ComputerGuess *comGuess) {
  if (plyr->ShipHits == 9) {
    drawBoardInitial(*plyr);
    return true;
  }
  if ((comGuess->guessesInArea0 == 0) && (comGuess->guessesInArea1 == 0) && (comGuess->guessesInArea2 == 0) && (comGuess->guessesInArea0 == 0)) {
    drawBoardInitial(*plyr);
    return true;
  }
  return false;
}
//function that determines whether the computer has hit a target in the previous
//2 turns and hence should stay and locate the ship, or if it should move on
void StayInZone(PlayerBoard *plyr, ComputerGuess *comGuess, bool *targetHit, bool *guessInvalid) {
  if (comGuess->ShipInArea == true) {
    *targetHit = true;
    *guessInvalid = false;
    while(*targetHit) {
      if (HitAgain(plyr, comGuess, guessInvalid) == false) {
        *targetHit = false;
      }
    }
  }
}
//function that deals with giving the computer it's turn
int ComputerPlay(ComputerBoard *com, ComputerGuess *comGuess, PlayerBoard *plyr) {
  int rowGuess = 0;
  int colGuess = 0;
  bool targetHit = false;
  bool guessInvalid = true;
  StayInZone(plyr, comGuess, &targetHit, &guessInvalid);
  setGuessArea(&*comGuess);
  GenerateGuess (plyr, comGuess, &rowGuess, &colGuess, &guessInvalid, &targetHit);
  if(CheckComputerWin(plyr, comGuess) == true) {
    return 0;
  }
  return 1;
}
//outputs whether the player has sunk a computer's ship
void ComputerShipSunk(PlayerGuess *com) {
  for (int i = 0; i < 3; i++) {
    if (com->ships[i][4] == 0) {
      if (com->cells[com->ships[i][0]][com->ships[i][1]] == H &&
          com->cells[com->ships[i][0]][com->ships[i][2]] == H &&
          com->cells[com->ships[i][0]][com->ships[i][3]] == H) {
            com->ships[i][4] = 1;
            printf("Ship sunk! Tango down!\n");
          }
      else if (com->cells[com->ships[i][1]][com->ships[i][0]] == H &&
              com->cells[com->ships[i][2]][com->ships[i][0]] == H &&
              com->cells[com->ships[i][3]][com->ships[i][0]] == H) {
            com->ships[i][4] = 1;
            printf("Ship sunk! Tango down!\n");
          }
    }
  }
}
//function that checks whether the player has won
void CheckPlayerWin(ComputerBoard *com, PlayerGuess *guessBoard, bool *inputInvalid, bool *gameEnd) {
  if (com->ShipHits == 9) {
    printf("All targets destroyed! You win!\n");
    *inputInvalid = false;
    *gameEnd = true;
  } else {
    ComputerShipSunk(guessBoard);
    printf("Target hit! Take another shot...\n");
    *inputInvalid = true;
  }
}
//function that checks if the player hit or missed, and deals with the result
void CheckShot(ComputerBoard *com, PlayerGuess *guessBoard, bool *inputInvalid, bool *gameEnd) {
  if (com->cells[guessBoard->row][guessBoard->col] != B) {
    com->ShipHits = com->ShipHits + 1;
    guessBoard->cells[guessBoard->row][guessBoard->col] = H;
    CheckPlayerWin(com, guessBoard, inputInvalid, gameEnd);
  }
  else {
    guessBoard->cells[guessBoard->row][guessBoard->col] = B;
    *inputInvalid = false;
  }
}
//function that handles player's guess
void PlayerGuesses(ComputerBoard *com, PlayerGuess *guessBoard, PlayerBoard *plyr, bool *inputInvalid, bool *gameEnd) {
  char guessInput[10];
  while (*inputInvalid) { //keep going until an input is valid
    printf("Guess where an enemy ship is hiding in the left board below.....\n");
    drawBoardGuesses(*guessBoard, *plyr);
    fgets(guessInput, 10, stdin);
    if (validGuess(guessBoard, guessInput)) {
      CheckShot(com, guessBoard, inputInvalid, gameEnd);
    }
    else {
      printf("Input invalid. Try again.\n");
    }
  }
}
//function that determines whether the computer should keep guessing (if nobody has won)
void ComputerGuesses(ComputerBoard *com, ComputerGuess *comGuess, PlayerBoard *plyr, bool *inputInvalid, bool *gameEnd) {
  if(ComputerPlay(com, comGuess, plyr) == 0) {
    *inputInvalid = false;
    *gameEnd = true;
    printf("Computer Wins!\n");
  }
}
//function that tells the user if their ship has been sunk
void PlayerShipSunk(PlayerBoard *plyr) {
  for (int i = 0; i < 3; i++) {
    if (plyr->ships[i][4] == 0) {
      if (plyr->cells[plyr->ships[i][0]][plyr->ships[i][1]] == H &&
          plyr->cells[plyr->ships[i][0]][plyr->ships[i][2]] == H &&
          plyr->cells[plyr->ships[i][0]][plyr->ships[i][3]] == H) {
            plyr->ships[i][4] = 1;
            printf("Computer sunk your battleship!\n");
          }
      else if (plyr->cells[plyr->ships[i][1]][plyr->ships[i][0]] == H &&
              plyr->cells[plyr->ships[i][2]][plyr->ships[i][0]] == H &&
              plyr->cells[plyr->ships[i][3]][plyr->ships[i][0]] == H) {
            plyr->ships[i][4] = 1;
            printf("Computer sunk your battleship!\n");
          }
    }
  }
}
//function that deals with keeping the game running
int Play(PlayerBoard *plyr, ComputerBoard *com, PlayerGuess *guessBoard, ComputerGuess *comGuess) {
  bool inputInvalid = true;
  bool gameEnd = false;

  while (! gameEnd) {
    inputInvalid = true;
    PlayerGuesses(com, guessBoard, plyr, &inputInvalid, &gameEnd);
    ComputerShipSunk(guessBoard);
    drawBoardGuesses(*guessBoard, *plyr);
    if (gameEnd == false) {
      ComputerGuesses(com, comGuess, plyr, &inputInvalid, &gameEnd);
      PlayerShipSunk(plyr);
    }
  }
  return 0;
}
//function to kick off the game
int InitialiseGame() {
  PlayerBoard plyr;
  ComputerBoard com;
  PlayerGuess guessBoard;
  ComputerGuess comGuess;
  InitialiseBoards(&com, &plyr, &guessBoard, &comGuess);
  PlayerInput(&plyr);
  InitComputer(&com, &guessBoard);
  Play(&plyr, &com, &guessBoard, &comGuess);
  return 0;
}

//AutoTesting-******************************************************************

//I have used the oxo test functions (eqc, eqi, eqb) since they are relatively
//concise compared to the test function i would have used. I hope this is ok. I
//have made sure to understand how the test functions work.

enum type { CHAR, INT, BOOL };

// Check that two ints, chars or bools are equal
int CheckEq(enum type t, int x, int y) {
    static int n = 0;
    n++;
    if (x != y) {
        if (t==CHAR) fprintf(stderr, "Test %d gives %c not %c", n, x, y);
        if (t==INT) fprintf(stderr, "Test %d gives %d not %d", n, x, y);
        if (t==BOOL && x) fprintf(stderr, "Test %d gives true not false", n);
        if (t==BOOL && y) fprintf(stderr, "Test %d gives false not true", n);
        exit(1);
    }
    return n;
}

// More specific versions of the eq function
int CheckEqChar(char x, char y) { return CheckEq(CHAR, x, y); }
int CheckEqInt(int x, int y) { return CheckEq(INT, x, y); }
int CheckEqBool(bool x, bool y) { return CheckEq(BOOL, x, y); }

void testing() {
  PlayerBoard Player;
  ComputerBoard Computer;
  ComputerGuess ComGuess;
  PlayerGuess PlayGuess;
  bool guessInvalid = false;
  bool gameEnd = false;

  InitialiseBoards(&Computer, &Player, &PlayGuess, &ComGuess);
  // Tests 1 to 30 test initialisation of boards and computer's guess structure
  CheckEqChar(Player.cells[0][0], B);
  CheckEqChar(Player.cells[6][6], B);
  CheckEqChar(Player.cells[3][4], B);
  CheckEqChar(Player.cells[4][3], B);
  CheckEqChar(Player.cells[2][1], B);
  CheckEqChar(Computer.cells[0][0], B);
  CheckEqChar(Computer.cells[6][6], B);
  CheckEqChar(Computer.cells[3][4], B);
  CheckEqChar(Computer.cells[4][3], B);
  CheckEqChar(Computer.cells[2][1], B);
  CheckEqChar(PlayGuess.cells[0][0], X);
  CheckEqChar(PlayGuess.cells[6][6], X);
  CheckEqChar(PlayGuess.cells[3][4], X);
  CheckEqChar(PlayGuess.cells[4][3], X);
  CheckEqChar(PlayGuess.cells[2][1], X);
  CheckEqChar(ComGuess.cells[0][0], B);
  CheckEqChar(ComGuess.cells[6][6], B);
  CheckEqChar(ComGuess.cells[3][4], B);
  CheckEqChar(ComGuess.cells[4][3], B);
  CheckEqChar(ComGuess.cells[2][1], B);
  CheckEqBool(ComGuess.ShipInArea, false);
  CheckEqInt(ComGuess.guessArea, 0);
  CheckEqInt(ComGuess.rowHit, 0);
  CheckEqInt(ComGuess.colHit, 0);
  CheckEqInt(ComGuess.guessesInArea, 0);
  CheckEqInt(ComGuess.shipDirection, 0);
  CheckEqInt(ComGuess.guessesInArea0, 12);
  CheckEqInt(ComGuess.guessesInArea1, 16);
  CheckEqInt(ComGuess.guessesInArea2, 12);
  CheckEqInt(ComGuess.guessesInArea3, 9);
  //tests 31-50 test valid user ship inputs
  CheckEqBool(valid("vc1\n",&Player), true);
  CheckEqInt(Player.orientation, 0);
  CheckEqInt(Player.row, 2);
  CheckEqInt(Player.col, 0);
  CheckEqBool(valid("he4\n",&Player), true);
  CheckEqInt(Player.orientation, 1);
  CheckEqInt(Player.row, 4);
  CheckEqInt(Player.col, 3);
  CheckEqBool(valid("vf5\n",&Player), true);
  CheckEqInt(Player.orientation, 0);
  CheckEqInt(Player.row, 5);
  CheckEqInt(Player.col, 4);
  CheckEqBool(valid("hf5\n",&Player), true);
  CheckEqInt(Player.orientation, 1);
  CheckEqInt(Player.row, 5);
  CheckEqInt(Player.col, 4);
  CheckEqBool(valid("ha2\n",&Player), true);
  CheckEqInt(Player.orientation, 1);
  CheckEqInt(Player.row, 0);
  CheckEqInt(Player.col, 1);
  //tests 51- 60 test invalid ship inputs
  CheckEqBool(valid("va1\n",&Player), false);
  CheckEqBool(valid("he6\n",&Player), false);
  CheckEqBool(valid("vb7\n",&Player), false);
  CheckEqBool(valid("f5v\n",&Player), false);
  CheckEqBool(valid("lh8\n",&Player), false);
  CheckEqBool(valid("\n",&Player), false);
  CheckEqBool(valid("x5\n",&Player), false);
  CheckEqBool(valid("12\n",&Player), false);
  CheckEqBool(valid("v\n",&Player), false);
  Player.cells[0][0] = S;
  CheckEqBool(valid("ha1\n",&Player), false);
  //tests 61-66 test the move function (that a ship has been placed correctly)
  Player.row = 4;
  Player.col = 3;
  Player.orientation = 0;
  movePlayer(&Player, 0);
  CheckEqChar(Player.cells[4][3], S);
  CheckEqChar(Player.cells[3][3], S);
  CheckEqChar(Player.cells[2][3], S);
  Player.orientation = 1;
  movePlayer(&Player, 0);
  CheckEqChar(Player.cells[4][3], S);
  CheckEqChar(Player.cells[4][4], S);
  CheckEqChar(Player.cells[4][5], S);
  //tests 67-72 test that a player guess was handled correctly
  CheckEqBool(validGuess(&PlayGuess, "a1\n"), true);
  CheckEqInt(PlayGuess.row, 0);
  CheckEqInt(PlayGuess.col, 0);
  CheckEqBool(validGuess(&PlayGuess, "d4\n"), true);
  CheckEqInt(PlayGuess.row, 3);
  CheckEqInt(PlayGuess.col, 3);
  //tests 73-75 test invalid player guesses
  CheckEqBool(validGuess(&PlayGuess, "32"), false);
  CheckEqBool(validGuess(&PlayGuess, "v"), false);
  PlayGuess.cells[1][5] = S;
  CheckEqBool(validGuess(&PlayGuess, "b6"), false);
  //tests 76-83 test valid computer ship inputs
  CheckEqBool(ValidComputer(0, 5, 1 ,&Computer), true);
  CheckEqInt(Computer.orientation, 0);
  CheckEqInt(Computer.row, 5);
  CheckEqInt(Computer.col, 1);
  CheckEqBool(ValidComputer(1, 5, 3,&Computer), true);
  CheckEqInt(Computer.orientation, 1);
  CheckEqInt(Computer.row, 5);
  CheckEqInt(Computer.col, 3);
  //tests 84-86 test invalid computer ship inputs
  CheckEqBool(ValidComputer(1, 2, 5, &Computer), false);
  CheckEqBool(ValidComputer(0, 1, 3 ,&Computer), false);
  Computer.cells[2][3] = S;
  CheckEqBool(ValidComputer(1, 2, 3,&Computer), false);
  //tests 87-89 test that a ship was added correctly for the computer
  Computer.orientation = 0;
  Computer.row = 5;
  Computer.col = 1;
  moveComputer(&Computer, &PlayGuess, 0);
  CheckEqChar(Computer.cells[5][1], S);
  CheckEqChar(Computer.cells[4][1], S);
  CheckEqChar(Computer.cells[3][1], S);
  //tests 90-93 specifically test the checkArea function and whether it correctly
  //determines area
  ComGuess.rowHit = 2;
  ComGuess.colHit = 1;
  checkArea(&ComGuess);
  CheckEqInt(ComGuess.guessesInArea0, 11);
  CheckEqInt(ComGuess.guessesInArea1, 16);
  CheckEqInt(ComGuess.guessesInArea2, 12);
  CheckEqInt(ComGuess.guessesInArea3, 9);
  //tests 94-97 test the countShots function and whether it correctly decreases shots remaining
  ComGuess.guessArea = 2;
  CountShots(&ComGuess);
  CheckEqInt(ComGuess.guessesInArea0, 11);
  CheckEqInt(ComGuess.guessesInArea1, 16);
  CheckEqInt(ComGuess.guessesInArea2, 11);
  CheckEqInt(ComGuess.guessesInArea3, 9);
  //tests 98-100 checks that the shipHit function correctly records the ship that was hit
  Player.ShipHits = 0;
  ShipHit(&Player, &ComGuess, 2, 2);
  CheckEqChar(Player.cells[2][2], H);
  CheckEqInt(Player.ShipHits, 1);
  CheckEqChar(ComGuess.cells[2][2], G);
  //tests 101-103 checks that the BlankHit function correctly records the blank that was hit
  BlankHit(&Player, &ComGuess, 5, 5);
  CheckEqChar(Player.cells[5][5], G);
  CheckEqInt(ComGuess.shipDirection, 1);
  CheckEqChar(ComGuess.cells[5][5], G);
  //tests 104-105 test that the edge cases for column0 are found (and hence the remaining edge case functions)
  ComGuess.shipDirection = 3;
  ComGuess.colHit = 0;
  CheckEqBool(Column0EdgeCase(&ComGuess, &guessInvalid), true);
  CheckEqBool(guessInvalid, true);
  //tests 106-113 test that the firreNorth function
  Player.cells[3][4] = S;
  ComGuess.rowHit = 4;
  ComGuess.colHit = 4;
  CheckEqInt(fireNorth(&Player, &ComGuess), 1);
  CheckEqInt(ComGuess.rowHit, 3);
  Player.cells[3][4] = G;
  ComGuess.rowHit = 4;
  ComGuess.colHit = 4;
  CheckEqInt(fireNorth(&Player, &ComGuess), 2);
  CheckEqInt(ComGuess.rowHit, 4);
  Player.cells[3][4] = H;
  ComGuess.rowHit = 4;
  ComGuess.colHit = 4;
  CheckEqInt(fireNorth(&Player, &ComGuess), 2);
  CheckEqInt(ComGuess.rowHit, 4);
  Player.cells[3][4] = B;
  ComGuess.rowHit = 4;
  ComGuess.colHit = 4;
  CheckEqInt(fireNorth(&Player, &ComGuess), 0);
  CheckEqInt(ComGuess.rowHit, 4);
  //tests 114-121 test that the fireSouth function
  Player.cells[5][4] = S;
  ComGuess.rowHit = 4;
  ComGuess.colHit = 4;
  CheckEqInt(fireSouth(&Player, &ComGuess), 1);
  CheckEqInt(ComGuess.rowHit, 5);
  Player.cells[5][4] = G;
  ComGuess.rowHit = 4;
  ComGuess.colHit = 4;
  CheckEqInt(fireSouth(&Player, &ComGuess), 2);
  CheckEqInt(ComGuess.rowHit, 4);
  Player.cells[5][4] = H;
  ComGuess.rowHit = 4;
  ComGuess.colHit = 4;
  CheckEqInt(fireSouth(&Player, &ComGuess), 2);
  CheckEqInt(ComGuess.rowHit, 4);
  Player.cells[5][4] = B;
  ComGuess.rowHit = 4;
  ComGuess.colHit = 4;
  CheckEqInt(fireSouth(&Player, &ComGuess), 0);
  CheckEqInt(ComGuess.rowHit, 4);
  //tests 122-129 test that the fireWest function
  Player.cells[4][3] = S;
  ComGuess.rowHit = 4;
  ComGuess.colHit = 4;
  CheckEqInt(fireWest(&Player, &ComGuess, &guessInvalid), 1);
  CheckEqInt(ComGuess.colHit, 3);
  Player.cells[4][3] = G;
  ComGuess.rowHit = 4;
  ComGuess.colHit = 4;
  CheckEqInt(fireWest(&Player, &ComGuess, &guessInvalid), 0);
  CheckEqInt(ComGuess.colHit, 4);
  Player.cells[4][3] = H;
  ComGuess.rowHit = 4;
  ComGuess.colHit = 4;
  CheckEqInt(fireWest(&Player, &ComGuess, &guessInvalid), 0);
  CheckEqInt(ComGuess.colHit, 4);
  Player.cells[4][3] = B;
  ComGuess.rowHit = 4;
  ComGuess.colHit = 4;
  CheckEqInt(fireWest(&Player, &ComGuess, &guessInvalid), 1);
  CheckEqInt(ComGuess.colHit, 4);
  //tests 130-131 test that the computer guesses in the correct area
  Player.ShipHits = 9;
  ComGuess.guessesInArea = 2;
  ComGuess.guessArea = 1;
  setGuessArea(&ComGuess);
  CheckEqInt(ComGuess.guessesInArea, 0);
  CheckEqInt(ComGuess.guessArea, 2);
  //tests 132-133 check if the computer has won
  Player.ShipHits = 9;
  CheckEqBool(CheckComputerWin(&Player, &ComGuess), true);
  Player.ShipHits = 8;
  CheckEqBool(CheckComputerWin(&Player, &ComGuess), false);
  //tests 134-135 check if the player has won
  Computer.ShipHits = 9;
  CheckPlayerWin(&Computer, &PlayGuess, &guessInvalid, &gameEnd);
  CheckEqBool(gameEnd, true);
  Computer.ShipHits = 8;
  gameEnd = false;
  CheckPlayerWin(&Computer,&PlayGuess, &guessInvalid, &gameEnd);
  CheckEqBool(gameEnd, false);

  printf("Tests passed: %d\n", CheckEqInt(0, 0) - 1);
}

int main() {
  setbuf(stdout, NULL);
  srand(time(NULL) % 86400); //pass a seed to the random function
  char CarryOn[5]; //store user input
  printf("Welcome to Battledship! For help and rules, enter 'h' into the terminal, "
         "or to play, just hit enter\n");
  //fgets(CarryOn, 5, stdin);
  if (strcmp(CarryOn, "h\n") == 0) { //check if the user wants to know the rules
    help();
  }
  InitialiseGame(); //start the game
  //testing();
}
