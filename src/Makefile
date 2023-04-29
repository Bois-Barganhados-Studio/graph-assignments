# Compiler
CC = g++

# Debug flags
DBFLAGS = -fsanitize=address -W -Wall -Werror -Wextra -pedantic -std=c++17

# Release flags
RFLAGS = -O2 -std=c++17

# Paths
INC_PATH = .

# Source files
SRC = Graph.cpp main.cpp

# Object files
OBJ = $(SRC:.cpp=.o)

# Output Binary
BIN = run

# Debug build
debug: $(OBJ)
	$(CC) $(DBFLAGS) -I$(INC_PATH) -o $(BIN) $(OBJ)

# Release build
release: $(OBJ)
	$(CC) $(RFLAGS) -I$(INC_PATH) -o $(BIN) $(OBJ)

# Clean rule
clean:
	rm -f $(OBJ)
