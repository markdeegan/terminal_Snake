CXX		 = gcc
CXXFLAGS = -iquote ../include
LOCALLIB = ../lib
LOCALBIN = ../bin

############################################################################
#rule to make all executables
############################################################################
all: cornersExample terminalControl snake_helpers rawModeExample cursorReadingTutorial asciiArtTitle snake 

############################################################################
## rules to make libraries
############################################################################
#rule to make the snake helpers static library
snake_helpers: snake_helpers.c
	$(CXX) $(CXXFLAGS) -c -o $(LOCALLIB)/$@.o $@.c

#rule to make terminalControl static library
terminalControl: terminalControl.c
	$(CXX) $(CXXFLAGS) -c -o $(LOCALLIB)/$@.o $@.c

############################################################################
## rules to make executables
############################################################################
#rule to make the corners example executable
cornersExample: cornersExample.c
	$(CXX) $(CXXFLAGS) $@.c -o $(LOCALBIN)/$@.exe

#rule to make the example raw mode program
rawModeExample: terminalControl rawModeExample.c
	$(CXX) $(CXXFLAGS) $(LOCALLIB)/$<.o $@.c -o $(LOCALBIN)/$@.exe

#rule to make example program for cursor position reporting
cursorReadingTutorial: terminalControl cursorReadingTutorial.c
	$(CXX) $(CXXFLAGS) $(LOCALLIB)/$<.o $@.c -o $(LOCALBIN)/$@.exe

#rule to make asciiArtTitle
asciiArtTitle: terminalControl asciiArtTitle.c
	$(CXX) $(CXXFLAGS) $(LOCALLIB)/$<.o $@.c -o $(LOCALBIN)/$@.exe

#rule to make the snake executable
snake: terminalControl snake_helpers snake.c
	$(CXX) $(CXXFLAGS) $(LOCALLIB)/$<.o $(LOCALLIB)/snake_helpers.o $@.c -o $(LOCALBIN)/$@.exe

############################################################################
# rules for housekeeping
############################################################################
# removes all executables in the LOCALBIN folder 
# removes all .o files in the LOCALBIN folder 
# removes all .o files in the LOCALIB folder
clean: 
	@-rm $(LOCALBIN)/*.exe 2> /dev/null || true
	@-rm $(LOCALBIN)/*.o 2> /dev/null || true
	@-rm $(LOCALLIB)/*.o 2> /dev/null || true
