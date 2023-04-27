#include <iostream>
#include <chrono>
#include <iterator>
#include "Graph.hpp"

std::ostream & operator<<(std::ostream & os, std::vector<int> vec)
{
    os << "{ ";
    std::copy(vec.begin(), --vec.end(), std::ostream_iterator<int>(os, ", "));
    os << vec.back() << " }\n";
    return os;
}

int main(int argc, char *argv[]) 
{
    if (argc < 3)
        return 1;
    Graph g(1, atoi(argv[1]), atoi(argv[2]));
    
    auto begin = std::chrono::high_resolution_clock::now();

    auto blocks = g.findBlocksByCycle();
    
    auto end = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < blocks.size(); i++) {
        std::cout << "Block " << i + 1 << ": " << blocks[i];
    }

    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms\n";
    
    return 0;
}