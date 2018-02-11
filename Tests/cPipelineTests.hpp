#include <iostream>
#include <iterator>
#include <cstdio>
#define TEST
#include <PatternsLib/cPipeline.hpp>
using namespace Patterns;

template < typename T >
class Interface {
public:
    virtual T get() = 0;
    virtual void set(T) = 0;
};

class A : 
    public PipelineEntry<Interface<int>> 
{
    int value;
public:
    A(int value_ = 0) : value(value_) {}
    int get() { return value; }
    void set(int newValue) { value = newValue; }
};

class B : 
    public PipelineEntry<Interface<int>> 
{
    float value;
public:
    B(float value_ = 1) : value(value_) {}
    int get() { return value; }
    void set(int newValue) { value = newValue; }
};

class C : 
    public PipelineEntry<Interface<int>>
{
    double value;
public:
    C(double value_ = 2) : value(value_) {}
    int get() { return value; }
    void set(int newValue) { value = newValue; }
};

class D : 
    public PipelineEntry<Interface<int>> 
{
    int v1, v2, v3;
public:
    D(int v1_, int v2_, int v3_) : v1(v1_), v2(v2_), v3(v3_) {}
    int get() { return v1; }
    void set(int newValue) { v1 = newValue; }
};