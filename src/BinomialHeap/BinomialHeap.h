#ifndef BINOMIAL_HEAP_HPP
#define BINOMIAL_HEAP_HPP

#include <limits>
#include <vector>
#include <unordered_map>

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

    void insert(int key, double priority);
    int min_priority_elem() const;
    double prio(int key) const;
    void extract_min();
    void decrease_prio(int key, double new_priority);
    void meld(BinomialHeap& other);

    bool empty() const;
    int size() const;

private:
    BinomialNode* head;
    int heap_size;

    BinomialNode* merge_roots(BinomialNode* h1, BinomialNode* h2);
    BinomialNode* union_heaps(BinomialNode* h1, BinomialNode* h2);
    void link_trees(BinomialNode* y, BinomialNode* z);
    BinomialNode* find_node(BinomialNode* node, int key) const;

    std::unordered_map<int, BinomialNode*> node_map;
};

#endif // BINOMIAL_HEAP_HPP
