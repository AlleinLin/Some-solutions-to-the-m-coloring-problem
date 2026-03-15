#ifndef GREEDY_H
#define GREEDY_H

#include "graph.h"
#include <vector>
#include <algorithm>

class GreedyColoring {
private:
    const Graph& graph;
    int m;

public:
    GreedyColoring(const Graph& g, int colors) : graph(g), m(colors) {}

    std::vector<int> solve() {
        int V = graph.V;
        std::vector<int> color(V, -1);
        std::vector<bool> available(m, true);

        for (int i = 0; i < V; i++) {
            std::fill(available.begin(), available.end(), true);

            for (int neighbor : graph.adj[i]) {
                if (color[neighbor] != -1 && color[neighbor] < m) {
                    available[color[neighbor]] = false;
                }
            }

            int c = 0;
            while (c < m && !available[c]) {
                c++;
            }

            if (c < m) {
                color[i] = c;
            } else {
                c = 0;
                while (c < m && !available[c]) {
                    c++;
                }
                color[i] = (c < m) ? c : 0;
            }
        }

        return color;
    }

    int getUsedColors(const std::vector<int>& color) const {
        if (color.empty()) return 0;
        int maxColor = 0;
        for (int c : color) {
            maxColor = std::max(maxColor, c);
        }
        return maxColor + 1;
    }
};

#endif
