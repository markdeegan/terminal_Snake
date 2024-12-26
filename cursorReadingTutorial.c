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

//decleration of termios type variable to store old terminal settings prior raw mode
struct termios originalSettings;

//decleration of function to exit raw mode
void exitRawMode(void);

//decleration of function to read user key input 
void moveCursor(char key);

//start of main method
int main(int argc, char** argv){
		
	//retrieve the current terminal settings
	tcgetattr(STDIN_FILENO, &originalSettings);

	//bind exit raw mode on exit
	atexit(exitRawMode);

	//function defined in terminalControl.h to enter raw mode
	enterRawMode(&originalSettings);

	//disable buffering in the output stream
	//setvbuf(stdout, NULL, _IONBF, 0);	
	
	//clear the terminal display and move cursor to home
	printf("\e[2J\e[H");
	
	//decleration of a read buffer to read the input stream
	char readBuffer[4096];

	int loopBreak = 0;

	//print the below content with white background
	printf("\e[47m");
	
	for(int i = 0; i < 20; i++){
	
		for(int j = 0; j < 40; j++){
		
			printf(" ");
		
		}

		printf("\n");
	
	}

	//end the white background control and return cursor to position 1,1
	printf("\e[m\e[1;1H");

	//start of program loop
	while(!loopBreak){
		
		memset(readBuffer,0,sizeof(readBuffer));

		//read the contents of stdin buffer
		ssize_t readSize = read(STDIN_FILENO, &readBuffer, sizeof(readBuffer));	
		
		//check if the buffer read something
		if( readSize > 0 ){

			//check the first character in the buffer is not a escape character 
			if( !(readBuffer[0] == ESCAPECHAR || readBuffer[0] == ESCAPECHAR - 32) ){

				//call function to move cursor
				moveCursor(readBuffer[0]);
				
				//variables to store x and y positions of cursor
				//int x,y;	
				
				//call function to request for cursor position
				//readCursorPos(&x,&y);

				//save cursor position, move to bottom of game area print the cursor pos and return to original position 	
				//printf("\e[s\e[21;0H\e[Kx pos: %d, y pos: %d\e[u",x,y);

			}else{//the character is an escape character
		
				loopBreak = 1;//break the while loop	

			}
		}else{//nothing was read from the stdin
		
			continue;
		
		} 
		
		int x,y;

		readCursorPos(&x,&y);

		printf("\e[s\e[21;0H\e[Kx pos: %d, y pos: %d\e[u",x,y);


	}
	
	//move cursor to a unused line 
	printf("\e[22;1H");

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

//definition of exitRawMode function
void exitRawMode(void){

	//set the terminal to the old settings
	tcsetattr(STDIN_FILENO, TCSANOW, &originalSettings);

}
