// IndexedBinaryHeap.cpp
// AI-generated comparison baseline; disclose its use in the report.
#include "IndexedBinaryHeap.hpp"

#include <stdexcept>
#include <utility>

void IndexedBinaryHeap::swap_entries(std::size_t a, std::size_t b) {
    std::swap(heap_[a], heap_[b]);
    positions_[heap_[a].id] = a;
    positions_[heap_[b].id] = b;
}

void IndexedBinaryHeap::sift_up(std::size_t index) {
    while (index > 0) {
        std::size_t parent = (index - 1) / 2;

        if (heap_[parent].key <= heap_[index].key) {
            break;
        }

        swap_entries(index, parent);
        index = parent;
    }
}

void IndexedBinaryHeap::sift_down(std::size_t index) {
    // Indices below size / 2 are exactly the nodes with children.
    while (index < heap_.size() / 2) {
        std::size_t left = 2 * index + 1;
        std::size_t right = left + 1;
        std::size_t smaller = left;

        if (right < heap_.size() &&
            heap_[right].key < heap_[left].key) {
            smaller = right;
        }

        if (heap_[index].key <= heap_[smaller].key) {
            break;
        }

        swap_entries(index, smaller);
        index = smaller;
    }
}

IndexedBinaryHeap::Handle IndexedBinaryHeap::insert(int key) {
    std::size_t id = positions_.size();
    positions_.push_back(invalid);

    try {
        heap_.push_back({key, id});
    } catch (...) {
        positions_.pop_back();
        throw;
    }

    positions_[id] = heap_.size() - 1;
    sift_up(heap_.size() - 1);
    return Handle(id);
}

void IndexedBinaryHeap::decrease_key(Handle handle, int new_key) {
    if (handle.id_ >= positions_.size() ||
        positions_[handle.id_] == invalid) {
        throw std::invalid_argument("Handle is not active.");
    }

    std::size_t index = positions_[handle.id_];

    if (new_key > heap_[index].key) {
        throw std::invalid_argument(
            "New key must not exceed the current key.");
    }

    heap_[index].key = new_key;
    sift_up(index);
}

int IndexedBinaryHeap::extract_min() {
    if (empty()) {
        throw std::out_of_range("extract_min() on an empty heap.");
    }

    int value = heap_.front().key;
    std::size_t removed_id = heap_.front().id;

    swap_entries(0, heap_.size() - 1);
    heap_.pop_back();
    positions_[removed_id] = invalid;

    if (!empty()) {
        sift_down(0);
    }

    return value;
}

int IndexedBinaryHeap::minimum() const {
    if (empty()) {
        throw std::out_of_range("minimum() on an empty heap.");
    }
    return heap_.front().key;
}

bool IndexedBinaryHeap::empty() const {
    return heap_.empty();
}

std::size_t IndexedBinaryHeap::size() const {
    return heap_.size();
}

bool IndexedBinaryHeap::validate() const {
    for (std::size_t i = 0; i < heap_.size(); ++i) {
        const Entry& entry = heap_[i];

        if (entry.id >= positions_.size() ||
            positions_[entry.id] != i) {
            return false;
        }

        if (i > 0 &&
            heap_[(i - 1) / 2].key > entry.key) {
            return false;
        }
    }

    for (std::size_t id = 0; id < positions_.size(); ++id) {
        std::size_t position = positions_[id];

        if (position == invalid) {
            continue;
        }

        if (position >= heap_.size() ||
            heap_[position].id != id) {
            return false;
        }
    }

    return true;
}