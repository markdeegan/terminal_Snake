//
// Matas NOREIKA, Mark Deegan Tue Dec 31 12:21:06 GMT 2024
// definitions of snake_helpers functions

#include <time.h>
#include <stdlib.h>

#include "snake.h"

//decleration of createGameArea()
void createGameArea(void);

//decleration of getDirection()
int getDirection(char*);

//decleration of printTitle()
void printTitle(void);

//decleration of addLenght()
void addLenght(struct snakePart[MAXSNAKESIZE], int*, int);

//decleration of moveSnake()
int moveSnake(struct snakePart[MAXSNAKESIZE],struct snakePart* fruit, int*, int, clock_t*);

//decleration of pickRandomLocation()
void pickRandomLocation(struct snakePart*);

//decleration of addFruit()
void addFruit(struct snakePart snake[MAXSNAKESIZE], struct snakePart*, int);

//decleration of changePos()
void changePos(struct snakePart*, int, int);

//decleration of updateOtherParts()
void updateOtherParts(struct snakePart snake[MAXSNAKESIZE], int);

//decleration of saveHighscore()
int saveHighscore(int);

// declaration of getHighScore
int getHighscore(void);