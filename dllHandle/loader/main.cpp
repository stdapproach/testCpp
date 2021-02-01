#include <iostream>
#include <vector>

#include "dllHandler.h"
#include "../dll0/h1.h"

using namespace std;

int main()
{
    cout << "Hello World_load Dll!" << endl;

    tmp::dlib dl("./libdll0.so", RTLD_LAZY);
    if(!dl) {
        cerr << "NOT LOADED" << endl;
        exit(1);
    }
    typedef resultSimu (*F_init)(fl t0, hlpr::vecWrapper<fl> wrapper);
    auto fInit = dl.resolve<F_init>("init");
    {
        if(!fInit) {
            cerr << "function " << fInit.name() <<" isn't loaded" << endl;
        } else {
            using namespace hlpr;
            const fl t0{0};
            const fl x0{2};
            const fl vx0{1};
            vector<fl> vec{vx0,x0};
            hlpr::vecWrapper<fl> wr{&vec.front(), &vec.back()};
            fInit.f(t0, wr);
            cout << "{init}t0=" << t0
                      << ", vx0=" << vx0
                      << ", x0=" << x0 << std::endl;
        }
    }
    {
        typedef resultSimu (*F_calcStep)(fl dt, fl& t, hlpr::vecWrapper<fl>& wrapper);
        auto f2 = dl.resolve<F_calcStep>("calcStep");
        if(!f2) {
            cerr << "function " << f2.name() <<" isn't loaded" << endl;
        } else {
            const fl dt = 0.01;
            fl t;
            hlpr::vecWrapper<fl> wrapper;
            for(int i=0; i<10; i++) {
                f2.f(dt, t, wrapper);
                vector<fl> vec{wrapper.begin, wrapper.end+1};
                std::cout << "{step}t=" << t
                          << ", vx=" << vec[0]
                          << ", x=" << vec[1] << std::endl;
            }
        }
    }
    return 0;
}
