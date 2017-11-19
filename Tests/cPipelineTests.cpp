#include "cPipelineTests.hpp"

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