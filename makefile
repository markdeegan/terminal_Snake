CXX		 = gcc
CXXFLAGS = -iquote ./include

#rule to make all executables
all: terminalControl snake_helpers rawModeExample cursorReadingTutorial asciiArtTitle snake 

#removes all executables in the binary folder 
clean: 
	@-rm ./bin/*.exe 2> /dev/null || true
	@-rm ./bin/*.o 2> /dev/null || true

#rule to make the snake helpers static library
snake_helpers: snake_helpers.c
	$(CXX) $(CXXFLAGS) -c -o ./bin/snake_helpers.o snake_helpers.c

#rule to make terminalControl static library
terminalControl: terminalControl.c
	$(CXX) $(CXXFLAGS) -c -o ./bin/terminalControl.o terminalControl.c

#rule to make the example raw mode program
rawModeExample: terminalControl rawModeExample.c
	$(CXX) $(CXXFLAGS) ./bin/terminalControl.o rawModeExample.c -o ./bin/rawModeExample.exe

#rule to make example program for cursor position reporting
cursorReadingTutorial: terminalControl cursorReadingTutorial.c
	$(CXX) $(CXXFLAGS) ./bin/terminalControl.o cursorReadingTutorial.c -o ./bin/cursorReadingTutorial.exe

#rule to make asciiArtTitle
asciiArtTitle: terminalControl asciiArtTitle.c
	$(CXX) $(CXXFLAGS) ./bin/terminalControl.o asciiArtTitle.c -o ./bin/asciiArtTitle.exe

#rule to make the snake executable
snake: terminalControl snake_helpers snake.c
	$(CXX) $(CXXFLAGS) ./bin/terminalControl.o ./bin/snake_helpers.o snake.c -o ./bin/snake.exe

