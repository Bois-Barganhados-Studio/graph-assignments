#include <vector>
#include <ostream>
#include <utility>
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
    void inline genMinConnectedGraph(int begin, int end);
    void inline populate(int begin, int end, size_t target);

public:
    Graph(size_t START, size_t N, size_t K, float bDensity);
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
    // Method I
    void bfs(int s, std::vector<int> &level);
    std::vector<std::vector<int>> findBlocksByCycle();
    void findCycles(int s, std::vector<int> &level, std::vector<std::unordered_set<int>> &cycles);
    // Method I - bogo version
    std::vector<std::vector<int>> findBlocksByDisjointPaths();
    bool bogoTwoDisjointPaths(int s, int t, std::unordered_set<int> &share_block);
    // Method II
    bool isJoint(int s);
    void jointDfs(int s, const std::unordered_set<int> &joints, std::vector<std::unordered_set<int>> &blocks, std::vector<int> &level);
    std::vector<std::vector<int>> findBlocksByJoints();
    /// @brief Method III - Tarjan iterative algorithm for finding biconnected components.
    /// @return A table containing the blocks of biconnected components of the graph.
    std::vector<std::vector<std::pair<int, int>>> findBlocksByTarjan();
};
