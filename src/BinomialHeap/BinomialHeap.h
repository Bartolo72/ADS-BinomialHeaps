#ifndef BINOMIALHEAP_HPP
#define BINOMIALHEAP_HPP

#include <unordered_map>
#include <stdexcept>
#include <limits>
#include <utility>

const double INFINITY_WEIGHT = std::numeric_limits<double>::max();

struct BinomialNode {
    int key;
    double priority;
    int degree;
    BinomialNode* parent;
    BinomialNode* child;
    BinomialNode* sibling;

    BinomialNode(int k, double p)
        : key(k), priority(p), degree(0), parent(nullptr), child(nullptr), sibling(nullptr) {}
};

class BinomialHeap {
public:
    BinomialHeap();

    bool empty() const;
    int size() const;

    void insert(int key, double priority);
    int min_priority_elem() const;
    double prio(int key) const;
    void extract_min();
    void decrease_prio(int key, double new_priority);
    void meld(BinomialHeap& other);
    bool contains(int key) const;

    size_t get_link_count() const { return link_count; }
    size_t get_swap_count() const { return swap_count; }
    size_t get_extract_count() const { return extract_count; }
    size_t get_insert_count() const { return insert_count; }
    size_t get_decrease_prio_count() const { return decrease_prio_count; }

    void reset_counters() {
        link_count = 0;
        swap_count = 0;
        extract_count = 0;
        insert_count = 0;
        decrease_prio_count = 0;
    }

private:
    BinomialNode* head;
    int heap_size;
    std::unordered_map<int, BinomialNode*> node_map;

    BinomialNode* merge_roots(BinomialNode* h1, BinomialNode* h2);
    void link_trees(BinomialNode* y, BinomialNode* z);
    BinomialNode* union_heaps(BinomialNode* h1, BinomialNode* h2);

    size_t link_count;
    size_t swap_count;
    size_t extract_count;
    size_t insert_count;
    size_t decrease_prio_count;
};

#endif
