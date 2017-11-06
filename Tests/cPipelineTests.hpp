#include <iostream>
#include <PatternsLib/cPipeline.hpp>

using namespace Patterns;

template < typename T >
class Interface {
public:
    virtual T get() = 0;
    virtual void set(T) = 0;
};
