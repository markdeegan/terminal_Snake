/*
 * Programmer: Matas Noreika Wed Dec 25 06:11:33 PM GMT 2024
 * Purpose: The program will report back terminal cursor position the program also allows users to move their cursor using wasd keys
*/

//include a standard c header for string manipulation
#include <string.h>

//include standard c header for memory and datatype manipulation
#include <stdlib.h>

//include my header to manipulate the terminal
#include "inc/terminalControl.h"

//macro definiton for character used to escape program loop
#define ESCAPECHAR 'q'

//decleration of function to read user key input 
void moveCursor(char key);

//start of main method
int main(int argc, char** argv){
		
	//call function to store original terminal settings into platform dependent variables
	getOriginalSettings();

	//bind exit raw mode on exit
	atexit(exitRawMode);

	//function defined in terminalControl.h to enter raw mode
	enterRawMode();	
	
	//clear the terminal display and move cursor to home
	printf("\e[2J\e[H");
	
	//decleration of a read buffer to read the input stream
	char readBuffer[4096];

	int loopBreak = 0;
	
	//start of program loop
	while(!loopBreak){
		
		//flush our readBuffer before read to prevent left over memeory		
		memset(readBuffer,0,sizeof(readBuffer));

		//read the contents of stdin buffer
		intmax_t readSize = readTerminalInput(readBuffer);	
		
		//check if the buffer read something
		if( readSize > 0 ){

			//check the first character in the buffer is not a escape character 
			if( !(readBuffer[0] == ESCAPECHAR || readBuffer[0] == ESCAPECHAR - 32) ){

				//call function to move cursor
				moveCursor(readBuffer[0]);
				
				//variables to store x and y positions of cursor
				int x,y;	
				
				//call function to request for cursor position
				readCursorPos(&x,&y);

				//save cursor position, move to bottom of game area print the cursor pos and return to original position 	
				printf("\e7\e[21;0H\e[Kx pos: %d, y pos: %d\e8",x,y);

			}else{//the character is an escape character
		
				loopBreak = 1;//break the while loop	

			}

		}else{//nothing was read from the stdin
		
			continue;
		
		} 

	}
	
	//clear output buffer and move cursor to home 
	printf("\e[2J\e[H");

	return 0;//return 0 indicating successful completion of program

}

//defintion of moveCursor function
void moveCursor(char key){

	//compare the pressed key to the mapped directions (W = forward, S = backwards, D = right, A = left)
	switch(key){
	
		case 'w':
				
			printf("\e[A");
			break;

		case 'W':

			printf("\e[A");
			break;

		case 's':

			printf("\e[B");
			break;

		case 'S':

			printf("\e[B");
			break;

		case 'd':
			
			printf("\e[C");
			break;

		case 'D':
			
			printf("\e[C");
			break;

		case 'a':

			printf("\e[D");
			break;

		case 'A':

			printf("\e[D");
			break;

	}

}


