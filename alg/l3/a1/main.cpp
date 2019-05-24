#include <iostream>
#include <vector>
#include <cassert>
#include <queue>
#include <stack>
#include <map>

struct IGraph {
    virtual ~IGraph() = 0;

    // Добавление ребра от from к to.
    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const  = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};

IGraph::~IGraph() {}

class CListGraph: public IGraph
{
public:
    CListGraph(int n) : adjacency_lists(n) {}
    CListGraph(const IGraph &graph)
    {
        adjacency_lists.resize(graph.VerticesCount());
        for (size_t i = 0; i < adjacency_lists.size(); i++)
        {
            adjacency_lists[i] = graph.GetNextVertices(i);
        }
    }
    ~CListGraph() {}

    void AddEdge(int from, int to) override
    {
        assert(0 <= from && from < adjacency_lists.size());
        assert(0 <= to && to < adjacency_lists.size());
        adjacency_lists[from].push_back(to);
    }

    int VerticesCount() const override
    {
        return (int)adjacency_lists.size();
    }

    std::vector<int> GetNextVertices(int vertex) const override
    {
        assert(0 <= vertex && vertex < adjacency_lists.size());
        return adjacency_lists[vertex];
    }

    std::vector<int> GetPrevVertices(int vertex) const override
    {
        std::vector<int> prev_vertices;

        for (int from = 0; from < adjacency_lists.size(); from++)
        {
            for (int to: adjacency_lists[from])
            {
                if (to == vertex)
                {
                    prev_vertices.push_back(from);
                }
            }
        }

        return prev_vertices;
    }

private:
    std::vector<std::vector<int>> adjacency_lists;
};


class CMatrixGraph: public IGraph
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
                matrix[i][j] = false;
            }
        }
    }
    CMatrixGraph(const IGraph &graph)
    {
        matrix.resize(graph.VerticesCount());
        for (size_t i = 0; i < matrix.size(); i++)
        {
            std::vector<int> next = graph.GetNextVertices(i);
            for (size_t j = 0; j < next.size(); j++)
            {
                matrix[i][next[j]] = true;
            }
        }
    }
    ~CMatrixGraph() {}

    // Добавление ребра от from к to.
    void AddEdge(int from, int to) override
    {
        matrix[from][to] = true;
    }

    int VerticesCount() const  override
    {
        return (int)matrix.size();
    }

    std::vector<int> GetNextVertices(int vertex) const override
    {
        std::vector<int> next;
        for (size_t i = 0; i < matrix.size(); i++)
        {
            if (matrix[vertex][i])
                next.push_back(i);
        }
        return next;

    }
    std::vector<int> GetPrevVertices(int vertex) const override
    {
        std::vector<int> prev;
        for (size_t i = 0; i < matrix.size(); i++)
        {
            if (matrix[i][vertex])
                prev.push_back(i);
        }
        return prev;
    }
private:
    std::vector<std::vector<bool>> matrix;

};

class CArcGraph: public IGraph
{
public:
    CArcGraph(int n) {}
    CArcGraph(const IGraph &graph)
    {
        for (size_t i = 0; i < graph.VerticesCount(); i++)
        {
            std::vector<int> next = graph.GetNextVertices(i);
            for (size_t j = 0; j < next.size(); j++)
            {
                pair p(i,next[j]);
                pair_array.push_back(p);
            }
        }

    }
    ~CArcGraph(){}
    void AddEdge(int from, int to) override
    {
        pair p(from, to);
        pair_array.push_back(p);
    }
    int VerticesCount() const override
    {
        int max = pair_array[0].from;
        for (size_t i = 0; i < pair_array.size(); i++)
        {
            if (pair_array[i].from > max)
                max = pair_array[i].from;
            if (pair_array[i].to > max)
                max = pair_array[i].to;
        }
        return max;
    }
    std::vector<int> GetNextVertices(int vertex) const override
    {
        std::vector<int> next;
        for (size_t i = 0; i < pair_array.size(); i++)
        {
            if (pair_array[i].from == vertex)
                next.push_back(pair_array[i].to);
        }
        return next;
    }
    std::vector<int> GetPrevVertices(int vertex) const override
    {
        std::vector<int> prev;
        for (size_t i = 0; i < pair_array.size(); i++)
        {
            if (pair_array[i].to == vertex)
                prev.push_back(pair_array[i].from);
        }
        return prev;
    }
private:
    struct pair
    {
        pair(int from, int to):from(from),to(to){}
        int from;
        int to;
    };
    std::vector<pair> pair_array;
};

class CSetGraph: public IGraph
{
public:
    CSetGraph(int n):map_array(n){}
    CSetGraph(const IGraph &graph)
    {
        map_array.resize(graph.VerticesCount());
        for (size_t i = 0; i < map_array.size(); i++)
        {
            std::vector<int> next = graph.GetNextVertices(i);
            map_array[i].
        }
    }
    ~CSetGraph(){}

    void AddEdge(int from, int to) override;
    int VerticesCount() const override;
    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;
private:
    std::vector<std::map<int, int>> map_array;

};

void BFS(const IGraph &graph, int vertex, void (*func)(int))
{
    std::vector<bool> visited(graph.VerticesCount(), false);
    std::queue<int> next_vertices;

    next_vertices.push(vertex);
    visited[vertex] = true;

    while (next_vertices.size())
    {
        int current = next_vertices.front();
        next_vertices.pop();

        func(current);

        for (int next_vertex: graph.GetNextVertices(current))
        {
            if (!visited[next_vertex])
            {
                visited[next_vertex] = true;
                next_vertices.push(next_vertex);
            }
        }
    }
}

void DFS(const IGraph &graph, int vertex, void (*func)(int))
{
    std::vector<bool> visited(graph.VerticesCount(), false);
    std::stack<int> next_vertices;

    next_vertices.push(vertex);
    visited[vertex] = true;

    while (next_vertices.size())
    {
        int current = next_vertices.top();
        next_vertices.pop();

        func(current);

        for (int next_vertex: graph.GetNextVertices(current))
        {
            if (!visited[next_vertex])
            {
                visited[next_vertex] = true;
                next_vertices.push(next_vertex);
            }
        }
    }
}


int main(int argc, const char * argv[]) {
    CArcGraph graph(7);
    graph.AddEdge(0, 1);
    graph.AddEdge(0, 5);
    graph.AddEdge(1, 2);
    graph.AddEdge(1, 3);
    graph.AddEdge(1, 5);
    graph.AddEdge(1, 6);
    graph.AddEdge(3, 2);
    graph.AddEdge(3, 4);
    graph.AddEdge(3, 6);
    graph.AddEdge(5, 4);
    graph.AddEdge(5, 6);
    graph.AddEdge(6, 4);

    BFS(graph, 0, [](int vertex){ std::cout << vertex << " "; });
    std::cout << std::endl;

    DFS(graph, 0, [](int vertex){ std::cout << vertex << " "; });
    std::cout << std::endl;

    return 0;
}
