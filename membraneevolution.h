#ifndef MEMBRANEEVOLUTION_H
#define MEMBRANEEVOLUTION_H

#include "graph.h"
#include <vector>
#include <random>
#include <algorithm>
#include <list>

class MembraneEvolutionColoring {
private:
    const Graph& graph;
    int m;
    int numMembranes;
    int maxIterations;
    std::mt19937 gen;

    struct Membrane {
        std::vector<int> coloring;
        int fitness;
        
        Membrane(int v) : coloring(v), fitness(0) {}
    };

    int calculateFitness(const std::vector<int>& coloring) const {
        return calculateConflicts(graph, coloring);
    }

    void initializeMembrane(Membrane& membrane) {
        std::uniform_int_distribution<> dis(1, m);
        for (int i = 0; i < graph.V; i++) {
            membrane.coloring[i] = dis(gen);
        }
        membrane.fitness = calculateFitness(membrane.coloring);
    }

    Membrane copy(const Membrane& src) {
        Membrane dst(graph.V);
        dst.coloring = src.coloring;
        dst.fitness = src.fitness;
        return dst;
    }

    Membrane fusion(const Membrane& m1, const Membrane& m2) {
        std::uniform_int_distribution<> dis(0, graph.V - 1);
        int crossPoint = dis(gen);
        
        Membrane child(graph.V);
        for (int i = 0; i < crossPoint; i++) {
            child.coloring[i] = m1.coloring[i];
        }
        for (int i = crossPoint; i < graph.V; i++) {
            child.coloring[i] = m2.coloring[i];
        }
        child.fitness = calculateFitness(child.coloring);
        return child;
    }

    void division(Membrane& membrane, std::list<Membrane>& membranes) {
        std::uniform_int_distribution<> vertexDis(0, graph.V - 1);
        std::uniform_int_distribution<> colorDis(1, m);
        
        Membrane child1 = copy(membrane);
        Membrane child2 = copy(membrane);
        
        int idx = vertexDis(gen);
        child1.coloring[idx] = colorDis(gen);
        child1.fitness = calculateFitness(child1.coloring);
        
        idx = vertexDis(gen);
        child2.coloring[idx] = colorDis(gen);
        child2.fitness = calculateFitness(child2.coloring);
        
        membranes.push_back(child1);
        membranes.push_back(child2);
    }

    void dissolution(Membrane& membrane) {
        std::uniform_int_distribution<> vertexDis(0, graph.V - 1);
        std::uniform_int_distribution<> colorDis(1, m);
        
        for (int i = 0; i < graph.V / 4; i++) {
            int idx = vertexDis(gen);
            membrane.coloring[idx] = colorDis(gen);
        }
        membrane.fitness = calculateFitness(membrane.coloring);
    }

    void localSearch(Membrane& membrane) {
        std::uniform_int_distribution<> vertexDis(0, graph.V - 1);
        std::uniform_int_distribution<> colorDis(1, m);
        
        for (int i = 0; i < graph.V; i++) {
            if (membrane.fitness == 0) break;
            
            int oldColor = membrane.coloring[i];
            for (int c = 1; c <= m; c++) {
                if (c != oldColor) {
                    membrane.coloring[i] = c;
                    int newFitness = calculateFitness(membrane.coloring);
                    if (newFitness < membrane.fitness) {
                        membrane.fitness = newFitness;
                        break;
                    }
                }
            }
            if (membrane.coloring[i] != oldColor && membrane.fitness >= calculateFitness(membrane.coloring)) {
                membrane.coloring[i] = oldColor;
            }
        }
        membrane.fitness = calculateFitness(membrane.coloring);
    }

public:
    MembraneEvolutionColoring(const Graph& g, int colors, int membranes = 10, int maxIter = 500, unsigned int seed = 42)
        : graph(g), m(colors), numMembranes(membranes), maxIterations(maxIter), gen(seed) {}

    std::vector<int> solve() {
        std::list<Membrane> membranes;
        for (int i = 0; i < numMembranes; i++) {
            Membrane membrane(graph.V);
            initializeMembrane(membrane);
            membranes.push_back(membrane);
        }

        Membrane bestMembrane(graph.V);
        bestMembrane.fitness = std::numeric_limits<int>::max();

        std::uniform_real_distribution<> probDis(0.0, 1.0);

        for (int iter = 0; iter < maxIterations; iter++) {
            for (auto it = membranes.begin(); it != membranes.end(); ) {
                if (it->fitness < bestMembrane.fitness) {
                    bestMembrane = copy(*it);
                }

                double op = probDis(gen);
                
                if (op < 0.2 && membranes.size() >= 2) {
                    auto it2 = it;
                    ++it2;
                    if (it2 != membranes.end()) {
                        Membrane child = fusion(*it, *it2);
                        membranes.push_back(child);
                        it2 = membranes.erase(it2);
                    }
                    ++it;
                } else if (op < 0.4 && membranes.size() < numMembranes * 2) {
                    division(*it, membranes);
                    it = membranes.erase(it);
                } else if (op < 0.6) {
                    dissolution(*it);
                    ++it;
                } else if (op < 0.8) {
                    localSearch(*it);
                    ++it;
                } else {
                    ++it;
                }
            }

            while (membranes.size() > numMembranes * 2) {
                auto worst = membranes.begin();
                for (auto it = membranes.begin(); it != membranes.end(); ++it) {
                    if (it->fitness > worst->fitness) {
                        worst = it;
                    }
                }
                membranes.erase(worst);
            }

            if (bestMembrane.fitness == 0) break;
        }

        return bestMembrane.coloring;
    }
};

#endif
