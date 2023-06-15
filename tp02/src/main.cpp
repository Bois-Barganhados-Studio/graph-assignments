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

// Params:
// - The number of the instance to be tested e.g. 1 for pmed1.txt
// - The algorithm to be tested (1 - Exact(Brute Force), 2 - Farthest First Traversal Greedy, 3 - Greedy Pure Algorithm) 
int main (int argc, char *argv[])
{
    if (argc < 3)
        return EXIT_FAILURE;
    int i = std::atoi(argv[1]);
    if (i < 1 || i > 40) {
        std::cerr << "first arg must be a number between 1 and 40\n";
        return EXIT_FAILURE;
    }
    std::string fname = "pmed";
    fname.append(std::string(argv[1]) + ".txt");
    Graph g("./instances/" + fname);
    KcInfo kci;
    auto begin = std::chrono::high_resolution_clock::now();
    if (argv[2][0] == '1') {
        kci = g.exactKCenter();
        std::cout << "\ninstance: " << fname << "\nalgorithm: brute force\n";
    } else if (argv[2][0] == '2') {
        kci = g.farthestFirstTraversalGreedy();
        std::cout << "\ninstance: " << fname << "\nalgorithm: farthest first traversal greedy\n";
    } else if (argv[2][0] == '3') {
        kci = g.greedyPureKCenter();
        std::cout << "\ninstance: " << fname << "\nalgorithm: greedy pure\n";
    } else {
        std::cerr << "second arg must be either 1, 2 or 3\n";
        return EXIT_FAILURE;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "k: " << g.getK() << "\n" << kci << "time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms\n";
    return EXIT_SUCCESS;
}