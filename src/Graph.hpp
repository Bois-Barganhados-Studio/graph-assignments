#include <vector>
#include <ostream>
#include <unordered_set>

/// @brief A Depth First Search table that contains the lifetime and parent of each vertex found.
struct dfsTable
{
    std::vector<int> discTime, 
    endTime,
    parent;
    dfsTable(size_t n);
};

/// @brief Simple non-directed graph adjacency list implementation.
class Graph
{
private:
    size_t start, n, m;
    std::vector<std::vector<int>> adjList;
    void genMinConnectedGraph();
    void populate(size_t target);
public:
    Graph(size_t start_, size_t n_, size_t m_);
    Graph(std::string filename);
    ~Graph();
    int getN();
    int getM(); 
    friend std::ostream &operator<<(std::ostream &os, Graph g);
    bool addEdge(int v, int w);
    void addVertex();
    void removeEdge(int v, int w);
    void removeVertex(int v);
    bool isConnected();
    void dump(std::string filename);
    dfsTable dfs(int root);
    bool bfs(int s, int t, std::vector<int>& level);
    bool findCycle(int s, int t, std::vector<int>& level, std::unordered_set<int>& cycle);
    std::vector<std::vector<int>> findBlocksByCycle();

    // bogo version
    std::vector<std::vector<int>> findBlocksByDisjointPaths();
    bool bogoTwoDisjointPaths(int s, int t, std::unordered_set<int>& share_block);
};
