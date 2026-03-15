#ifndef BRANCHANDBOUND_H
#define BRANCHANDBOUND_H

#include "graph.h"
#include <vector>
#include <queue>
#include <limits>

struct Node {
    std::vector<int> coloring;
    int level;
    int bound;

    Node(int v, int lvl) : coloring(v, 0), level(lvl), bound(0) {}
    Node(const std::vector<int>& col, int lvl) : coloring(col), level(lvl), bound(0) {}
};

struct CompareNode {
    bool operator()(const Node* a, const Node* b) const {
        return a->bound > b->bound;
    }
};

class BranchAndBoundColoring {
private:
    const Graph& graph;
    int m;
    std::vector<int> bestColoring;
    int bestCost;

    bool isComplete(const Node& node) const {
        for (int c : node.coloring) {
            if (c == 0) return false;
        }
        return true;
    }

    bool isSafe(int v, const std::vector<int>& coloring, int c) const {
        for (int neighbor : graph.adj[v]) {
            if (coloring[neighbor] == c) {
                return false;
            }
        }
        return true;
    }

    int findNextVertex(const std::vector<int>& coloring) const {
        for (int i = 0; i < graph.V; i++) {
            if (coloring[i] == 0) return i;
        }
        return -1;
    }

    int calculateBound(const Node& node) const {
        int conflicts = 0;
        int uncolored = 0;
        for (int i = 0; i < graph.V; i++) {
            if (node.coloring[i] == 0) {
                uncolored++;
            } else {
                for (int neighbor : graph.adj[i]) {
                    if (node.coloring[i] == node.coloring[neighbor]) {
                        conflicts++;
                    }
                }
            }
        }
        return conflicts / 2 + uncolored;
    }

    int evaluateSolution(const std::vector<int>& coloring) const {
        return calculateConflicts(graph, coloring);
    }

public:
    BranchAndBoundColoring(const Graph& g, int colors) : graph(g), m(colors), bestCost(std::numeric_limits<int>::max()) {}

    std::vector<int> solve() {
        Node root(graph.V, 0);
        std::priority_queue<Node*, std::vector<Node*>, CompareNode> pq;
        pq.push(&root);

        while (!pq.empty()) {
            Node* node = pq.top();
            pq.pop();

            if (isComplete(*node)) {
                int cost = evaluateSolution(node->coloring);
                if (cost < bestCost) {
                    bestCost = cost;
                    bestColoring = node->coloring;
                }
                delete node;
                continue;
            }

            int v = findNextVertex(node->coloring);
            if (v == -1) {
                delete node;
                continue;
            }

            for (int c = 1; c <= m; c++) {
                if (isSafe(v, node->coloring, c)) {
                    Node* child = new Node(node->coloring, node->level + 1);
                    child->coloring[v] = c;
                    child->bound = calculateBound(*child);

                    if (child->bound < bestCost) {
                        pq.push(child);
                    } else {
                        delete child;
                    }
                }
            }
            delete node;
        }

        return bestColoring;
    }

    int getBestCost() const { return bestCost; }
};

#endif
