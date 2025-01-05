/*
 * Programmers: Matas Noreika, Mark Deegan Tue Dec 31 12:19:17 GMT 2024
 * Purpose: definitions for snake_helpers static library functions
*/

//include the variable and function definitions of the snake_helpers header
#include "snake_helpers.h"

/* definition of function to generate the game area */
void createGameArea(void){
	
	//loop through all the game area rows
	for(int i = 0; i < AREAHEIGHT; i++){

		if(i == 0){
			
			printf("\e(0l\e(B");//draws left top corner using designated character set 0
		
		}else if(i == AREAHEIGHT-1){
		
			printf("\e(0m\e(B");//draws bottom right corner
		
		}else{
		
			printf("%c", '|');//draw a straight line
			
		}

		//loop through all the column values
		for(int j = 0; j < (AREAHEIGHT * ASPECTRATIO); j++){
		
			//check if the current line is the top or bottom
			if( (i == 0 ) ||  (i == (AREAHEIGHT-1)) ){
			
				printf("-");//prints a heavy straight bar
			
			}else{//anything in between
			
				printf(" ");//print an empty space	
			
			}
		
		}

		if(i == 0){
		
			printf("\e(0k\e(B");//prints top right corner
		
		}else if(i == AREAHEIGHT - 1){
		
			printf("\e(0j\e(B");//prints bottom right corner
		
		}else{
		
			printf("%c",'|');//prints a straight line
		
		}

		//start on a new line	
		printf("\n");

	}
	
	//reset graphics mode and move cursor to the middle of the screen
	printf("\e[%d;%dH", LOWERBOUND-((LOWERBOUND-UPPERBOUND)/2), RIGHTBOUND-((RIGHTBOUND-LEFTBOUND)/2));

} //definition of function to generate the game area

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
	
		default://invaild character handling
			return 0;//return 0 indicating no change
	}
} // end definition of snake control function (returns direction int 1 = forward, 2 = backwards, 3 = left, 4 = right)

//Matas - added additional comment to functionality of printTitle()
//definition of function to print the game title
void printTitle(void){

	//open the text file
	FILE* titleFile = fopen("title.txt", "r");
	
	//read buffer to read data from title file
	char buffer[256];

	//file didnt failed to open	
	if(!(titleFile == NULL)){
	
		do{
		
			fgets(buffer, sizeof(buffer), titleFile);//retrieve file content
			printf("%s", buffer);//print the buffer content

		}while(!(feof(titleFile)));//repeat the do-while loop until the end of file is reached
	
		fclose(titleFile);//close the file stream

	}

} // //definition of snake control function (returns direction int 1 = forward, 2 = backwards, 3 = left, 4 = right)


//definition of function to add to snake lenght
//Matas - relocated addLenght() to header for less clutter in snake.c
//Matas - added additional argument to function to pass current snake lenght and snake variable
void addLenght(struct snakePart snake[MAXSNAKESIZE], int* lenght, int direction){

	*lenght += 1;//incrament the lenght variable
	
	//we do a switch case on direction to determine where new snake part should spawn relative to previous snake part
	switch(direction){
	
		case 1://we are moving up
			snake[*lenght-1].xPos = snake[*lenght-2].xPos + 1;
		       	snake[*lenght-1].yPos = snake[*lenght-2].yPos;	
			break;
		case 2://we are moving down
			snake[*lenght-1].xPos = snake[*lenght-2].xPos - 1;
			snake[*lenght-1].yPos = snake[*lenght-2].yPos;
			break;
		case 3://we are moving left
			snake[*lenght-1].xPos = snake[*lenght-2].xPos;
			snake[*lenght-1].yPos = snake[*lenght-2].yPos - 1;
			break;
		case 4://we are moving right
			snake[*lenght-1].xPos = snake[*lenght-2].xPos;
			snake[*lenght-1].yPos = snake[*lenght-2].yPos + 1;
			break;

	}

} // end definition of function to add to snake lenght


// definition of function to move snake in give provided direction
//Matas - relocated function to snake header
//Matas - added additional argument to pass current snake lenght
int moveSnake(struct snakePart snake[MAXSNAKESIZE],struct snakePart* fruit,int* lenght, int direction, clock_t* timeToMove){ 
	
	//check if enough time has passed since last movement
	if( (float) (clock() - *timeToMove)/CLOCKS_PER_SEC >= MOVEDELAY){
		
		//rest the timer
		*timeToMove = clock();
		
		//call function to shift all snake parts up a position in the chain
		updateOtherParts(snake, *lenght);

		//check if we reached upper boundary
		if(direction == 1 && (snake[0].xPos - 1) == UPPERBOUND){
			
			printf("\e[%d;%dH", LOWERBOUND, snake[0].yPos);
			changePos(&snake[0],LOWERBOUND, snake[0].yPos);
		
		}else if(direction == 3 && (snake[0].yPos-1) == LEFTBOUND){//snake reached left boundary
			
			printf("\e[%d;%dH",snake[0].xPos, RIGHTBOUND);	
			changePos(&snake[0],snake[0].xPos, RIGHTBOUND);
		
		}else if(direction == 4 && ((snake[0].yPos+1) == RIGHTBOUND)){//snake reached right boundary
			
			printf("\e[%d;%dH",snake[0].xPos, LEFTBOUND);
			changePos(&snake[0],snake[0].xPos, LEFTBOUND);
		
		}else if(direction == 2 && (snake[0].xPos + 1) == LOWERBOUND ){//snake reached bottom boundary
			
			printf("\e[%d;%dH", UPPERBOUND, snake[0].yPos);
			changePos( &snake[0],UPPERBOUND, snake[0].yPos);	
		
		}

		//switch case to move snake
		switch(direction){
	
			case 1:
				printf("\e[1A\b@");// move cursor right delete the character and move cursor up and print new head
				changePos(&snake[0],snake[0].xPos - 1, snake[0].yPos);
				break;
			case 2:
				printf("\e[1B\b@");//move cursor down
				changePos(&snake[0],snake[0].xPos + 1, snake[0].yPos);	
				break;
			case 3:
				printf("\e[1D\b@");//move cursor left
				changePos(&snake[0],snake[0].xPos, snake[0].yPos - 1);	
				break;
			case 4:
				printf("\e[1C\b@"); //move cursor right
				changePos(&snake[0],snake[0].xPos, snake[0].yPos + 1);
				break;

		}
		
		//variable to determine if fruit should be printed
		int rePrintFruit = 1;

		//check if head collided with fruit (we add extra lenght and a new fruit)
		if(snake[0].xPos == fruit->xPos && snake[0].yPos == fruit->yPos){
	
			addLenght(snake, lenght, direction);//call function to add to snake lenght
			addFruit(snake, fruit, *lenght);//call the add fruit function
			rePrintFruit = 0;//wont print the fruit again

		}

		//loop through all snake parts to check for collision
		for(int i = 1; i < *lenght; i++){
		
			//checks if snake collided with self
			if(snake[0].xPos == snake[i].xPos && snake[0].yPos == snake[i].yPos){
			
				//collision happened (set game to game over)
				return 1;
			}

			//checks if fruit is currently underneath a snake part
			if(snake[i].xPos == fruit->xPos && snake[i].yPos == fruit->yPos){
			
				rePrintFruit = 0;//we wont print the fruit over the snake part
			
			}

		}
		
		//if the fruit needs to be reprinted	
		if(rePrintFruit){
			
			//we reprint the fruit on the screen
			printf("\e7\e[%d;%dH\bo\e8", fruit->xPos, fruit->yPos);
		
		}

	}
	// MD20241228-07 inserting retutn 0 call, as this function (moveSnake) is defined as retutning an int
	return 0;
} // MD20241228-08 end definition of the moveSnake function

// Matas - relocated function to snake header and renamed function to match functionality
// definition of pickRandomLocation function to choose a random location for fruit to spawn
void pickRandomLocation(struct snakePart* object){
	
	object->xPos = rand() % ((LOWERBOUND-1) - (UPPERBOUND+1) + 1) + (UPPERBOUND+1);//choose a random row position
	object->yPos = rand() % ((RIGHTBOUND-1) - (LEFTBOUND+1) + 1) + (LEFTBOUND+1);//choose a random column position

} // MD20241228-02 end definition of the pickLocation function

//Matas - relocated function to snake header
//definition of function to add fruit
void addFruit(struct snakePart snake[MAXSNAKESIZE], struct snakePart* fruit, int lenght){

	//variable to control looping
	int positionTaken = 1;

	//repeat picking a location and testing to see if the position is taken
	while(positionTaken){
		
		positionTaken = 0;//set the position as not taken
		pickRandomLocation(fruit);//pick a random location for fruit

		//loop through all snake parts in lenght
		for(int i = 0; i < lenght; i++){
			
			if(fruit->xPos == snake[i].xPos && fruit->yPos == snake[i].yPos){
				
				positionTaken = 1;//set the position as taken
				break;//dont continue the rest of the loop
			
			}
		}	
	}
	
	//print the fruit out
	printf("\e7\e[%d;%dH\bo\e8",fruit->xPos,fruit->yPos);

} // end definition of function to add fruit


// Matas - relocated function to snake header
//function to change the position to new location
void changePos(struct snakePart* object, int x, int y){

	object->xPos = x;

	object->yPos = y;

} // MD20241228-04 end definition of the changePos function

//Matas - relocated function to snake header
//function to update the trailling part positions and delete the last location
void updateOtherParts(struct snakePart snake[MAXSNAKESIZE], int lenght){
	
	//if the trailling part isnt affecting borders
	if( !((snake[lenght-1].yPos == LEFTBOUND) || (snake[lenght -1].yPos == RIGHTBOUND ) || ( snake[lenght - 1].xPos == UPPERBOUND) || (snake[lenght - 1].xPos == LOWERBOUND )) ){
			
		//delete the last trailling part
		printf("\e7\e[%d;%dH\b \e8",snake[lenght-1].xPos, snake[lenght-1].yPos);


	}

	//for loop will shift all the snake parts up
	for(int i = lenght; i > 0; i--){

		snake[i].xPos = snake[i-1].xPos;
		snake[i].yPos = snake[i-1].yPos;

	}

} // MD20241228-06 end definition of the updateOtherParts function

// definition of saveHighScore function
int saveHighscore(int score){
	
	//attempt to open high score file 
	FILE* hsFile = fopen("data/highscore.txt", "a");
	
	//decleration of highscore variable
	int highscore = 0;

	//check if opening high score file failed
	if(hsFile == NULL){
	
		fprintf(stderr, "\e[31mfopen() failed to open highscore.txt");
		
		//return none positive value
		return -1;
	}


	fseek(hsFile,0,SEEK_SET);//move back to the start of the file
	
	//read the old data 
	fscanf(hsFile,"highscore: %d", &highscore);	
	
	//check if new score is higher than previous highscore
	if(highscore < score){
		
		//move file pointer back to the beginning
		fseek(hsFile, 0, SEEK_SET);	

		//set the new score as the high score
		fprintf(hsFile,"highscore: %d", score);
	
	}

	fclose(hsFile);//close the high score file
	
	return 0;

} // end definition of saveHighScore function

//function to return current highscore
int getHighscore(void){
	
	//call function to open highscore file
	FILE* hsFile = fopen("data/highscore.txt", "r");

	int highscore = 0;

	//failed to open highscore file
	if(hsFile == NULL){

		return 0;
	
	}

	//move file pointer to start of file
	fseek(hsFile, 0, SEEK_SET);
	
	//read the highscore value
	fscanf(hsFile,"highscore: %d", &highscore);

	//close the file
	fclose(hsFile);
	
	//return the highscore value
	return highscore;

} // end of function to return current highscore
