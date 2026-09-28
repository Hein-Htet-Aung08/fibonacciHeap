#include "FibonacciHeap.hpp"
#include <cassert>
#include <stdexcept>

int main(){
    FibonacciHeap fbh;
    assert(fbh.empty());
    assert(fbh.size() == 0);

    bool error = false;
    try{
        fbh.minimum();
    }catch(const std::out_of_range&){
        error = true;
    }
    assert(error);

    fbh.insert(10);
    assert(!fbh.empty());
    assert(fbh.size() == 1);
    assert(fbh.minimum() == 10);

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

}