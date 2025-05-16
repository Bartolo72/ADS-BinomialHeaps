#include "Experiments/Experiments.h"
#include <iostream>
#include <vector>
#include <fstream>

int main() {
    const std::string outputFilename = "output.csv";
    std::ifstream file(outputFilename);
    bool notFileEmpty = file.peek() != std::ifstream::traits_type::eof();
    if (notFileEmpty) {
        std::ofstream clear_file(outputFilename, std::ios::trunc); 
        clear_file.close();
    }

    std::ofstream csv(outputFilename);
    csv << "n,c,M,"
        << "avg_time,var_time,min_time,max_time,"
        << "avg_links,var_links,min_links,max_links,"
        << "avg_swaps,var_swaps,min_swaps,max_swaps,"
        << "avg_extracts,var_extracts,min_extracts,max_extracts,"
        << "avg_edges,var_edges,min_edges,max_edges,"
        << "avg_decrease,var_decrease,min_decrease,max_decrease\n";
    csv.close();


    int M = 50;            
    double max_weight = 1000.0;

    std::vector<int> n_sizes = {500, 750, 1000, 1250, 1500, 1750, 2000, 2250, 2500};
    std::vector<double> cs = {1.5, 1.75, 2};

    for (int n : n_sizes) {
        for (double c : cs) {
            Experiments::run(n, c, M, max_weight);
        }
    }

    return 0;
}
