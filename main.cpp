#include <iostream>
#include <chrono>
#include <iomanip>
#include "graph.h"
#include "backtracking.h"
#include "greedy.h"
#include "branchandbound.h"
#include "genetic.h"
#include "simulatedannealing.h"
#include "membraneevolution.h"
#include "taboosearch.h"
#include "antcolony.h"
#include "pso.h"

void printSeparator() {
    std::cout << std::string(60, '=') << "\n";
}

void printResult(const std::string& algorithmName, const std::vector<int>& coloring, 
                 const Graph& graph, double timeMs) {
    std::cout << "\n" << algorithmName << ":\n";
    std::cout << "  Time: " << std::fixed << std::setprecision(2) << timeMs << " ms\n";
    
    if (coloring.empty()) {
        std::cout << "  Result: No solution found\n";
    } else {
        int conflicts = calculateConflicts(graph, coloring);
        std::cout << "  Conflicts: " << conflicts << "\n";
        std::cout << "  Valid: " << (conflicts == 0 ? "Yes" : "No") << "\n";
        
        int maxColor = 0;
        for (int c : coloring) {
            maxColor = std::max(maxColor, c);
        }
        std::cout << "  Colors used: " << maxColor << "\n";
    }
}

int main() {
    printSeparator();
    std::cout << "Graph m-Coloring Problem - Algorithm Comparison\n";
    printSeparator();

    int vertices = 20;
    double connectivity = 0.15;
    int m = 5;
    unsigned int seed = 42;

    std::cout << "\nGraph Parameters:\n";
    std::cout << "  Vertices: " << vertices << "\n";
    std::cout << "  Connectivity: " << connectivity << "\n";
    std::cout << "  Colors (m): " << m << "\n";
    std::cout << "  Random seed: " << seed << "\n";

    Graph graph = Graph::generateRandom(vertices, connectivity, seed);
    std::cout << "  Generated edges: " << graph.E << "\n";

    printSeparator();
    std::cout << "Running algorithms...\n";
    printSeparator();

    {
        auto start = std::chrono::high_resolution_clock::now();
        BacktrackingColoring bc(graph, m);
        auto result = bc.solve();
        auto end = std::chrono::high_resolution_clock::now();
        double timeMs = std::chrono::duration<double, std::milli>(end - start).count();
        printResult("Backtracking Algorithm", result, graph, timeMs);
    }

    {
        auto start = std::chrono::high_resolution_clock::now();
        GreedyColoring gc(graph, m);
        auto result = gc.solve();
        auto end = std::chrono::high_resolution_clock::now();
        double timeMs = std::chrono::duration<double, std::milli>(end - start).count();
        printResult("Greedy Algorithm", result, graph, timeMs);
    }

    {
        auto start = std::chrono::high_resolution_clock::now();
        BranchAndBoundColoring bbc(graph, m);
        auto result = bbc.solve();
        auto end = std::chrono::high_resolution_clock::now();
        double timeMs = std::chrono::duration<double, std::milli>(end - start).count();
        printResult("Branch and Bound Algorithm", result, graph, timeMs);
    }

    {
        auto start = std::chrono::high_resolution_clock::now();
        GeneticColoring gc(graph, m, 100, 500, 0.1, seed);
        auto result = gc.solve();
        auto end = std::chrono::high_resolution_clock::now();
        double timeMs = std::chrono::duration<double, std::milli>(end - start).count();
        printResult("Genetic Algorithm", result, graph, timeMs);
    }

    {
        auto start = std::chrono::high_resolution_clock::now();
        SimulatedAnnealingColoring sac(graph, m, 10000.0, 0.99, 10000, seed);
        auto result = sac.solve();
        auto end = std::chrono::high_resolution_clock::now();
        double timeMs = std::chrono::duration<double, std::milli>(end - start).count();
        printResult("Simulated Annealing Algorithm", result, graph, timeMs);
    }

    {
        auto start = std::chrono::high_resolution_clock::now();
        MembraneEvolutionColoring mec(graph, m, 10, 500, seed);
        auto result = mec.solve();
        auto end = std::chrono::high_resolution_clock::now();
        double timeMs = std::chrono::duration<double, std::milli>(end - start).count();
        printResult("Membrane Evolution Algorithm", result, graph, timeMs);
    }

    {
        auto start = std::chrono::high_resolution_clock::now();
        TabooSearchColoring tsc(graph, m, 10, 1000, seed);
        auto result = tsc.solve();
        auto end = std::chrono::high_resolution_clock::now();
        double timeMs = std::chrono::duration<double, std::milli>(end - start).count();
        printResult("Taboo Search Algorithm", result, graph, timeMs);
    }

    {
        auto start = std::chrono::high_resolution_clock::now();
        AntColonyColoring acc(graph, m, 20, 100, 1.0, 2.0, 0.5, seed);
        auto result = acc.solve();
        auto end = std::chrono::high_resolution_clock::now();
        double timeMs = std::chrono::duration<double, std::milli>(end - start).count();
        printResult("Ant Colony Algorithm", result, graph, timeMs);
    }

    {
        auto start = std::chrono::high_resolution_clock::now();
        PSOColoring psoc(graph, m, 30, 500, 0.729, 1.49445, 1.49445, seed);
        auto result = psoc.solve();
        auto end = std::chrono::high_resolution_clock::now();
        double timeMs = std::chrono::duration<double, std::milli>(end - start).count();
        printResult("Particle Swarm Optimization Algorithm", result, graph, timeMs);
    }

    printSeparator();
    std::cout << "All algorithms completed.\n";
    printSeparator();

    return 0;
}
