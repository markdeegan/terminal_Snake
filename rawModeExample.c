/*
 * Programmer: Matas Noreika Tue Dec 24 08:50:51 PM GMT 2024
 * Purpose: To showcase the nonconoical or raw mode terminal configuration. this program will read the stdin buffer and print the read character to the terminal until a escape character is pressed.
 * */

//standard c library for input/output control
#include <stdio.h>

//standard c library for misc controls for memeory and error handling 
#include <stdlib.h>

//include my custom made library to ease terminal configuration
#include "terminalControl.h" 

//macro for escape character to terminate the program once read
#define escapeChar 'q' 

//decleration of original settings variable
struct termios original_Settings;

//decleration of function to exit raw mode
void exitRawMode(void);

//start of main method
int main(int argc, char** argv){	
	
	//store the original settings
	tcgetattr(STDIN_FILENO, &original_Settings);
	
	//bind the exitRawMode fuction on exit of this main method (resets terminal to orignal settings)
	atexit(exitRawMode);

	//call the function to enter raw mode
	enterRawMode(&original_Settings);

	//loop break variable to stop read loop
	int exitCondition = 0;

	//decleration of char buffer to read the contents of stdin stream (4096 is the defined limit of stdin)
	char buffer[4096];

	//loop until exit condition is met
	while(!exitCondition){
	
		//using unistd.h read() function we read the contents of the buffer into the buffer variable
		ssize_t readSize = read(STDIN_FILENO, &buffer, sizeof(buffer));
		
		//check if the stdin buffer has no content or failed
		if(readSize <= 0){
			
			//we skip that read cycle
			continue;
		
		}

		//loop through all characters that were read and print them in line
		for(int i = 0; i < readSize; i++){
			
			//check if the escape condition is met (-32 refers to the upper case version of target character )
			if (buffer[i] == escapeChar || buffer[i] == (escapeChar  - 32) ){
			
				//set the exit condition
				exitCondition = 1;

				//break out of for loop
				break;
			
			}

			//print the character onto the terminal
			printf("%c", buffer[i]);
		
		}
		
		//print the next buffer read on a new line
		printf("\n");

	}

	return 0;//returns an exit code of 0 indicating successful operation

}

//definition of function to reset terminal to old settings 
void exitRawMode(void){
	
	//set the terminal to the old settings provided as a parameter
	tcsetattr(STDIN_FILENO, TCSANOW, &original_Settings);

}
