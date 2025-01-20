# Introduction
This repository is made to showcase the ability to control the terminal to behave as a game rendering window. Currently the programs provided on this repository are only compliant with POSIX operating systems.

# Content
* cursor position reading
* terminal setting configuration
* frame rendering control
* ANSI escape codes
* stream buffer manipulation

# Cursor position reading
The cursor position within the terminal is stored within local variables related to the terminal instance. Windows and POSIX systems have built-in functionalities to obtain the position. In this project ANSI escape sequences were used to obtain the cursor position. Using ESC[6n a report is requested and a response in the format ESC[r;cR is sent to the stdin buffer, where: r is the row and c is the column. In order to obtain the report value the terminal must be set in non-conoical (raw) mode in order to be able to read from the buffer without requiring a user to enter a new line feed character. In the terminalControl.h a function defined as getCursorPos() performs the described process above and stores the row and column values into the passed arguments.

# Terminal setting configuration
The terminal can be configured using the windows console api <windows.h> or termios.h in POSIX. The major difference between platforms dicovered is the way the terminals handle the input timeout. POSIX systems used a time delay to wait a duration before inputs, this is defined as termios.c_cc[VTIME]. termios.c_cc[VMIN] is also used to control the minimum character count in the stdin buffer before read() returns the stdin contents. Windows uses event driven conditions to complete tasks. On Windows GetNumberOfConsoleInputEvents() returns the size of the stdin buffer which a conditional check can be used to recreate a similar functionality of VTIME and VMIN on windows. In the man pages of termios.h when VTIME = 0 and VMIN = 0 the operation is considered as polling mode. This is the setting applied in order read key inputs rapidly. 

# Frame rendering
The traditional method of rendering frames is by updating the image space at a set rate which is dependent on hardware. In this project the image space is updated using known locations for objects like the snakeParts and fruit. Every frame a call is made to a function moveSnake() which shifts all snake parts to the position of its next neighbour. The collision detection is performed last when the head position is updated. ANSI escape seqences allow for easy cursor position changes which eliminates the need to reprint the whole image space. 

## Currently in the works
The current tasks that are planned to implemented are as follows:
* cross compiling to windows and linux distrbutions
* additional check on keypress to prevent backwards movement
* Add speed adjust on y axis to match the difference in aspect ratio
* score system
* global score system using MQTT protocol
* event handle on terminal resize to adjust game area dimensions and title
* game launch menu UI to allow for preferance configuration
* colours for fruits and snakes* JSON format parser to use for mqtt messaging and game configuration save file
* JSON format parser to use for mqtt messaging and game configuration save file

# Useful links
* [ANSI ESCAPE CODES](https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797)
* [TEXT TO ASCII ART](https://patorjk.com/software/taag/#p=display&f=Graffiti&t=Type%20Something%20)
* [MQTT V3.1.1 RFC document](https://docs.oasis-open.org/mqtt/mqtt/v3.1.1/os/mqtt-v3.1.1-os.html#_Toc398718009)
