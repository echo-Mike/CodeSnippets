#include "cPipelineTests.hpp"
using namespace Patterns;

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

void PipelineObjectConstruction()
{
    LOG("\nStarting test.")
    Pipeline<Interface<int>> p;
    LOG("Empty pipeline object created with %d insatnces.", p.size())
    Pipeline<Interface<int>> p2{ new A, new B, new C };
    LOG("Pipeline object created with %d different object insatnces.", p2.size())

    const Pipeline<Interface<int>> p3;
    LOG("Const empty pipeline object created.")
    const Pipeline<Interface<int>> p4{ new A, new B, new C };
    LOG("Const pipeline object created with %d different object insatnces.", p4.size())

    auto p5 = new Pipeline<Interface<int>>;
    LOG("Empty pipeline object allocated on heap with %d insatnces.", p5->size())
    auto p6 = new Pipeline<Interface<int>>{ new A, new B, new C };
    LOG("Pipeline object allocated on heap with %d different object insatnces.", p6->size())
    delete p5;
    LOG("Empty pipeline object deallocated on heap.")
    delete p6;
    LOG("Pipeline object deallocated on heap.")
}

void PipelineObjectMoveConstruction()
{
    LOG("\nStarting test.")    
    Pipeline<Interface<int>> p{ new A, new B, new C };
    LOG("Pipeline object created with %d different object insatnces.", p.size())
    Pipeline<Interface<int>> p2(std::move(p));
    LOG("Pipeline object created by move constructor with %d different object insatnces. Old object size now %d.", p2.size(), p.size())
}

void PipelineObjectMoveAssignment()
{
    LOG("\nStarting test.")    
    Pipeline<Interface<int>> p{ new A, new B, new C };
    LOG("Pipeline object created with %d different object insatnces.", p.size())
    Pipeline<Interface<int>> p2 = std::move(p);
    LOG("Pipeline object created by move assignment with %d different object insatnces. Old object size now %d.", p2.size(), p.size())
}

void PushPopClearEmpty()
{
    LOG("\nStarting test.")   
    Pipeline<Interface<int>> p, p2;
    LOG("Empty pipeline object created with %d insatnces.", p.size())

    populate(100, p);
    for(std::size_t i = 0; i < 100; ++i)
        p2.push_back(p.pop_back());
    LOG("100 objects are poped back via pop_back call. Count of object now %d insatnces. Object succesfully popped %d.", p.size(), p2.size())
    p2.clear();
    LOG("100 objects are cleared via clear call. Count of object now %d insatnces.", p2.size())
    
    populate(100, p, false);
    for(std::size_t i = 0; i < 100; ++i)
        p2.push_front(p.pop_front());
    LOG("100 objects are poped front via pop_front call. Count of object now %d insatnces. Object succesfully popped %d.", p.size(), p2.size())
    p2.clear();
    LOG("100 objects are cleared via clear call. Count of object now %d insatnces.", p2.size())
    p2.clear();
    LOG("Is call to empty after call to clear returned true? : %d", p2.empty())
}

void SwapTest()
{
    LOG("\nStarting test.")   
    Pipeline<Interface<int>> a, b;
    LOG("Two empty pipeline objects created with %d and %d insatnces.", a.size(), b.size())
    populate(100, a);
    populate(200, b);
    LOG("Count of objects in a : %d. Count of objects in b : %d.", a.size(), b.size())
    std::swap(a,b);
    LOG("Count of objects in a : %d. Count of objects in b : %d.", a.size(), b.size())
}

void Insert()
{
    LOG("\nStarting test.")   
    Pipeline<Interface<int>> p;
    LOG("Empty pipeline object created with %d insatnces.", p.size())
    populate(100, p);

    auto iter2 = p.insert(p.begin(), new C);
    LOG("Object is inserted before begin. Count of object now %d insatnces.", p.size())
    LOG("Is returned iterator == begin() : %d", iter2 == p.begin())

    auto iter = p.begin();
    std::advance(iter, 58);
    auto iter3 = p.insert(iter, new C);
    LOG("Object is inserted before 58's object. Count of object now %d insatnces.", p.size())
    auto iter4 = p.begin();
    std::advance(iter4, 58);
    LOG("Is returned iterator == iterator to 58 object : %d", iter4 == iter3)

    auto iter5 = p.insert(p.end(), new C);
    LOG("Object is inserted before end(). Count of object now %d insatnces.", p.size())
    LOG("Is returned iterator == --end() : %d", iter5 == --p.end())
}

void Emplace()
{
    LOG("\nStarting test.")   
    Pipeline<Interface<int>> p;
    LOG("Empty pipeline object created with %d insatnces.", p.size())
    populate(100, p);

    {
        p.emplace_front<D>(1,2,3);
        LOG("Object is emplaced before begin via emplace_front call. Count of object now %d insatnces.", p.size())
    }
    {
        auto iter = p.begin();
        std::advance(iter, 58);
        auto iter2 = p.emplace<D>(iter, 1,2,3);
        LOG("Object is emplaced via emplace call before 58's object. Count of object now %d insatnces.", p.size())
        auto iter3 = p.begin();
        std::advance(iter3, 58);
        LOG("Is returned iterator == iterator to 58 object : %d", iter2 == iter3)
    }
    {
        p.emplace_back<D>(1,2,3);
        LOG("Object is emplaced in the end via emplace_back call. Count of object now %d insatnces.", p.size())
    }
}

void EraseSingle()
{
    LOG("\nStarting test.")   
    Pipeline<Interface<int>> p;
    LOG("Empty pipeline object created with %d insatnces.", p.size())
    populate(100, p);

    {
        auto iter = p.erase(p.begin());
        LOG("Object is erased in begin. Count of object now %d insatnces.", p.size())
        LOG("Is returned iterator == begin() : %d", iter == p.begin())
    }
    {
        auto iter = p.begin();
        std::advance(iter, 58);
        auto iter2 = iter--; // iter2 -> 58; iter -> 57
        auto iter3 = p.erase(iter);
        LOG("Object is erased in 57's place. Count of object now %d insatnces.", p.size())
        LOG("Is returned iterator == iterator to previous 58's object : %d", iter2 == iter3)
    }
    {
        auto iter = p.erase(--p.end());
        LOG("Object is erased in --end(). Count of object now %d insatnces.", p.size())
        LOG("Is returned iterator == end() : %d", iter == p.end())
    }
    {
        LOG("Count before call to erase with end iterator %d insatnces.", p.size())
        auto iter = p.erase(p.end());
        LOG("Count after call to erase with end iterator %d insatnces.", p.size())
        LOG("Is returned iterator == end() : %d", iter == p.end())
    }
}

void EraseMultiple()
{
    LOG("\nStarting test.")   
    Pipeline<Interface<int>> p;
    LOG("Empty pipeline object created with %d insatnces.", p.size())
    populate(100, p);
    
    LOG("Is call 'erase(p.end(), some_iter) returned end()? : %d", (p.erase(p.end(), p.begin()) == p.end()))
    {
        auto iter = p.begin();
        std::advance(iter, 58);
        LOG("Is call 'erase(iter, iter) returned end()? : %d. Count of object now %d insatnces.", (p.erase(iter, iter) == p.end()), p.size())
    }
    {
        auto iter = p.erase(--p.end(), p.end());
        LOG("Is call 'erase(--end(), end()) returned end()? : %d. Count of object now %d insatnces.", iter == p.end(), p.size())
    }
    {
        auto iter = p.begin();
        std::advance(iter, 89);
        auto iter2 = p.erase(iter, p.end());
        LOG("Is call 'erase(iter(89), end()) returned end()? : %d. Count of object now %d insatnces.", iter2 == p.end(), p.size())
    }
    populate(10, p);
    {
        auto iter = p.begin();
        std::advance(iter, 98);
        auto iter2 = p.erase(iter, p.begin());
        LOG("Is call 'erase(iter(98), begin()) returned end()? : %d. Count of object now %d insatnces.", iter2 == p.end(), p.size())
    }
    {
        auto iter = p.begin();
        std::advance(iter, 10);
        auto iter2 = p.erase(p.begin(), iter);
        LOG("Is call 'erase(begin(), iter(10)) returned iter(10')? : %d. Count of object now %d insatnces.", iter == p.begin(), p.size())
    }
    populate(10, p);
    {
        auto iter = p.begin();
        std::advance(iter, 51);
        auto iter2 = iter--; // iter2 -> 51; iter -> 50
        auto iter3 = p.begin();
        std::advance(iter3, 60);
        auto iter4 = p.erase(iter2, iter3);
        LOG("Is call 'erase(iter(51), iter(60)) returned iter(60')? : %d. Count of object now %d insatnces.", iter4 == iter3, p.size())
        --iter4;
        LOG("Are iter(50') and iter(60') now next to each other? : %d", iter == iter4)
    }
    {
        auto iter = p.erase(p.begin(), p.end());
        LOG("Is call 'erase(begin(), end()) returned end()? : %d. Count of object now %d insatnces.", iter == p.end(), p.size())
    }
}

int main(int /*argc*/, char** /*argv[]*/) 
{
    PipelineObjectConstruction();
    PipelineObjectMoveConstruction();
    PipelineObjectMoveAssignment();
    PushPopClearEmpty();
    SwapTest();
    Insert();
    Emplace();
    EraseSingle();
    EraseMultiple();
    std::cout << "All tests done. Press ENTER to exit.";
    std::cin.get();
    return 0;
}