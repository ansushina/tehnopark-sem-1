#include <iostream>

using namespace std;
#include <vector>
#include <queue>
#include <limits.h>
#include <set>


class CMatrixGraph
{
public:
    CMatrixGraph(int n) :
        matrix(n)
    {
        for (size_t i = 0; i < matrix.size(); i++)
        {
            matrix[i].resize(n);
            for (int j = 0; j < matrix.size(); j++)
            {
                matrix[i][j] = 0;
            }
        }
    }
    ~CMatrixGraph() {}

    // Добавление ребра от from к to.
    void AddEdge(int from, int to, int weight)
    {
        matrix[from][to] = weight;
        matrix[to][from] = weight;
    }

    int VerticesCount() const
    {
        return (int)matrix.size();
    }

    std::vector<int> GetNextVertices(int vertex) const
    {
        std::vector<int> next;
        for (size_t i = 0; i < matrix.size(); i++)
        {
            if (matrix[vertex][i])
                next.push_back(i);
        }
        return next;

    }
    std::vector<int> GetPrevVertices(int vertex) const
    {
        std::vector<int> prev;
        for (size_t i = 0; i < matrix.size(); i++)
        {
            if (matrix[i][vertex])
                prev.push_back(i);
        }
        return prev;
    }

    int w(int from, int to)
    {
        return matrix[from][to];
    }
private:
    std::vector<std::vector<int>> matrix;

};

bool Relax(CMatrixGraph &graph, std::vector<int> pi, std::vector<int> d, int u, int v)
{
    if (d[v] > d[u] + graph.w(u,v))
    {
        d[v] = d[u] +graph.w(u,v);
        pi[v] = u;
        return true;
    }
    return false;
}


int Dijkstra(CMatrixGraph &graph, int from, int to)
{
    std::vector<int> pi(graph.VerticesCount(), -1);
    std::vector<int> d(graph.VerticesCount(), INT_MAX);
    std::set<std::pair<int, int>> q;
    q.insert({from,0});
    d[from] = 0;
    while (!q.empty())
    {
        int u = q.begin()->first;
        q.erase(q.begin());
        std::vector<int> next = graph.GetNextVertices(u);
        for (int v: next)
        {
            if (d[v] > d[u]+graph.w(u,v))
            {
                if (d[v] != INT_MAX)
                {
                    q.erase({v,d[v]});
                }
                d[v] = d[u] + graph.w(u,v);
                pi[v] = u;
                q.insert({v, d[v]});
            }
            /*
            if (d[v] == INT_MAX)
            {
                d[v] = d[u] + graph.w(u,v);
                pi[v] = u;
                q.insert({v, d[v]});
            }
            else if (Relax(graph,pi,d,u,v))
            {
                q.erase({v,d[v]});
                */
        }
    }
    if (d[to] == INT_MAX)
        return -1;
    return d[to];
}


int main()
{
    int N, M;
    cin >> N >> M;
    CMatrixGraph graph(N);
    for (int i = 0; i < M; i++)
    {
        int from, to, weight;
        cin >>from>>to>>weight;
        graph.AddEdge(from,to,weight);
    }
    int from, to;
    cin >>from>>to;
    cout << Dijkstra(graph,from,to);
    return 0;
}
