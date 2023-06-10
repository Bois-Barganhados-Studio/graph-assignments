#include "Graph.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <deque>

bool operator==(const edge& e1, const edge& e2)
{
    return e1.w == e2.w && e1.weight == e2.weight;
}

void Graph::addEdge(int v, int w, int weight)
{
    edge e1 = { w, weight };
    edge e2 = { v, weight };
    auto vEdge = std::find_if(adjList[v].begin(), adjList[v].end(), [e1](const edge& e){
        return e1.w == e.w;
    });
    auto wEdge = std::find_if(adjList[w].begin(), adjList[w].end(), [e2](const edge& e){
        return e2.w == e.w;
    });
    if (vEdge == adjList[v].end() && wEdge == adjList[w].end()) {
        adjList[v].push_back(e1);
        adjList[w].push_back(e2);
    } else {
        *vEdge = e1;
        *wEdge = e2;
    }
}

Graph::Graph(std::string filename)
    : start(1), n(0), m(0), k(0), adjList() 
{
    std::ifstream ifs;
    int v = 0, w = 0, weight = 0;
    ifs.open(filename);
    ifs >> n >> m >> k;
    adjList = std::vector<std::vector<edge>>(++n);
    while (!ifs.eof()) {
        ifs >> v >> w >> weight;
        addEdge(v, w, weight);
    }
    ifs.close();
}
Graph::~Graph()
{
}

void Graph::print()
{
    for (int v = start; v < n; v++) {
        for (edge e : adjList[v]) {
            std::cout << v << " - " << e.w << ": " << e.weight << "\n"; 
        }
    }
}

void Graph::dijkstra(int root, std::vector<int> &dist)
{
    std::deque<int> heap;
    auto heap_comp = [&dist](const int v, const int w) {
        return dist[v] > dist[w];
    };
    for (int i = start; i < n; i++) {
        heap.push_back(i);
    }
    dist[root] = 0;
    std::make_heap(heap.begin(), heap.end(), heap_comp);
    while (heap.size()) {
        int v = heap.front();
        heap.pop_front();
        for (auto &e : adjList[v]) {
            if (dist[v] < dist[e.w] - e.weight) {
                dist[e.w] = dist[v] + e.weight;
            }
        }
        std::make_heap(heap.begin(), heap.end(), heap_comp);
    }
}

int Graph::getRadius(std::vector<std::vector<int>> &dists, std::vector<int> centers)
{
    int radius = 0, aux = INT_MAX;
    for (int v = start; v < n; v++) {
        aux = dists[centers[0]][v];
        for (int i = 1; i < centers.size(); i++) {
            if (dists[centers[i]][v] < aux) {
                aux = dists[centers[i]][v];
            }
        }
        if (radius < aux) {
            radius = aux;
        }
    }
    return radius;
}

void Graph::tryCombinations(std::vector<std::vector<int>> &dists, int i, std::vector<int> &curr, std::vector<int> &ans, int &radius) {
    if (curr.size() == k) {
        int tmp = getRadius(dists, curr);
        if (radius > tmp) {
            radius = tmp;
            ans = curr;
        }
        return;
    }

    if (i >= n)
        return;

    // Include the current element and recurse
    curr.push_back(i);
    tryCombinations(dists, i + 1, curr, ans, radius);

    // Exclude the current element and recurse
    curr.pop_back();
    tryCombinations(dists, i + 1, curr, ans, radius);
}

KcInfo Graph::exactKCenter()
{
    std::vector<std::vector<int>> dists(n, std::vector<int>(n,  INT_MAX));
    std::vector<int> curr;
    KcInfo kci = {INT_MAX, std::vector<int>()};
    for (int v = start; v < n; v++) {
        dijkstra(v, dists[v]);
    }
    tryCombinations(dists, 0, curr, kci.centers, kci.radius);
    return kci;
}