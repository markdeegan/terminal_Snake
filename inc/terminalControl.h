/*
 * Programmer: Matas Noreika Tue Dec 24 08:33:07 PM GMT 2024
 * Purpose: Holds definitions of functions that control the terminal dependedent on the target platform.  
*/

//standard c library to manipulate input and output
#include <stdio.h>

//POSIX c header for POSIX specific control
#include <unistd.h>

//POSIX c library to control the terminal settings and behaviour
#include <termios.h>

//POSIX c library used to communicate with kernel (used to get terminal width)
#include <sys/ioctl.h>

void getTerminalSize(){

	struct winsize sz;//create an instance of structure winsize defined in ioctl.h
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &sz);

	printf("x size: %d, y size: %d\n",sz.ws_row,sz.ws_col);//print out the size retrieved

}

//function to enter raw mode from concoical mode
void enterRawMode(struct termios* originalSettings){

	//confgiures the standard out in no buffering mode
	setvbuf(stdout, NULL, _IONBF, 0);

	//set the raw settings to the defernaced old settings
	struct termios raw = *originalSettings;

	//enable polling mode ()
	raw.c_cc[VTIME] = 0;//set timeout time for raw mode to 0 ns
	raw.c_cc[VMIN] = 0;//set minimum read characters to 0

	//use bit wise operators to disable the ICANON and ECHO flags
	//ECH0: control flag to enable stdin echo or not
	//ICANON: Configures terminal in cononical mode or nonconoical (raw) mode if disabled
	raw.c_lflag &= ~(ECHO | ICANON);
	
	//set the terminal to the raw mode settings
	tcsetattr(STDIN_FILENO, TCSANOW, &raw);

}

//function to use ANSI escape codes to retrieve the cursor position
void readCursorPos(int*x, int* y){

	char buffer[4096];//read buffer with the size of stdin buffer

	ssize_t readSize;//variable to store the read size of read()

	write(STDOUT_FILENO,"\e[6n",4);//request cursor position (will be reported back to stdin)
	
	readSize = read(STDIN_FILENO, &buffer, sizeof(buffer));//call function to read the stdin into buffer
	
	//we repeatedly read until report is sent back (there is a slight delay on the messaging)
	while((readSize <= 0)){
	
		readSize = read(STDIN_FILENO, &buffer, sizeof(buffer));
	
	}
		
	//do a formatted scan of buffer and retrieve the x and y position of the cursor
	sscanf(buffer, "\e[%d;%dR", x, y);	
			
}
