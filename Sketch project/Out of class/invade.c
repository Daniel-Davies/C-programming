#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//structure to handle SDL components and window
struct display {
    bool testing;
    char *file;
    int width, height;
    SDL_Window *window;
    SDL_Renderer *renderer;
    char *actual;
    char **expected;
};

//structure to store information about the aliens
struct invaders {
  bool cells[5][4]; //store state of each alien
  int hits; //how many aliens have been hit
  int depthsX[5]; //X co-ordinate of edge of deepest aliens
  int depthsY[5]; //depth of bottom of deepest aliens on y axis
  int aliveInCol[5]; //how many aliens alive in each column
};

//synonym for the invaders structure
typedef struct invaders invaders;
typedef struct display display;
//initialisation of the invaders structure
void InitialiseStruct(invaders *i) {
  int counter = 0;
  for (int k = 0; k < 5; k++) {
    i->depthsX[k] = 27 + counter; //store the edge co-ordinate of each invader (depth on X axis)
    i->depthsY[k] = 190; //depth on Y axis for bottom-most invaders will be 190
    i->aliveInCol[k] = 4; //there are 4 invaders per column
    for (int j = 0; j < 4; j++) {
      i->cells[k][j] = false; //all invaders alive to start with
    }
    counter = counter + 90; //invaders are 90 units apart
  }
  i->hits = 0; //invaders hit
}

//function to exit SDL
void end(display *d) {
    SDL_Delay(100);
    SDL_Quit();
    exit(0);
}

//function to check for a fail
static void fail(char *s1, char *s2) {
    fprintf(stderr, "%s %s %s\n", s1, s2, SDL_GetError());
    SDL_Quit();
    exit(1);
}

//function that delays execution in ms
void pause(int ms) {
    if (ms > 0) SDL_Delay(ms);
}

//function to clear the SDL window
void clear(display *d) {
    SDL_SetRenderDrawColor(d->renderer, 0, 0, 0, 0);
    SDL_RenderClear(d->renderer);
}

//function that checks for a key press at an instant in time
char keyPoll(display *d) {
    SDL_Event event_structure;
    SDL_Event *event = &event_structure;
    int r = SDL_PollEvent(event); //Poll event will check for a key press at an instant, rather than wait
    if (r == 0) {return 0;}
    else if (event->type == SDL_KEYUP) {
      int sym = event->key.keysym.sym;
      return (char)sym;
    }
    else {return 0;}
}

//draw the line for the aliens from (x0, y0) to (x1, y1). Colour has been selected as (shade of) green, full opacity
void AlienLines(display *d, int x0, int y0, int x1, int y1) {
    int rc = SDL_SetRenderDrawColor(d->renderer, 0, 200, 0, 255);
    rc = SDL_RenderDrawLine(d->renderer, x0, y0, x1, y1);
    if (rc < 0) fail("Bad cline", "");
}

//draw the line for the player's ship. White, full opacity.
void PlayerLines(display *d, int x0, int y0, int x1, int y1) {
    int rc = SDL_SetRenderDrawColor(d->renderer, 255, 255, 255, 255);
    rc = SDL_RenderDrawLine(d->renderer, x0, y0, x1, y1);
    if (rc < 0) fail("Bad cline", "");
}

//function that draws the shape of the player ship by drawing of individual lines
void MovePlayer(display *d, int ChangeX) {
  PlayerLines(d,375 + ChangeX, 650, 365 + ChangeX, 670);
  PlayerLines(d,375 + ChangeX, 650, 385 + ChangeX, 670);
  PlayerLines(d,365 + ChangeX, 670, 340 + ChangeX, 680);
  PlayerLines(d,385 + ChangeX, 670, 410 + ChangeX, 680);
  PlayerLines(d,340 + ChangeX, 680, 340 + ChangeX, 687);
  PlayerLines(d,410 + ChangeX, 680, 410 + ChangeX, 687);
  PlayerLines(d,340 + ChangeX, 687, 410 + ChangeX, 687);
  PlayerLines(d,355 + ChangeX, 687, 355 + ChangeX, 697);
  PlayerLines(d,355 + ChangeX, 697, 363 + ChangeX, 697);
  PlayerLines(d,363 + ChangeX, 697, 363 + ChangeX, 687);
  PlayerLines(d,395 + ChangeX, 687, 395 + ChangeX, 697);
  PlayerLines(d,395 + ChangeX, 697, 387 + ChangeX, 697);
  PlayerLines(d,387 + ChangeX, 697, 387 + ChangeX, 687);
  PlayerLines(d,355 + ChangeX, 687, 355 + ChangeX, 697);
  PlayerLines(d,355 + ChangeX, 697, 363 + ChangeX, 697);
  PlayerLines(d,370 + ChangeX, 660, 375 + ChangeX, 668);
  PlayerLines(d,375 + ChangeX, 668, 379 + ChangeX, 660);
}

//draws individual lines to resemble the shape of the space invader aliens
void AlienFrame(display *d, int ChangeAlienX, int ChangeAlienY, int base) {
  //left arm
  AlienLines(d, 27+ChangeAlienX+base, 10+ChangeAlienY, 27+ChangeAlienX+base, 33+ChangeAlienY);
  AlienLines(d, 33+ChangeAlienX+base, 10+ChangeAlienY, 33+ChangeAlienX+base, 33+ChangeAlienY);
  AlienLines(d, 27+ChangeAlienX+base, 10+ChangeAlienY, 33+ChangeAlienX+base, 10+ChangeAlienY);
  AlienLines(d, 27+ChangeAlienX+base, 33+ChangeAlienY, 33+ChangeAlienX+base, 33+ChangeAlienY);

  AlienLines(d, 33+ChangeAlienX+base, 23+ChangeAlienY, 40+ChangeAlienX+base, 23+ChangeAlienY);
  AlienLines(d, 33+ChangeAlienX+base, 30+ChangeAlienY, 40+ChangeAlienX+base, 30+ChangeAlienY);
  AlienLines(d, 40+ChangeAlienX+base, 23+ChangeAlienY, 40+ChangeAlienX+base, 30+ChangeAlienY);
  //frame (central box)
  AlienLines(d, 40+ChangeAlienX+base, 20+ChangeAlienY, 40+ChangeAlienX+base, 40+ChangeAlienY);
  AlienLines(d, 80+ChangeAlienX+base, 20+ChangeAlienY, 80+ChangeAlienX+base, 40+ChangeAlienY);
  AlienLines(d, 40+ChangeAlienX+base, 20+ChangeAlienY, 80+ChangeAlienX+base, 20+ChangeAlienY);
  AlienLines(d, 40+ChangeAlienX+base, 40+ChangeAlienY, 80+ChangeAlienX+base, 40+ChangeAlienY);
  //right arm
  AlienLines(d, 85+ChangeAlienX+base, 10+ChangeAlienY, 85+ChangeAlienX+base, 33+ChangeAlienY);
  AlienLines(d, 91+ChangeAlienX+base, 10+ChangeAlienY, 91+ChangeAlienX+base, 33+ChangeAlienY);
  AlienLines(d, 85+ChangeAlienX+base, 10+ChangeAlienY, 91+ChangeAlienX+base, 10+ChangeAlienY);
  AlienLines(d, 85+ChangeAlienX+base, 33+ChangeAlienY, 91+ChangeAlienX+base, 33+ChangeAlienY);

  AlienLines(d, 85+ChangeAlienX+base, 23+ChangeAlienY, 80+ChangeAlienX+base, 23+ChangeAlienY);
  AlienLines(d, 85+ChangeAlienX+base, 30+ChangeAlienY, 80+ChangeAlienX+base, 30+ChangeAlienY);
  AlienLines(d, 85+ChangeAlienX+base, 23+ChangeAlienY, 85+ChangeAlienX+base, 30+ChangeAlienY);

  //Leg beggining left
  AlienLines(d, 48+ChangeAlienX+base, 45+ChangeAlienY, 53+ChangeAlienX+base, 45+ChangeAlienY);
  AlienLines(d, 48+ChangeAlienX+base, 40+ChangeAlienY, 48+ChangeAlienX+base, 45+ChangeAlienY);
  AlienLines(d, 53+ChangeAlienX+base, 40+ChangeAlienY, 53+ChangeAlienX+base, 45+ChangeAlienY);
  //Leg beginning right
  AlienLines(d, 63+ChangeAlienX+base, 45+ChangeAlienY, 68+ChangeAlienX+base, 45+ChangeAlienY);
  AlienLines(d, 63+ChangeAlienX+base, 40+ChangeAlienY, 63+ChangeAlienX+base, 45+ChangeAlienY);
  AlienLines(d, 68+ChangeAlienX+base, 40+ChangeAlienY, 68+ChangeAlienX+base, 45+ChangeAlienY);
  //leg end left
  AlienLines(d, 43+ChangeAlienX+base, 45+ChangeAlienY, 48+ChangeAlienX+base, 45+ChangeAlienY);
  AlienLines(d, 43+ChangeAlienX+base, 50+ChangeAlienY, 48+ChangeAlienX+base, 50+ChangeAlienY);
  AlienLines(d, 48+ChangeAlienX+base, 45+ChangeAlienY, 48+ChangeAlienX+base, 50+ChangeAlienY);
  AlienLines(d, 43+ChangeAlienX+base, 45+ChangeAlienY, 43+ChangeAlienX+base, 50+ChangeAlienY);
  //leg end right
  AlienLines(d, 68+ChangeAlienX+base, 45+ChangeAlienY, 73+ChangeAlienX+base, 45+ChangeAlienY);
  AlienLines(d, 68+ChangeAlienX+base, 50+ChangeAlienY, 73+ChangeAlienX+base, 50+ChangeAlienY);
  AlienLines(d, 68+ChangeAlienX+base, 45+ChangeAlienY, 68+ChangeAlienX+base, 50+ChangeAlienY);
  AlienLines(d, 73+ChangeAlienX+base, 45+ChangeAlienY, 73+ChangeAlienX+base, 50+ChangeAlienY);
  //left eye
  AlienLines(d, 43+ChangeAlienX+base, 25+ChangeAlienY, 48+ChangeAlienX+base, 25+ChangeAlienY);
  AlienLines(d, 43+ChangeAlienX+base, 30+ChangeAlienY, 48+ChangeAlienX+base, 30+ChangeAlienY);
  AlienLines(d, 43+ChangeAlienX+base, 25+ChangeAlienY, 43+ChangeAlienX+base, 30+ChangeAlienY);
  AlienLines(d, 48+ChangeAlienX+base, 25+ChangeAlienY, 48+ChangeAlienX+base, 30+ChangeAlienY);
  //right eye
  AlienLines(d, 73+ChangeAlienX+base, 25+ChangeAlienY, 68+ChangeAlienX+base, 25+ChangeAlienY);
  AlienLines(d, 73+ChangeAlienX+base, 30+ChangeAlienY, 68+ChangeAlienX+base, 30+ChangeAlienY);
  AlienLines(d, 73+ChangeAlienX+base, 25+ChangeAlienY, 73+ChangeAlienX+base, 30+ChangeAlienY);
  AlienLines(d, 68+ChangeAlienX+base, 25+ChangeAlienY, 68+ChangeAlienX+base, 30+ChangeAlienY);
  //antenna left
  AlienLines(d, 46+ChangeAlienX+base, 15+ChangeAlienY, 51+ChangeAlienX+base, 15+ChangeAlienY);
  AlienLines(d, 46+ChangeAlienX+base, 20+ChangeAlienY, 51+ChangeAlienX+base, 20+ChangeAlienY);
  AlienLines(d, 46+ChangeAlienX+base, 15+ChangeAlienY, 46+ChangeAlienX+base, 20+ChangeAlienY);
  AlienLines(d, 51+ChangeAlienX+base, 15+ChangeAlienY, 51+ChangeAlienX+base, 20+ChangeAlienY);
  //antenna right
  AlienLines(d, 70+ChangeAlienX+base, 15+ChangeAlienY, 65+ChangeAlienX+base, 15+ChangeAlienY);
  AlienLines(d, 70+ChangeAlienX+base, 20+ChangeAlienY, 65+ChangeAlienX+base, 20+ChangeAlienY);
  AlienLines(d, 70+ChangeAlienX+base, 15+ChangeAlienY, 70+ChangeAlienX+base, 20+ChangeAlienY);
  AlienLines(d, 65+ChangeAlienX+base, 15+ChangeAlienY, 65+ChangeAlienX+base, 20+ChangeAlienY);
}

//draw a row of aliens (excluding drawing those that have been hit)
void MoveAlien(display *d, invaders *i, int ChangeAlienX, int ChangeAlienY, int counter) {

  if (i->cells[0][counter] == false) {AlienFrame(d, ChangeAlienX, ChangeAlienY, 0);} //leftmost
  if (i->cells[1][counter] == false) {AlienFrame(d, ChangeAlienX, ChangeAlienY, 90);}
  if (i->cells[2][counter] == false) {AlienFrame(d, ChangeAlienX, ChangeAlienY, 180);}
  if (i->cells[3][counter] == false) {AlienFrame(d, ChangeAlienX, ChangeAlienY, 270);}
  if (i->cells[4][counter] == false) {AlienFrame(d, ChangeAlienX, ChangeAlienY, 360);} //rightmost
}

//draw the player, full set of aliens and store their (edge) X-co-ordinate
void MoveGroup(display *d, invaders *i, int width, int depthCount, int PosOnX) {
  int depth = 0;
  clear(d);
  for (int j = 0; j < 4; j++) { //loop for 4 rows
    MoveAlien(d, i, width, depth+depthCount, j);
    depth = depth + 50;
  }
  i->depthsX[0] = 27 + width; //store the edge of each of the deepest aliens
  i->depthsX[1] = 117 + width;
  i->depthsX[2] = 207 + width;
  i->depthsX[3] = 297 + width;
  i->depthsX[4] = 387 + width;
  MovePlayer(d, PosOnX);
}

//check if the left or right arrow key has been pressed, and move the player if so
int checkKeyPress(display *d, invaders *i, int keyVal, int PosOnX, int width, int depthCount) {
  if (keyVal == 80 && PosOnX >= -300) { //key is left
    clear(d);
    MoveGroup(d, i, width, depthCount, PosOnX-25);
    PosOnX = PosOnX - 25;
  }
  else if (keyVal == 79 && PosOnX <= 300) { //key is right
    clear(d);
    MoveGroup(d, i, width, depthCount, PosOnX+25);
    PosOnX = PosOnX + 25;
  }
  return PosOnX;
}

//check if the alines have reached the player, and hence have won
bool CheckAlienWin(invaders *i, int depthCount) {
  int depthExtra = 0;
  for (int k = 0; k < 4; k++)  {
    //first check how many rows of aliens have been defeated
    if (i->cells[0][k] == true && i->cells[1][k] == true && i->cells[2][k] == true && i->cells[3][k] == true && i->cells[4][k] == true) {
      depthExtra = depthExtra + 50; //for each row, the player gains 50 units of space
    }
  }
  if (depthCount > (450+depthExtra)) {return true;} else {return false;}
}

//check if all aliens have been destroyed
bool CheckPlayerWin(invaders *i) {
  if (i->hits == 20) {return true;} else {return false;}
}

//function that deals with an alien at a particular location being hit
void AlienHit(invaders *i, int col, int k) {
  if (i->aliveInCol[k] == 1) { //check if is last remaining
    i->aliveInCol[k] = 0;
    i->cells[k][col] = true;
    i->depthsY[k] = 0;
  }
  else {
    i->cells[k][col] = true;
    i->aliveInCol[k] = i->aliveInCol[k] - 1;
  }
}

//function to check (and deal with) the deepest alien being hit
bool DeepestAlien(invaders *i, int bulletY, int tempDepth, int newDeepest, int k) {
  if (bulletY <= (tempDepth) && bulletY >= (tempDepth - 23)) { //check bullet depth
    if (i->cells[k][newDeepest] != true) {
      AlienHit(i, newDeepest, k); //deal with the hit
      i->hits = i->hits + 1;
      return true;
    }
  }
  return false;
}

//function to check (and deal with) the second deepest alien being hit
bool SecondDeepest(invaders *i, int bulletY, int tempDepth, int newDeepest, int k) {
  if (bulletY <= (tempDepth - 50) && bulletY >= (tempDepth - 73)) { //check if depth matches
    if (i->cells[k][newDeepest-1] != true) {
      AlienHit(i, newDeepest-1, k); //deal with hit
      i->hits = i->hits + 1;
      return true;
    }
  }
  return false;
}

//function to check (and deal with) the third deepest alien being hit
bool ThirdDeepest(invaders *i, int bulletY, int tempDepth, int newDeepest, int k) {
  if (bulletY <= (tempDepth - 100) && bulletY >= (tempDepth - 123)) { //check if depth matches
    if (i->cells[k][newDeepest-2] != true) {
      AlienHit(i, newDeepest-2, k); //deal with hit
      i->hits = i->hits + 1;
      return true;
    }
  }
  return false;
}

//function to check (and deal with) the top-most alien being hit
bool FourthDeepest(invaders *i, int bulletY, int tempDepth, int newDeepest,int k) {
  if (bulletY <= (tempDepth - 150) && bulletY >= (tempDepth - 173)) { //check if depth matches
    if (i->cells[k][newDeepest-3] != true) {
      AlienHit(i, newDeepest-3, k); //deal with hit
      i->hits = i->hits + 1;
      return true;
    }
  }
  return false;
}

//function that finds the deepest alien in a column
void findDeepest(invaders *i, int *tempDepth, int *newDeepest, int k) {
  for (int l = 3; l > -1; l--) {
    if (i->cells[k][l] == true) {
      *tempDepth = *tempDepth - 50; //depth of deepest alien
      *newDeepest = *newDeepest - 1; //order of deepest alien in column
    }
    else {l = -1;}
  }
}

//function that deals with the edge case of the bullet going deeper than all aliens
bool deepBulletEdgeCase(invaders *i, int bulletY, int k) {
  if (bulletY < (i->depthsY[k]-170)) {
    return true;
  }
  return false;
}

//function that deals with handling the bullet fired by the player
bool AlienDown(display *d, invaders *i, int bulletX, int bulletY) {
  for (int k = 0; k < 5; k++) {
    if (deepBulletEdgeCase(i, bulletY, k)) {return false;} //edge case
    if ((bulletX <= (i->depthsX[k]+64)) && (bulletX >= (i->depthsX[k]))) { //check if widths match
      int tempDepth = i->depthsY[k];
      int newDeepest = 3;
      //check, alien by alien, if the bullet has struck a target
      findDeepest(i, &tempDepth, &newDeepest, k);
      if(DeepestAlien(i, bulletY, tempDepth, newDeepest, k)) {return true;}
      if(SecondDeepest(i, bulletY, tempDepth, newDeepest, k)) {return true;}
      if(ThirdDeepest(i, bulletY, tempDepth, newDeepest, k)) {return true;}
      if(FourthDeepest(i, bulletY, tempDepth, newDeepest, k)) {return true;}
     }
  }
  return false;
}

//function to refresh the position of the aliens and the player
bool RefreshPlayers(display *d, invaders *i, int *AlienMovement, int *width, int *depthCount, int *PosOnX, int *loopCounter, int *moveConst, int loop) {
  if ((*AlienMovement) == 1000) { //check if it is time to refresh the aliens
    MoveGroup(d, i, *width, *depthCount, *PosOnX); //refresh them
    (*width) = (*width) + (*moveConst); //shift the width to current position
    *AlienMovement = 0; //reset the counter
    if (*loopCounter > loop) { //check if edge of screen is reached
      (*width) = (*width) - (*moveConst); //change direction
      (*moveConst) = (*moveConst) * -1;
      (*depthCount) = (*depthCount) + 50; //move aliens downscreen
      for (int k = 0; k < 5; k++) { //update the structure of the aliens
        i->depthsY[k] = i->depthsY[k] + 50;
      }
      (*loopCounter) = -1; //reset the counter
    }
    (*loopCounter)++; //one step closer to edge of screen
    return true;
  }
  return false;
}

//function to progressively move the bullet upward through the screen
void moveBullet(display *d, invaders *i, int bulletConst, int *DecrementY, int *counter) {
  PlayerLines(d, bulletConst, *DecrementY, bulletConst, (*DecrementY) - 10); //draw bullet
  PlayerLines(d, bulletConst+1, *DecrementY, bulletConst+1, (*DecrementY) - 10);
  PlayerLines(d, bulletConst+2, *DecrementY, bulletConst+2, (*DecrementY) - 10);
  (*DecrementY) = (*DecrementY) - 10; //refresh position
  if(AlienDown(d, i, bulletConst, *DecrementY)) {//check for hit alien
    *counter = 0;
  }
  (*counter) = (*counter) - 1;
}

//check if anyone has won the game, end game if so, and print to terminal if so
void checkingWins(invaders *i, int depthCount, bool *AllDown) {
  bool alienWin = CheckAlienWin(i, depthCount); //check for each win
  bool playerWin = CheckPlayerWin(i);
  *AllDown = (alienWin || playerWin); //game loop terminator
  if (playerWin) {printf("All targets down! Your OS is safe...for now\n");} //print who won
  else if (alienWin) {
    printf("The aliens are through! Your OS is doomed!\n");
    if (i->hits != 0) {
      printf("At least you got %d of them, for a total score of %d...\n", i->hits, i->hits *50);
    }
  }
  pause(10);
}

//function to refresh the screen while the bullet is in motion
void RefreshBoard(display *d, invaders *i, int *counter, int *AlienMovement2, int *width, int *depthCount, int *PosOnX, int *loopCounter, int *moveConst, int *DecrementY, int *bulletConst, int loop) {
  //first refresh characters
  if(! RefreshPlayers(d, i, AlienMovement2, width, depthCount, PosOnX, loopCounter, moveConst, loop)) {
      MoveGroup(d, i, *width, *depthCount, *PosOnX);
  }
  moveBullet(d, i, *bulletConst, DecrementY, counter); //now refresh bullet
  SDL_UpdateWindowSurface(d->window); //update surface
}

//function that deals with the space bar being hit
void SpaceHit(display *d, invaders *i, int *KeyPress, int *PosOnX, int *width, int *depthCount, int *loopCounter, int *moveConst, int *AlienMovement, int loop) {
  if (*KeyPress == 32) { //check spacebar hit
    //initialise key variables
    int AlienMovement2 = 800; //800 to give optimum transition between alien movements
    int bulletConst = 375+*PosOnX;
    int counter = 65;
    int DecrementY = 650;
    bool alienWin = false;
    bool playerWin = false;
    while (counter > 0) { //loop while the bullet hasnt reached the top of the screen
      //refresh the screen
      RefreshBoard(d, i, &counter, &AlienMovement2, width, depthCount, PosOnX, loopCounter, moveConst, &DecrementY, &bulletConst, loop);
      //check for player movement while bullet fired
      *KeyPress = keyPoll(d);
      *PosOnX = checkKeyPress(d, i, *KeyPress, *PosOnX, *width, *depthCount);
      //check if anyone has won
      alienWin = CheckAlienWin(i, *depthCount);
      playerWin = CheckPlayerWin(i);
      if (alienWin || playerWin) {counter = 0;}
      pause(50); //pause for 50ms for smooth bullet transition
      AlienMovement2 = AlienMovement2 + 100; //variable that controls when aliens move
    }
    *AlienMovement = 800; //again, a value of 800 gives optimum transfer time between alien movement
  }
}

//main game frame
void HandleGame(display *d, int widthConst, int loop) {
  int KeyPress; //initialise key variables
  int PosOnX = 0;
  bool AllDown = false;
  int AlienMovement = 0;
  int depthCount = 0;
  int width = 0;
  int moveConst = widthConst;
  int loopCounter = 0;
  int iterations = loop;
  invaders aliens;
  invaders *i = &aliens;
  InitialiseStruct(i); //initialise the invaders structure
  while (! AllDown) { //game loop
    //refresh the screen
    RefreshPlayers(d, i, &AlienMovement, &width, &depthCount, &PosOnX, &loopCounter, &moveConst, iterations);
    SDL_UpdateWindowSurface(d->window);
    //check if the user has chosen to move their ship
    KeyPress = keyPoll(d);
    PosOnX = checkKeyPress(d, i, KeyPress, PosOnX, width, depthCount);
    //check if the user has fired a shot
    SpaceHit(d, i, &KeyPress, &PosOnX, &width, &depthCount, &loopCounter, &moveConst, &AlienMovement, iterations);
    //check if anyone has won the game
    checkingWins(i, depthCount, &AllDown);
    AlienMovement = AlienMovement + 20; //variable that controls how often aliens move
  }
}

display *newDisplay(int width, int height) {
    display *d = malloc(sizeof(display));
    d->testing = false;
    d->width = width;
    d->height = height;
    int x = SDL_WINDOWPOS_UNDEFINED, y = SDL_WINDOWPOS_UNDEFINED;
    d->window = SDL_CreateWindow("SKETCH", x, y, width, height, 0);
    if (d->window == NULL) fail("Bad window", "");
    SDL_Surface *surface = SDL_GetWindowSurface(d->window);
    d->renderer = SDL_CreateSoftwareRenderer(surface);
    if (d->renderer == NULL) fail("Bad renderer", "");
    SDL_SetRenderDrawColor(d->renderer, 0, 0, 0, 0);
    SDL_RenderClear(d->renderer);
    SDL_UpdateWindowSurface(d->window);
    SDL_Delay(1);
    return d;
}

//Initilaise SDL
void InitSDL() {
  int result = SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER);
  if (result < 0) fail("Bad SDL", "");
}

//print a game introduction to the terminal
void terminalIntro() {
  printf("Welcome to SDL invaders! It's your job to save your OS from an alien invasion!\n");
  pause(3000);
  printf("Aliens inbound! Get ready!\n");
  pause(2000);
  printf("3\n");
  pause(1000);
  printf("2\n");
  pause(1000);
  printf("1\n");
  pause(1000);
}


//Autotesting*******************************************************************************************
enum type { CHAR, INT, BOOL };

int CheckEq(enum type t, int x, int y) {
    static int n = 0;
    n++;
    if (x != y) {
        if (t==INT) fprintf(stderr, "Test %d gives %d not %d", n, x, y);
        if (t==BOOL && x) fprintf(stderr, "Test %d gives true not false", n);
        if (t==BOOL && y) fprintf(stderr, "Test %d gives false not true", n);
        exit(1);
    }
    return n;
}

// More specific versions of the eq function
int CheckEqInt(int x, int y) { return CheckEq(INT, x, y); }
int CheckEqBool(bool x, bool y) { return CheckEq(BOOL, x, y); }

//note: tests assume that the difficulty level is on "hard"
void tests(display *d) {
  invaders aliens;
  invaders *i = &aliens;
  int newDeepest = 3;
  int temp = 0;
  int AlienMovement = 0;
  int loopCounter = 0;
  bool AllDown = false;
  InitialiseStruct(i);

  //test for correct initialisation of invaders structure (1-5)
  CheckEqInt(i->cells[0][0], false);
  CheckEqInt(i->cells[4][3], false);
  CheckEqInt(i->depthsX[3], 297);
  CheckEqInt(i->depthsY[4], 190);
  CheckEqInt(i->hits, 0);

  MoveGroup(d, i, 10, 10, 10);

  //test the MoveGroup function, for refreshing the screen (6-10)
  CheckEqInt(i->cells[0][0], false);
  CheckEqInt(i->cells[4][3], false);
  CheckEqInt(i->depthsX[3], 307);
  CheckEqInt(i->depthsX[2], 217);
  CheckEqInt(i->depthsY[4], 190);

  //test the function that deals with user key input (11-15)
  CheckEqInt(checkKeyPress(d, i, 80, -100, 10, 10), -125);
  CheckEqInt(checkKeyPress(d, i, 80, -325, 10, 10), -325);
  CheckEqInt(checkKeyPress(d, i, 79, 200, 10, 10), 225);
  CheckEqInt(checkKeyPress(d, i, 79, 325, 10, 10), 325);
  CheckEqInt(checkKeyPress(d, i, 70, -100, 10, 10), -100);

  //test the function that checks if the aliens have won (16-20)
  CheckEqBool(CheckAlienWin(i, 190), false); //the starting state
  CheckEqBool(CheckAlienWin(i, 400), false); //not there yet
  i->cells[0][3] = true;
  i->cells[1][3] = true;
  i->cells[2][3] = true;
  i->cells[3][3] = true;
  i->cells[4][3] = true;
  CheckEqBool(CheckAlienWin(i, 450), false); //player should get extra space if last row taken out
  CheckEqBool(CheckAlienWin(i, 501), true);
  i->cells[0][0] = true;
  i->cells[1][0] = true;
  i->cells[2][0] = true;
  i->cells[3][0] = true;
  i->cells[4][0] = true;
  CheckEqBool(CheckAlienWin(i, 501), false); //should stay the same as top row doesnt influence the players game

  //test the function that checks if the player has won (21-22)
  CheckEqBool(CheckPlayerWin(i), false);
  i->hits = 20;
  CheckEqBool(CheckPlayerWin(i), true);

  InitialiseStruct(i); //reset test data

  AlienHit(i, 2, 2);

  //test the function that should record if an alien was hit (at the correct position) (23-27)
  CheckEqBool(i->cells[2][2], true);
  CheckEqInt(i->aliveInCol[2], 3);

  i->aliveInCol[2] = 1;

  AlienHit(i, 2, 2);

  CheckEqBool(i->cells[2][2], true);
  CheckEqInt(i->aliveInCol[2], 0);
  CheckEqInt(i->depthsY[2], 0);

  //test the function that locates the deepest alien in a column (28-29)
  i->cells[0][3] = true;
  i->cells[1][3] = true;
  i->cells[2][3] = true;
  i->cells[3][3] = true;
  i->cells[4][3] = true;
  findDeepest(i, &temp, &newDeepest, 3);
  CheckEqInt(newDeepest, 2);
  i->cells[0][0] = true;
  i->cells[1][0] = true;
  i->cells[2][0] = true;
  i->cells[3][0] = true;
  i->cells[4][0] = true;
  findDeepest(i, &temp, &newDeepest, 0);
  CheckEqInt(newDeepest, 1);

  InitialiseStruct(i);

  //check the function that deals with the edge case of the bullet going deep (30-31)
  CheckEqBool(deepBulletEdgeCase(i, 200, 2), false);
  CheckEqBool(deepBulletEdgeCase(i, 10, 2), true);

  //test the function that refreshes the alien position (and screen) (32-35)
  AlienMovement = 1000;
  loopCounter = 7;
  temp = 40;
  RefreshPlayers(d, i, &AlienMovement, &temp, &temp, &temp, &loopCounter, &temp, 6);
  CheckEqInt(i->depthsY[2], 240);

  AlienMovement = 1000;
  loopCounter = 7;
  RefreshPlayers(d, i, &AlienMovement, &temp, &temp, &temp, &loopCounter, &temp, 6);
  CheckEqInt(i->depthsY[4], 290);

  AlienMovement = 800;
  loopCounter = 7;
  RefreshPlayers(d, i, &AlienMovement, &temp, &temp, &temp, &loopCounter, &temp, temp);
  CheckEqInt(i->depthsY[0], 290);

  AlienMovement = 1000;
  loopCounter = 1;
  RefreshPlayers(d, i, &AlienMovement, &temp, &temp, &temp, &loopCounter, &temp, temp);
  CheckEqInt(i->depthsY[2], 290);

  //test the function that deals with a win (36-37)
  i->hits = 20;
  checkingWins(i, temp, &AllDown);
  CheckEqBool(AllDown, true);

  i->hits = 0;
  AllDown = false;
  temp = 600;
  checkingWins(i, temp, &AllDown);
  CheckEqBool(AllDown, true);

  //test the functions that deal with each alien in a column being hit (alien 0-3, shallowest-deepest)
  //deepest - (38-41)
  CheckEqBool(DeepestAlien(i, 35, 50, 2, 2), true);
  CheckEqBool(i->cells[2][2], true);
  CheckEqInt(i->hits, 1);
  CheckEqBool(DeepestAlien(i, 10, 50, 2, 2), false);

  InitialiseStruct(i);
  i->hits = 0;

  //(42-44)
  CheckEqBool(SecondDeepest(i, 40, 100, 1, 1), true);
  CheckEqBool(i->cells[1][0], true);
  CheckEqBool(SecondDeepest(i, 10, 100, 2, 2), false);

  InitialiseStruct(i);

  //(45-47)
  CheckEqBool(ThirdDeepest(i, 40, 150, 2, 2), true);
  CheckEqBool(i->cells[2][0], true);
  CheckEqBool(ThirdDeepest(i, 200, 150, 2, 2), false);

  InitialiseStruct(i);

  //(48-50)
  CheckEqBool(FourthDeepest(i, 40, 200, 3, 3), true);
  CheckEqBool(i->cells[3][0], true);
  CheckEqBool(DeepestAlien(i, 10, 200, 3, 3), false);

  printf("Tests passed: %d\n", CheckEqInt(0, 0) - 1);
}

void makeGame(int moveConst, int loopCounter) {
  InitSDL(); //initialise SDL
  terminalIntro(); //print an introduction to terminal
  display *d = newDisplay(750, 750); //initialise the window, SDL structure, graphics
  HandleGame(d, moveConst, loopCounter); //begin the game
  end(d); //exit SDL
}


int main(int n, char *args[n]) {
  if (n == 1) { //check arguments
    InitSDL(); //initialise SDL
    display *d = newDisplay(750, 750); //initialise the window, SDL structure, graphics
    tests(d);
    end(d); //exit SDL
  }
  else if (n == 2 && (strcmp(args[1],"h") == 0)) {
    makeGame(40, 6);
  }
  else if (n == 2 && (strcmp(args[1],"e") == 0)) {
    makeGame(20, 12);
  }
  else {
      fprintf(stderr, "Enter ./invade e or ./invade h only\n");
      exit(1);
  }
}
