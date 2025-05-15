#include "BinomialHeap/BinomialHeap.h"
#include <iostream>
#include <vector>
#include <cassert>

int main() {
    BinomialHeap heap;

    // Insert some keys
    heap.insert(1, 10.0);
    heap.insert(2, 5.0);
    heap.insert(3, 7.0);
    heap.insert(4, 3.0);
    heap.insert(5, 12.0);

    std::cout << "Size after insertions: " << heap.size() << "\n";
    assert(heap.min_priority_elem() == 4); // Priority 3.0
    assert(heap.prio(2) == 5.0);
    assert(heap.prio(5) == 12.0);

    // Decrease priority
    heap.decrease_prio(5, 2.0);
    assert(heap.prio(5) == 2.0);
    assert(heap.min_priority_elem() == 5); // Now min

    // Extract min (5)
    heap.extract_min();
    std::cout << "Extracted min.\n";
    assert(heap.min_priority_elem() == 4); // Next min is 4

    // Meld test
    BinomialHeap other;
    other.insert(10, 1.0);
    other.insert(11, 8.0);

    heap.meld(other);
    assert(heap.min_priority_elem() == 10);
    assert(other.empty());

    // Final extract all
    while (!heap.empty()) {
        int key = heap.min_priority_elem();
        std::cout << "Extracting min: " << key << " (prio=" << heap.prio(key) << ")\n";
        heap.extract_min();
    }

    std::cout << "All tests passed!\n";
    return 0;
}
