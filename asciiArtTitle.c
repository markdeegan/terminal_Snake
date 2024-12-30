/*
 * Programmer: Matas Noreika Thu Dec 26 06:00:32 PM GMT 2024
 * Purpose: To generate an ascii art title for the snake game
*/

//include standard c header for I/O manipulation
#include <stdio.h>

//include my custom terminal control header
#include "terminalControl.h"

//start of main method
int main(int argc, char** argv){

	printf("\e[2J\e[H");//clear the screen at the start of program and move cursor to home (1,1)
	
	//decleration of variables to old the dimensions of the terminal window
	int x,y;

	//calls a function to print the current window size
	if(getTerminalSize(&x, &y) == GET_SIZE_FAIL){
	
		printf("\e[31mgetTerminalSize failed to retrieve the terminal window size!\e[m\n");
	
		return 1;

	}

	//print out to the terminal the size of the terminal window dimensions
	printf("\e[32mrow count: %d, column count: %d\e[m\n",x, y);
	
	//decleration of file pointer to point to title.txt
	FILE* titleFile;

	//open the title art for reading and check if the opening failed
	if( (titleFile = fopen("title.txt", "r")) == NULL){
	
		return 2;//failed to open file
	
	}

	//intitalise read buffer
	char buffer[256];
	
	//loop until end of file character is reached
	do{
		
		//read until buffer is full or \n character reached
		fgets(buffer, sizeof(buffer), titleFile);
		
		//print back out the row or contents of the full buffer
		printf("%s", buffer);

	}while(!(feof(titleFile)));

	fclose(titleFile);//close the file once reading is complete

	return 0;//return exit code indicating program was successful

}
