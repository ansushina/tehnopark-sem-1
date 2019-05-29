#include <iostream>

#include <cassert>
#include <queue>
#include <stack>
#include "igraph.h"
#include "clistgraph.h"
#include "carcgraph.h"
#include "cmatrixgraph.h"
#include "csetgraph.h"








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
    CListGraph graph(7);
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
