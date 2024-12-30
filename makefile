CXX		 = gcc
CXXFLAGS = -iquote ./include

#rule to make all executables
all: snake rawModeExample cursorReadingTutorial asciiArtTitle

#removes all executables in the binary folder 
clean: 
	@-rm ./bin/*.exe > /dev/null || true

#rule to make the snake executable
snake: snake.c
	$(CXX) $(CXXFLAGS) -o ./bin/snake.exe snake.c

#rule to make the example raw mode program
rawModeExample: rawModeExample.c
	$(CXX) $(CXXFLAGS) -o ./bin/rawModeExample.exe rawModeExample.c

#rule to make example program for cursor position reporting
cursorReadingTutorial: cursorReadingTutorial.c
	$(CXX) $(CXXFLAGS) -o ./bin/cursorReadingTutorial.exe cursorReadingTutorial.c

asciiArtTitle: asciiArtTitle.c
	$(CXX) $(CXXFLAGS) -o ./bin/asciiArtTitle.exe asciiArtTitle.c
