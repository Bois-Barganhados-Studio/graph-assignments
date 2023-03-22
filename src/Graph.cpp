#include "Graph.hpp"
#include <stdlib.h> // srand, rand
#include <time.h> // time
#include <iterator>
#include <algorithm>

/// @brief Contructs and populates the graph based on the given criteria.
/// @param start_ Index to start using the vertex vector.
/// @param n_ Number of vertices in the graph.
/// @param density A value from 0 to 100 representing the density percentage of the graph.
Graph::Graph(int start_, int n_, int density)
    : start(start_), n(n_ + start_), m(0), adjList(n, std::vector<int>())
{
    if (density < 101) {
        srand(time(NULL));
        while (m < n - 1) {
            for (int v = start; v < n; v++) {
                for (int w = v + 1; w < n; w++) {
                    int vDensity = adjList[v].size() / ((n * n - 1) / 2) * 100;
                    if (vDensity >= density) w = n;
                    if ((rand() % 100) < density) {
                        addEdge(v, w);
                    }
                }
            }
        }
    }
}

Graph::~Graph()
{
}

int Graph::getN()
{
    return n;
}

int Graph::getM()
{
    return m;
}

void Graph::addEdge(int v, int w)
{
    if (v < n && w < n && !std::count(adjList[v].begin(), adjList[v].end(), w)) {
        adjList[v].push_back(w);
        adjList[w].push_back(v);
        m++;
    }
}

void Graph::addVertex()
{
    adjList.push_back(std::vector<int>());
    n++;
}

void Graph::removeEdge(int v, int w) {
    if (v < n && w < n) {
        adjList[v].erase(std::remove(adjList[v].begin(), adjList[v].end(), w), adjList[v].end());
        adjList[w].erase(std::remove(adjList[w].begin(), adjList[w].end(), v), adjList[w].end());
        m--;
    }
}

void Graph::removeVertex(int v) {
    if (v < 0) return;
    std::vector tmp = adjList[v];
    for (const auto& it : tmp) {
        removeEdge(v, it);
    }
}

std::ostream & operator<<(std::ostream & os, Graph g)
{
    for (int i = g.start; i < g.n; i++) {
        os << "[" << i << "] = { ";
        std::copy(g.adjList[i].begin(), --g.adjList[i].end(), std::ostream_iterator<int>(os, ", "));
        os << g.adjList[i].back() << " }\n";
    }
    return os;
}