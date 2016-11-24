# The compiler we use to compile our cpp files
CC= g++

# Additional include directories
INC= -Iinclude/

# We wish to compile all .cpp files located in src/
SRC= $(wildcard src/*.cpp)

# Linker options
LINK= -lSDL2main -lSDL2 -lGL  -lIL -lILUT -lILU

# Place the output file in bin/
OUT= bin/boulder.out

# Additional flags passed to the compiler
FLAGS= -std=gnu++11

default: target

target:
	$(CC) $(INC) $(SRC) $(LINK) $(FLAGS) -o $(OUT)

clean:
	rm $(OUT)
