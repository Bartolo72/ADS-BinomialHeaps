#include "BinomialHeap/BinomialHeap.h"
#include <iostream>

int main() {
    BinomialHeap bh;

    // Insert nodes
    bh.insert(1, 10.0);
    bh.insert(2, 5.0);
    bh.insert(3, 20.0);
    bh.insert(4, 7.0);

    std::cout << "Min: " << bh.min_priority_elem() << " (prio: " << bh.prio(bh.min_priority_elem()) << ")\n";

    // Decrease priority of node 3
    bh.decrease_prio(3, 3.0);
    std::cout << "After decrease_prio(3, 3.0): Min: " << bh.min_priority_elem() << "\n";

    // Extract min repeatedly
    while (!bh.empty()) {
        int min_elem = bh.min_priority_elem();
        double prio = bh.prio(min_elem);
        std::cout << "Extracting min: " << min_elem << " (prio: " << prio << ")\n";
        bh.extract_min();
    }

    std::cout << "Operations:\n";
    std::cout << "  Tree links: " << bh.get_link_count() << "\n";
    std::cout << "  Swaps:      " << bh.get_swap_count() << "\n";
    std::cout << "  Root removals: " << bh.get_root_removal_count() << "\n";

    return 0;
}
