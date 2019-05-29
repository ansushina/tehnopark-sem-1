#include <iostream>

using namespace std;
#include <vector>
#include <queue>
#include <list>

class CListGraph
{
public:
    CListGraph(int n) : adjacency_lists(n) {}

    ~CListGraph() {}

    void AddEdge(int from, int to)
    {
        adjacency_lists[from].push_back(to);
        adjacency_lists[to].push_back(from);
    }

    int VerticesCount() const
    {
        return (int)adjacency_lists.size();
    }

    std::vector<int> GetNextVertices(int vertex) const
    {
        std::vector<int> next;
        for(int v: adjacency_lists[vertex])
        {
            next.push_back(v);
        }
        return next;
    }

    int CountShortWays(int from, int to);
private:
    std::vector< std::list<int> > adjacency_lists;
};


int CListGraph::CountShortWays(int from, int to)
{
    std::vector<int> ShortestWays(VerticesCount(), 0);
    std::vector<int> WaysLenght(VerticesCount(), 0);
    std::queue<int> queue;

    queue.push(from);
    ShortestWays[from] = 1;

    while(!queue.empty())
    {
        int v = queue.front();
        queue.pop();

        std::vector<int> next = GetNextVertices(v);
        for (int u: next)
        {
            if (ShortestWays[u] == 0)
            {
                queue.push(u);
                ShortestWays[u] = ShortestWays[v];
                WaysLenght[u] = WaysLenght[v] + 1;
            }
            else if (WaysLenght[u] == WaysLenght[v]+1)
            {
                ShortestWays[u] += ShortestWays[v];
            }
        }
    }
    return ShortestWays[to];

}

int main()
{
    int countPoints,countPairs,from,to, mainfrom, mainto;
    std::cin >> countPoints;
    std::cin >> countPairs;
    CListGraph graph(countPoints);
    for (int i = 0; i < countPairs; i++)
    {
        std::cin >> from;
        std::cin >> to;
        graph.AddEdge(from, to);
    }
    std::cin >> mainfrom;
    std::cin >> mainto;
    std::cout<<graph.CountShortWays(mainfrom, mainto)<<std::endl;

    return 0;
}
