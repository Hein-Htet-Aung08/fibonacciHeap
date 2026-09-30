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

    fbh.insert(3);
    auto five = fbh.insert(5);
    auto seven = fbh.insert(7);
    auto nine = fbh.insert(9);
    auto eleven = fbh.insert(11);
    //Decrease a root below the minimum
    assert(fbh.minimum() == 3);
    fbh.decrease_key(eleven, 2);
    assert(fbh.minimum() == 2);
    //Decrease to the same key
    bool error2 = false;
    try{  
        fbh.decrease_key(five, 5);
    }catch(const std::invalid_argument&){
        error2 = true;
    }
    assert(!error2);
    //Attempt to increase a key
    bool error3 = false;
    try{  
        fbh.decrease_key(seven, 9);
    }catch(const std::invalid_argument&){
        error3 = true;
    }
    assert(error3);
    //Decrease using an extracted item's handle
    fbh.extract_min();
    bool error4 = false;
    try{
        fbh.decrease_key(eleven, 1);
    }catch(const std::invalid_argument){
        error4 = true;
    }
    assert(error4);
    assert(fbh.minimum() == 3);
    //Decrease a child below its parent
    fbh.decrease_key(nine, 1);
    assert(fbh.minimum() == 1);
    assert(fbh.size() == 4);
    std::vector<int> testVec3;
    while(!fbh.empty()){
        testVec3.push_back(fbh.extract_min());
    }
    assert((testVec3 == std::vector<int>{1, 3, 5, 7}));
    assert(fbh.empty());
}