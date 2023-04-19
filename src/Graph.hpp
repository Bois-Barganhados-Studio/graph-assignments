#include <vector>
//#include <unordered_set>
#include <ostream>

#define DEFAULT_DENSITY 30

/// @brief Simple non-directed graph adjacency list implementation
class Graph
{
private:
    int start, n, m;
    std::vector<std::vector<int>> adjList;
    void genMinConnectedGraph();
public:
    Graph(int start_, int n_, int density);
    Graph(std::string filename);
    ~Graph();
    int getN();
    int getM(); 
    void addEdge(int v, int w);
    void addVertex();
    void removeEdge(int v, int w);
    void removeVertex(int v);
    bool isConnected();
    void dump(std::string filename);
    friend std::ostream &operator<<(std::ostream &os, Graph g);
};