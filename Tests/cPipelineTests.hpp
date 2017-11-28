#include <iostream>
#include <PatternsLib/cPipeline.hpp>

using namespace Patterns;

template < typename T >
class Interface {
public:
    virtual T get() = 0;
    virtual void set(T) = 0;
};
class A : public PipelineEntry<Interface<int>> {
    int value = 0;
public:
    int get() { return value; }
    void set(int newValue) { value = newValue; }
};

class B : public PipelineEntry<Interface<int>> {
    float value = 1;
public:
    int get() { return value; }
    void set(int newValue) { value = newValue; }
};

class C : public PipelineEntry<Interface<int>> {
    double value = 2;
public:
    int get() { return value; }
    void set(int newValue) { value = newValue; }
};