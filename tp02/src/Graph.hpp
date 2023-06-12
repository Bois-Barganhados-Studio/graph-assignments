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
    void print();
    void dijkstra(int root, std::vector<int> &dist);
    int getRadius(std::vector<std::vector<int>> &dists, std::vector<int> centers);
    void tryCombinations(std::vector<std::vector<int>> &dists, int i, std::vector<int> &curr, std::vector<int> &ans, int &radius);
    KcInfo exactKCenter();
    KcInfo tryCenters(std::vector<int>& centroids);
};