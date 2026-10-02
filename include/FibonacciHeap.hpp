#pragma once
#include <cstddef>
#include <vector>

class FibonacciHeap {

    public:
        int minimum() const;
        bool empty() const;
        std::size_t size() const;
        bool validate() const;

        class Handle {
            friend class FibonacciHeap;
            std::size_t id_;
            explicit Handle(std::size_t id) : id_(id) {}

        public:
            bool operator==(const Handle& other) const {
                return id_ == other.id_;
            }
        };

        Handle insert(int key);
        void decrease_key(Handle handle, int new_key);

        FibonacciHeap() = default;
        ~FibonacciHeap();

        FibonacciHeap(const FibonacciHeap&) = delete;
        FibonacciHeap& operator=(const FibonacciHeap&) = delete;

        int extract_min();

    private:
        struct Node;
        Node* min_ = nullptr;
        std::size_t size_ = 0;

        static void insert_after(Node* position, Node* node);
        std::vector<Node*> nodes_;

        static void detach(Node* node);
        static void link(Node* child, Node* parent);
        void consolidate();
        void cut(Node* node, Node* parent);
        void cascading_cut(Node* node);

};