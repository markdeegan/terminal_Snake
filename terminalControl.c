/*
 * Programmer: Matas Noreika Tue Dec 24 08:33:07 PM GMT 2024
 * Purpose: Holds function definitions for terminalControl static library.  
*/

//include the terminalControl header with function declerations and variable definitions
#include "terminalControl.h"

//Decleration of global terminal settings variable
#ifdef _WIN32// Windows platform

	HANDLE hstdin;//windows handle type to hold referance to stdin handle or file descriptor is POSIX
	DWORD fdwOriginalSettings;//windows dword type to hold old settings of console similar to termios structure	

#else// POSIX platform

	struct termios originalSettings;//variable to hold old terminal settings to reset to on exit of raw mode	

#endif

//function to return the terminal row & column size
int getTerminalSize(int* x, int* y){
	
	#ifdef _WIN32//windows platform
		
		//decleratrion for stdout handle
		HANDLE hstdout;
		
		//decleration for the buffer infromation returned 
		CONSOLE_SCREEN_BUFFER_INFO consoleInfo;

		//retrieve the handle for stdout
		hstdout = GetStdHandle(STD_OUTPUT_HANDLE);

		//call function to retrieve the consoles screen buffer infromation
		if(!GetConsoleScreenBufferInfo(hstdout, &consoleInfo)){
		
			//print error out to terminal
			fprintf(stderr, "\e[31mGetConsoleScreenBufferInfo() failed to retrieve screen info!\e[m\n");
			
			//return out of function
			return GET_SIZE_FAIL;

		}
		
		//store the result into the dereferanced passed arguments
		*x = (int) consoleInfo.dwSize.X;
		*y = (int) consoleInfo.dwSize.Y;
		
		//return an exit code indicating success
		return TERMINALCONTROLSUCCESS;

	#else//POSIX platform

		struct winsize sz;//create an instance of structure winsize defined in ioctl.h
		
		//check if the ioctl() failed
		if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &sz) == -1){
			
			//print error out to terminal
			fprintf(stderr, "\e[31mioctl() failed to retrieve screen info!\e[m\n");
			
			//return with exit code indicating failed window size request
			return GET_SIZE_FAIL;

		}
		
		//store the result into dereferanced passed arguments
		*x = sz.ws_row;
		*y = sz.ws_col;
		
		//return with exit code indicating success
		return TERMINALCONTROLSUCCESS;
		
	#endif

}

//function to store the current terminal settings into target platform variable location
int getOriginalSettings(){
	
	#ifdef _WIN32//windows platform
		
		//retrieve the stdout handle
		HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
		
		DWORD fdwOutputFlags = (ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN);

		if(!SetConsoleMode(hstdout,fdwOutputFlags)){
			
			DWORD errNum = GetLastError();
			
			//print error to terminal
			fprintf(stderr, "\e[31mError occured while trying to set stdout buffer settings!\e[m\n");
			
			//return with exit code related to setting output buffer configurations
			return SET_OUTPUT_FAIL;

		}

		hstdin = GetStdHandle(STD_INPUT_HANDLE);//retrieve the stdin handle
		
		//call getConsoleMode and perform error check 
		if(! GetConsoleMode(hstdin, &fdwOriginalSettings) ){
			
			DWORD errNum = GetLastError(); 

			//print error out to terminal (debugging)
			fprintf(stderr, "\e[31mError occured while trying to retrieve settings from stdin! code: %d\e[m\n", errNum);		

			//return with exit code indicating failed retrieval of terminal settings
			return GET_SETTINGS_FAIL;

		}

		//return with exit code indicating successful retrieval of terminal settings
		return TERMINALCONTROLSUCCESS;

	#else //POSIX platform
	
		//check if tcgetattr() failed
		if (tcgetattr(STDIN_FILENO, &originalSettings) == -1){
	
			//print error out to terminal (debugging)
			fprintf(stderr, "\e[31mError occured while trying to retrieve settings from stdin!\e[m\n");
			
			//return with exit code indicating failed retreival of terminal settings
			return GET_SETTINGS_FAIL;

		}	
		
		//return with exit code indicating successful retrieval of terminal settings
		return TERMINALCONTROLSUCCESS;

	#endif

}

//function to enter raw mode from concoical mode
int enterRawMode(){
	
	//configure the stdout in no buffering mode
	setvbuf(stdout, NULL, _IONBF, 0);

	#ifdef _WIN32 //windows platform rawMode

		//use bit wise operators to set terminal flags
		//ENABLE_ECHO_INPUT: controls if the input is echoed back to console
		//ENABLE_LINE_INPUT: controls if buffer read yields until new line escape sequence is passed similar to (ICANON)

		DWORD fdwRaw = (ENABLE_WINDOW_INPUT) & ~( ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT );	
		
		//check if setConsoleMode failed to set the terminal settings in raw mode
		if(!SetConsoleMode(hstdin,fdwRaw)){
			
			//retrieve last error from setConsoleMode()
			DWORD errNum = GetLastError();

			//print error message out to terminal
			fprintf(stderr, "\e[31msetConsole() failed to set terminal in raw mode! error code: %d\e[m\n", errNum);
			
			//return with exit code indicating failed to enter raw mode
			return RAW_MODE_FAIL;

		}
		
		//return with exit code indicating success to enter raw mode
		return TERMINALCONTROLSUCCESS;

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
		if(tcsetattr(STDIN_FILENO, TCSANOW, &raw) == -1){
			
			//print error out to terminal
			fprintf(stderr,"\e[31mtcsetattr() failed to set terminal into raw mode!\e[m\n");
			
			//return exit code indicating function failed to enter terminal into raw mode
			return RAW_MODE_FAIL;

		}
		
		//return with exit code indicating success to enter raw mode
		return TERMINALCONTROLSUCCESS;

	#endif

}

//function to exit raw mode
//no return can be set due to this function usually being binded to atexit()
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
		
		//read how much is in the stdin buffer
		//this is needed as windows is event driven unlike POSIX which uses timers on input and output
		GetNumberOfConsoleInputEvents(hstdin,&readSize);

		//if there is nothing in the buffer
		if(readSize == 0){
			
			//return nothing as the buffer is empty
			return (intmax_t) readSize;
		
		}

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
int readCursorPos(int*x, int* y){
	
	//decleration of readBuffer
	char buffer[4096];
	
	//request cursor position report
	printf("\e[6n");

	//read cursor position sent to stdin buffer
	intmax_t readSize = readTerminalInput(buffer);
	
	//check if reading the stdin buffer failed
	if(readSize == READ_FAIL){
		
		//print error out to terminal
		fprintf(stderr,"\e[31mreadTerminalInput() failed to read stdin buffer!\e[m\n");
		
		//return with exit code indicating readTerminalInput() failed within readCursorPos()
		return GET_POS_READ_FAIL;

	}

	//while nothing is read (there is a minor delay from reporting to recieving)
	while((readSize <= 0)){
	
		//continue reading input buffer
		readSize = readTerminalInput(buffer);
		
		//check if reading the stdin buffer failed
		if(readSize == READ_FAIL){
		
			//print error out to terminal
			fprintf(stderr,"\e[31mreadTerminalInput() failed to read stdin buffer!\e[m\n");
		
			//return with exit code indicating readTerminalInput() failed within readCursorPos()
			return GET_POS_READ_FAIL;

		}	

	}
	
	//check if the sscanf failed due to buffer data
	if(sscanf(buffer, "\e[%d;%dR",x,y) == EOF){
	
		//print message out to terminal
		fprintf(stderr,"\e[31msscanf() failed to read data from buffer!\e[m\n");

		//return with exit code indicating the sscanf() failed to read buffer
		return GET_POS_FAIL;
	
	}	
	
	//return exit code indicating success on retrieving cursor position
	return TERMINALCONTROLSUCCESS;

}
