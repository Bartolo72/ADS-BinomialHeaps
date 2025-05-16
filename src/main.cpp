#include "Experiments/Experiments.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

// Ensure header and clear file if not empty
void clearAndEnsureCsvHeader(const std::string& path) {
    std::ifstream file(path);
    bool notFileEmpty = file.peek() != std::ifstream::traits_type::eof();
    file.close();

    if (notFileEmpty) {
        std::ofstream clear_file(path, std::ios::trunc);
        clear_file.close();
    }

    std::ofstream csv(path);
    if (csv.is_open()) {
        csv << "n,c,M,"
            << "avg_time,var_time,min_time,max_time,"
            << "avg_links,var_links,min_links,max_links,"
            << "avg_swaps,var_swaps,min_swaps,max_swaps,"
            << "avg_extracts,var_extracts,min_extracts,max_extracts,"
            << "avg_edges,var_edges,min_edges,max_edges,"
            << "avg_decrease,var_decrease,min_decrease,max_decrease\n";
        csv.close();
    } else {
        std::cerr << "Could not open CSV file: " << path << "\n";
    }
}

// Templated vector parser for CLI args like n=500,1000
template <typename T>
void parseArgumentAsVector(const std::string& argument, std::vector<T>& values) {
    size_t pos = argument.find('=');
    if (pos == std::string::npos) return;

    std::string list = argument.substr(pos + 1);
    values.clear();
    std::stringstream ss(list);
    std::string token;

    while (std::getline(ss, token, ',')) {
        if constexpr (std::is_same<T, int>::value)
            values.push_back(std::stoi(token));
        else if constexpr (std::is_same<T, double>::value)
            values.push_back(std::stod(token));
    }
}

// Templated scalar parser for int or double
template <typename T>
void parseArgument(const std::string& argument, T& value) {
    size_t pos = argument.find('=');
    if (pos != std::string::npos) {
        if constexpr (std::is_same<T, int>::value)
            value = std::stoi(argument.substr(pos + 1));
        else if constexpr (std::is_same<T, double>::value)
            value = std::stod(argument.substr(pos + 1));
    }
}

int main(int argc, char* argv[]) {
    int M = 50;
    double max_weight = 1000.0;
    std::vector<int> n_sizes = {500, 750, 1000, 1250, 1500, 1750, 2000, 2250, 2500};
    std::vector<double> cs = {1.5, 1.75, 2.0};
    std::string outputFile = "output.csv";

    // CLI parsing
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg.find("M=") == 0) {
            parseArgument(arg, M);
        } else if (arg.find("n=") == 0) {
            parseArgumentAsVector(arg, n_sizes);
        } else if (arg.find("c=") == 0) {
            parseArgumentAsVector(arg, cs);
        } else {
            std::cerr << "Unknown argument: " << arg << "\n";
        }
    }

    clearAndEnsureCsvHeader(outputFile);

    for (int n : n_sizes) {
        for (double c : cs) {
            Experiments::run(n, c, M, max_weight);
        }
    }

    return 0;
}
