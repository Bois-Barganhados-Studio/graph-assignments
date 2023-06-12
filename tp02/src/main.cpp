#include "Graph.hpp"
#include <iostream>
#include <iterator>
#include <chrono>

std::ostream &operator<<(std::ostream &os, std::vector<int> vec)
{
    os << "{ ";
    std::copy(vec.begin(), --vec.end(), std::ostream_iterator<int>(os, ", "));
    os << vec.back() << " }\n";
    return os;
}

std::ostream &operator<<(std::ostream &os, KcInfo kci)
{
    os << "radius: " << kci.radius << "\ncenters: " << kci.centers;
    return os;
}

int main ()
{
    Graph g("./instances/pmed1.txt");
    g.print();
    auto begin = std::chrono::high_resolution_clock::now();
    auto kci = g.exactKCenter();
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << kci << "time: " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "s\n";
    return 0;
}