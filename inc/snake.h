/*
 * Porgrammer: Matas Noreika Thu Dec 26 05:31:50 PM GMT 2024
 * Purpose: This header is designed to hold definitions of variables and functions associated with the terminal snake game
*/

//include the standard c header for I/O manipulation
#include <stdio.h>

//macro defiition of the game area height
#define AREAHEIGHT 20

//macro definition of the width to height aspect ratio
#define ASPECTRATIO 2 

//macro definition of escape character to break out of game loop
#define ESCAPECHAR 'q'

//macro definition of frame frame used to control the yield time of each frame
#define FRAMERATE 1/120 

//macro definition of snake movement delay time in seconds
#define MOVEDELAY .25

//macro definition of the number of lines offset due to the title of the game
#define TITLEOFFSET 9 

//macro definition to control the maximum size that is possible for the snake
#define MAXSNAKESIZE ((AREAHEIGHT * ASPECTRATIO) * AREAHEIGHT)

//macro definition to define the upper boundary of the game area
#define UPPERBOUND (TITLEOFFSET + 2)

//macro definition to define the lower boundary of the game area
#define LOWERBOUND (AREAHEIGHT + 4 + TITLEOFFSET)

//macro definition to define the left boundary of the game area
#define LEFTBOUND 2

//macro definition to define the right boundary of the game area
#define RIGHTBOUND ((ASPECTRATIO * AREAHEIGHT) + 3)

//definition of a snake part structure
struct snakePart {
	
	int xPos;
	int yPos;
		
};

//definition of function to generate the game area
void createGameArea(){
	
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
	printf("\e[%d;%dH", LOWERBOUND-((LOWERBOUND-UPPERBOUND)/2), RIGHTBOUND-((RIGHTBOUND-LEFTBOUND)/2));

}

//definition of snake control function (returns direction int 1 = forward, 2 = backwards, 3 = left, 4 = right)
int getDirection(char* buffer){
	
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

//definition of function to print the game title
void printTitle(){

	//open the text file
	FILE* titleFile = fopen("title.txt", "r");
	
	//read buffer to read data from title file
	char buffer[256];

	//file didnt failed to open	
	if(!(titleFile == NULL)){
	
		do{
		
			fgets(buffer, sizeof(buffer), titleFile);//retrieve file content
			printf("%s", buffer);//print the buffer content

		}while(!(feof(titleFile)));
	
		fclose(titleFile);//close the file stream

	}

}
