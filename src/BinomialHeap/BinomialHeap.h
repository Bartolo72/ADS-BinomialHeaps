#ifndef BINOMIAL_HEAP_HPP
#define BINOMIAL_HEAP_HPP

#include <vector>
#include <limits>
#include <unordered_map>

const double INFINITY_PRIO = std::numeric_limits<double>::max();

class BinomialHeap {
public:
    BinomialHeap();

    void insert(int x, double p);
    int min_priority_elem() const;
    double prio(int x) const;
    void extract_min();
    void decrease_prio(int x, double p);
    void meld(BinomialHeap& other);
    bool empty() const;
    int size() const;

    // Operation counters for empirical analysis
    int get_link_count() const;
    int get_swap_count() const;
    int get_root_removal_count() const;

private:
    struct Node {
        int key;
        double priority;
        int degree;
        Node* parent;
        Node* child;
        Node* sibling;

        Node(int k, double p) : key(k), priority(p), degree(0),
                                parent(nullptr), child(nullptr), sibling(nullptr) {}
    };

    std::vector<Node*> roots; // forest of binomial trees
    std::unordered_map<int, Node*> node_lookup;
    Node* min_root;
    int heap_size;

    // Operation counters
    int link_count;
    int swap_count;
    int root_removal_count;

    void consolidate();
    void link(Node* y, Node* z);
    void delete_all(Node* n);
};

#endif // BINOMIAL_HEAP_HPP
