#include "FibonacciHeap.hpp"
#include <cassert>
#include <stdexcept>

int main(){
    FibonacciHeap fbh;
    assert(fbh.empty());
    assert(fbh.size() == 0);

    bool error0 = false;
    try{
        fbh.minimum();
    }catch(const std::out_of_range&){
        error0 = true;
    }
    assert(error0);

    bool error1 = false;
    try{
        fbh.extract_min();
    }catch(const std::out_of_range&){
        error1 = true;
    }
    assert(error1);

    fbh.insert(10);
    assert(!fbh.empty());
    assert(fbh.size() == 1);
    assert(fbh.minimum() == 10);

    auto value = fbh.extract_min();
    assert(value == 10);
    assert(fbh.size() == 0);

    fbh.insert(10);
    auto four = fbh.insert(4);
    assert(fbh.minimum() == 4);
    assert(fbh.size() == 2);

    fbh.insert(7);
    assert(fbh.minimum() == 4);
    assert(fbh.size() == 3);

    auto four2 = fbh.insert(4);
    assert(fbh.minimum() == 4);
    assert(fbh.size() == 4);
    assert(!(four == four2)); //Checking whether they have different handles.

    std::vector<int> testVec;
    std::size_t expected = fbh.size();
    while(!fbh.empty()){
        testVec.push_back(fbh.extract_min());
        --expected;
        assert(expected == fbh.size());
    }
    assert((testVec == std::vector<int>{4, 4, 7, 10}));

    fbh.insert(1);
    fbh.insert(4);
    fbh.insert(7);
    fbh.insert(10);
    fbh.insert(12);
    std::vector<int> testVec2;
    expected = fbh.size();
    while(!fbh.empty()){
        testVec2.push_back(fbh.extract_min());
        --expected;
        assert(expected == fbh.size());
    }
    assert((testVec2 == std::vector<int>{1, 4, 7, 10, 12}));
}