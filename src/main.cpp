#include <iostream>
#include <fstream>
#include <chrono>
#include <iterator>
#include <utility>
#include "Graph.hpp"

std::ostream &operator<<(std::ostream &os, std::vector<std::pair<int, int>> vec)
{
    os << "{";
    for (auto it = vec.begin(); it != vec.end(); it++)
    {
        os << "(" << it->first << " - " << it->second << ")";
        if (it != vec.end() - 1)
            os << ", ";
    }
    os << "}\n";
    return os;
}

std::ostream &operator<<(std::ostream &os, std::vector<int> vec)
{
    os << "{ ";
    std::copy(vec.begin(), --vec.end(), std::ostream_iterator<int>(os, ", "));
    os << vec.back() << " }\n";
    return os;
}

// Main args:
// 1 - starting number of the graph e.g. 1
// 2 - Number of vertices e.g. 100
// 3 - Number of initial sub-graphs (< |V|) e.g. 10
// 4 - Density percentage of edges in the sub-graphs (0 - 100) e.g. 50
// 5 - Desired method:
//     1 - disjoint paths in each vertex pair (too slow),
//     2 - joint identification,
//     3 - tarjan's method.
int main(int argc, char *argv[])
{
    if (argc < 6)
        return 1;
    Graph g(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
    int choosen_method = atoi(argv[5]);
    if (choosen_method == 1) {
        auto begin = std::chrono::high_resolution_clock::now();
        auto blocks = g.findBlocksByDisjointPaths();
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "us\n";
        for (int i = 0; i < blocks.size(); i++)
        {
            std::cout << "Block " << i + 1 << ": " << blocks[i];
        }
    } else if (choosen_method == 2) {
        auto begin = std::chrono::high_resolution_clock::now();
        auto blocks = g.findBlocksByJoints();
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "us\n";
        for (int i = 0; i < blocks.size(); i++)
        {
            std::cout << "Block " << i + 1 << ": " << blocks[i];
        }
    } else if (choosen_method == 3) {
        auto begin = std::chrono::high_resolution_clock::now();
        auto blocks = g.findBlocksByTarjan();
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "us\n";
        for (int i = 0; i < blocks.size(); i++)
        {
            std::cout << "Block " << i + 1 << ": " << blocks[i];
        }
    } else {
        std::cout << "Invalid method ID\n";
    }
    return 0;
}