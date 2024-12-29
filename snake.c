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

// MD20241229-04 custom snake error codes
#include "errors.h"

//decleration of variable to store old terminal settings
struct termios original_Settings;

//decleration of exit raw mode function to prevent complier errors
void exitRawMode(void);

//decleration of function to print debugging stats to terminal if debug mode is enabled
void printDebugStats(struct snakePart snake[MAXSNAKESIZE], int lenght, int x, int y);

//start definition of main method
int main(int argc, char** argv){
	
	//defines if the game is ran in debugging mode (infromation printed on screen)
	int debuggingMode;

	//decleration of variable to hold the time since last snake move
	clock_t timeToMove;

	//decleration of snake variable
	struct snakePart snake[MAXSNAKESIZE];

	//definition of snake lenght variable
	int lenght = 3;
	
	//definition of snake direction variable
	int direction = 4;

	//decleration of moveCondition variable used to hold return value of moveSnake()
	int moveCondition;

	//definition of gameBreak variable to control the game loop
	int gameBreak = 0;

	//decleration of read buffer to hold input buffer contents for processing key presses
	char buffer[4096];	

	//decleration of fruit variable
	struct snakePart fruit;

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

	//Matas - merged two printf function calls into one to reduces printf calls
	//clear screen, move the cursor to 0,0 and make cursor invisible
	printf("\e[2J\e[H\e[?25l");

	//prints the game title at the top of the screen
	printTitle();

	//create the game area
	createGameArea();
	
	//configurations for the head position of the snake	
	snake[0].xPos = LOWERBOUND - ((LOWERBOUND-UPPERBOUND)/2);
	snake[0].yPos = RIGHTBOUND - ((RIGHTBOUND-LEFTBOUND)/2);

	//create the trail parts of the snake
	for(int i = 1; i < lenght;i++){
	
		snake[i].xPos = snake[i-1].xPos;
		snake[i].yPos = snake[i-1].yPos -1;
	
	}

	//initalise the clock timer for movement
	timeToMove = clock();

	addFruit(snake, &fruit, lenght);

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
			printDebugStats(snake,lenght,snake[0].xPos,snake[0].yPos);
				
		}
		

		//move snake in direction that was set
		moveCondition = moveSnake(snake, &fruit,&lenght,direction, &timeToMove );
		
		//collision with self game over
		if(moveCondition == 1){
		
			gameBreak = 1;//end game loop
			
			//print the game over reason
			printf("\e7\e[%d;0H\e[K\e[31mcollided with %s\e[m\e8", LOWERBOUND + 2, "snake part (Game over)");	
		}
		

		//wait till the frame rate is matched
		while((float) FRAMERATE > (float) (clock() - startTime)/CLOCKS_PER_SEC );
	
	}
	
	printf("\e[%d;0HUpper: %d, lower: %d, left: %d, right: %d\n", LOWERBOUND + 3, UPPERBOUND, LOWERBOUND, LEFTBOUND, RIGHTBOUND);
	printf("\e[?25h\e[%d;0H",LOWERBOUND+4);//returns cursor to out of bound area of game space and make it visible again

	return SNAKE_SUCCESS;
	// MD20241229-04 changes return code from 0 to SNAKE_SUCCESS
	//exit with error code 0 to indicate successful completion of program

} // MD20241229-03 end definition of main method

//definition of exit raw mode function 
void exitRawMode(void){

	//set terminal to old saved settings
	tcsetattr(STDIN_FILENO, TCSANOW, &original_Settings);

}

//definition of function to print program debugging stats
void printDebugStats(struct snakePart snake[MAXSNAKESIZE], int lenght, int x, int y){

	//print cursor position
	printf("\e7\e[%d;0H\e[Kx pos: %d, y pos: %d\e8", LOWERBOUND + 1,x,y);
	
	//print the difference in position from head to last snake part	
	printf("\e7\e[%d;0H\e[Kx diff: %d, y diff: %d\e8", LOWERBOUND + 2,snake[0].xPos - snake[lenght-1].xPos,snake[0].yPos - snake[lenght-1].yPos);
	
}
