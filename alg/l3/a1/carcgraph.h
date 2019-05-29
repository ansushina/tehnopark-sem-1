#ifndef CARCGRAPH_H
#define CARCGRAPH_H

#include "igraph.h"
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

#endif // CARCGRAPH_H
