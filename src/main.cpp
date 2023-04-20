#include <iostream>
#include "Graph.hpp"

int main(int argc, char *argv[]) 
{
    Graph g("./tests/graph-test-8.txt");
    std::cout << g << g.getM() << "\n";
    return 0;
}