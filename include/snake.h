/*
 * Porgrammer: Matas Noreika Thu Dec 26 05:31:50 PM GMT 2024
 * Purpose: This header is designed to hold definitions of variables and functions associated with the terminal snake game
*/

//include the standard c header for I/O manipulation
#include <stdio.h>

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
	
	int xPos;
	int yPos;
		
};