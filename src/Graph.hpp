#include <vector>
//#include <unordered_set>
#include <ostream>

/// @brief Simple non-directed graph adjacency list implementation
class Graph
{
private:
    int start, n, m;
    std::vector<std::vector<int>> adjList;
public:
    Graph(int start_, int n_, int density);
    ~Graph();
    int getN();
    int getM(); 
    void addEdge(int v, int w);
    void addVertex();
    void removeEdge(int v, int w);
    void removeVertex(int v);
    friend std::ostream &operator<<(std::ostream &os, Graph g);
};