/*
 * Programmer: Matas Noreika Tue Dec 24 08:33:07 PM GMT 2024
 * Purpose: Holds definitions of functions that control the terminal dependedent on the target platform.  
*/

//standard c header to manipulate input and output
#include <stdio.h>

//standard c header for int type definitions
#include <stdint.h>

#ifdef _WIN32 //check for windows platform definition

	//windows header file
	#include <windows.h>

	HANDLE hstdin;//windows handle type to hold referance to stdin handle or file descriptor is POSIX
	DWORD fdwOriginalSettings;//windows dword type to hold old settings of console similar to termios structure

#else //Non windows platform

	//POSIX c header for POSIX specific control
	#include <unistd.h>

	//POSIX c library to control the terminal settings and behaviour
	#include <termios.h>

	//POSIX c library used to communicate with kernel (used to get terminal width)
	#include <sys/ioctl.h>

	struct termios originalSettings;//variable to hold old terminal settings to reset to on exit of raw mode

#endif

//definition of error return code of readTerminalInput()
#define READ_FAIL -1

void getTerminalSize(){
	
	#ifdef _WIN32//windows platform

		//code to be yet added

	#else//POSIX platform

		struct winsize sz;//create an instance of structure winsize defined in ioctl.h
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &sz);

		printf("x size: %d, y size: %d\n",sz.ws_row,sz.ws_col);//print out the size retrieved

	#endif

}

//function to store the current terminal settings into target platform variable location
void getOriginalSettings(){
	
	#ifdef _WIN32//windows platform

		hstdin = GetStdHandle(STD_INPUT_HANDLE);//retrieve the stdin handle
		
		//call getConsoleMode and perform error check 
		if(! GetConsoleMode(hstdin, &fdwOriginalSettings) ){
			
			//print error out to terminal (debugging)
			fprintf(stderr, "\e[31mError occured while trying to retrieve settings from stdin!\e[m\n");
		
		}

	#else //POSIX platform
	
		//check if tcgetattr() failed
		if (tcgetattr(STDIN_FILENO, &originalSettings) == -1){
	
			//print error out to terminal (debugging)
			fprintf(stderr, "\e[31mError occured while trying to retrieve settings from stdin!\e[m\n");
	
		}	

	#endif

}

//function to enter raw mode from concoical mode
void enterRawMode(){
	
	//configure the stdout in no buffering mode
	setvbuf(stdout, NULL, _IONBF, 0);

	#ifdef _WIN32 //windows platform rawMode

		//use bit wise operators to set terminal flags
		//ENABLE_ECHO_INPUT: controls if the input is echoed back to console
		//ENABLE_LINE_INPUT: controls if buffer read yields until new line escape sequence is passed similar to (ICANON)

		DWORD fdwRaw = ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT);	
		
		//check if setConsoleMode failed to set the terminal settings in raw mode
		if(!SetConsoleMode(hstdin,fdwRaw)){
		
			//print error message out to terminal
			fprintf(stderr, "\e[31msetConsole() failed to set terminal in raw mode!\e[m\n");
		}

	#else //POSIX platform rawMode

		//set the raw settings to the defernaced old settings
		struct termios raw = originalSettings;

		//enable polling mode
		raw.c_cc[VTIME] = 0;//set timeout time for raw mode to 0 ns
		raw.c_cc[VMIN] = 0;//set minimum read characters to 0

		//use bit wise operators to disable the ICANON and ECHO flags
		//ECH0: control flag to enable stdin echo or not
		//ICANON: Configures terminal in cononical mode or nonconoical (raw) mode if disabled
		raw.c_lflag &= ~(ECHO | ICANON);
	
		//set the terminal to the raw mode settings
		tcsetattr(STDIN_FILENO, TCSANOW, &raw);

	#endif

}

//function to exit raw mode
void exitRawMode(){

	#ifdef _WIN32 //windows platform
		
		//check if resetting terminal settings failed
		if ( ! SetConsoleMode(hstdin,fdwOriginalSettings) ){
			
			//print error message out to terminal (debugging)
			fprintf(stderr, "\e[31msetConsoleMode() failed to reset terminal settings to original settings!\e[m\n");
		
		}

	#else // POSIX platform

		//check if resetting terminal settings failed
		if(tcsetattr(STDIN_FILENO, TCSANOW, &originalSettings) == -1){
			
			//print error message out to terminal (debugging)
			fprintf(stderr, "\e[31mtcsetattr() failed to reset terminal settings to original settings!\e[m\n");
		
		}

	#endif

}

//function to read from terminal input buffer independent of platform
//returns the read size as max int size in current system to avoid conflict with ssize_t on other platforms 
intmax_t readTerminalInput(char buffer[4096]){
	
	#ifdef _WIN32 //windows platform
		
		//decleration of readSize variable
		DWORD readSize;

		//check if read from stdin failed by getting the last exit return code
		if(!ReadConsole(hstdin,buffer, 4096, &readSize, NULL)){
		
			//print error out to terminal
			fprintf(stderr, "\e[31mreadConsole() failed to read from stdin!\e[m\n");
			
			//return failed read exit code
			return READ_FAIL;
		
		}
		
		//return the readSize typecasted to max int size on system
		return (intmax_t) readSize;

	#else //POSIX platform
		
		//decleration of readSize variable
		ssize_t readSize;
		
		//assign readSize to the return of read()
		readSize = read(STDIN_FILENO, buffer, 4096);
		
		//check if read() failed
		if(readSize == -1){
			
			//print error out to terminal
			fprintf(stderr, "\e[31mread() failed to read from stdin!\e[m\n");
			
			//return the exit code for a failed read
			return READ_FAIL;
		}
		
		//return the readSize typcasted to max int size on system
		return (intmax_t) readSize;

	#endif

}

//function to use ANSI escape codes to retrieve the cursor position
void readCursorPos(int*x, int* y){
	
	#ifdef _WIN32//windows platform

		//code to be yet added for windows

	#else//POSIX platform

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

	#endif	
			
}
