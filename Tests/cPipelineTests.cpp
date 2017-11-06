#include "cPipelineTests.hpp"

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

struct Holder {
    Pipeline<Interface<int>> p;
    Holder() {
        p.push_back(new A);
        p.push_back(new B);
        p.push_back(new C);
    }
};

int main(int /*argc*/, char** /*argv[]*/) 
{
    Holder h;
    auto iter = h.p.begin();
    if (iter)
        do {
            std::cout << iter->get() << "\n";
            ++iter;
        } while (iter != h.p.end());

    std::cin.get();
    return 0;
}