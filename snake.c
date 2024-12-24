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

//macro definition of escape character to break out of game loop
#define escapeChar 'q'

//macro definition of frame frame used to control the yield time of each frame
#define frameRate 1/60 

//decleration of variable to store old terminal settings
struct termios original_Settings;

//decleration of exit raw mode function to prevent complier errors
void exitRawMode(void);

//start of main method
int main(int argc, char** argv){

	//store the current terminal settings into the original settings variable
	tcgetattr(STDIN_FILENO, &original_Settings);
	
	//bind exitRawMode function on exit of program
	atexit(exitRawMode);

	//call enterRawMode function to set the terminal to raw mode
	enterRawMode(&original_Settings);

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
		
			//loop through all characters
			for(int i = 0; i < readSize; i++){
		
				//check if the character read is not the escape character lower or upper case
				if(!(buffer[i] == escapeChar || buffer[i] == escapeChar - 32 )){
					
					//print the character to the terminal 				
					printf("%c", buffer[i]);

				}else{//the character is an escape character
				
					//set the game break to 1
					gameBreak = 1;
					break;//break out of the for loop
				
				}
		
			}

			//start print on a new line and print the time elapsed to complete the operation (debugging)
			printf("\ndelta time: %.6f\n", (float) (clock() - startTime)/CLOCKS_PER_SEC);

		}else{//the read failed
		
			//handle for the error
			continue;//skip this loop iteration
		
		}
		
		//wait till the frame rate is matched
		while((float) frameRate > (float) (clock() - startTime)/CLOCKS_PER_SEC );
		
		//debug print to see if the frames are rendering at target frame rate
		printf("\nframe completed: %.6f\n", (float) (clock() - startTime)/CLOCKS_PER_SEC);
	
	}

	return 0;//exit with error code 0 to indicate successful completion of program

}

//definition of exit raw mode function 
void exitRawMode(void){

	//set terminal to old saved settings
	tcsetattr(STDIN_FILENO, TCSANOW, &original_Settings);

}
