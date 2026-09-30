#include "FibonacciHeap.hpp"
#include <stdexcept>
#include <utility>
#include <unordered_set>

struct FibonacciHeap::Node {
    int key;
    Node* left;
    Node* right;
    Node* parent = nullptr;
    Node* child = nullptr;
    std::size_t degree = 0;
    bool mark = false; //Not full understanding.
    std::size_t id;

    explicit Node(int value, std::size_t node_id)
        : key(value), left(this), right(this), id(node_id) {}
};

void FibonacciHeap::insert_after(Node* position, Node* node){
    Node* next = position->right;

    node->left = position;
    node->right = next;
    position->right = node;
    next->left = node;
}

FibonacciHeap::Handle FibonacciHeap::insert(int key){
    std::size_t id = nodes_.size();

    auto owned = std::make_unique<Node>(key, id); //Don't really get it but sure.
    Node* node = owned.get();

    nodes_.push_back(node);

    if (min_ == nullptr){
        min_ = node;
    }else{
        insert_after(min_, node);

        if(key < min_->key){
            min_ = node;
        }
    }

    ++size_;
    owned.release();

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

void FibonacciHeap::detach(Node* node){
    node->left->right = node->right;
    node->right->left = node->left;

    node->left = node;
    node->right = node;
}

void FibonacciHeap::link(Node* child, Node* parent){
    detach(child);

    child->parent = parent;
    child->mark = false;
    
    if(parent->child == nullptr){
        parent->child = child;
    }else{
        insert_after(parent->child, child);
    }

    ++parent->degree;
}

void FibonacciHeap::consolidate(){
    if(min_ == nullptr) return;

    std::vector<Node*> roots;
    Node* current = min_;

    do{
        roots.push_back(current);
        current = current->right;
    }while(current != min_);

    std::vector<Node*> by_degree;

    for (Node* root: roots){
        Node* x = root;
        std::size_t d = x->degree;

        while(true){
            if (d >= by_degree.size()){
                by_degree.resize(d + 1, nullptr);
            }

            if (by_degree[d] == nullptr){
                break;
            }

            Node* y = by_degree[d];

            if (x->key > y->key){
                std::swap(x, y);
            }

            link(y, x);
            by_degree[d] = nullptr;
            d = x->degree;
        }

        by_degree[d] = x;
    }

    min_ = nullptr;

    for(Node* node: by_degree){
        if(node == nullptr) continue;

        detach(node);

        if(min_ == nullptr){
            min_ = node;
        }else{
            insert_after(min_, node);
            if(node->key < min_->key){
                min_ = node;
            }
        }
    }
}

int FibonacciHeap::extract_min(){
    if(size_ == 0){
        throw std::out_of_range("The Heap is empty to return a minimum value.");
    }

    Node* z = min_;
    int value = z->key;

    std::vector<Node*> children;
    Node* directChildren = z->child;
    
    if(directChildren != nullptr){
        Node* current = directChildren;
        do{
            children.push_back(current);
            current = current->right;
        }while(current != directChildren);

        for(Node* child: children){
            detach(child);
            child->parent = nullptr;
            child->mark = false;
            insert_after(z, child);
        }
    }

    if(z->right == z){
        min_ = nullptr;
    }else{
        Node* rNeighbor = z->right;
        detach(z);
        min_ = rNeighbor;
    }

    nodes_[z->id] = nullptr;
    delete(z);
    --size_;

    if(size_ != 0){
        consolidate();
    }

    return value;
}

void FibonacciHeap::cut(Node* node, Node* parent){
    if(parent->child == node){
        if(node->right == node){
            parent->child = nullptr;
        }else{
            parent->child = node->right;
        }
    }

    detach(node);
    --parent->degree;

    node->parent = nullptr;
    node->mark = false;
    insert_after(min_, node);
}

void FibonacciHeap::cascading_cut(Node* node){
    Node* p = node->parent;

    if(p==nullptr) return;

    if(node->mark == false){
        node->mark = true;
        return;
    }else{
        cut(node, p);
        cascading_cut(p);
    }
}

void FibonacciHeap::decrease_key(Handle handle, int new_key){
    if(handle.id_ >= nodes_.size() || nodes_[handle.id_] == nullptr){
        throw std::invalid_argument("Handle does not exist in nodes.");
    }

    Node* node = nodes_[handle.id_];

    if(new_key > node->key){
        throw std::invalid_argument("The new key entered must not be greater than the current key.");
    }

    node->key = new_key;
    Node* p = node->parent;

    if(p != nullptr && node->key < p->key){
        cut(node, p);
        cascading_cut(p);
    }

    if(node->key < min_->key){
        min_ = node;
    }
}

bool FibonacciHeap::validate() const{
    if(size_==0){
        if(min_ != nullptr) return false;
        for(Node* node: nodes_){
            if(node != nullptr) return false;
        }

        return true;
    }

    if(min_ == nullptr) return false;

    std::size_t liveNodeCount = 0;
    std::unordered_set<Node*> uniqueSet;
    for(std::size_t i = 0; i < nodes_.size(); ++i){
        if(nodes_.at(i) == nullptr) continue;
        if(nodes_.at(i)->id != i) return false;
        ++liveNodeCount;
        uniqueSet.insert(nodes_.at(i));
    }
    if(liveNodeCount != size_) return false;
    if(uniqueSet.size() != size_) return false;
    if(uniqueSet.find(min_) == uniqueSet.end()) return false;

    for(Node* node: uniqueSet){
        if(node->left == nullptr) return false;
        if(node->right == nullptr) return false;

        if(uniqueSet.find(node->left) == uniqueSet.end()) return false;
        if(uniqueSet.find(node->right) == uniqueSet.end()) return false;

        if(node->left->right != node) return false;
        if(node->right->left != node) return false;

        Node* p = node->parent;
        if(p != nullptr){
            if(uniqueSet.find(p) == uniqueSet.end()) return false;
            if(p->key > node->key) return false;
        }else{
            if(node->mark) return false;
            if(node->key < min_->key) return false;
        }

        Node* c = node->child;
        if(c == nullptr){
            if(node->degree != 0) return false;
        }else{
            if(uniqueSet.find(c) == uniqueSet.end()) return false;
            if(node->degree <= 0) return false;
        }
    }

    if(min_->parent != nullptr) return false;

    std::unordered_set<Node*> visited;
    std::vector<std::pair<Node*, Node*>> worklist;
    worklist.emplace_back(min_, nullptr);

    while(!worklist.empty()){
        auto [start, expectedParent] = worklist.back();
        worklist.pop_back();

        std::size_t count = 0;
        Node* current = start;

        do{
            if(uniqueSet.find(current) == uniqueSet.end()) return false;

            if(!visited.insert(current).second) return false;

            if(current->parent != expectedParent) return false;

            if(current->child != nullptr){
                worklist.emplace_back(current->child, current);
            }

            ++count;
            current = current->right;
        }while(current != start);

        if(expectedParent != nullptr){
            if(count != expectedParent->degree) return false;
        }
    }

    if(visited.size() != uniqueSet.size()) return false;


    return true;
}