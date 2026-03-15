#ifndef GENETIC_H
#define GENETIC_H

#include "graph.h"
#include <vector>
#include <random>
#include <algorithm>
#include <numeric>

class GeneticColoring {
private:
    const Graph& graph;
    int m;
    int populationSize;
    int generations;
    double mutationRate;
    std::mt19937 gen;

    double calculateFitness(const std::vector<int>& individual) const {
        int conflicts = calculateConflicts(graph, individual);
        return 1.0 / (conflicts + 1);
    }

    void initializePopulation(std::vector<std::vector<int>>& population) {
        std::uniform_int_distribution<> dis(1, m);
        for (int i = 0; i < populationSize; i++) {
            for (int j = 0; j < graph.V; j++) {
                population[i][j] = dis(gen);
            }
        }
    }

    std::vector<int> selection(const std::vector<std::vector<int>>& population, 
                                const std::vector<double>& fitnesses) {
        double totalFitness = std::accumulate(fitnesses.begin(), fitnesses.end(), 0.0);
        std::uniform_real_distribution<> dis(0.0, totalFitness);
        double randValue = dis(gen);
        double sumFitness = 0.0;
        
        for (int i = 0; i < populationSize; i++) {
            sumFitness += fitnesses[i];
            if (sumFitness >= randValue) {
                return population[i];
            }
        }
        return population[populationSize - 1];
    }

    std::vector<int> crossover(const std::vector<int>& parent1, 
                                const std::vector<int>& parent2) {
        std::vector<int> child(graph.V);
        std::uniform_int_distribution<> dis(0, graph.V - 1);
        int crossPoint = dis(gen);
        
        for (int i = 0; i < crossPoint; i++) {
            child[i] = parent1[i];
        }
        for (int i = crossPoint; i < graph.V; i++) {
            child[i] = parent2[i];
        }
        return child;
    }

    void mutate(std::vector<int>& individual) {
        std::uniform_int_distribution<> vertexDis(0, graph.V - 1);
        std::uniform_int_distribution<> colorDis(1, m);
        int index = vertexDis(gen);
        individual[index] = colorDis(gen);
    }

public:
    GeneticColoring(const Graph& g, int colors, int popSize = 100, int gens = 500, double mutRate = 0.1, unsigned int seed = 42)
        : graph(g), m(colors), populationSize(popSize), generations(gens), mutationRate(mutRate), gen(seed) {}

    std::vector<int> solve() {
        std::vector<std::vector<int>> population(populationSize, std::vector<int>(graph.V));
        initializePopulation(population);

        std::vector<int> bestIndividual;
        double bestFitness = 0.0;

        for (int gen = 0; gen < generations; gen++) {
            std::vector<double> fitnesses(populationSize);
            for (int i = 0; i < populationSize; i++) {
                fitnesses[i] = calculateFitness(population[i]);
                if (fitnesses[i] > bestFitness) {
                    bestFitness = fitnesses[i];
                    bestIndividual = population[i];
                }
            }

            std::vector<std::vector<int>> newPopulation(populationSize, std::vector<int>(graph.V));
            std::uniform_real_distribution<> probDis(0.0, 1.0);

            for (int i = 0; i < populationSize; i++) {
                std::vector<int> parent1 = selection(population, fitnesses);
                std::vector<int> parent2 = selection(population, fitnesses);
                std::vector<int> child = crossover(parent1, parent2);
                
                if (probDis(this->gen) < mutationRate) {
                    mutate(child);
                }
                newPopulation[i] = child;
            }
            population = newPopulation;
        }

        return bestIndividual;
    }
};

#endif
