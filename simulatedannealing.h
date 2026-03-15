#ifndef SIMULATEDANNEALING_H
#define SIMULATEDANNEALING_H

#include "graph.h"
#include <vector>
#include <random>
#include <cmath>
#include <limits>

class SimulatedAnnealingColoring {
private:
    const Graph& graph;
    int m;
    double initialTemperature;
    double coolingRate;
    int maxIterations;
    std::mt19937 gen;

    int calculateEnergy(const std::vector<int>& color) const {
        return calculateConflicts(graph, color);
    }

public:
    SimulatedAnnealingColoring(const Graph& g, int colors, 
                                double initTemp = 10000.0, 
                                double coolRate = 0.99,
                                int maxIter = 10000,
                                unsigned int seed = 42)
        : graph(g), m(colors), initialTemperature(initTemp), 
          coolingRate(coolRate), maxIterations(maxIter), gen(seed) {}

    std::vector<int> solve() {
        std::vector<int> color(graph.V);
        std::uniform_int_distribution<> colorDis(1, m);
        std::uniform_int_distribution<> vertexDis(0, graph.V - 1);
        std::uniform_real_distribution<> probDis(0.0, 1.0);

        for (int i = 0; i < graph.V; i++) {
            color[i] = colorDis(gen);
        }

        double temperature = initialTemperature;
        std::vector<int> bestColor = color;
        int bestEnergy = calculateEnergy(color);

        int iter = 0;
        while (temperature > 1 && iter < maxIterations) {
            int i = vertexDis(gen);
            int oldColor = color[i];
            int newColor = colorDis(gen);
            color[i] = newColor;

            int oldEnergy = calculateEnergy(color);
            if (oldEnergy > 0) {
                color[i] = oldColor;
                int newEnergy = calculateEnergy(color);
                double deltaE = newEnergy - oldEnergy;

                if (deltaE < 0 || exp(-deltaE / temperature) > probDis(gen)) {
                    color[i] = newColor;
                } else {
                    color[i] = oldColor;
                }
            }

            int currentEnergy = calculateEnergy(color);
            if (currentEnergy < bestEnergy) {
                bestEnergy = currentEnergy;
                bestColor = color;
            }

            temperature *= coolingRate;
            iter++;
        }

        return bestColor;
    }
};

#endif
