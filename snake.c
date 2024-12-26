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
#define FRAMERATE 1/60 

//macro definition of snake movement delay time in seconds
#define MOVEDELAY .25

#define MAXSNAKESIZE ((AREAHEIGHT * ASPECTRATIO) * AREAHEIGHT)

//definition of a snake part structure
struct snakePart {
	
	int xPos;
	int yPos;
		
};

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
void moveSnake(int direction,struct snakePart snake[MAXSNAKESIZE]);

//decleration of function to print debugging stats to terminal if debug mode is enabled
void printDebugStats(int x, int y);

//decleration of variable to hold the time since last snake movement
clock_t timeToMove;

//decleration of snake lenght variable
int lenght = 3;

//decleration of snake variable that holds an array of snake parts
struct snakePart snake[MAXSNAKESIZE];

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
	
	snake[0].xPos = (AREAHEIGHT)/2;
	snake[0].yPos = (AREAHEIGHT * ASPECTRATIO)/2;

	//create a second part that will follow head
	snake[1].xPos = snake[0].xPos;
	snake[1].yPos = snake[0].yPos - 1;
	
	snake[2].xPos = snake[1].xPos;
	snake[2].yPos = snake[1].yPos - 1;

	int direction = 4;//start with the direction going right
	
	//lenght = 3;	

	//initalise the clock timer for movement
	timeToMove = clock();

	//decleration of read buffer
	char buffer[4096];
	
	

	//printf("\e[?25l");//make cursor invisible

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
			
			//check if user pressed a vaild direction key and return direction
			int newDirection = checkControl(buffer);

			if(!(newDirection == 0)){
			
				direction = newDirection;//set the new direction to the direction provided
				
			}

		}else{//the read failed or nothing was inputted
				
			//continue snake movement in previous direction

		}

		//check if debugging mode is on
		if(debuggingMode){
			
			//print out the debug stats
			printDebugStats(snake[0].xPos,snake[0].yPos);
			printf("\e[s\e[%d;0H\e[Kx pos: %d, y pos: %d\e[u", (AREAHEIGHT + 2) + 3, snake[1].xPos, snake[1].yPos);//print for debugging the snake 1 part follow location	
			printf("\e[s\e[%d;0H\e[Kx pos: %d, y pos: %d\e[u", (AREAHEIGHT + 2) + 4, snake[2].xPos, snake[2].yPos);//print for debugging the snake 2 part follow location	
		}
		

		//move snake in direction that was set
		moveSnake(direction,snake);

		//wait till the frame rate is matched
		while((float) FRAMERATE > (float) (clock() - startTime)/CLOCKS_PER_SEC );
	
	}

	printf("\e[?25h\e[%d;0H",(AREAHEIGHT + 2) + 5);//returns cursor to out of bound area of game space and make it visible again

	return 0;//exit with error code 0 to indicate successful completion of program

}

//definition of exit raw mode function 
void exitRawMode(void){

	//set terminal to old saved settings
	tcsetattr(STDIN_FILENO, TCSANOW, &original_Settings);

}

//moves snake in give provided direction
void moveSnake(int direction, struct snakePart snake[MAXSNAKESIZE]){ 
	
	//check if enough time has passed since last movement
	if( (float) (clock() - timeToMove)/CLOCKS_PER_SEC >= MOVEDELAY){
		
		//function to change the position to new location
		void changePos(int x, int y){
		
			snake[0].xPos = x;

			snake[0].yPos = y;
		
		}

		void updateOtherParts(){
		
			//delete the last trailling part
			
			//if the trailling part isnt affecting borders
			if( !((snake[lenght-1].xPos == 1) || (snake[lenght -1].xPos == AREAHEIGHT + 2 ) || ( snake[lenght - 1].yPos == 2) || (snake[lenght - 1].yPos == ((AREAHEIGHT * ASPECTRATIO) +2) )) ){
					
				//delete the last trailling part
				printf("\e[s\e[%d;%dH\b \e[u",snake[lenght-1].xPos, snake[lenght-1].yPos);
	

			}

			//for loop will shift all the snake parts up
			for(int i = lenght; i > 0; i--){
				
				snake[i].xPos = snake[i-1].xPos;
				snake[i].yPos = snake[i-1].yPos;

			}
		
		}

		//rest the timer
		timeToMove = clock();
		
		updateOtherParts();

		//check if we reached upper boundary
		if(direction == 1 && (snake[0].xPos - 1) == 2){
			
			printf("\e[%d;%dH", AREAHEIGHT + 2 , snake[0].yPos);
			changePos(AREAHEIGHT + 2, snake[0].yPos);
	
			//return;
		
		}else if(direction == 3 && (snake[0].yPos-1) == 2){//snake reached left boundary
			
			printf("\e[%d;%dH",snake[0].xPos, (AREAHEIGHT * ASPECTRATIO) + 1);	
			changePos(snake[0].xPos, (AREAHEIGHT * ASPECTRATIO) + 1);
			//return;
		
		}else if(direction == 4 && ((snake[0].yPos+1) == (AREAHEIGHT * ASPECTRATIO) + 2)){//snake reached right boundary
			
			printf("\e[%d;%dH",snake[0].xPos, 2);
			changePos(snake[0].xPos, 2);
			//return;
		
		}else if(direction == 2 && (snake[0].xPos + 1) == (AREAHEIGHT + 2) ){//snake reached bottom boundary
			
			printf("\e[%d;%dH", 1, snake[0].yPos);
			changePos(1, snake[0].yPos);	
			//return;
		
		}

		//switch case to move snake
		switch(direction){
	
			case 1:
				printf("\b\e[1A@");// move cursor right delete the character and move cursor up and print new head
				changePos(snake[0].xPos - 1, snake[0].yPos);
				break;
			case 2:
				printf("\b\e[1B@");//move cursor down
				changePos(snake[0].xPos + 1, snake[0].yPos);	
				break;
			case 3:
				printf("\b\e[1D@");//move cursor left
				changePos(snake[0].xPos, snake[0].yPos - 1);	
				break;
			case 4:
				printf("\b\e[1C@"); //move cursor right
				changePos(snake[0].xPos, snake[0].yPos + 1);
				break;

		}	
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
	
		default:
			return 0;//return 0 indicating no change
	}
}

//definition of function to generate the game area
void createGameArea(){
	
	//clear screen, move the cursor to 0,0
	printf("\e[2J\e[H");

	//loop through all the game area rows
	for(int i = 0; i <= AREAHEIGHT + 2; i++){

		if(i == 0){
		
			printf("\u250F");//draws left top corner
		
		}else if(i == AREAHEIGHT + 2){
		
			printf("\u2517");//draws bottom right corner
		
		}else{
		
			printf("%c", '|');//draw a straight line
			
		}

		//loop through all the column values
		for(int j = 0; j < (AREAHEIGHT * ASPECTRATIO); j++){
		
			//check if the current line is the top or bottom
			if( (i == 0 ) ||  (i == AREAHEIGHT + 2) ){
			
				printf("-");//prints a heavy straight bar
			
			}else{//anything in between
			
				printf(" ");//print an empty space	
			
			}
		
		}

		if(i == 0){
		
			printf("\u2513");//prints top right corner
		
		}else if(i == AREAHEIGHT + 2){
		
			printf("\u251B");//prints bottom right corner
		
		}else{
		
			printf("%c",'|');//prints a straight line
		
		}

		//start on a new line	
		printf("\n");

	}
	
	//reset graphics mode and move cursor to the middle of the screen
	printf("\e[%d;%dH", AREAHEIGHT/2, (AREAHEIGHT * ASPECTRATIO)/2);

}

//definition of function to print program debugging stats
void printDebugStats(int x, int y){

	//print cursor position
	printf("\e[s\e[%d;0H\e[Kx pos: %d, y pos: %d\e[u", (AREAHEIGHT + 2) + 2 ,x,y);
	
}
