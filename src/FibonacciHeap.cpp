#include "FibonacciHeap.hpp"
#include <stdexcept>

struct FibonacciHeap::Node {
    int key;
    Node* left;
    Node* right;

    explicit Node(int value)
        : key(value), left(this), right(this) {}
};

void FibonacciHeap::insert_after(Node* position, Node* node){
    Node* next = position->right;

    node->left = position;
    node->right = next;
    position->right = node;
    next->left = node;
}

FibonacciHeap::Handle FibonacciHeap::insert(int key){
    Node* node = new Node(key);

    if (min_ == nullptr){
        min_ = node;
    }else{
        insert_after(min_, node);

        if(key < min_->key){
            min_ = node;
        }
    }

    std::size_t id = nodes_.size();
    nodes_.push_back(node);
    ++size_;

    return Handle(id);
}

bool FibonacciHeap::empty() const {
    return !(size_ > 0);
}

std::size_t FibonacciHeap::size() const {
    return size_;
}

int FibonacciHeap::minimum() const {
    if(min_ == nullptr){
        throw std::out_of_range("minimum() function is called on an empty heap.");
    }

    return min_->key;
}

FibonacciHeap::~FibonacciHeap(){
    for(Node* node: nodes_){
        delete node;
    }
}
