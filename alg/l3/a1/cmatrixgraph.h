#ifndef CMATRIXGRAPH_H
#define CMATRIXGRAPH_H
#include "igraph.h"
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

#endif // CMATRIXGRAPH_H
