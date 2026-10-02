#include "FibonacciHeap.hpp"
#include "IndexedBinaryHeap.hpp"
#include <cassert>
#include <stdexcept>

int main(){
    //Fibonacci Heap Tests
    FibonacciHeap fbh;
    assert(fbh.empty());
    assert(fbh.size() == 0);
    assert(fbh.validate());

    bool error0 = false;
    try{
        fbh.minimum();
    }catch(const std::out_of_range&){
        error0 = true;
    }
    assert(error0);
    assert(fbh.validate());

    bool error1 = false;
    try{
        fbh.extract_min();
    }catch(const std::out_of_range&){
        error1 = true;
    }
    assert(error1);
    assert(fbh.validate());

    fbh.insert(10);
    assert(!fbh.empty());
    assert(fbh.size() == 1);
    assert(fbh.minimum() == 10);
    assert(fbh.validate());

    auto value = fbh.extract_min();
    assert(value == 10);
    assert(fbh.size() == 0);
    assert(fbh.validate());

    fbh.insert(10);
    auto four = fbh.insert(4);
    assert(fbh.minimum() == 4);
    assert(fbh.size() == 2);
    assert(fbh.validate());

    fbh.insert(7);
    assert(fbh.minimum() == 4);
    assert(fbh.size() == 3);
    assert(fbh.validate());

    auto four2 = fbh.insert(4);
    assert(fbh.minimum() == 4);
    assert(fbh.size() == 4);
    assert(!(four == four2)); //Checking whether they have different handles.
    assert(fbh.validate());

    std::vector<int> testVec;
    std::size_t expected = fbh.size();
    while(!fbh.empty()){
        testVec.push_back(fbh.extract_min());
        --expected;
        assert(expected == fbh.size());
    }
    assert((testVec == std::vector<int>{4, 4, 7, 10}));
    assert(fbh.validate());

    fbh.insert(1);
    fbh.insert(4);
    fbh.insert(7);
    fbh.insert(10);
    fbh.insert(12);
    assert(fbh.validate());
    std::vector<int> testVec2;
    expected = fbh.size();
    while(!fbh.empty()){
        testVec2.push_back(fbh.extract_min());
        --expected;
        assert(expected == fbh.size());
    }
    assert((testVec2 == std::vector<int>{1, 4, 7, 10, 12}));
    assert(fbh.validate());

    fbh.insert(3);
    auto five = fbh.insert(5);
    auto seven = fbh.insert(7);
    auto nine = fbh.insert(9);
    auto eleven = fbh.insert(11);
    assert(fbh.validate());
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

    for(int i = 0; i < 6; ++i){
        fbh.insert(i);
    }
    auto six = fbh.insert(6);
    auto seven1 = fbh.insert(7);
    fbh.insert(8);
    int min = fbh.extract_min();
    assert(min == 0);
    fbh.decrease_key(six, -1);
    assert(fbh.validate());
    fbh.decrease_key(seven1, -2);
    assert(fbh.validate());
    std::vector<int> testVec4;
    while(!fbh.empty()){
        testVec4.push_back(fbh.extract_min());
    }
    assert((testVec4 == std::vector<int>{-2, -1, 1, 2, 3, 4, 5, 8}));

    //Indexed Binary Heap Tests
    IndexedBinaryHeap ibh;
    assert(ibh.empty());
    assert(ibh.size() == 0);
    assert(ibh.validate());

    bool error0_ibh = false;
    try{
        ibh.minimum();
    }catch(const std::out_of_range&){
        error0_ibh = true;
    }
    assert(error0_ibh);
    assert(ibh.validate());

    bool error1_ibh = false;
    try{
        ibh.extract_min();
    }catch(const std::out_of_range&){
        error1_ibh = true;
    }
    assert(error1_ibh);
    assert(ibh.validate());

    ibh.insert(10);
    assert(!ibh.empty());
    assert(ibh.size() == 1);
    assert(ibh.minimum() == 10);
    assert(ibh.validate());

    auto value_ibh = ibh.extract_min();
    assert(value_ibh == 10);
    assert(ibh.size() == 0);
    assert(ibh.validate());

    ibh.insert(10);
    auto four_ibh = ibh.insert(4);
    assert(ibh.minimum() == 4);
    assert(ibh.size() == 2);
    assert(ibh.validate());

    ibh.insert(7);
    assert(ibh.minimum() == 4);
    assert(ibh.size() == 3);
    assert(ibh.validate());

    auto four2_ibh = ibh.insert(4);
    assert(ibh.minimum() == 4);
    assert(ibh.size() == 4);
    assert(!(four_ibh == four2_ibh)); //Checking whether they have different handles.
    assert(ibh.validate());

    std::vector<int> testVec_ibh;
    std::size_t expected_ibh = ibh.size();
    while(!ibh.empty()){
        testVec_ibh.push_back(ibh.extract_min());
        --expected_ibh;
        assert(expected_ibh == ibh.size());
    }
    assert((testVec_ibh == std::vector<int>{4, 4, 7, 10}));
    assert(ibh.validate());

    ibh.insert(1);
    ibh.insert(4);
    ibh.insert(7);
    ibh.insert(10);
    ibh.insert(12);
    assert(ibh.validate());
    std::vector<int> testVec2_ibh;
    expected_ibh = ibh.size();
    while(!ibh.empty()){
        testVec2_ibh.push_back(ibh.extract_min());
        --expected_ibh;
        assert(expected_ibh == ibh.size());
    }
    assert((testVec2_ibh == std::vector<int>{1, 4, 7, 10, 12}));
    assert(ibh.validate());

    ibh.insert(3);
    auto five_ibh = ibh.insert(5);
    auto seven_ibh = ibh.insert(7);
    auto nine_ibh = ibh.insert(9);
    auto eleven_ibh = ibh.insert(11);
    assert(ibh.validate());
    //Decrease a root below the minimum
    assert(ibh.minimum() == 3);
    ibh.decrease_key(eleven_ibh, 2);
    assert(ibh.minimum() == 2);
    //Decrease to the same key
    bool error2_ibh = false;
    try{  
        ibh.decrease_key(five_ibh, 5);
    }catch(const std::invalid_argument&){
        error2_ibh = true;
    }
    assert(!error2_ibh);
    //Attempt to increase a key
    bool error3_ibh = false;
    try{  
        ibh.decrease_key(seven_ibh, 9);
    }catch(const std::invalid_argument&){
        error3_ibh = true;
    }
    assert(error3_ibh);
    //Decrease using an extracted item's handle
    ibh.extract_min();
    bool error4_ibh = false;
    try{
        ibh.decrease_key(eleven_ibh, 1);
    }catch(const std::invalid_argument){
        error4_ibh = true;
    }
    assert(error4_ibh);
    assert(ibh.minimum() == 3);
    //Decrease a child below its parent
    ibh.decrease_key(nine_ibh, 1);
    assert(ibh.minimum() == 1);
    assert(ibh.size() == 4);
    std::vector<int> testVec3_ibh;
    while(!ibh.empty()){
        testVec3_ibh.push_back(ibh.extract_min());
    }
    assert((testVec3_ibh == std::vector<int>{1, 3, 5, 7}));
    assert(ibh.empty());

    for(int i = 0; i < 6; ++i){
        ibh.insert(i);
    }
    auto six_ibh = ibh.insert(6);
    auto seven1_ibh = ibh.insert(7);
    ibh.insert(8);
    int min_ibh = ibh.extract_min();
    assert(min_ibh == 0);
    ibh.decrease_key(six_ibh, -1);
    assert(ibh.validate());
    ibh.decrease_key(seven1_ibh, -2);
    assert(ibh.validate());
    std::vector<int> testVec4_ibh;
    while(!ibh.empty()){
        testVec4_ibh.push_back(ibh.extract_min());
    }
    assert((testVec4_ibh == std::vector<int>{-2, -1, 1, 2, 3, 4, 5, 8}));
}