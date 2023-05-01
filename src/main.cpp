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

int main(int argc, char *argv[])
{
    if (argc < 5)
        return 1;

    Graph g(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atof(argv[4]));

    auto begin = std::chrono::high_resolution_clock::now();

    // auto blocks = g.findBlocksByCycle();
    auto blocks = g.findBlocksByTarjan();
    // auto blocks = g.findBlocksByJoints();

    auto end = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < blocks.size(); i++)
    {
        std::cout << "Block " << i + 1 << ": " << blocks[i];
    }
    // std::ofstream out("./results/graph-test-100-joints.txt");
    // for (int i = 0; i < blocks.size(); i++)
    // {
    //     out << "Block " << i + 1 << ": " << blocks[i];
    // }

    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms\n";

    return 0;
}