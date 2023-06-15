#pragma once

#include <vector>
#include <string>

typedef struct edge
{
    int w;
    int weight;
} edge;

typedef struct KcInfo
{
    int radius;
    std::vector<int> centers;
} KcInfo;

/// @brief Simple non-directed graph adjacency list implementation.
class Graph
{
private:
    std::vector<std::vector<edge>> adjList;
    int start, n, m, k;
    void addEdge(int v, int w, int weight);
public:
    Graph(std::string filename);
    ~Graph();
    int getK();
    void print();
    void dijkstra(int root, std::vector<int> &dist);
    int getRadius(std::vector<std::vector<int>> &dists, std::vector<int> centers);
    void tryCombinations(std::vector<std::vector<int>> &dists, int i, std::vector<int> &curr, std::vector<int> &ans, int &radius);
    int maxIndex(std::vector<int>& dist);
    int tryCenters(std::vector<int>& centroids, std::vector<std::vector<int>>& dists);
    int getFirstCenter(std::vector<std::vector<int>> &dists);

    /// @brief Tries every combination of k center vertices to find the best one.
    KcInfo exactKCenter();

    /// @brief Greedy algorithm that starts with arbitrary first vertex as center (the k = 1 solution was chosen)
    /// then each iteration picks the vertex farthest away from the center as the vertex in the center.
    KcInfo farthestFirstTraversalGreedy();

    /// @brief Classic greedy algorithm takes best local option, starts with best solution for k = 1, 
    /// then for the remaining k - 1 picks the vertex with smallest radius in combination with the already picked vertices. 
    KcInfo greedyPureKCenter();
};