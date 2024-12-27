#rule to make all executables
all: snake rawModeExample cursorReadingTutorial asciiArtTitle

#removes all executables in the binary folder 
clean: 
	rm ./bin/*.exe

#rule to make the snake executable
snake: snake.c
	gcc -o ./bin/snake.exe snake.c

#rule to make the example raw mode program
rawModeExample: rawModeExample.c
	gcc -o ./bin/rawModeExample.exe rawModeExample.c

#rule to make example program for cursor position reporting
cursorReadingTutorial: cursorReadingTutorial.c
	gcc -o ./bin/cursorReadingTutorial.exe cursorReadingTutorial.c

asciiArtTitle: asciiArtTitle.c
	gcc -o ./bin/asciiArtTitle.exe asciiArtTitle.c
