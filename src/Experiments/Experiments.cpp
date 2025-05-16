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
#include <fstream>
#include <algorithm>


bool is_connected(const WeightedGraph& G) {
    int n = G.size();
    std::vector<bool> visited(n, false);

    std::function<void(int)> dfs = [&](int u) {
        visited[u] = true;
        for (const auto& [v, _] : G[u]) {
            if (!visited[v]) {
                dfs(v);
            }
        }
    };

    dfs(0);

    for (bool v : visited) {
        if (!v) return false;
    }
    return true;
}


void Experiments::run(int n, double c, int M, double max_weight) {
    const std::string outputFilename = "output.csv";

    using namespace std;
    using Clock = chrono::high_resolution_clock;

    vector<double> times;
    vector<size_t> links, swaps, deletes;
    vector<int> edge_counts;
    vector<int> decrease_counts;

    int accepted = 0;
    while (accepted < M) {
        WeightedGraph G(n);
        int edge_count = GraphGenerator::generate(G, c, max_weight);

        if (!is_connected(G)) {
            continue; // Skip disconnected graphs
        }

        edge_counts.push_back(edge_count);
        accepted++;

        vector<double> D(n, numeric_limits<double>::infinity());
        vector<int> path(n, -1);
        BinomialHeap heap;

        auto start = Clock::now();
        Dijkstra(G, 0, D, path, heap);
        auto end = Clock::now();

        chrono::duration<double> diff = end - start;
        times.push_back(diff.count());
        links.push_back(heap.get_link_count());
        swaps.push_back(heap.get_swap_count());
        deletes.push_back(heap.get_extract_count());
        decrease_counts.push_back(heap.get_decrease_prio_count());
    }

    auto avg = [](const vector<double>& v) {
        return accumulate(v.begin(), v.end(), 0.0) / v.size();
    };

    auto avg_int = [](const vector<int>& v) {
        return static_cast<double>(accumulate(v.begin(), v.end(), 0)) / v.size();
    };

    auto var = [](const vector<double>& v, double mean) {
        double sum_sq = 0.0;
        for (double x : v) sum_sq += (x - mean) * (x - mean);
        return sum_sq / v.size();
    };

    auto max_val = [](const vector<double>& v) {
        return *max_element(v.begin(), v.end());
    };

    auto min_val = [](const vector<double>& v) {
        return *min_element(v.begin(), v.end());
    };

    // Calculated metrics
    double time_avg = avg(times);
    double time_var = var(times, time_avg);

    double link_avg = avg(vector<double>(links.begin(), links.end()));
    double link_var = var(vector<double>(links.begin(), links.end()), link_avg);

    double swap_avg = avg(vector<double>(swaps.begin(), swaps.end()));
    double swap_var = var(vector<double>(swaps.begin(), swaps.end()), swap_avg);

    double delete_avg = avg(vector<double>(deletes.begin(), deletes.end()));
    double delete_var = var(vector<double>(deletes.begin(), deletes.end()), delete_avg);

    double edge_avg = avg_int(edge_counts);
    double edge_var = var(vector<double>(edge_counts.begin(), edge_counts.end()), edge_avg);

    double decrease_avg = avg_int(decrease_counts);
    double decrease_var = var(vector<double>(decrease_counts.begin(), decrease_counts.end()), decrease_avg);

    // Console output
    cout << "n = " << n << ", c = " << c << ", M = " << M << "\n";
    cout << "Average time: " << time_avg << " s\n";
    cout << "Average edges: " << edge_avg << "\n";
    cout << "Average links: " << link_avg << "\n";
    cout << "Average swaps: " << swap_avg << "\n";
    cout << "Average extracts: " << delete_avg << "\n";
    cout << "Average decrease-prio: " << decrease_avg << "\n";
    cout << "--------------------------------------------------\n";

    // CSV output
    ofstream csv(outputFilename, ios::app);
    if (csv) {
        csv << n << "," << c << "," << M << ","
            << time_avg << "," << time_var << "," << min_val(times) << "," << max_val(times) << ","
            << link_avg << "," << link_var << "," << min_val(vector<double>(links.begin(), links.end())) << "," << max_val(vector<double>(links.begin(), links.end())) << ","
            << swap_avg << "," << swap_var << "," << min_val(vector<double>(swaps.begin(), swaps.end())) << "," << max_val(vector<double>(swaps.begin(), swaps.end())) << ","
            << delete_avg << "," << delete_var << "," << min_val(vector<double>(deletes.begin(), deletes.end())) << "," << max_val(vector<double>(deletes.begin(), deletes.end())) << ","
            << edge_avg << "," << edge_var << "," << *min_element(edge_counts.begin(), edge_counts.end()) << "," << *max_element(edge_counts.begin(), edge_counts.end()) << ","
            << decrease_avg << "," << decrease_var << "," << *min_element(decrease_counts.begin(), decrease_counts.end()) << "," << *max_element(decrease_counts.begin(), decrease_counts.end())
            << "\n";
        csv.close();
    }
}
