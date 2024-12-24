/*
 * Programmer: Matas Noreika Tue Dec 24 08:33:07 PM GMT 2024
 * Purpose: Holds definitions of functions that control the terminal dependedent on the target platform.  
*/


//POSIX c header for POSIX specific control
#include <unistd.h>

//POSIX c library to control the terminal settings and behaviour
#include <termios.h>

//function to enter raw mode from concoical mode
void enterRawMode(struct termios* originalSettings){

	//set the raw settings to the defernaced old settings
	struct termios raw = *originalSettings;

	//enable polling mode ()
	raw.c_cc[VTIME] = 0;//set timeout time for raw mode to 0 ns
	raw.c_cc[VMIN] = 0;//set minimum read characters to 0

	//use bit wise operators to disable the ICANON and ECHO flags
	//ECH0: Prints the stdin buffer to terminal
	//ICANON: Configures terminal in cononical mode or nonconoical (raw) mode if disabled
	raw.c_lflag &= ~(ECHO | ICANON);
	
	//set the terminal to the raw mode settings
	tcsetattr(STDIN_FILENO, TCSANOW, &raw);

}


