#ifndef GRAPH_GENERATOR_HPP
#define GRAPH_GENERATOR_HPP

#include <vector>
#include <list>
#include <random>

struct Edge {
    int target;
    double weight;
};

using WeightedGraph = std::vector<std::list<Edge>>;

class GraphGenerator {
public:
    // Generates a random undirected weighted graph
    static void generate(WeightedGraph& G, int c, double max_weight);
};

#endif // GRAPH_GENERATOR_HPP
