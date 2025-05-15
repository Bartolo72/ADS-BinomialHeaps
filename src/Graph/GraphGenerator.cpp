#include "Graph/GraphGenerator.h"
#include <cmath>
#include <random>
#include <chrono>

void GraphGenerator::generate(WeightedGraph& G, int c, double max_weight) {
    int n = static_cast<int>(G.size());
    double p = c * std::log(n) / n;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> prob_dist(0.0, 1.0);
    std::uniform_real_distribution<> weight_dist(1.0, max_weight);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            if (prob_dist(gen) < p) {
                double weight = weight_dist(gen);
                G[i].push_back({j, weight});
                G[j].push_back({i, weight}); // Undirected edge
            }
        }
    }
}
