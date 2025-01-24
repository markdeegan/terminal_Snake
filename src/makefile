CXX		 = gcc
CXXFLAGS = -iquote ../include
BINLIB = ../bin
LOCALLIB = ../lib
LOCALBIN = ../bin

#rule to make all executables
all: cornersExample terminalControl snake_helpers rawModeExample cursorReadingTutorial asciiArtTitle snake 

#removes all executables in the binary folder 
#removing echoing and ignoring error due to possibility of directory being empty
clean: 
	@-rm ../bin/*.exe 2> /dev/null || true
	@-rm ../bin/*.o 2> /dev/null || true
	@-rm $(LOCALLIB)/*.o 2> /dev/null || true

#rule to make the snake helpers static library
snake_helpers: snake_helpers.c
	$(CXX) $(CXXFLAGS) -c $@.c -o $(LOCALLIB)/$@.o

#rule to make terminalControl static library
terminalControl: terminalControl.c
	$(CXX) $(CXXFLAGS) -c -o $(LOCALLIB)/terminalControl.o terminalControl.c

#rule to make the corners example
cornersExample: cornersExample.c
	$(CXX) $(CXXFLAGS) cornersExample.c -o ../bin/cornersExample.exe

#rule to make the example raw mode program
rawModeExample: terminalControl rawModeExample.c
	$(CXX) $(CXXFLAGS) $(LOCALLIB)/terminalControl.o rawModeExample.c -o ../bin/rawModeExample.exe

#rule to make example program for cursor position reporting
cursorReadingTutorial: terminalControl cursorReadingTutorial.c
	$(CXX) $(CXXFLAGS) $(LOCALLIB)/terminalControl.o cursorReadingTutorial.c -o $(LOCALBIN)/cursorReadingTutorial.exe

#rule to make asciiArtTitle
asciiArtTitle: terminalControl asciiArtTitle.c
	$(CXX) $(CXXFLAGS) $(LOCALLIB)/terminalControl.o asciiArtTitle.c -o ../bin/asciiArtTitle.exe

#rule to make the snake executable
snake: terminalControl snake_helpers snake.c
	$(CXX) $(CXXFLAGS) $(LOCALLIB)/terminalControl.o $(LOCALLIB)/snake_helpers.o snake.c -o ../bin/snake.exe

