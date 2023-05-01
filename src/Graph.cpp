#include "Graph.hpp"
#include <cstdlib> // srand, rand
#include <ctime>   // time
#include <iterator>
#include <algorithm>
#include <fstream>
#include <deque>

Graph::Graph(size_t START, size_t N, size_t K, float bDensity)
    : start(START), n(N + START), m(0), adjList(n, std::vector<int>())
{
    srand(time(NULL));
    if (K > N)
        K = N - 1;
    int begin = start, end = start;
    for (int i = 0; i < K - 1; i++)
    {
        int offset = rand() % (N / (K - i)) + 2;
        end += offset;
        N -= offset;
        genMinConnectedGraph(begin, end);
        populate(begin, end, bDensity);
        begin = end - 1;
    }
    begin = end - 1;
    end += N;
    genMinConnectedGraph(begin, end);
    populate(begin, end, bDensity);
    for (auto &v : adjList)
        std::sort(v.begin(), v.end());
}

void inline Graph::genMinConnectedGraph(int begin, int end)
{
    std::vector<int> in, out;
    in.push_back(rand() % end + begin);
    for (int i = begin; i < end; i++)
        if (i != in[0])
            out.push_back(i);
    while (!out.empty())
    {
        int i = rand() % out.size(),
            j = rand() % in.size();
        addEdge(in[j], out[i]);
        in.push_back(out[i]);
        out.erase(out.begin() + i);
    }
}

void inline Graph::populate(int begin, int end, float density)
{
    for (int v = begin; v < end - 1; v++) {
        for(int w = v + 1; w < end; w++) {
            if (rand() % 100 < density) {
                addEdge(v, w);
            }
        }
    }
}

Graph::Graph(std::string filename)
    : start(0), n(0), m(0)
{
    std::ifstream ifs;
    int v = 0, w = 0;
    ifs.open(filename);
    ifs >> start >> n;
    adjList = std::vector<std::vector<int>>(n, std::vector<int>());
    while (ifs >> v >> w)
    {
        addEdge(v, w);
    }
    ifs.close();
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

/// @brief Ostream operator overload to allow printing the graph with std::cout
std::ostream &operator<<(std::ostream &os, Graph g)
{
    os << "start: " << g.start << ", n: " << g.n << ", m: " << g.m << ";\n";
    for (int i = g.start; i < g.n; i++)
    {
        os << "[" << i << "] = { ";
        std::copy(g.adjList[i].begin(), --g.adjList[i].end(), std::ostream_iterator<int>(os, ", "));
        os << g.adjList[i].back() << " }\n";
    }
    return os;
}

bool Graph::addEdge(int v, int w)
{
    bool success = false;
    if (v != w && v < n && w < n && !std::count(adjList[v].begin(), adjList[v].end(), w))
    {
        adjList[v].push_back(w);
        adjList[w].push_back(v);
        m++;
        success = true;
    }
    return success;
}

void Graph::addVertex()
{
    adjList.push_back(std::vector<int>());
    n++;
}

void Graph::removeEdge(int v, int w)
{
    if (v < start || w < start || v >= n || w >= n)
        return;
    if ((adjList[v].erase(std::remove(adjList[v].begin(), adjList[v].end(), w), adjList[v].end()) != adjList[v].end())
     && adjList[w].erase(std::remove(adjList[w].begin(), adjList[w].end(), v), adjList[w].end()) != adjList[v].end())
    {
        m--;
    }
}

void Graph::removeVertex(int v)
{
    if (v < 0)
        return;
    std::vector<int> tmp = adjList[v];
    for (const auto &it : tmp)
    {
        removeEdge(v, it);
    }
    adjList[v].push_back(-1);
}

bool Graph::isConnected()
{
    std::vector<int> level(n, -1);
    bfs(start, level);
    return std::find(level.begin() + start, level.end(), -1) == level.end();
}

void Graph::dump(std::string filename)
{
    std::ofstream ofs;
    ofs.open(filename);
    ofs << start << " " << n << "\n";
    for (int v = start; v < n; v++)
    {
        for (int w : adjList[v])
        {
            if (w > v)
                ofs << v << " " << w << "\n";
        }
    }
    ofs.close();
}

/// @brief Constructs a Depth First Search table.
/// @param n The size of the table.
dfsTable::dfsTable(size_t n)
    : discTime(n, 0), endTime(n, 0), parent(n, 0)
{
}

dfsTable Graph::dfs(int root)
{
    dfsTable res(n);
    std::vector<int> clojure, visitCount(n, 0);
    size_t t = 0;
    clojure.push_back(root);
    res.discTime[root] = ++t;
    while (!clojure.empty())
    {
        int v = clojure.back();
        if (visitCount[v] < adjList[v].size())
        {
            int w = adjList[v][visitCount[v]++];
            if (!res.discTime[w])
            {
                res.parent[w] = v;
                res.discTime[w] = ++t;
                clojure.push_back(w);
            }
        }
        else
        {
            res.endTime[v] = ++t;
            clojure.pop_back();
        }
    }
    return res;
}

void Graph::bfs(int s, std::vector<int> &level)
{
    level = std::vector<int>(n, -1);
    std::deque<int> q;
    q.push_back(s);
    level[s] = 0;
    while (!q.empty())
    {
        int v = q.back();
        q.pop_back();
        for (int w : adjList[v])
        {
            if (level[w] == -1)
            {
                level[w] = level[v] + 1;
                q.push_front(w);
            }
        }
    }
}

std::vector<std::vector<int>> Graph::findBlocksByCycle()
{
    std::vector<std::vector<int>> blocks;
    std::vector<std::unordered_set<int>> cycles;
    std::vector<int> level;
    for (int v = start; v < n; v++)
    {
        std::vector<std::unordered_set<int>> new_cycles;
        bfs(v, level);
        findCycles(v, level, new_cycles);
        bool anexed = false;
        for (auto &tmp : new_cycles)
        {
            for (auto &i : cycles)
            {
                int count = 0;
                for (auto j = tmp.begin(); j != tmp.end(); j++)
                {
                    if (i.find(*j) != i.end())
                    {
                        count++;
                    }
                    if (count == 2)
                        break;
                }
                if (count == 2)
                {
                    for (auto j = tmp.begin(); j != tmp.end(); j++)
                    {
                        i.insert(*j);
                    }
                    anexed = true;
                    break;
                }
            }
            if (!anexed)
            {
                cycles.push_back(tmp);
            }
        }
    }
    for (auto &it : cycles)
    {
        if (!it.empty())
        {
            std::vector<int> tmp(it.begin(), it.end());
            std::sort(tmp.begin(), tmp.end());
            blocks.emplace_back(tmp);
            it.clear();
        }
    }
    return blocks;
}

void Graph::findCycles(int s, std::vector<int> &level,
                       std::vector<std::unordered_set<int>> &cycles)
{
    std::vector<int> clojure, visitCount(n, 0);
    std::vector<bool> visited(n, false);
    clojure.push_back(s);
    visited[s] = true;
    while (!clojure.empty())
    {
        int v = clojure.back();
        if (visitCount[v] == adjList[v].size())
        {
            clojure.pop_back();
        }
        else
        {
            int w = adjList[v][visitCount[v]++];
            if (w == s)
            {
                std::unordered_set<int> tmp(clojure.begin(), clojure.end());
                cycles.push_back(tmp);
            }
            else if (!visited[w])
            {
                std::sort(adjList[w].begin(), adjList[w].end(), [level](int a, int b)
                          { return level[a] > level[b]; });
                visited[w] = true;
                clojure.push_back(w);
            }
        }
    }
}

std::vector<std::vector<int>> Graph::findBlocksByDisjointPaths()
{
    std::vector<std::vector<int>> blocks;
    std::vector<std::unordered_set<int>> shared_blocks(n);
    for (int v = start; v < n; v++)
    {
        for (int w = v + 1; w < n; w++)
        {
            if (shared_blocks[v].find(w) == shared_blocks[v].end())
            {
                findTwoDisjointPaths(v, w, shared_blocks[v]);
            }
            if (v != w && shared_blocks[v].find(w) != shared_blocks[v].end())
            {
                shared_blocks[w].insert(v);
            }
        }
    }
    for (int v = start; v < n; v++)
    {
        auto &set = shared_blocks[v];
        if (set.empty())
            continue;
        set.insert(v);
        for (auto i = set.begin(); i != set.end(); i++)
        {
            if (*i == v)
                continue;
            auto &set_i = shared_blocks[*i];
            for (auto j = set_i.begin(); j != set_i.end();)
            {
                if (set.find(*j) != set.end())
                {
                    set_i.erase(j++);
                }
                else
                {
                    ++j;
                }
            }
        }
    }
    for (auto &it : shared_blocks)
    {
        if (!it.empty())
        {
            std::vector<int> tmp(it.begin(), it.end());
            std::sort(tmp.begin(), tmp.end());
            blocks.emplace_back(tmp);
            it.clear();
        }
    }
    return blocks;
}

bool Graph::findTwoDisjointPaths(int s, int t, std::unordered_set<int> &share_block)
{
    if (std::find(adjList[s].begin(), adjList[s].end(), t) != adjList[s].end())
    {
        share_block.insert(t);
        return true;
    }
    std::vector<bool> stacked(n, false);
    std::vector<std::unordered_set<int>> paths;
    std::vector<int> clojure, visitCount(n, 0);
    clojure.push_back(s);
    stacked[s] = true;
    while (!clojure.empty())
    {
        int v = clojure.back();
        if (visitCount[v] == adjList[v].size())
        {
            stacked[v] = false;
            visitCount[v] = 0;
            clojure.pop_back();
        }
        else
        {
            while (visitCount[v] < adjList[v].size())
            {
                int w = adjList[v][visitCount[v]++];
                if (w == t)
                {
                    std::unordered_set<int> tmp;
                    for (int i = 1; i < clojure.size(); i++)
                        tmp.insert(clojure[i]);
                    paths.emplace_back(tmp);
                }
                else if (!stacked[w])
                {
                    clojure.push_back(w);
                    stacked[w] = true;
                    break;
                }
            }
        }
    }
    clojure.clear();
    stacked.clear();
    visitCount.clear();
    bool match = false;
    for (int i = 0; i < paths.size(); i++)
    {
        for (int j = i + 1; j < paths.size(); j++)
        {
            int min = i, max = j;
            if (paths[i].size() > paths[j].size())
            {
                max = i;
                min = j;
            }
            for (int it : paths[min])
            {
                match = paths[max].find(it) != paths[max].end();
                if (match)
                {
                    break;
                }
            }
            if (!match)
            {
                for (int it : paths[i])
                    share_block.insert(it);
                for (int it : paths[i])
                    share_block.insert(it);
                share_block.insert(t);
                return true;
            }
        }
    }
    return false;
}

struct hash_pair
{
    size_t operator()(const std::pair<int,
                                      int> &x) const
    {
        return x.first ^ x.second;
    }
};

std::vector<std::vector<std::pair<int, int>>> Graph::findBlocksByTarjan()
{
    std::vector<int> stack, low(n, 0), disc_time(n, 0), children(n, 0), parent(n, 0), visit_count(n, 0);
    std::vector<std::pair<int, int>> edges;
    std::vector<std::vector<std::pair<int, int>>> blocks;
    std::unordered_set<std::pair<int, int>, hash_pair> block;
    int v = 0, w = 0, u = 0;
    size_t t = 0;

    stack.push_back(start);
    disc_time[start] = low[start] = ++t;

    while (!stack.empty())
    {
        v = stack.back();
        if (visit_count[v] < adjList[v].size())
        {
            w = adjList[v][visit_count[v]++];
            if (!disc_time[w])
            {
                stack.push_back(w);
                edges.emplace_back(v, w);
                parent[w] = v;
                children[v]++;
                disc_time[w] = low[w] = ++t;
            }
            else if (w != parent[v])
            {
                low[v] = std::min(low[v], disc_time[w]);
                if (disc_time[w] < disc_time[v])
                {
                    edges.emplace_back(v, w);
                }
            }
        }
        else
        {
            stack.pop_back();
            if (!stack.size())
                continue;
            u = stack.back();
            low[u] = std::min(low[u], low[v]);

            if ((disc_time[u] == 1 && children[u] > 1) || (disc_time[u] > 1 && low[v] >= disc_time[u]))
            {
                while (edges.back() != std::make_pair(u, v))
                {
                    block.insert(edges.back());
                    edges.pop_back();
                }
                block.insert(edges.back());
                edges.pop_back();

                std::vector<std::pair<int, int>> tmp(block.begin(), block.end());
                std::sort(tmp.begin(), tmp.end());
                blocks.push_back(tmp);
                block.clear();
            }
        }
    }

    while (!edges.empty())
    {
        block.insert(edges.back());
        edges.pop_back();
    }
    if (!block.empty())
    {
        std::vector<std::pair<int, int>> tmp(block.begin(), block.end());
        std::sort(tmp.begin(), tmp.end());
        blocks.push_back(tmp);
    }

    return blocks;
}

bool Graph::isJoint(int s)
{
    std::vector<int> stack, visit_count(n, 0);
    std::vector<bool> visited(n, false);
    int root = s == start ? start + 1 : start, v = 0, w = 0;

    stack.push_back(root);
    visited[root] = true;

    while (!stack.empty())
    {
        v = stack.back();
        if (visit_count[v] < adjList[v].size())
        {
            w = adjList[v][visit_count[v]++];
            if (w != s && !visited[w])
            {
                stack.push_back(w);
                visited[w] = true;
            }
        }
        else
        {
            stack.pop_back();
        }
    }

    return std::count(visited.begin(), visited.end(), false) > start + 1;
}

void Graph::jointDfs(int s, const std::unordered_set<int> &joints, std::vector<std::unordered_set<int>> &blocks, std::vector<int> &level)
{
    std::vector<int> stack, visit_count(n, 0);
    std::vector<bool> visited(n, false);
    int v = 0, w = 0;

    stack.push_back(s);
    visited[s] = true;
    std::sort(adjList[s].begin(), adjList[s].end(), [&](int a, int b)
              { return level[a] > level[b]; });

    while (!stack.empty())
    {
        v = stack.back();
        if (visit_count[v] < adjList[v].size())
        {
            w = adjList[v][visit_count[v]++];
            if (joints.find(w) != joints.end())
            {
                std::unordered_set<int> block(stack.begin(), stack.end());

                block.insert(w);
                blocks.push_back(block);
            }
            else if (w != s && !visited[w])
            {
                stack.push_back(w);
                std::sort(adjList[w].begin(), adjList[w].end(), [&](int a, int b)
                          { return level[a] > level[b]; });
                visited[w] = true;
            }
        }
        else
        {
            stack.pop_back();
        }
    }
}

std::vector<std::vector<int>> Graph::findBlocksByJoints()
{
    std::vector<int> stack, level(n, -1);
    std::unordered_set<int> joints;
    std::vector<std::vector<int>> res;
    std::vector<std::unordered_set<int>> blocks, new_blocks;
    bool anexed = false;

    for (int i = start; i < n; i++)
    {
        if (isJoint(i))
            joints.insert(i);
    }

    for (auto it : joints)
    {
        bfs(it, level);
        jointDfs(it, joints, new_blocks, level);
        for (auto &tmp : new_blocks)
        {
            anexed = false;
            for (auto &i : blocks)
            {
                int count = 0;
                for (auto j = tmp.begin(); j != tmp.end(); j++)
                {
                    if (i.find(*j) != i.end())
                    {
                        count++;
                    }
                    if (count == 2)
                        break;
                }
                if (count == 2)
                {
                    for (auto j = tmp.begin(); j != tmp.end(); j++)
                    {
                        i.insert(*j);
                    }
                    anexed = true;
                    break;
                }
            }
            if (!anexed)
            {
                blocks.push_back(tmp);
            }
        }
        new_blocks.clear();
    }

    for (auto &it : blocks)
    {
        if (!it.empty())
        {
            std::vector<int> tmp(it.begin(), it.end());
            std::sort(tmp.begin(), tmp.end());
            res.emplace_back(tmp);
            it.clear();
        }
    }

    return res;
}