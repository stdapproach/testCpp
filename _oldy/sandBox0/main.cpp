#include <iostream>
#include <iterator>

#include "processor/Processor.h"
#include "model/M0.h"

using namespace std;

namespace {

template <typename T>
void printVec(const std::vector<T>& v, std::string post="\n") {
    std::copy(v.begin(), v.end(), std::ostream_iterator<T>(std::cout, ", "));
    cout << post;
}

template <typename T>
void printVec2(const std::vector<T>& v, std::string delim, std::string post="\n") {
    std::copy(v.begin(), v.end(), std::ostream_iterator<T>(std::cout, delim.data()));
    cout << post;
}

void printCorrectnessIC(std::tuple<bool, std::string/*why not*/> v) {
    if (std::get<0>(v)) {
        cout << "OK\n";
    } else {
        cout << "NOK, due to: " << std::get<1>(v) << endl;
    }
}

template <typename T>
void printState(const T& tpl, std::string post="\n") {
    const auto delim = "\t";
    cout << "t=" << std::get<0>(tpl) << delim;
    const auto& v = std::get<1>(tpl);
    for(auto i: v) {cout << i << delim;}
    cout << post;
}
template <typename T>
void printState2(const T& tpl, std::string delim, std::string post="\n") {
    cout << "t=" << std::get<0>(tpl) << delim;
    const auto& v = std::get<1>(tpl);
    for(auto i: v) {cout << i << delim;}
    cout << post;
}
}


int main()
{
    cout << "Hello from sandBox0!" << endl;
    using namespace sandBox;
    //Processor p;
    using namespace model;
    M0 m;
    cout << "1. modelDescr:" << m.modelDesription() << endl;
    cout << "2. dim(phaseSpace)=" << m.phaseVectorSize() << endl;
    cout << "3. descr(phaseVector): ";
    printVec(m.phaseVectorDescription());
    cout << "4. defIC: ";
    printVec(m.defaultIC());
    cout << "5. is defIC correct: ";
    printCorrectnessIC(m.checkCorrectnessIC(m.defaultIC()));
    cout << "6. if spatial speed correct: ";
    printCorrectnessIC(m.checkCorrectnessIC({-10000.0, 0}));
    cout << "7. check incorrect IC: ";
    printCorrectnessIC(m.checkCorrectnessIC({0,1,2}));
    cout << "8. parameter's description: size=" << m.paramSize() << ", ";
    printVec(m.paramDescription());
    cout << "9. digital parameters: ";
    printVec(m.params());
    cout << "10. stringParameter: " << m.strParam() << endl;
    //init
    m.init(0.5, {1,2}, m.params(), "some json");
    auto st = m.state();
    cout << "11. state after init" << endl;
    printState(st);
    cout << "12. digitParams ";
    printVec(m.params());
    cout << "13. stringParameter after init: " << m.strParam() << endl;
    {
        const auto delim = "\t";
        cout << "14. simulation\ntime[s]" << delim;
        printVec2(m.phaseVectorDescription(), delim, "");
        printVec2(m.paramDescription(), delim, "");
        cout <<" jsonDescr\n";
        const auto dt{0.01};
        for (auto i=0; i < 20; ++i) {
            m.step(dt);
            auto st = m.state();
            printState(st, ": ");
            printVec2(m.params(), delim, ": ");
            cout << m.strParam() << endl;
        }
    }

    //p.init();
    return 0;
}
