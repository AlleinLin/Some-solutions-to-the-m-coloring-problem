#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <random>
#include <algorithm>

class Graph {
public:
    int V;
    int E;
    std::vector<std::vector<int>> adj;

    Graph() : V(0), E(0) {}

    Graph(int vertices) : V(vertices), E(0) {
        adj.resize(V);
    }

    void addEdge(int u, int v) {
        if (u >= 0 && u < V && v >= 0 && v < V) {
            adj[u].push_back(v);
            adj[v].push_back(u);
            E++;
        }
    }

    static Graph generateRandom(int vertices, double connectivity, unsigned int seed = 42) {
        Graph g(vertices);
        std::mt19937 gen(seed);
        std::uniform_real_distribution<> dis(0.0, 1.0);

        for (int i = 0; i < vertices; i++) {
            for (int j = i + 1; j < vertices; j++) {
                if (dis(gen) < connectivity) {
                    g.addEdge(i, j);
                }
            }
        }
        return g;
    }

    static Graph loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            return Graph();
        }

        int vertices, edges;
        file >> vertices >> edges;
        Graph g(vertices);

        for (int i = 0; i < edges; i++) {
            int u, v;
            file >> u >> v;
            g.addEdge(u, v);
        }
        return g;
    }

    void saveToFile(const std::string& filename) const {
        std::ofstream file(filename);
        file << V << " " << E << "\n";
        for (int i = 0; i < V; i++) {
            for (int neighbor : adj[i]) {
                if (i < neighbor) {
                    file << i << " " << neighbor << "\n";
                }
            }
        }
    }
};

inline int calculateConflicts(const Graph& graph, const std::vector<int>& color) {
    int conflicts = 0;
    for (int i = 0; i < graph.V; i++) {
        for (int neighbor : graph.adj[i]) {
            if (color[i] == color[neighbor]) {
                conflicts++;
            }
        }
    }
    return conflicts / 2;
}

inline bool isValidColoring(const Graph& graph, const std::vector<int>& color) {
    return calculateConflicts(graph, color) == 0;
}

inline bool isSafe(int v, const Graph& graph, const std::vector<int>& color, int c) {
    for (int neighbor : graph.adj[v]) {
        if (color[neighbor] == c) {
            return false;
        }
    }
    return true;
}

inline void printColoring(const std::vector<int>& color) {
    for (size_t i = 0; i < color.size(); i++) {
        std::cout << "Vertex " << i << ": Color " << color[i] << "\n";
    }
}

#endif
