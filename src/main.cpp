#include <iostream>
#include <cstdlib>
#include "Graph.hpp"

// Test to generate and print a graph with the given parameters.
int main(int argc, char *argv[]) 
{
    if (argc != 4) return 1;
    Graph g(atoi(argv[1]), atoi(argv[2]),  atoi(argv[3]));
    std::cout << "m = " << g.getM() << "\n" << g;
    return 0;
}