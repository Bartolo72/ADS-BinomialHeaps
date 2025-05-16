#include "BinomialHeap/BinomialHeap.h"
#include "Graph/GraphGenerator.h"
#include <vector>
#include <limits>

const int NULL_VERTEX = -1;

void Dijkstra(const WeightedGraph &G, int s,
              std::vector<double> &D, std::vector<int> &path,
              BinomialHeap &heap) {
    int n = G.size();
    D.assign(n, std::numeric_limits<double>::infinity());
    path.assign(n, NULL_VERTEX);

    D[s] = 0.0;
    path[s] = s;
    heap.insert(s, 0.0);

    std::vector<bool> visited(n, false);
    while (!heap.empty()) {
        int u = heap.min_priority_elem();
        heap.extract_min();

        if (visited[u]) continue; 
        visited[u] = true;

        double dist_u = D[u];

        for (const auto &edge : G[u]) {
            int v = edge.target;
            double weight = edge.weight;
            if (!visited[v] && dist_u + weight < D[v]) {
                D[v] = dist_u + weight;
                path[v] = u;
                if (heap.contains(v)) {
                    heap.decrease_prio(v, D[v]);
                } else {
                    heap.insert(v, D[v]);
                }
            }
        }
    }
}
