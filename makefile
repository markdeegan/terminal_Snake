CXX		 = gcc
CXXFLAGS = -iquote ./include

#rule to make all executables
all: snake_helpers rawModeExample cursorReadingTutorial asciiArtTitle snake

#removes all executables in the binary folder 
clean: 
	@-rm ./bin/*.exe 2> /dev/null || true
	@-rm ./bin/*.o 2> /dev/null || true

#rule to make the snake helpers
snake_helpers: snake_helpers.c
	$(CXX) $(CXXFLAGS) -c -o ./bin/snake_helpers.o snake_helpers.c

#rule to make the example raw mode program
rawModeExample: rawModeExample.c
	$(CXX) $(CXXFLAGS) -o ./bin/rawModeExample.exe rawModeExample.c

#rule to make example program for cursor position reporting
cursorReadingTutorial: cursorReadingTutorial.c
	$(CXX) $(CXXFLAGS) -o ./bin/cursorReadingTutorial.exe cursorReadingTutorial.c

asciiArtTitle: asciiArtTitle.c
	$(CXX) $(CXXFLAGS) -o ./bin/asciiArtTitle.exe asciiArtTitle.c

#rule to make the snake executable
snake: snake_helpers snake.c
	$(CXX) $(CXXFLAGS) ./bin/snake_helpers.o snake.c -o ./bin/snake.exe

