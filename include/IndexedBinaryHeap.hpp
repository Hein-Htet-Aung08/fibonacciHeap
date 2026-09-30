// IndexedBinaryHeap.hpp
// AI-generated comparison baseline; disclose its use in the report.
#pragma once

#include <cstddef>
#include <vector>

class IndexedBinaryHeap {
public:
    class Handle {
        friend class IndexedBinaryHeap;
        std::size_t id_;
        explicit Handle(std::size_t id) : id_(id) {}

    public:
        bool operator==(const Handle& other) const {
            return id_ == other.id_;
        }
    };

    IndexedBinaryHeap() = default;
    ~IndexedBinaryHeap() = default;

    IndexedBinaryHeap(const IndexedBinaryHeap&) = delete;
    IndexedBinaryHeap& operator=(const IndexedBinaryHeap&) = delete;

    Handle insert(int key);
    void decrease_key(Handle handle, int new_key);
    int extract_min();

    int minimum() const;
    bool empty() const;
    std::size_t size() const;
    bool validate() const;

private:
    struct Entry {
        int key;
        std::size_t id;
    };

    static constexpr std::size_t invalid =
        static_cast<std::size_t>(-1);

    std::vector<Entry> heap_;
    std::vector<std::size_t> positions_;

    void swap_entries(std::size_t a, std::size_t b);
    void sift_up(std::size_t index);
    void sift_down(std::size_t index);
};