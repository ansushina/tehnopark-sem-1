#ifndef CLISTGRAPH_H
#define CLISTGRAPH_H
#include "igraph.h"
#include <list>
#include <cassert>

class CListGraph: public IGraph
{
public:
    CListGraph(int n) : adjacency_lists(n) {}
    CListGraph(const IGraph &graph)
    {
        for (size_t i = 0; i < graph.VerticesCount(); i++)
        {
           std::vector<int> vertices = graph.GetNextVertices(i);

           for(int to: vertices)
           {
               AddEdge(i,to);
           }
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
        std::vector<int> next;
        for(int v: adjacency_lists[vertex])
        {
            next.push_back(v);
        }
        return next;
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
    std::vector<std::list<int>> adjacency_lists;
};

#endif // CLISTGRAPH_H
