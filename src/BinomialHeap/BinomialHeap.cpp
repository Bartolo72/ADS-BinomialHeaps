#include "BinomialHeap/BinomialHeap.h"
#include <cmath>
#include <iostream>

BinomialHeap::BinomialHeap() : min_root(nullptr), heap_size(0),
    link_count(0), swap_count(0), root_removal_count(0) {}

bool BinomialHeap::empty() const {
    return heap_size == 0;
}

int BinomialHeap::size() const {
    return heap_size;
}

int BinomialHeap::get_link_count() const {
    return link_count;
}

int BinomialHeap::get_swap_count() const {
    return swap_count;
}

int BinomialHeap::get_root_removal_count() const {
    return root_removal_count;
}

void BinomialHeap::insert(int x, double p) {
    BinomialHeap temp;
    Node* node = new Node(x, p);
    temp.roots.push_back(node);
    temp.min_root = node;
    temp.node_lookup[x] = node;
    temp.heap_size = 1;
    meld(temp);
}

int BinomialHeap::min_priority_elem() const {
    if (!min_root) throw std::runtime_error("Heap is empty");
    return min_root->key;
}

double BinomialHeap::prio(int x) const {
    if (node_lookup.find(x) == node_lookup.end())
        throw std::runtime_error("Element not in heap");
    return node_lookup.at(x)->priority;
}

void BinomialHeap::extract_min() {
    if (!min_root) throw std::runtime_error("Heap is empty");
    Node* old_min = min_root;
    root_removal_count++;

    // Remove min_root from roots
    roots.erase(std::remove(roots.begin(), roots.end(), min_root), roots.end());

    // Reverse min_root's children into a new heap
    Node* child = old_min->child;
    BinomialHeap new_heap;
    while (child) {
        Node* next = child->sibling;
        child->sibling = nullptr;
        child->parent = nullptr;
        new_heap.roots.insert(new_heap.roots.begin(), child);
        new_heap.node_lookup[child->key] = child;
        new_heap.heap_size++;
        child = next;
    }

    node_lookup.erase(old_min->key);
    delete old_min;
    heap_size--;

    meld(new_heap);
}

void BinomialHeap::decrease_prio(int x, double new_p) {
    if (node_lookup.find(x) == node_lookup.end())
        throw std::runtime_error("Element not in heap");

    Node* node = node_lookup[x];
    if (new_p > node->priority)
        throw std::runtime_error("New priority is greater than current");

    node->priority = new_p;
    Node* y = node;
    Node* z = y->parent;

    while (z && y->priority < z->priority) {
        std::swap(y->key, z->key);
        std::swap(y->priority, z->priority);
        node_lookup[y->key] = y;
        node_lookup[z->key] = z;
        y = z;
        z = y->parent;
        swap_count++;
    }

    if (!min_root || y->priority < min_root->priority)
        min_root = y;
}

void BinomialHeap::meld(BinomialHeap& other) {
    std::vector<Node*> new_roots;
    Node* carry = nullptr;

    auto it1 = roots.begin();
    auto it2 = other.roots.begin();

    int max_deg = std::max(roots.size(), other.roots.size());
    for (int d = 0; d <= max_deg || carry; ++d) {
        Node* A = (it1 != roots.end() && (*it1)->degree == d) ? *it1++ : nullptr;
        Node* B = (it2 != other.roots.end() && (*it2)->degree == d) ? *it2++ : nullptr;

        std::vector<Node*> trees = { A, B, carry };
        int count = std::count_if(trees.begin(), trees.end(), [](Node* n) { return n != nullptr; });

        if (count == 0) {
            new_roots.push_back(nullptr);
            carry = nullptr;
        } else if (count == 1) {
            carry = nullptr;
            new_roots.push_back(A ? A : (B ? B : carry));
        } else if (count == 2) {
            carry = nullptr;
            Node* T1 = A ? A : B;
            Node* T2 = (T1 == A) ? (B ? B : carry) : (A ? A : carry);
            link(T1, T2);
            carry = T1;
        } else if (count == 3) {
            new_roots.push_back(carry);
            link(A, B);
            carry = A;
        }
    }

    roots.clear();
    min_root = nullptr;

    for (Node* n : new_roots) {
        if (n) {
            roots.push_back(n);
            if (!min_root || n->priority < min_root->priority)
                min_root = n;
        }
    }

    for (auto& kv : other.node_lookup)
        node_lookup[kv.first] = kv.second;

    heap_size += other.heap_size;
    other.roots.clear();
    other.node_lookup.clear();
    other.min_root = nullptr;
    other.heap_size = 0;
}

void BinomialHeap::link(Node* y, Node* z) {
    if (y->priority > z->priority)
        std::swap(y, z);

    y->parent = z;
    y->sibling = z->child;
    z->child = y;
    z->degree++;
    link_count++;
}

void BinomialHeap::delete_all(Node* n) {
    if (!n) return;
    delete_all(n->child);
    delete_all(n->sibling);
    delete n;
}

