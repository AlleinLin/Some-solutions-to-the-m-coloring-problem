#ifndef PSO_H
#define PSO_H

#include "graph.h"
#include <vector>
#include <random>
#include <cmath>
#include <limits>
#include <algorithm>

class PSOColoring {
private:
    const Graph& graph;
    int m;
    int numParticles;
    int iterations;
    double w;
    double c1;
    double c2;
    std::mt19937 gen;

    double fitness(const std::vector<int>& position) const {
        int conflicts = calculateConflicts(graph, position);
        return 1.0 / (conflicts + 1);
    }

    void initializeParticles(std::vector<std::vector<int>>& positions,
                             std::vector<std::vector<double>>& velocities) {
        std::uniform_int_distribution<> posDis(1, m);
        std::uniform_real_distribution<> velDis(-m / 2.0, m / 2.0);

        for (int i = 0; i < numParticles; i++) {
            for (int j = 0; j < graph.V; j++) {
                positions[i][j] = posDis(gen);
                velocities[i][j] = velDis(gen);
            }
        }
    }

public:
    PSOColoring(const Graph& g, int colors, int particles = 30, int iter = 500,
                double inertia = 0.729, double cognitive = 1.49445, double social = 1.49445,
                unsigned int seed = 42)
        : graph(g), m(colors), numParticles(particles), iterations(iter),
          w(inertia), c1(cognitive), c2(social), gen(seed) {}

    std::vector<int> solve() {
        std::vector<std::vector<int>> positions(numParticles, std::vector<int>(graph.V));
        std::vector<std::vector<double>> velocities(numParticles, std::vector<double>(graph.V));
        std::vector<std::vector<int>> pbest(numParticles, std::vector<int>(graph.V));
        std::vector<double> pbestFitness(numParticles, 0.0);
        std::vector<int> gbest(graph.V);
        double gbestFitness = 0.0;

        initializeParticles(positions, velocities);

        for (int i = 0; i < numParticles; i++) {
            pbest[i] = positions[i];
            pbestFitness[i] = fitness(positions[i]);
            if (pbestFitness[i] > gbestFitness) {
                gbestFitness = pbestFitness[i];
                gbest = positions[i];
            }
        }

        std::uniform_real_distribution<> rDis(0.0, 1.0);

        for (int iter = 0; iter < iterations; iter++) {
            for (int i = 0; i < numParticles; i++) {
                for (int j = 0; j < graph.V; j++) {
                    double r1 = rDis(gen);
                    double r2 = rDis(gen);

                    velocities[i][j] = w * velocities[i][j] 
                                      + c1 * r1 * (pbest[i][j] - positions[i][j])
                                      + c2 * r2 * (gbest[j] - positions[i][j]);

                    positions[i][j] = static_cast<int>(std::round(positions[i][j] + velocities[i][j]));
                    positions[i][j] = std::max(1, std::min(m, positions[i][j]));
                }

                double currentFitness = fitness(positions[i]);
                if (currentFitness > pbestFitness[i]) {
                    pbest[i] = positions[i];
                    pbestFitness[i] = currentFitness;
                }

                if (currentFitness > gbestFitness) {
                    gbest = positions[i];
                    gbestFitness = currentFitness;
                }
            }

            if (gbestFitness >= 1.0) break;
        }

        return gbest;
    }
};

#endif
