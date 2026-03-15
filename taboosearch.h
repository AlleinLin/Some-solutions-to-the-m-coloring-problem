#ifndef TABOOSEARCH_H
#define TABOOSEARCH_H

#include "graph.h"
#include <vector>
#include <list>
#include <random>
#include <algorithm>
#include <utility>

class TabooSearchColoring {
private:
    const Graph& graph;
    int m;
    int tabooTenure;
    int maxIterations;
    std::mt19937 gen;

    int calculateEnergy(const std::vector<int>& color) const {
        return calculateConflicts(graph, color);
    }

    bool isInTabooList(const std::list<std::pair<int, int>>& tabooList, int vertex, int color) const {
        for (const auto& item : tabooList) {
            if (item.first == vertex && item.second == color) {
                return true;
            }
        }
        return false;
    }

public:
    TabooSearchColoring(const Graph& g, int colors, int tenure = 10, int maxIter = 1000, unsigned int seed = 42)
        : graph(g), m(colors), tabooTenure(tenure), maxIterations(maxIter), gen(seed) {}

    std::vector<int> solve() {
        std::vector<int> color(graph.V);
        std::uniform_int_distribution<> colorDis(1, m);
        
        for (int i = 0; i < graph.V; i++) {
            color[i] = colorDis(gen);
        }

        std::vector<int> bestColor = color;
        int bestEnergy = calculateEnergy(color);

        std::list<std::pair<int, int>> tabooList;

        for (int iter = 0; iter < maxIterations; iter++) {
            int bestNeighborEnergy = std::numeric_limits<int>::max();
            int bestVertex = -1;
            int bestNewColor = -1;

            for (int v = 0; v < graph.V; v++) {
                for (int c = 1; c <= m; c++) {
                    if (color[v] != c) {
                        int oldColor = color[v];
                        color[v] = c;
                        int energy = calculateEnergy(color);
                        color[v] = oldColor;

                        if (!isInTabooList(tabooList, v, c) || energy < bestEnergy) {
                            if (energy < bestNeighborEnergy) {
                                bestNeighborEnergy = energy;
                                bestVertex = v;
                                bestNewColor = c;
                            }
                        }
                    }
                }
            }

            if (bestVertex != -1) {
                color[bestVertex] = bestNewColor;

                tabooList.push_back({bestVertex, bestNewColor});
                if (tabooList.size() > static_cast<size_t>(tabooTenure)) {
                    tabooList.pop_front();
                }

                if (bestNeighborEnergy < bestEnergy) {
                    bestEnergy = bestNeighborEnergy;
                    bestColor = color;
                }
            }

            if (bestEnergy == 0) break;
        }

        return bestColor;
    }
};

#endif
