#ifndef ANTCOLONY_H
#define ANTCOLONY_H

#include "graph.h"
#include <vector>
#include <random>
#include <cmath>
#include <limits>
#include <algorithm>

class AntColonyColoring {
private:
    const Graph& graph;
    int m;
    int numAnts;
    int iterations;
    double alpha;
    double beta;
    double rho;
    std::mt19937 gen;
    std::vector<std::vector<double>> tau;

    int evaluateSolution(const std::vector<int>& solution) const {
        return calculateConflicts(graph, solution);
    }

    double heuristicValue(int vertex, int color, const std::vector<int>& currentColoring) const {
        int conflicts = 0;
        for (int neighbor : graph.adj[vertex]) {
            if (currentColoring[neighbor] == color) {
                conflicts++;
            }
        }
        return 1.0 / (conflicts + 1);
    }

    std::vector<int> constructSolution(double alpha, double beta) {
        std::vector<int> color(graph.V, 0);
        std::vector<bool> available(m, true);

        for (int i = 0; i < graph.V; i++) {
            std::fill(available.begin(), available.end(), true);
            for (int neighbor : graph.adj[i]) {
                if (color[neighbor] != 0) {
                    available[color[neighbor] - 1] = false;
                }
            }

            std::vector<double> probabilities(m, 0.0);
            double sumProbabilities = 0.0;

            for (int c = 0; c < m; c++) {
                if (available[c]) {
                    double pheromone = std::pow(tau[i][c], alpha);
                    double heuristic = std::pow(heuristicValue(i, c + 1, color), beta);
                    probabilities[c] = pheromone * heuristic;
                    sumProbabilities += probabilities[c];
                }
            }

            if (sumProbabilities == 0) {
                std::uniform_int_distribution<> dis(1, m);
                color[i] = dis(gen);
            } else {
                std::uniform_real_distribution<> dis(0.0, sumProbabilities);
                double randNum = dis(gen);
                double cumulative = 0.0;
                for (int c = 0; c < m; c++) {
                    if (available[c]) {
                        cumulative += probabilities[c];
                        if (cumulative >= randNum) {
                            color[i] = c + 1;
                            break;
                        }
                    }
                }
                if (color[i] == 0) {
                    for (int c = 0; c < m; c++) {
                        if (available[c]) {
                            color[i] = c + 1;
                            break;
                        }
                    }
                }
            }
        }

        return color;
    }

    void updatePheromones(const std::vector<std::vector<int>>& solutions) {
        for (int i = 0; i < graph.V; i++) {
            for (int c = 0; c < m; c++) {
                tau[i][c] *= (1 - rho);
            }
        }

        for (const auto& solution : solutions) {
            int cost = evaluateSolution(solution);
            if (cost > 0) {
                for (int i = 0; i < graph.V; i++) {
                    tau[i][solution[i] - 1] += 1.0 / cost;
                }
            }
        }
    }

public:
    AntColonyColoring(const Graph& g, int colors, int ants = 20, int iter = 100,
                      double a = 1.0, double b = 2.0, double r = 0.5,
                      unsigned int seed = 42)
        : graph(g), m(colors), numAnts(ants), iterations(iter),
          alpha(a), beta(b), rho(r), gen(seed) {
        tau.resize(graph.V, std::vector<double>(m, 1.0));
    }

    std::vector<int> solve() {
        std::vector<int> bestSolution;
        int bestCost = std::numeric_limits<int>::max();

        for (int iter = 0; iter < iterations; iter++) {
            std::vector<std::vector<int>> solutions(numAnts);
            
            for (int ant = 0; ant < numAnts; ant++) {
                solutions[ant] = constructSolution(alpha, beta);
                int cost = evaluateSolution(solutions[ant]);
                
                if (cost < bestCost) {
                    bestCost = cost;
                    bestSolution = solutions[ant];
                }
            }

            updatePheromones(solutions);

            if (bestCost == 0) break;
        }

        return bestSolution;
    }
};

#endif
