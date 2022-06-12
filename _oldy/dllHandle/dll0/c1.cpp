#include "h1.h"
#include "state.h"
#include <iostream>
#include <vector>

#define DLL_EXPORT

using namespace std;

int Add( int a, int b )
{
    return a+b;
}

resultSimu initDefault()
{
    cout << "[]model initDefault\n";
    return resultSimu::ok;
}

model0 model;

resultSimu init(fl t0, hlpr::vecWrapper<fl> wrapper)
{
    using namespace hlpr;
    vector<fl> vec{wrapper.begin, wrapper.end+1};
    //cout << "[]model init t0=" << t0 << ", vec.size=" << vec.size() << endl;
    //cout << "[]vec=" << vec << endl;
    model.init(t0, vec);
    return resultSimu::ok;
}

resultSimu _calcStep(fl dt)
{
    model.step(dt);
    return resultSimu::ok;
}

resultSimu calcStep(fl dt, fl& t, hlpr::vecWrapper<fl>& wrapper)
{
    auto res = _calcStep(dt);
    t = model.t;
    auto& vec = model.x;
    hlpr::vecWrapper<fl> wr{&vec.front(), &vec.back()};
    wrapper = wr;
    return res;
}
