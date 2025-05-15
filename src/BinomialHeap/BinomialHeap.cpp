#include "BinomialHeap/BinomialHeap.h"
#include <stdexcept>
#include <iostream>
#include <cassert>

BinomialHeap::BinomialHeap() : head(nullptr), heap_size(0) {}

bool BinomialHeap::empty() const {
    return head == nullptr;
}

int BinomialHeap::size() const {
    return heap_size;
}

void BinomialHeap::insert(int key, double priority) {
    BinomialHeap temp;
    temp.head = new BinomialNode(key, priority);
    temp.node_map[key] = temp.head;
    temp.heap_size = 1;
    meld(temp);
}

int BinomialHeap::min_priority_elem() const {
    if (empty()) throw std::runtime_error("Heap is empty");

    BinomialNode* y = nullptr;
    BinomialNode* x = head;
    double min = INFINITY_WEIGHT;

    while (x != nullptr) {
        if (x->priority < min) {
            min = x->priority;
            y = x;
        }
        x = x->sibling;
    }

    return y->key;
}

double BinomialHeap::prio(int key) const {
    auto it = node_map.find(key);
    if (it == node_map.end()) throw std::runtime_error("Key not found");
    return it->second->priority;
}

void BinomialHeap::extract_min() {
    if (empty()) throw std::runtime_error("Heap is empty");

    BinomialNode* prev_min = nullptr;
    BinomialNode* min_node = head;
    BinomialNode* prev = nullptr;
    BinomialNode* curr = head;
    double min = curr->priority;

    while (curr != nullptr) {
        if (curr->priority < min) {
            min = curr->priority;
            prev_min = prev;
            min_node = curr;
        }
        prev = curr;
        curr = curr->sibling;
    }

    // Remove min_node from root list
    if (prev_min != nullptr)
        prev_min->sibling = min_node->sibling;
    else
        head = min_node->sibling;

    // Reverse child list
    BinomialNode* child = min_node->child;
    BinomialNode* rev_child = nullptr;
    while (child != nullptr) {
        BinomialNode* next = child->sibling;
        child->sibling = rev_child;
        child->parent = nullptr;
        rev_child = child;
        child = next;
    }

    BinomialHeap temp;
    temp.head = rev_child;
    temp.heap_size = (1 << min_node->degree) - 1; // Approximate

    node_map.erase(min_node->key);
    delete min_node;

    meld(temp);
    heap_size--;
}

void BinomialHeap::decrease_prio(int key, double new_priority) {
    auto it = node_map.find(key);
    if (it == node_map.end()) throw std::runtime_error("Key not found");
    BinomialNode* x = it->second;

    if (new_priority > x->priority)
        throw std::invalid_argument("New priority is greater than current priority");

    x->priority = new_priority;
    BinomialNode* y = x;
    BinomialNode* z = y->parent;

    while (z != nullptr && y->priority < z->priority) {
        std::swap(y->key, z->key);
        std::swap(y->priority, z->priority);

        node_map[y->key] = y;
        node_map[z->key] = z;

        y = z;
        z = y->parent;
    }
}

void BinomialHeap::meld(BinomialHeap& other) {
    head = union_heaps(head, other.head);
    heap_size += other.heap_size;

    for (const auto& entry : other.node_map)
        node_map[entry.first] = entry.second;

    other.head = nullptr;
    other.heap_size = 0;
    other.node_map.clear();
}

// Internal helpers
BinomialNode* BinomialHeap::merge_roots(BinomialNode* h1, BinomialNode* h2) {
    if (!h1) return h2;
    if (!h2) return h1;

    BinomialNode* head = nullptr;
    BinomialNode** pos = &head;

    while (h1 && h2) {
        if (h1->degree < h2->degree) {
            *pos = h1;
            h1 = h1->sibling;
        } else {
            *pos = h2;
            h2 = h2->sibling;
        }
        pos = &((*pos)->sibling);
    }

    *pos = h1 ? h1 : h2;
    return head;
}

void BinomialHeap::link_trees(BinomialNode* y, BinomialNode* z) {
    y->parent = z;
    y->sibling = z->child;
    z->child = y;
    z->degree++;
}

BinomialNode* BinomialHeap::union_heaps(BinomialNode* h1, BinomialNode* h2) {
    BinomialNode* new_head = merge_roots(h1, h2);
    if (!new_head) return nullptr;

    BinomialNode* prev = nullptr;
    BinomialNode* x = new_head;
    BinomialNode* next = x->sibling;

    while (next != nullptr) {
        if ((x->degree != next->degree) ||
            (next->sibling != nullptr && next->sibling->degree == x->degree)) {
            prev = x;
            x = next;
        } else if (x->priority <= next->priority) {
            x->sibling = next->sibling;
            link_trees(next, x);
        } else {
            if (prev == nullptr)
                new_head = next;
            else
                prev->sibling = next;

            link_trees(x, next);
            x = next;
        }
        next = x->sibling;
    }

    return new_head;
}
