/*
 * Programmer: Matas Noreika Thu Dec 26 06:00:32 PM GMT 2024
 * Purpose: To generate an ascii art title for the snake game
*/

//include standard c header for I/O manipulation
#include <stdio.h>

//include my custom terminal control header
#include "inc/terminalControl.h"

//start of main method
int main(int argc, char** argv){

	printf("\e[2J\e[H");//clear the screen at the start of program
	
	//calls a function to print the current window size
	getTerminalSize();

	FILE* titleFile;

	//open the title art for reading and check if the opening failed
	if( (titleFile = fopen("title.txt", "r")) == NULL){
	
		return 1;//failed to open file
	
	}

	//intitalise read buffer
	char buffer[256];

	do{

		fgets(buffer, sizeof(buffer), titleFile);
		printf("%s", buffer);

	}while(!(feof(titleFile)));

	fclose(titleFile);//close the file once reading is complete

	return 0;

}
