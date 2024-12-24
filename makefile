#rule to make all executables
all: snake rawModeExample

#removes all executables in the binary folder 
clean: 
	rm ./bin/*.exe

#rule to make the snake executable
snake: snake.c
	gcc -o ./bin/snake.exe snake.c

#rule to make the example raw mode program
rawModeExample: rawModeExample.c
	gcc -o ./bin/rawModeExample.exe rawModeExample.c	
