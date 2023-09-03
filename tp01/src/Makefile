CXX=clang++
RELEASE_FLAGS=-std=c++17 -O2 -DNDEBUG
DEBUG_FLAGS=-std=c++17 -g -O0 -Wall -Wextra -Werror -fsanitize=address
SRCS=Graph.cpp main.cpp
OBJS=$(SRCS:.cpp=.o)
EXECUTABLE=graph.exe

all: release

release: CXXFLAGS=$(RELEASE_FLAGS)
release: $(EXECUTABLE)

debug: CXXFLAGS=$(DEBUG_FLAGS)
debug: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(EXECUTABLE)
	./$(EXECUTABLE)

clean:
	rm -f $(OBJS) $(EXECUTABLE)
