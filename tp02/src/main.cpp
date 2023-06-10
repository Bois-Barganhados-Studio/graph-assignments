#include "Graph.hpp"


int main ()
{
    Graph g("./instances/test.txt");
    auto kci = g.exactKCenter();
    int nop = 0;
    return 0;
}