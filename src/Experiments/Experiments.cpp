#include "Graph/GraphGenerator.h"
#include "BinomialHeap/BinomialHeap.h"
#include "Dijkstra/Dijkstra.h"
#include "Experiments/Experiments.h"
#include <vector>
#include <chrono>
#include <iostream>
#include <cmath>
#include <numeric>
#include <limits>

void Experiments::run(int n, double c, int M, double max_weight) {
    using namespace std;
    using Clock = chrono::high_resolution_clock;

    std::vector<double> times(M);
    std::vector<size_t> links(M);
    std::vector<size_t> swaps(M);
    std::vector<size_t> deletes(M);

    for (int i = 0; i < M; ++i) {
        WeightedGraph G(n);
        GraphGenerator::generate(G, c, max_weight);

        std::vector<double> D(n, std::numeric_limits<double>::infinity());
        std::vector<int> path(n, -1);

        BinomialHeap heap;

        auto start = Clock::now();
        Dijkstra(G, 0, D, path, heap);  // Use your existing Dijkstra implementation
        auto end = Clock::now();

        std::chrono::duration<double> diff = end - start;
        times[i] = diff.count();

        links[i] = heap.get_link_count();
        swaps[i] = heap.get_swap_count();
        deletes[i] = heap.get_extract_count();
    }

    auto avg = [](const std::vector<double>& v) {
        return std::accumulate(v.begin(), v.end(), 0.0) / v.size();
    };

    std::cout << "n = " << n << ", c = " << c << ", M = " << M << "\n";
    std::cout << "Average time: " << avg(times) << " s\n";
    std::cout << "Average links: " << avg(std::vector<double>(links.begin(), links.end())) << "\n";
    std::cout << "Average swaps: " << avg(std::vector<double>(swaps.begin(), swaps.end())) << "\n";
    std::cout << "Average extracts: " << avg(std::vector<double>(deletes.begin(), deletes.end())) << "\n";
    std::cout << "--------------------------------------------------\n";
}
