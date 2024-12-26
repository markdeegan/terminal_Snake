/*
 * programmer: Matas Noreika Tue Dec 24 08:19:06 PM GMT 2024
 * Purpose: To create a basic snake game using the terminal.
*/

//standard c header for memeory allocation control and exit handlling
#include <stdlib.h>

//standard c header for time functionality
#include <time.h>

//custom terminal control header
#include "inc/terminalControl.h"

//macro defiition of the game area height
#define AREAHEIGHT 20

//macro definition of the width to height aspect ratio
#define ASPECTRATIO 2 

//macro definition of escape character to break out of game loop
#define ESCAPECHAR 'q'

//macro definition of frame frame used to control the yield time of each frame
#define FRAMERATE 1/120 

//defines if game is ran in debugging mode (information printed to screen)
int debuggingMode;

//decleration of variable to store old terminal settings
struct termios original_Settings;

//decleration of exit raw mode function to prevent complier errors
void exitRawMode(void);

//decleration of function to make the game area
void createGameArea();

//decleration of function to determine the direction based on key press
int checkControl(char* buffer);

//decleration of function to move cursor to the give location
void moveSnake(int direction);

//decleration of function to print debugging stats to terminal if debug mode is enabled
void printDebugStats(int x, int y);

//start of main method
int main(int argc, char** argv){

	//if additional parameters are passed into the program
	if(!(argc == 1)){
	
		//argument 1 controls debugging mode
		debuggingMode = atoi(argv[1]);
		
		//enable debugging mode if the atoi returns exactly 1
		if(debuggingMode > 1){
			
			//debugging mode is off
			debuggingMode = 0;
		
		}

	}

	//store the current terminal settings into the original settings variable
	tcgetattr(STDIN_FILENO, &original_Settings);
	
	//bind exitRawMode function on exit of program
	atexit(exitRawMode);
	
	//call enterRawMode function to set the terminal to raw mode
	enterRawMode(&original_Settings);

	//create the game area
	createGameArea();

	//defintion of game break condition variable
	int gameBreak = 0;
	
	//decleration of cursor position variables
	int x = AREAHEIGHT/2, y = (AREAHEIGHT * ASPECTRATIO)/2;

	//decleration of read buffer
	char buffer[4096];
	
	//start of game loop
	while(!gameBreak){

		//definition of varible to store the time at the start of the frame
		clock_t startTime = clock();

		//unistd.h function to read the stdin buffer
		int readSize = read(STDIN_FILENO, &buffer, sizeof(buffer));

		//check if the read didn't fail or read nothing
		if(!(readSize <= 0)){
			
			//check if the first character is the escape character
			if( (buffer[0] == ESCAPECHAR || buffer[0] == ESCAPECHAR -32) ){
				
				gameBreak = 1;//stop the game loop
				
			}

			//call two functions one that checks the pressed key and the other moves in direction based on key
			moveSnake(checkControl(buffer));	
			
			//read the position after the cursor is moved by input
			readCursorPos(&x,&y);

		}else{//the read failed
				
			//handle for the error

		}
		
		//check if debugging mode is on
		if(debuggingMode){
			
			//print out the debug stats
			printDebugStats(x,y);
		
		}

		//wait till the frame rate is matched
		while((float) FRAMERATE > (float) (clock() - startTime)/CLOCKS_PER_SEC );

		//debug print to see if the frames are rendering at target frame rate
		//printf("frame completed: %.6f", (float) (clock() - startTime)/CLOCKS_PER_SEC);
	
	}

	printf("\e[22;0H");//returns cursor to out of bound area of game space

	return 0;//exit with error code 0 to indicate successful completion of program

}

//definition of exit raw mode function 
void exitRawMode(void){

	//set terminal to old saved settings
	tcsetattr(STDIN_FILENO, TCSANOW, &original_Settings);

}

//moves snake in give provided direction
void moveSnake(int direction){

	switch(direction){
	
		case 1:
			printf("\e[1A");//move cursor up
			break;
		case 2:
			printf("\e[1B");//move cursor down
			break;
		case 3:
			printf("\e[1D");//move cursor left
			break;
		case 4:
			printf("\e[1C"); //move cursor right
			break;

	}

}

//definition of snake control function (returns direction int 1 = forward, 2 = backwards, 3 = left, 4 = right)
int checkControl(char* buffer){
	
	//perform switch case to move cursor
	switch (buffer[0]){
	
		case 'w':
			return 1;
		case 'W':
			return 1;//return forward direction
		case 's':
			return 2;
		case 'S':
			return 2;//return backward direction
		case 'a':
			return 3;
		case 'A':
			return 3;//return left direction
		case 'd':
			return 4;
		case 'D':
			return 4;//return right direction
	
	}

}

//definition of function to generate the game area
void createGameArea(){
	
	//clear screen, move the cursor to 0,0 and make the background colour white
	printf("\e[2J\e[H\e[47m");

	//loop through all the game area rows
	for(int i = 0; i < AREAHEIGHT; i++){
	
		//loop through all the column values
		for(int j = 0; j < AREAHEIGHT * ASPECTRATIO; j++){
		
			//print an empty character
			printf(" ");
		
		}
		//start on a new line	
		printf("\n");

	}
	
	//reset graphics mode and move cursor to the middle of the screen
	printf("\e[m\e[%d;%dH", AREAHEIGHT/2, (AREAHEIGHT * ASPECTRATIO)/2);

}

//definition of function to print program debugging stats
void printDebugStats(int x, int y){

	//print cursor position
	printf("\e[s\e[21;0H\e[Kx pos: %d, y pos: %d\e[u",x,y);
	
}
