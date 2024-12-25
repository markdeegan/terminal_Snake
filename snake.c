/*
 * programmer: Matas Noreika Tue Dec 24 08:19:06 PM GMT 2024
 * Purpose: To create a basic snake game using the terminal in raw mode.
 *
*/


//standard c header to have functionality to print and read from streams
#include <stdio.h>

//standard c header for memeory allocation control and exit handlling
#include <stdlib.h>

//standard c library for time functionality
#include <time.h>

//custom terminal control header
#include "terminalControl.h"

//macro defiition of the game area height
#define AREAHEIGHT 20

//macro definition of the width to height aspect ratio
#define ASPECTRATIO 2 

//macro definition of escape character to break out of game loop
#define ESCAPECHAR 'q'

//macro definition of frame frame used to control the yield time of each frame
#define FRAMERATE 1/60 

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

//decleration of function to return the cursor position
void returnCursorPos(int* x, int* y);

//start of main method
int main(int argc, char** argv){

	
	
	//store the current terminal settings into the original settings variable
	tcgetattr(STDIN_FILENO, &original_Settings);
	
	//bind exitRawMode function on exit of program
	atexit(exitRawMode);

	//sets the stdout to not buffer any content
	//_IONBF: mode to set file stream to not buffer
	setvbuf(stdout, NULL, _IONBF, 0);
	
	int x,y;
	returnCursorPos(&x,&y);
	printf("x : %d, y : %d", x, y);
	
	//call enterRawMode function to set the terminal to raw mode
	enterRawMode(&original_Settings);
	/*
	//clear the terminal screen
	printf("\e[2J\n");

	//create the game area
	createGameArea();

	//move cursor to the middle of the game area
	printf("\e[10;20H");

	//defintion of game break condition variable
	int gameBreak = 0;

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
			

		}else{//the read failed
				
			//handle for the error

		}
		
		//wait till the frame rate is matched
		while((float) FRAMERATE > (float) (clock() - startTime)/CLOCKS_PER_SEC );

		//debug print to see if the frames are rendering at target frame rate
		//printf("frame completed: %.6f", (float) (clock() - startTime)/CLOCKS_PER_SEC);
	
	}
*/
	return 0;//exit with error code 0 to indicate successful completion of program

}

//definition of exit raw mode function 
void exitRawMode(void){

	//set terminal to old saved settings
	tcsetattr(STDIN_FILENO, TCSANOW, &original_Settings);

}

//definition of function to return the position of cursor
void returnCursorPos(int* x, int* y){
	
	//request cursor position report
	char writeBuf = "\e[6n";

	write(STDOUT_FILENO,writeBuf, sizeof(writeBuf));
	
	//decleration of read buffer
	char buffer[4096];
	
	//x string to append each x character value to
	char* xValue = "";

	//y string to append each y character value to
	char* yValue = "";

	//read the stdin where the cursor report is sent 
	ssize_t readSize = read(STDIN_FILENO, &buffer, sizeof(buffer));

	//loop through all characters in the buffer
	//report format is ESC[X;YR
	//we will skip the first two values as we are only interested in the x and y
	
	int xFinished = 0;
	int lastXPos = 0;

	for(int i = 2; i < readSize; i++){
		
		//check if its the end of the report
		if(buffer[i] == 'R'){
		
			break;//break for loop
		
		}

		//check if the x value is finished
		if(buffer[i] == ';'){
		
			xFinished = 1;
			lastXPos = i+1;
			continue;
		
		}

		if(!xFinished){
			
			//append the value to the x string
			xValue[i-2] = buffer[i];
		
		}else{
			//offset from last x value
			yValue[i-2-lastXPos] = buffer[i]; 
		
		}	
	
	}

	*x = atoi(xValue);
	*y = atoi(yValue);

}

//moves snake in give provided direction
void moveSnake(int direction){

	switch(direction){
	
		case 1:
			printf("\e[1A");
			break;
		case 2:
			printf("\e[1B");
			break;
		case 3:
			printf("\e[1D");
			break;
		case 4:
			printf("\e[1C");
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
	
	//move the cursor to 0,0 and make the background colour white
	printf("\e[H\e[47m");

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
	
	//reset graphics mode
	printf("\e[m");

}
