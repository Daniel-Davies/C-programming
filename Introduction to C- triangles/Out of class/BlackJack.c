#include <stdio.h> //import the standard c libraries
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> //import the boolean type and functions
#include <time.h> //import the time function

//a simple function that prints the rules of the game for a beginner
int help() {
  //print game rules
  printf("The aim of this game is to score as close to 21 as possible. But be "
         "aware-the computer is playing against you! You (and the computer) are "
         "dealt two cards at the start of the game. The cards 2 through 10 have "
         "their face value, J, Q, and K are worth 10 points each, and the Ace is "
         "worth either 1 or 11 points (player choice). Whoever gets the card "
         "values closest to 21 is the winner. As a player you have two options- "
         "stand, to keep the value of cards you have, or hit if you think you're "
         "not close enough to 21. A maximum of 5 cards in one game are allowed, "
         "but it's best to take as few as possible, since if you draw, the "
         "player with fewest cards wins!\n");
  //check that the user is happy to continue playing
  char onwards[10];
  printf("Ready to play on? Enter 'y' for yes, and anything else for no\n");
  scanf("%s", onwards);
  if (strcmp(onwards, "y") == 0) { //check that the user has entered yes
    return 0; //return back to the main function to start game
  }
  else {
    help(); //if unhappy, the user can print the rules again if they wish
    return 0;
  }
  return 0;
}
//function that returns the visually represented (string) value of a card
char *Compare(int value) {
  static char str[3]; //3 spaces allocated- 10 is the longest string (len 2), and a \0 null
  //check the value given from the random function
  if (value == 0) { //a value of 0 denotes an Ace
    return "Ace";
  }
  else if (value == 12) { //a value of 12 denotes a king
    return "King";
  }
  else if (value == 11) { //a value of 11 denotes a queen
    return "Queen";
  }
  else if (value == 10) { //a value of 10 denotes a Jack
    return "Jack";
  }
  else{ //the card must be a numeric type (aka non royalty/ ace)
    value++; //add 1 to the value (because the counting starts at 2- not 1)
    sprintf(str,"%d ",value);
    return str; //return the string representation of the card for output
  }
}
//function that returns the values of the card generated in the random function
int ActualVal(int CardValue) {
  if (CardValue == 0) { //an ace is worth 11 points
    return 11;
  }
  //any card above value 8 from the random function (10, J, Q, K) is worth 10
  else if (CardValue > 8) {
    return 10;
  }
  else { //anything else is worth 1 more than it's random function value
    return (CardValue + 1);
  }
}
//function that deals with giving the user a hint
char *hint(int CardVal) {
  //probability dictates that if the user has 15 or under, they should generally
  //take a risk on another card
  if (CardVal < 16) {
    return "The odds of winning with your value are pretty low! "
            "You should probably take another card...";
  }
  //if the user has 16-19, the user has quite a large chance of busting out
  //if they take another card
  else if (CardVal < 20) {
    return "The chances of going bust is really big! You should stick with what "
    "you have...";
  }
  //quite clearly if the user has 20, or the winning value 21, they should
  //not take a card
  else if (CardVal == 20 || CardVal == 21){
    return "Whatever you do, don't take another card!";
  }
  return "Take another card....";
}

//function that generates a random number equivalent to a card
//there are 13 cards, which are equally likely of getting selected
//it is assumed that the list of integers [0,1,2,3,4,5,6,7,8,9,10,11,12]
//can be mapped to the card set [A,2,3,4,5,6,7,8,9,10,J,Q,K]
int hitMe() {
  int rndInt = (rand() % 13); //use mod 13 to get the value between 0 and 12
  return rndInt;
}
//function that deals with storing and allocating card values for the computer
//as well as setting up a sum and card count
//really this should be a "void" function, but since it is important to test this
//function, it has been made an int to be able to return a value
int AllocateComputerCards(bool *HasAce, int *ComputerSum, int *ComputerCardsTaken) {
  int val = hitMe(); //get a random number from 0 to 12
  int ComputerCard = ActualVal(val); //store the value/score of the card
  if (ComputerCard == 11) { //check that the computer has an ace
    *HasAce = true;
  }
  *ComputerSum = *ComputerSum + ComputerCard; //sum the computer's cards
  *ComputerCardsTaken = *ComputerCardsTaken + 1; //store how many cards the computer has
  return ComputerCard;
}
//sets up the game for the computer by dealing it cards
void InitComputer(int *ComputerSum, int *ComputerCardsTaken) {
  bool TakeMore = true;
  bool HasAce = false;
  //loop twice to generate 2 cards
  for (int LoopCounter = 0; LoopCounter < 2; LoopCounter++) {
    //get and store the values of the card
    AllocateComputerCards(&HasAce, &*ComputerSum, &*ComputerCardsTaken);
  }
  //the "algorithm" responsible for the computer's card playing logic
  while (TakeMore) {
    if (*ComputerSum < 16 && *ComputerCardsTaken < 5) { //this is the general idea when playing blackjack-
      //it is worth a risk to take another card if your sum is less than 16
      AllocateComputerCards(&HasAce, &*ComputerSum, &*ComputerCardsTaken);
    }
    else if (*ComputerCardsTaken == 5 || *ComputerSum >= 16){ //max of 5 cards are allowed
      TakeMore = false;                                 //prevent more cards being taken
    }
    if (*ComputerSum > 21 && HasAce == true) { //if the computers goes over 21,
      *ComputerSum = *ComputerSum - 10;       //ace is automatically counted as a 1
      HasAce = false;
    }
  }
}
//function that deals with storing and allocating card values for the player
//as well as setting up a sum and card count
int AllocatePlayerCard(int PlayerCards[5], int LoopCounter, int *PlayerSum, int *PlayerCardsTaken, bool *AceSelected) {
  int val = hitMe(); //get a random number
  int PlayerCard;
  PlayerCards[LoopCounter] = val; //convert number to a card
  printf("Your card is %s\n", Compare(val)); //print for user
  PlayerCard = ActualVal(val); //convert the card to points
  *PlayerSum = *PlayerSum + PlayerCard; //sum the score of player's cards
  *PlayerCardsTaken = *PlayerCardsTaken + 1; //number of cards dealt is incremented
  if (PlayerCard == 11) { //check for the "ace" special case
    *AceSelected = true; //store whether the user has an ace
    printf("You got an Ace! It's been counted as a value of 11, "
    "but if at any time you want to count it as 1, enter 'a'\n");
  }
  return PlayerCard;
}
//function that generates the 2 cards for the user
void InitPlayer(int PlayerCards[5], int *PlayerSum, int *PlayerCardsTaken, bool *AceSelected) {
  //loop twice for 2 cards
  for (int LoopCounter = 0; LoopCounter < 2; LoopCounter++) {
    AllocatePlayerCard(PlayerCards, LoopCounter, &*PlayerSum, &*PlayerCardsTaken, &*AceSelected);
    if (*PlayerSum > 21 && *AceSelected == true) {
      //must also now check for the special case that the user goes bust with 2 aces
      printf("One of your aces has been counted as a 1 as your sum exceeds 21!\n");
      *PlayerSum = *PlayerSum - 10; //immediately remove 10 points from the score
    }
  }
}

//routine that gives the player extra cards if needed
void PlayerExtra(int PlayerCards[5], int *PlayerSum, int *PlayerCardsTaken, bool *AceSelected, bool *GameLost) {
  bool TakeMore = true;
  char Another[10];
  // loop while player doesnt have more than 5 cards, and the sum doesnt exceed 21
  while (TakeMore && *PlayerCardsTaken < 5 && *PlayerSum < 21) {
    printf("Your current card sum is %d - would you like another card? Enter y for yes, "
    "n for no or h for a hint\n", *PlayerSum);
    scanf("%s", Another);
    if (strcmp(Another, "y") == 0) { //check that the user wants another card
      AllocatePlayerCard(PlayerCards, *PlayerCardsTaken, &*PlayerSum, &*PlayerCardsTaken, &*AceSelected);
    }
    else if (strcmp(Another, "n") == 0) { //if the user selects no, then exit the loop
      TakeMore = false;
    }
    else if (strcmp(Another, "h") == 0) { //check if the user wants a hint
      printf("%s\n", hint(*PlayerSum));
    }
    //if the user has an ace, they can choose to make it value 1 instead of 11
    else if (strcmp(Another, "a") == 0 && *AceSelected == true) {
    //remove the option of decreasing the score (since the user basically has no ace anymore, but a card worth 1)
      *AceSelected = false;
      *PlayerSum = *PlayerSum - 10; //the player basically loses 10 points from the score in the conversion (11 - 1)
      printf("Ok, Ace counted as a value of 1\n"); //alert the user to the operation
    }
    //check if the user has busted with or without an ace
    if (*PlayerSum > 21 && *AceSelected == false) { //if no ace, they have lost
      printf("Your card sum is %d\n", *PlayerSum);
      printf("Your cards total to more than 21! Computer wins. Better luck next time...\n");
      TakeMore = false;
      *GameLost = true;
    }
    else if (*PlayerSum > 21 && *AceSelected == true) {
      printf("Your ace has been counted as a 1, not 11, as the sum would exceed 21\n");
      *PlayerSum = *PlayerSum - 10; //for the player to continue, they must be forced to covert their ace to a 1
      *AceSelected = false; //remove the ace option for the user
    }
  }
}
//function to determine the winner of the game
void Winner(int PlayerSum, int ComputerSum, int PlayerCardsTaken, int ComputerCardsTaken, int PlayerCards[5]) {
  //first print the value of the cards visually for the user
  printf("Your cards are\n");
  for (int i = 0; i < PlayerCardsTaken; i++) { //loop for all of the player's cards
    if (PlayerCards[i] == 0 || PlayerCards[i] == 7) { //choose the correct grammer (an or a)
      printf("an %s\n", Compare(PlayerCards[i]));
    }
    else {
      printf("a %s\n", Compare(PlayerCards[i]));
    }
  }
  printf("Your card value is %d\n", PlayerSum);
  printf("The computer's card value is %d\n", ComputerSum);
  //firstly check the case that the computer went bust
  if (ComputerSum > 21) {
    printf("You win! Congratulations\n");
  }
  //proceed to check which user had closer to 21
  else if (PlayerSum > ComputerSum) {
    printf("You win! Congratulations\n");
  }
  else if (ComputerSum > PlayerSum) {
    printf("Computer wins! Better luck next time...\n");
  }
  else { //scores are equal
    //standard practice is to check who used less cards
    if (ComputerCardsTaken < PlayerCardsTaken) {
      printf("The computer has used less cards than you, so computer wins! You used %d cards, computer only used %d\n", PlayerCardsTaken, ComputerCardsTaken);
    }
    else if(PlayerCardsTaken < ComputerCardsTaken) {
      printf("You used less cards than the computer, so you win! You only used %d cards, computer used %d\n", PlayerCardsTaken, ComputerCardsTaken);
    }
    else { //if scores and number of cards taken is equal, the game is a draw
      printf("It's a draw!\n");
    }
  }
}
//routine that deals with creating a game for the user
void play() {
  //initialise the variables for the game
  int PlayerCards[5];
  int PlayerCardsTaken = 0;
  int ComputerCardsTaken = 0;
  int PlayerSum = 0;
  int ComputerSum = 0;
  bool AceSelected = false;
  bool GameLost = false; //this variable is important to control whether the last sub routine is executede. It shouldnt be if an early return takes place is the routine beforehand
  //initialise the computer's cards
  InitComputer(&ComputerSum, &ComputerCardsTaken);
  //initialise the player's 2 first cards
  InitPlayer(PlayerCards, &PlayerSum, &PlayerCardsTaken, &AceSelected);
  //give the user the option of extra cards
  PlayerExtra(PlayerCards, &PlayerSum, &PlayerCardsTaken, &AceSelected, &GameLost);
  //check who has won the game
  if (GameLost == false) { //this check is necessary because the winner could have already been determined in the "PlayerExtra" function
    Winner(PlayerSum, ComputerSum, PlayerCardsTaken, ComputerCardsTaken, PlayerCards);
  }
}
//function to control the session for the user
void InitialiseGame() {
  char playAgain[10]; //store user input- 10 should be enough to prevent stack smashing
  play(); //start the game
  bool playing = true; //set variable to control how many times the loop is executed
  while(playing) { //loop while the player still wants to play
    printf("--------------------------------------------------------------------------------\n");
    printf("Would you like to play again? Enter 'y' for yes and 'n' for no\n");
    scanf("%s", playAgain);
    if (strcmp(playAgain, "y") == 0) { //check the user still wants to play
      play();
    }
    else if (strcmp(playAgain, "n") == 0) { //check the user wants to finish and end the game
      printf("Thank you for playing!\n");
      playing = false; //exit the loop
    }
    else {
      printf("Invalid input! Try again, and remember to type only 'y' or 'n'\n"); //alert the user of invalid input
    }
  }
}

//AUTOTESTING *************************************************************
//routine to compare 2 string inputs
int StringTests(int n, const char *actual, const char *expected) {
    n++;
    if (strcmp(actual, expected) != 0) {
        fprintf(stderr, "Test %d fails\n", n);
        fprintf(stderr, "result: %s\n", actual);
        fprintf(stderr, "instead of: %s\n", expected);
        exit(1);
    }
    return n;
}
//routine to compare 2 integer inputs
int IntegerTests(int n, int actual, int expected) {
    n++;
    if (actual != expected) {
        fprintf(stderr, "Test %d fails\n", n);
        fprintf(stderr, "result: %d\n", actual);
        fprintf(stderr, "instead of: %d\n", expected);
        exit(1);
    }
    return n;
}
//routine to compare 2 boolean values
int BoolTests(int n, bool actual, bool expected) {
    n++;
    if (actual != expected) {
        fprintf(stderr, "Test %d fails\n", n);
        fprintf(stderr, "result: %d\n", actual);
        fprintf(stderr, "instead of: %d\n", expected);
        exit(1);
    }
    return n;
}
void test() {
    int n = 0;
    //test the 'hitMe' function by printing out numbers
    //(can't test by comparison because numbers are random!)
    //so must visually check instead
    for (int i = 0; i <= 20; i++) {
      int val = hitMe();
      printf("%d\n", val);   //print the ouput of the random function
      //also need to check that the random number is in the expected range 0-12
      if (val < 0 || val > 12) {
        printf("Random function fails! It generates %d as a random number\n", val);
        exit(0);
      }
    }
    //test the 'compare' function with all possible inputs
    //which rely on the random function
    n = StringTests(n, Compare(0), "Ace");
    n = StringTests(n, Compare(1), "2 ");
    n = StringTests(n, Compare(2), "3 ");
    n = StringTests(n, Compare(3), "4 ");
    n = StringTests(n, Compare(4), "5 ");
    n = StringTests(n, Compare(5), "6 ");
    n = StringTests(n, Compare(6), "7 ");
    n = StringTests(n, Compare(7), "8 ");
    n = StringTests(n, Compare(8), "9 ");
    n = StringTests(n, Compare(9), "10 ");
    n = StringTests(n, Compare(10), "Jack");
    n = StringTests(n, Compare(11), "Queen");
    n = StringTests(n, Compare(12), "King");
    //test the 'actualval' function with all possible inputs
    n = IntegerTests(n, ActualVal(0), 11);
    n = IntegerTests(n, ActualVal(1), 2);
    n = IntegerTests(n, ActualVal(2), 3);
    n = IntegerTests(n, ActualVal(3), 4);
    n = IntegerTests(n, ActualVal(4), 5);
    n = IntegerTests(n, ActualVal(5), 6);
    n = IntegerTests(n, ActualVal(6), 7);
    n = IntegerTests(n, ActualVal(7), 8);
    n = IntegerTests(n, ActualVal(8), 9);
    n = IntegerTests(n, ActualVal(9), 10);
    n = IntegerTests(n, ActualVal(10), 10);
    n = IntegerTests(n, ActualVal(11), 10);
    n = IntegerTests(n, ActualVal(12), 10);
    //test the 'hint' function
    char *response1 = "The odds of winning with your value are pretty low! You should probably take another card...";
    char *response2 = "The chances of going bust is really big! You should stick with what you have...";
    char *response3 = "Whatever you do, don't take another card!";
    n = StringTests(n, hint(4), response1); //4 is the lowest value the function can take (2 lots of 2)
    n = StringTests(n, hint(10), response1);
    n = StringTests(n, hint(16), response2);
    n = StringTests(n, hint(15), response1);
    n = StringTests(n, hint(20), response3); //20 is the highest value the function can take (cannot take 21 or over as game is auto-ended in that case)

    //test the AllocateComputerCards function (and hence also the InitComputer function)
    //we test here the most important part of the function which is whether the function
    //is able to change the parameters passed by value, and whether the paramenters
    //are changed to the correct value

    int Sum = 0; //declare variables to pass to the function
    int CardsTaken = 0;
    int RandomVal = 0;
    bool Aces = true;
    for (int i = 0; i < 10; i++) { //10 tests should suffice
      //pass the variables to the function and receive the random score back
      RandomVal = AllocateComputerCards(&Aces, &Sum, &CardsTaken);
      n = IntegerTests(n, Sum, RandomVal); //if the function works correctly, then the sum should be the same value as that of the random score
      n = IntegerTests(n, CardsTaken, i + 1); //the cards taken should increase by 1 each time (since the function is called once every loop)
      Sum = 0; //reset the sum to 0 for the next test
    }
    //now do the same for the player version
    int tempArray[5]; //declare  variables to pass to the function
    tempArray[0] = -999; //set the arrays value to an obiously wrong value
    CardsTaken = 0; //reset the variables from above to be re-used here
    Sum = 0;
    for (int j = 0; j < 10; j++) {
      RandomVal = AllocatePlayerCard(tempArray,0, &Sum, &CardsTaken, &Aces);
      n = IntegerTests(n, Sum, RandomVal); //again, check that the sum and cardstaken are having the correct values assigned in the function
      n = IntegerTests(n, CardsTaken, j + 1);
      n = BoolTests(n, tempArray[0] != -999, true); //check also that the array stores the card's value
      Sum = 0; //reset variables
      tempArray[0] = -999;
    }

    printf("Tests passed: %d\n", n); //print how many tests succeed
}
//*****************************************************************************
int main() {
  setbuf(stdout, NULL);
  //test();  //run auto tests on the main functions
  srand(time(NULL) % 86400); //pass a seed to the random function
  char CarryOn[10]; //store user input- 10 should be enough to prevent stack smashing
  printf("Welcome to Blackjack! For help and rules, enter 'h' into the terminal, or to play, "
         "enter anything into terminal and press enter\n");
  scanf("%s", CarryOn); //get user input
  if (strcmp(CarryOn, "h") == 0) { //check if the user wants to know the rules
    help();
  }
  InitialiseGame(); //start the game
}
