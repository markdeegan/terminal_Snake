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

//custom snake game header
#include "inc/snake.h"

//defines if game is ran in debugging mode (information printed to screen)
int debuggingMode;

//decleration of variable to store old terminal settings
struct termios original_Settings;

//decleration of exit raw mode function to prevent complier errors
void exitRawMode(void);

//decleration of function to move cursor to the give location
int moveSnake(int direction,struct snakePart snake[MAXSNAKESIZE]);

//decleration of function to print debugging stats to terminal if debug mode is enabled
void printDebugStats(int x, int y);

//decleration of variable to hold the time since last snake movement
clock_t timeToMove;

//decleration of snake variable
struct snakePart snake[MAXSNAKESIZE];

//decleration of snake lenght variable
int lenght = 3;

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

	//clear screen, move the cursor to 0,0
	printf("\e[2J\e[H");

	//prints the game title at the top of the screen
	printTitle();

	//create the game area
	createGameArea();

	//defintion of game break condition variable
	int gameBreak = 0;	
	
	//configurations for the head position of the snake	
	snake[0].xPos = LOWERBOUND - ((LOWERBOUND-UPPERBOUND)/2);
	snake[0].yPos = RIGHTBOUND - ((RIGHTBOUND-LEFTBOUND)/2);

	//create the trail parts of the snake
	for(int i = 1; i < lenght;i++){
	
		snake[i].xPos = snake[i-1].xPos;
		snake[i].yPos = snake[i-1].yPos -1;
	
	}

	int direction = 4;//start with the direction going right
	
	//the output of the move
	int moveCondition;	

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
			int newDirection = getDirection(buffer);

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
				
		}
		

		//move snake in direction that was set
		moveCondition = moveSnake(direction,snake);
		
		//collision with self game over
		if(moveCondition == 1){
		
			gameBreak = 1;//end game loop
			
			//print the game over reason
			printf("\e7\e[%d;0H\e[K\e[31mcollided with %s\e[m\e8", LOWERBOUND + 2, "snake part (Game over)");	
		}
		

		//wait till the frame rate is matched
		while((float) FRAMERATE > (float) (clock() - startTime)/CLOCKS_PER_SEC );
	
	}

	printf("\e[?25h\e[%d;0H",LOWERBOUND+3);//returns cursor to out of bound area of game space and make it visible again

	return 0;//exit with error code 0 to indicate successful completion of program

}

//definition of exit raw mode function 
void exitRawMode(void){

	//set terminal to old saved settings
	tcsetattr(STDIN_FILENO, TCSANOW, &original_Settings);

}

//moves snake in give provided direction
int moveSnake(int direction, struct snakePart snake[MAXSNAKESIZE]){ 
	
	//check if enough time has passed since last movement
	if( (float) (clock() - timeToMove)/CLOCKS_PER_SEC >= MOVEDELAY){
		
		//function to change the position to new location
		void changePos(int x, int y){
		
			snake[0].xPos = x;

			snake[0].yPos = y;
		
		}
		
		//function to update the trailling part positions and delete the last location
		void updateOtherParts(){
			
			
			//if the trailling part isnt affecting borders
			if( !((snake[lenght-1].xPos == LEFTBOUND) || (snake[lenght -1].xPos == RIGHTBOUND ) || ( snake[lenght - 1].yPos == UPPERBOUND) || (snake[lenght - 1].yPos == LOWERBOUND )) ){
					
				//delete the last trailling part
				printf("\e7\e[%d;%dH\b^\e8",snake[lenght-1].xPos, snake[lenght-1].yPos);
	

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
		if(direction == 1 && (snake[0].xPos - 1) == UPPERBOUND){
			
			printf("\e[%d;%dH", LOWERBOUND - 1, snake[0].yPos);
			changePos(LOWERBOUND - 1, snake[0].yPos);
		
		}else if(direction == 3 && (snake[0].yPos-1) == LEFTBOUND){//snake reached left boundary
			
			printf("\e[%d;%dH",snake[0].xPos, RIGHTBOUND - 1);	
			changePos(snake[0].xPos, RIGHTBOUND - 1);
		
		}else if(direction == 4 && ((snake[0].yPos+1) == RIGHTBOUND)){//snake reached right boundary
			
			printf("\e[%d;%dH",snake[0].xPos, LEFTBOUND + 1);
			changePos(snake[0].xPos, LEFTBOUND + 1);
		
		}else if(direction == 2 && (snake[0].xPos + 1) == LOWERBOUND ){//snake reached bottom boundary
			
			printf("\e[%d;%dH", UPPERBOUND + 1, snake[0].yPos);
			changePos( UPPERBOUND + 1, snake[0].yPos);	
		
		}

		//switch case to move snake
		switch(direction){
	
			case 1:

				printf("\e[1A\b@");// move cursor right delete the character and move cursor up and print new head
				changePos(snake[0].xPos - 1, snake[0].yPos);
				break;
			case 2:
				printf("\e[1B\b@");//move cursor down
				changePos(snake[0].xPos + 1, snake[0].yPos);	
				break;
			case 3:
				printf("\e[1D\b@");//move cursor left
				changePos(snake[0].xPos, snake[0].yPos - 1);	
				break;
			case 4:
				printf("\e[1C\b@"); //move cursor right
				changePos(snake[0].xPos, snake[0].yPos + 1);
				break;

		}

		//updateOtherParts();
		
		//loop through all snake parts to check for collision
		for(int i = 1; i < lenght; i++){
		
			if(snake[0].xPos == snake[i].xPos && snake[0].yPos == snake[i].yPos){
			
				//collision happened
				return 1;
			}
		
		}


	}
}

//definition of function to print program debugging stats
void printDebugStats(int x, int y){

	//print cursor position
	printf("\e7\e[%d;0H\e[Kx pos: %d, y pos: %d\e8", LOWERBOUND + 1,x,y);
	
	printf("\e7\e[%d;0H\e[Kx diff: %d, y diff: %d\e8", LOWERBOUND + 2,snake[0].xPos - snake[lenght-1].xPos,snake[0].yPos - snake[lenght-1].yPos);
	
}
