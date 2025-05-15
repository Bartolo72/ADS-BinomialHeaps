#include "Experiments/Experiments.h"
#include <iostream>
#include <vector>

int main() {
    int M = 5;            // Number of repetitions per setting
    double max_weight = 1000.0; // Max edge weight

    std::vector<int> sizes = {100, 500, 1000};
    std::vector<double> cs = {0.5, 1.0, 1.5};

    for (int n : sizes) {
        for (double c : cs) {
            Experiments::run(n, c, M, max_weight);
        }
    }

    return 0;
}
