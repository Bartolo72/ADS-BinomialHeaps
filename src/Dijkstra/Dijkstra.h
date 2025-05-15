#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP

#include "Graph/GraphGenerator.h"
#include "BinomialHeap/BinomialHeap.h"
#include <vector>

void Dijkstra(const WeightedGraph& G, int source, std::vector<double>& D, std::vector<int>& path, BinomialHeap &heap);

#endif // DIJKSTRA_HPP
