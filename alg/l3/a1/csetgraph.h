#ifndef CSETGRAPH_H
#define CSETGRAPH_H
#include "igraph.h"
#include <unordered_set>
class CSetGraph: public IGraph
{
public:
    CSetGraph(int n):set_array(n){}
    CSetGraph(const IGraph &graph)
    {
        for (size_t i = 0; i < graph.VerticesCount(); i++)
        {
            std::vector<int> next = graph.GetNextVertices(i);
            for (int to: next)
            {
                AddEdge(i,to);
            }
        }
    }
    ~CSetGraph(){}

    void AddEdge(int from, int to) override
    {
        set_array[from].insert(to);
    }
    int VerticesCount() const override
    {
        return set_array.size();
    }
    std::vector<int> GetNextVertices(int vertex) const override
    {
        std::vector<int> next;
        for (auto v: set_array[vertex])
            next.push_back(v);
        return next;
    }
    std::vector<int> GetPrevVertices(int vertex) const override
    {
        std::vector<int> prev;
        for (int i = 0; i < set_array.size(); i++)
        {
            if (set_array[i].count(vertex) != 0)
            {
                prev.push_back(i);
            }
        }
        return prev;
    }
private:
    std::vector<std::unordered_set<int>> set_array;

};

#endif // CSETGRAPH_H
