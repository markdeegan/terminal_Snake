/*
 * Programmer: Matas Noreika Tue Dec 24 08:33:07 PM GMT 2024
 * Purpose: Holds declerations of functions that control the terminal dependedent on the target platform. The header also declares global variables needed for functionality.  
*/

//standard c header to manipulate input and output
#include <stdio.h>

//standard c header for int type definitions
#include <stdint.h>

#ifdef _WIN32 //Windows platform
	
	//windows header file
	#include <windows.h>	
	
#else //POSIX platform

	//POSIX c header for POSIX specific control
	#include <unistd.h>

	//POSIX c library to control the terminal settings and behaviour
	#include <termios.h>

	//POSIX c library used to communicate with kernel (used to get terminal width)
	#include <sys/ioctl.h>

		
#endif

//definition of success return code of all functions in header
//added prefix TERMINALCONTROL to avoid conflict with other definitions of success
#define TERMINALCONTROLSUCCESS 0

//definition of error return code of readTerminalInput()
#define READ_FAIL -1

//definition of error return code of getTerminalSize()
#define GET_SIZE_FAIL -2

//definition of error return code of enterRawMode()
#define RAW_MODE_FAIL -3

//definition of error return code of getOriginalSettings()
#define GET_SETTINGS_FAIL -4

//definition of error return code of readCursorPos() when readTerminalInput() fails
#define GET_POS_READ_FAIL -5

//definition of error return code of readCursorPos() when sscanf() fails to read buffer data
#define GET_POS_FAIL -6

//definition of error return code for windows platform in getOriginalSettings()
#define SET_OUTPUT_FAIL -7

//function to return the terminal row & column size
int getTerminalSize();	
	
//function to store the current terminal settings into target platform variable location
//this function must be called prior enterRawMode()
int getOriginalSettings();

//function to enter raw mode from concoical mode
int enterRawMode();

//function to exit raw mode
//no return can be set due to this function usually being binded to atexit()
void exitRawMode();
	
//function to read from terminal input buffer independent of platform
//returns the read size as max int size in current system to avoid conflict with ssize_t on other platforms 
intmax_t readTerminalInput();

//function to use ANSI escape codes to retrieve the cursor position
int readCursorPos();
