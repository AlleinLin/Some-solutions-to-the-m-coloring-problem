#ifndef BACKTRACKING_H
#define BACKTRACKING_H

#include "graph.h"
#include <vector>

class BacktrackingColoring {
private:
    const Graph& graph;
    int m;
    std::vector<int> color;
    bool found;

    bool solve(int v) {
        if (v == graph.V) {
            return true;
        }

        for (int c = 1; c <= m; c++) {
            if (isSafe(v, graph, color, c)) {
                color[v] = c;
                if (solve(v + 1)) {
                    return true;
                }
                color[v] = 0;
            }
        }
        return false;
    }

public:
    BacktrackingColoring(const Graph& g, int colors) 
        : graph(g), m(colors), color(g.V, 0), found(false) {}

    std::vector<int> solve() {
        found = solve(0);
        if (found) {
            return color;
        }
        return std::vector<int>();
    }

    bool isSolutionFound() const {
        return found;
    }
};

#endif
