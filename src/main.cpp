#include <iostream>
#include "Graph.hpp"

int main(int argc, char *argv[]) 
{
    Graph g("./tests/graph-test-11.txt");
    g.findBlocksByDisjointPaths();
    return 0;
}