#include "cPipelineTests.hpp"
using namespace Patterns;

#define TEST_PASSED(cond) if(!(cond)) throw 1;

#define AorB(i) ( ((i) % 2) ? reinterpret_cast<PipelineEntry<Interface<int>>*>(new A((i))) : reinterpret_cast<PipelineEntry<Interface<int>>*>(new B((i))) ) 

Pipeline<Interface<int>>& populate(std::size_t count, Pipeline<Interface<int>>& pipe, bool byBack = true) 
{
    if (byBack) {
        for(std::size_t i = 0; i < count; ++i)
            pipe.push_back(AorB(i));
        LOG("%d objects are pushed back via push_back call. Count of object now %d insatnces.",count, pipe.size())
    } else {
       for(std::size_t i = 0; i < count; ++i)
            pipe.push_front(AorB(i));
        LOG("%d objects are pushed back via push_front call. Count of object now %d insatnces.",count, pipe.size())
    }
    return pipe;
}

// Total test count: 6 @ Total: 6
std::size_t PipelineObjectConstruction()
{
    std::size_t result = 0;
    LOG("\nStarting test. Total test count: 6")
    try {
        Pipeline<Interface<int>> p;
        ++result;
        LOG("Empty pipeline object created with %d insatnces.", p.size())
    }
    catch(...) {
        LOG("\nTest 1 not passed.")
    }
    
    try {
        Pipeline<Interface<int>> p{ new A, new B, new C };
        ++result;
        LOG("Pipeline object created with %d different object insatnces via initializer list constructor.", p.size())
    }
    catch(...) {
        LOG("\nTest 2 not passed.")
    }
    
    try {
        const Pipeline<Interface<int>> p;
        ++result;
        LOG("Const empty pipeline object created with %d insatnces.", p.size())
    }
    catch(...) {
        LOG("\nTest 3 not passed.")
    }

    try {
        const Pipeline<Interface<int>> p{ new A, new B, new C };
        ++result;
        LOG("Const pipeline object created with %d different object insatnces via initializer list constructor.", p.size())
    }
    catch(...) {
        LOG("\nTest 4 not passed.")
    }

    try {
        auto p = new Pipeline<Interface<int>>;
        LOG("Empty pipeline object allocated on heap with %d insatnces.", p->size())
        delete p;
        LOG("Empty pipeline object deallocated on heap.")
        ++result;
    }
    catch(...) {
        LOG("\nTest 5 not passed.")
    }
    
    try {
        auto p = new Pipeline<Interface<int>>{ new A, new B, new C };
        LOG("Pipeline object allocated on heap with %d different object insatnces via initializer list constructor.", p->size())
        delete p;
        LOG("Empty pipeline object deallocated on heap.")
        ++result;
    }
    catch(...) {
        LOG("\nTest 6 not passed.")
    }
    LOG("Passed tests count: %d\n", result)
    return result;
}
// Total test count: 1 @ Total: 7
std::size_t PipelineObjectMoveConstruction()
{
    std::size_t result = 0;
    LOG("\nStarting test. Total test count: 1")
    try {
        Pipeline<Interface<int>> p{ new A, new B, new C };
        LOG("Pipeline object created with %d different object insatnces.", p.size())
        Pipeline<Interface<int>> p2(std::move(p));
        LOG("Pipeline object created by move constructor with %d different object insatnces. Old object size now %d.", p2.size(), p.size())
        ++result;
    }
    catch(...) {
        LOG("\nTest 1 not passed.")
    }
    LOG("Passed tests count: %d\n", result)
    return result;
}
// Total test count: 1 @ Total: 8
std::size_t PipelineObjectMoveAssignment()
{
    std::size_t result = 0;
    LOG("\nStarting test. Total test count: 1")
    try {   
        Pipeline<Interface<int>> p{ new A, new B, new C };
        LOG("Pipeline object created with %d different object insatnces.", p.size())
        Pipeline<Interface<int>> p2 = std::move(p);
        LOG("Pipeline object created by move assignment with %d different object insatnces. Old object size now %d.", p2.size(), p.size())
        ++result;
    }
    catch(...) {
        LOG("\nTest 1 not passed.")
    }
    LOG("Passed tests count: %d\n", result)
    return result;
}
// Total test count: 3 @ Total: 11
std::size_t PushPopClearEmpty()
{
    std::size_t result = 0;
    LOG("\nStarting test. Total test count: 3")
    try {
        Pipeline<Interface<int>> p, p2;
        LOG("Empty pipeline object created with %d insatnces.", p.size())
        populate(100, p);
        TEST_PASSED(p.size() == 100)
        for(std::size_t i = 0; i < 100; ++i)
            p2.push_back(p.pop_back());
        TEST_PASSED(p.size() == 0 && p2.size() == 100)
        LOG("100 objects are poped back via pop_back call. Count of object now %d insatnces. Object succesfully popped %d.", p.size(), p2.size())
        p2.clear();
        TEST_PASSED(p2.size() == 0)
        LOG("100 objects are cleared via clear call. Count of object now %d insatnces.", p2.size())
        ++result;
    }
    catch(...) {
        LOG("\nTest 1 not passed.")
    }

    try {
        Pipeline<Interface<int>> p, p2;
        LOG("Empty pipeline object created with %d insatnces.", p.size())
        populate(100, p, false);
        TEST_PASSED(p.size() == 100)
        for(std::size_t i = 0; i < 100; ++i)
            p2.push_front(p.pop_front());
        TEST_PASSED(p.size() == 0 && p2.size() == 100)
        LOG("100 objects are poped front via pop_front call. Count of object now %d insatnces. Object succesfully popped %d.", p.size(), p2.size())
        p2.clear();
        TEST_PASSED(p2.size() == 0)
        LOG("100 objects are cleared via clear call. Count of object now %d insatnces.", p2.size())
        ++result;
        TEST_PASSED(p2.empty())
        LOG("Is call to empty after call to clear returned true? : 1")
        ++result;
    }
    catch(...) {
        LOG("\nTest 2 or 3 not passed.")
    }

    LOG("Passed tests count: %d\n", result)
    return result;
}
// Total test count: 1 @ Total: 12
std::size_t SwapTest()
{
    std::size_t result = 0;
    LOG("\nStarting test. Total test count: 1")
    try {
        Pipeline<Interface<int>> a, b;
        LOG("Two empty pipeline objects created with %d and %d insatnces.", a.size(), b.size())
        populate(100, a);
        populate(200, b);
        TEST_PASSED(a.size() == 100 && b.size() == 200)
        LOG("Count of objects in a : %d. Count of objects in b : %d.", a.size(), b.size())
        std::swap(a,b);
        TEST_PASSED(a.size() == 200 && b.size() == 100)
        LOG("Count of objects in a : %d. Count of objects in b : %d.", a.size(), b.size())
        ++result;
    }
    catch(...) {
        LOG("\nTest 1 not passed.")
    }
    LOG("Passed tests count: %d\n", result)
    return result;
}
// Total test count: 3 @ Total: 15
std::size_t Insert()
{
    std::size_t result = 0;
    LOG("\nStarting test. Total test count: 3")
    try {
        Pipeline<Interface<int>> p;
        LOG("Empty pipeline object created with %d insatnces.", p.size())
        populate(100, p);
        TEST_PASSED(p.size() == 100)
        auto iter = p.insert(p.begin(), new C);
        TEST_PASSED(p.size() == 101)
        LOG("Object is inserted before begin. Count of object now %d insatnces.", p.size())
        TEST_PASSED(iter == p.begin())
        LOG("Is returned iterator == begin() : 1")
        ++result;
    }
    catch(...) {
        LOG("\nTest 1 not passed.")
    }

    try {
        Pipeline<Interface<int>> p;
        LOG("Empty pipeline object created with %d insatnces.", p.size())
        populate(100, p);
        TEST_PASSED(p.size() == 100)
        auto iter = p.begin();
        std::advance(iter, 58);
        auto iter2 = p.insert(iter, new C);
        TEST_PASSED(p.size() == 101)
        LOG("Object is inserted before 58's object. Count of object now %d insatnces.", p.size())
        auto iter3 = p.begin();
        std::advance(iter3, 58);
        TEST_PASSED(iter2 == iter3)
        LOG("Is returned iterator == iterator to 58 object : 1")
        ++result;
    }
    catch(...) {
        LOG("\nTest 2 not passed.")
    }
    
    try {
        Pipeline<Interface<int>> p;
        LOG("Empty pipeline object created with %d insatnces.", p.size())
        populate(100, p);
        TEST_PASSED(p.size() == 100)
        auto iter = p.insert(p.end(), new C);
        TEST_PASSED(p.size() == 101)
        LOG("Object is inserted before end(). Count of object now %d insatnces.", p.size())
        TEST_PASSED(iter == --p.end())
        LOG("Is returned iterator == --end() : 1")
        ++result;
    }
    catch(...) {
        LOG("\nTest 3 not passed.")
    }
    LOG("Passed tests count: %d\n", result)
    return result;
}
// Total test count: 3 @ Total: 18
std::size_t Emplace()
{
    std::size_t result = 0;
    LOG("\nStarting test. Total test count: 3")
    try {
        Pipeline<Interface<int>> p;
        LOG("Empty pipeline object created with %d insatnces.", p.size())
        populate(100, p);
        TEST_PASSED(p.size() == 100);
        p.emplace_front<D>(1,2,3);
        TEST_PASSED(p.size() == 101);
        LOG("Object is emplaced before begin via emplace_front call. Count of object now %d insatnces.", p.size())
        ++result;
    }
    catch(...) {
        LOG("\nTest 1 not passed.")
    }

    try {
        Pipeline<Interface<int>> p;
        LOG("Empty pipeline object created with %d insatnces.", p.size())
        populate(100, p);
        TEST_PASSED(p.size() == 100);
        auto iter = p.begin();
        std::advance(iter, 58);
        auto iter2 = p.emplace<D>(iter, 1,2,3);
        TEST_PASSED(p.size() == 101);
        LOG("Object is emplaced via emplace call before 58's object. Count of object now %d insatnces.", p.size())
        auto iter3 = p.begin();
        std::advance(iter3, 58);
        TEST_PASSED(iter2 == iter3);
        LOG("Is returned iterator == iterator to 58 object : 1")
        ++result;
    }
    catch(...) {
        LOG("\nTest 2 not passed.")
    }

    try {
        Pipeline<Interface<int>> p;
        LOG("Empty pipeline object created with %d insatnces.", p.size())
        populate(100, p);
        TEST_PASSED(p.size() == 100);
        p.emplace_back<D>(1,2,3);
        TEST_PASSED(p.size() == 101);
        LOG("Object is emplaced in the end via emplace_back call. Count of object now %d insatnces.", p.size())
        ++result;
    }
    catch(...) {
        LOG("\nTest 3 not passed.")
    }
    LOG("Passed tests count: %d\n", result)
    return result;
}
// Total test count: 4 @ Total: 22
std::size_t EraseSingle()
{
    std::size_t result = 0;
    LOG("\nStarting test. Total test count: 4")
    try {
        Pipeline<Interface<int>> p;
        LOG("Empty pipeline object created with %d insatnces.", p.size())
        populate(100, p);
        TEST_PASSED(p.size() == 100);
        auto iter = p.erase(p.begin());
        TEST_PASSED(p.size() == 99);
        LOG("Object is erased in begin. Count of object now %d insatnces.", p.size())
        TEST_PASSED(iter == p.begin());
        LOG("Is returned iterator == begin() : 1")
        ++result;
    }
    catch(...) {
        LOG("\nTest 1 not passed.")
    }

    try {
        Pipeline<Interface<int>> p;
        LOG("Empty pipeline object created with %d insatnces.", p.size())
        populate(100, p);
        TEST_PASSED(p.size() == 100);
        auto iter = p.begin();
        std::advance(iter, 58);
        auto iter2 = iter--; // iter2 -> 58; iter -> 57
        auto iter3 = p.erase(iter);
        TEST_PASSED(p.size() == 99);
        LOG("Object is erased in 57's place. Count of object now %d insatnces.", p.size())
        TEST_PASSED(iter2 == iter3);
        LOG("Is returned iterator == iterator to previous 58's object : 1")
        ++result;
    }
    catch(...) {
        LOG("\nTest 2 not passed.")
    }

    try {
        Pipeline<Interface<int>> p;
        LOG("Empty pipeline object created with %d insatnces.", p.size())
        populate(100, p);
        TEST_PASSED(p.size() == 100);
        auto iter = p.erase(--p.end());
        TEST_PASSED(p.size() == 99);
        LOG("Object is erased in --end(). Count of object now %d insatnces.", p.size())
        TEST_PASSED(iter == p.end());
        LOG("Is returned iterator == end() : 1")
        ++result;
    }
    catch(...) {
        LOG("\nTest 3 not passed.")
    }

    try {
        Pipeline<Interface<int>> p;
        LOG("Empty pipeline object created with %d insatnces.", p.size())
        populate(100, p);
        TEST_PASSED(p.size() == 100);
        LOG("Count before call to erase with end iterator %d insatnces.", p.size())
        auto iter = p.erase(p.end());
        TEST_PASSED(p.size() == 100);
        LOG("Count after call to erase with end iterator %d insatnces.", p.size())
        TEST_PASSED(iter == p.end());
        LOG("Is returned iterator == end() : 1")
        ++result;
    }
    catch(...) {
        LOG("\nTest 4 not passed.")
    }
    LOG("Passed tests count: %d\n", result)
    return result;
}
// Total test count: 8 @ Total: 30
std::size_t EraseMultiple()
{
    std::size_t result = 0;
    LOG("\nStarting test. Total test count: 8") 
    try {
        Pipeline<Interface<int>> p;
        LOG("Empty pipeline object created with %d insatnces.", p.size())
        populate(10, p);
        TEST_PASSED(p.size() == 10);
        auto iter = p.erase(p.end(), p.begin());
        TEST_PASSED(iter == p.end());
        LOG("Is call 'erase(p.end(), some_iter) returned end()? : 1")
        ++result;
    }
    catch(...) {
        LOG("\nTest 1 not passed.")
    }

    try {
        Pipeline<Interface<int>> p;
        LOG("Empty pipeline object created with %d insatnces.", p.size())
        populate(100, p);
        TEST_PASSED(p.size() == 100);
        auto iter = p.begin();
        std::advance(iter, 58);
        auto iter2 = p.erase(iter, iter);
        TEST_PASSED(iter2 == p.end() && p.size() == 100);
        LOG("Is call 'erase(iter, iter) returned end()? : 1. Count of object now %d insatnces.", p.size())
        ++result;
    }
    catch(...) {
        LOG("\nTest 2 not passed.")
    }

    try {
        Pipeline<Interface<int>> p;
        LOG("Empty pipeline object created with %d insatnces.", p.size())
        populate(100, p);
        TEST_PASSED(p.size() == 100);
        auto iter = p.erase(--p.end(), p.end());
        TEST_PASSED(iter == p.end() && p.size() == 99);
        LOG("Is call 'erase(--end(), end()) returned end()? : 1. Count of object now %d insatnces.", p.size())
        ++result;
    }
    catch(...) {
        LOG("\nTest 3 not passed.")
    }

    try {
        Pipeline<Interface<int>> p;
        LOG("Empty pipeline object created with %d insatnces.", p.size())
        populate(100, p);
        TEST_PASSED(p.size() == 100);
        auto iter = p.begin();
        std::advance(iter, 89);
        auto iter2 = p.erase(iter, p.end());
        TEST_PASSED(iter2 == p.end() && p.size() == 89);
        LOG("Is call 'erase(iter(89), end()) returned end()? : 1. Count of object now %d insatnces.", p.size())
        ++result;
    }
    catch(...) {
        LOG("\nTest 4 not passed.")
    }

    try {
        Pipeline<Interface<int>> p;
        LOG("Empty pipeline object created with %d insatnces.", p.size())
        populate(100, p);
        TEST_PASSED(p.size() == 100);
        auto iter = p.begin();
        std::advance(iter, 99);
        auto iter2 = p.erase(iter, p.begin());
        TEST_PASSED(iter2 == p.end() && p.size() == 100);
        LOG("Is call 'erase(iter(99), begin()) returned end()? : 1. Count of object now %d insatnces.", p.size())
        ++result;
    }
    catch(...) {
        LOG("\nTest 5 not passed.")
    }

    try {
        Pipeline<Interface<int>> p;
        LOG("Empty pipeline object created with %d insatnces.", p.size())
        populate(100, p);
        TEST_PASSED(p.size() == 100);
        auto iter = p.begin();
        std::advance(iter, 10);
        auto iter2 = p.erase(p.begin(), iter);
        TEST_PASSED(iter2 == p.begin() && p.size() == 90);
        LOG("Is call 'erase(begin(), iter(10)) returned iter(10')? : 1. Count of object now %d insatnces.", p.size())
        ++result;
    }
    catch(...) {
        LOG("\nTest 6 not passed.")
    }

    try {
        Pipeline<Interface<int>> p;
        LOG("Empty pipeline object created with %d insatnces.", p.size())
        populate(100, p);
        TEST_PASSED(p.size() == 100);
        auto iter = p.begin();
        std::advance(iter, 51);
        auto iter2 = iter--; // iter2 -> 51; iter -> 50
        auto iter3 = p.begin();
        std::advance(iter3, 60);
        auto iter4 = p.erase(iter2, iter3);
        TEST_PASSED(iter4 == iter3 && p.size() == 91);
        LOG("Is call 'erase(iter(51), iter(60)) returned iter(60')? : 1. Count of object now %d insatnces.", p.size())
        --iter4;
        TEST_PASSED(iter == iter4);
        LOG("Are iter(50') and iter(60') now next to each other? : 1")
        ++result;
    }
    catch(...) {
        LOG("\nTest 7 not passed.")
    }

    try {
        Pipeline<Interface<int>> p;
        LOG("Empty pipeline object created with %d insatnces.", p.size())
        populate(100, p);
        TEST_PASSED(p.size() == 100);
        auto iter = p.erase(p.begin(), p.end());
        TEST_PASSED(iter == p.end() && p.size() == 0);
        LOG("Is call 'erase(begin(), end()) returned end()? : 1. Count of object now %d insatnces.", p.size())
        ++result;
    }
    catch(...) {
        LOG("\nTest 8 not passed.")
    }

    LOG("Passed tests count: %d\n", result)
    return result;
}

int main(int /*argc*/, char** /*argv[]*/) 
{
    std::size_t result = 0, test_count = 30;
    result += PipelineObjectConstruction();
    result += PipelineObjectMoveConstruction();
    result += PipelineObjectMoveAssignment();
    result += PushPopClearEmpty();
    result += SwapTest();
    result += Insert();
    result += Emplace();
    result += EraseSingle();
    result += EraseMultiple();
    LOG("\nTotal tests passed %d out of %d", result, test_count)
    std::cout << "All tests done. Total passed tests " << result << " out of " << test_count << ". Press ENTER to exit.";
    std::cin.get();
    return 0;
}