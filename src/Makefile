# Compiler
CC = g++

# Compiler flags
CFLAGS = -W -Wall -Werror -Wextra -pedantic -std=c++17

# Paths
INC_PATH = .

# Source files
SRC = Graph.cpp main.cpp

# Object files
OBJ = $(SRC:.cpp=.o)

# Output Binary
BIN = run

# Default target
all: $(OBJ)
	$(CC) $(CFLAGS) -I$(INC_PATH) -o $(BIN) $(OBJ)

# Clean rule
clean:
	rm -f $(OBJ)
