#include "Graph.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <deque>
#include <climits>

bool operator==(const edge &e1, const edge &e2)
{
    return e1.w == e2.w && e1.weight == e2.weight;
}

void Graph::addEdge(int v, int w, int weight)
{
    edge e1 = {w, weight};
    edge e2 = {v, weight};
    auto vEdge = std::find_if(adjList[v].begin(), adjList[v].end(), [e1](const edge &e)
                              { return e1.w == e.w; });
    auto wEdge = std::find_if(adjList[w].begin(), adjList[w].end(), [e2](const edge &e)
                              { return e2.w == e.w; });
    if (vEdge == adjList[v].end() && wEdge == adjList[w].end())
    {
        adjList[v].push_back(e1);
        adjList[w].push_back(e2);
    }
    else
    {
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
    while (!ifs.eof())
    {
        ifs >> v >> w >> weight;
        addEdge(v, w, weight);
    }
    ifs.close();
}
Graph::~Graph()
{
}

int Graph::getK()
{
    return k;
}

void Graph::print()
{
    std::cout << "|V|: " << n - 1 << ", |E|: " << m << "\nEdge list:\n";
    for (int v = start; v < n; v++) {
        for (const auto& e : adjList[v]) {
            if (v < e.w)
                std::cout << "{" << v << ", " << e.w << "} weight: " << e.weight << "\n";
        }
    }
}

void Graph::dijkstra(int root, std::vector<int> &dist)
{
    std::deque<int> heap;
    auto heap_comp = [&dist](const int v, const int w)
    {
        return dist[v] > dist[w];
    };
    for (int i = start; i < n; i++)
    {
        heap.push_back(i);
    }
    dist[root] = 0;
    std::make_heap(heap.begin(), heap.end(), heap_comp);
    while (heap.size())
    {
        int v = heap.front();
        heap.pop_front();
        for (const auto &e : adjList[v]) {
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
    for (int v = start; v < n; v++)
    {
        aux = dists[centers[0]][v];
        for (int i = 1; i < centers.size(); i++)
        {
            if (dists[centers[i]][v] < aux)
            {
                aux = dists[centers[i]][v];
            }
        }
        if (radius < aux)
        {
            radius = aux;
        }
    }
    return radius;
}

void Graph::tryCombinations(std::vector<std::vector<int>> &dists, int i, 
std::vector<int> &curr, std::vector<int> &ans, int &radius)
{
    if (curr.size() == k)
    {
        int tmp = getRadius(dists, curr);
        if (radius > tmp)
        {
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
    std::vector<std::vector<int>> dists(n, std::vector<int>(n, INT_MAX));
    std::vector<int> curr;
    KcInfo kci = {INT_MAX, std::vector<int>()};
    for (int v = start; v < n; v++)
    {
        dijkstra(v, dists[v]);
    }
    tryCombinations(dists, 0, curr, kci.centers, kci.radius);
    return kci;
}

int Graph::getFirstCenter(std::vector<std::vector<int>> &dists)
{
    std::vector<int> furthest(n, INT_MAX);
    int first = 0, aux = INT_MAX;
    for (int v = start; v < n; v++) {
        furthest[v] = *std::max_element(dists[v].begin() + 1, dists[v].end());
        if (furthest[v] < aux || (furthest[v] == aux && adjList[v].size() > adjList[first].size())) {
            first = v;
            aux = furthest[v];
        }
    }
    return first;
}

int Graph::maxIndex(std::vector<int>& dist)
{
    int mi = start;
    for (int i = start + 1; i < n; i++)
    {
        if (dist[i] > dist[mi])
            mi = i;
    }
    return mi;
}

KcInfo Graph::farthestFirstTraversalGreedy()
{
    std::vector<std::vector<int>> dists(n, std::vector<int>(n, INT_MAX));
    for (int v = start; v < n; v++)
    {
        dijkstra(v, dists[v]);
    }
    std::vector<int> dist(n, INT_MAX);
    std::vector<int> centers;
    KcInfo kci = {INT_MAX, std::vector<int>()};
    int max = getFirstCenter(dists);
    for (int i = 0; i < k; i++)
    {
        centers.push_back(max);
        dist[max] = 0;
        for (int j = start; j < n; j++)
        {
            dist[j] = std::min(dist[j], dists[max][j]);
        }
        max = maxIndex(dist);
    }
    kci.radius = dist[max];
    kci.centers = centers;
    return kci;
}

int Graph::tryCenters(std::vector<int>& centers, std::vector<std::vector<int>>& dists)
{
    for (int i = 0; i < centers.size(); i++) {
        dijkstra(centers[i], dists[centers[i]]);
    }
    KcInfo kci = {-1, centers};
    return kci.radius;
}

KcInfo Graph::greedyPureKCenter()
{
    std::vector<int> centers;
    std::vector<std::vector<int>> dists(n, std::vector<int>(n,  INT_MAX));
    int min = INT_MAX, curr = 0, center = 0;
    for (int v = start; v < n; v++) {
        dijkstra(v, dists[v]);
    }
    for (int i = 0; i < k; i++) {
        min = INT_MAX; 
        for (int j = start; j < n; j++) {
            if (!std::count(centers.begin(), centers.end(), j)) {
                centers.push_back(j);
                curr = getRadius(dists, centers);
                centers.pop_back();
                if (curr < min) {
                    center = j;
                    min = curr;
                }
            }
        }
        centers.push_back(center);
        center = 0;
    }
    return {min, centers};
}