/*
 * Programmers: Matas Noreika, Mark Deegan Tue Dec 31 12:21:06 GMT 2024
 * Purpose: definitions of snake_helpers functions and variable definitions
*/


//////////////////////HEADER INCLUDES


//include standard c library dependencies
#include <stdio.h>
#include <time.h>
#include <stdlib.h>


/////////////////////END OF HEADER INCLUDES


/////////////////////GAMEPLAY VARIABLE DEFINTIONS


//macro defiition of the game area height
#define AREAHEIGHT 20

//macro definition of the width to height aspect ratio
#define ASPECTRATIO 2 

//macro definition of escape character to break out of game loop
#define ESCAPECHAR 'q'

//macro definition of frame frame used to control the yield time of each frame
#define FRAMERATE 1/30 

//macro definition of snake movement delay time in seconds
#define MOVEDELAY .25

//macro definition of the number of lines offset due to the title of the game
#define TITLEOFFSET 9 

//macro definition to control the maximum size that is possible for the snake
#define MAXSNAKESIZE ((AREAHEIGHT * ASPECTRATIO) * AREAHEIGHT)

//macro definition to define the upper boundary of the game area
#define UPPERBOUND (TITLEOFFSET + 2)

//macro definition to define the lower boundary of the game area
#define LOWERBOUND (AREAHEIGHT + 1 + TITLEOFFSET)

//macro definition to define the left boundary of the game area
#define LEFTBOUND 2

//macro definition to define the right boundary of the game area
#define RIGHTBOUND (ASPECTRATIO * AREAHEIGHT) + 3

//definition of a snake part structure
struct snakePart {
	
	int xPos;//holds the row position
	int yPos;//holds the column position
		
};


//////////////////////END OF GAMEPLAY VARIABLE DEFINITIONS


//////////////////////EXIT CODE DEFINITIONS


// MD20241229-02 errors.h to define some error codes
//
#define SNAKE_SUCCESS 0
#define SNAKE_FAILURE 1

//definition of error exit code for getOriginalSettings()
#define INIT_ERROR_SETTINGS 2 

//definition of error exit code for enterRawMode()
#define INIT_ERROR_RAW_MODE 3


/////////////////////END OF EXIT CODE DEFINITIONS


/////////////////////FUNCTION DEFINITIONS


//decleration of createGameArea()
void createGameArea();

//decleration of getDirection()
// MD20250104-03 Addressing declaration and definition conflicts
int getDirection(char *);

//decleration of printTitle()
void printTitle();

//decleration of addLenght()
// MD20250104-03 Addressing declaration and definition conflicts
void addLenght(struct snakePart*, int*, int);

//decleration of moveSnake()
int moveSnake();

//decleration of pickRandomLocation()
void pickRandomLocation();

//decleration of addFruit()
// MD20250104-03 Addressing declaration and definition conflicts
void addFruit(struct snakePart*, struct snakePart*, int);

//decleration of changePos()
void changePos();

//decleration of updateOtherParts()
void updateOtherParts();

//decleration of saveHighscore()
int saveHighscore();

// declaration of getHighScore
int getHighscore();


/////////////////////END OF FUNCTION DEFINITIONS
