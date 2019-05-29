#ifndef IGRAPH_H
#define IGRAPH_H

#include <vector>

struct IGraph {
    virtual ~IGraph() = 0;

    // Добавление ребра от from к to.
    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const  = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};

IGraph::~IGraph() {}



#endif // IGRAPH_H
