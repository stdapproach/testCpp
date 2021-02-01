#ifndef STATE_H
#define STATE_H

#include <vector>
#include <iostream>

//state-space related model
struct model0
{// one degree stown with free fall acceleration
    typedef double fl;
    typedef std::vector<fl> vec;

    const vec gr{-9.81, 0};
    const std::vector<vec> A{{0,0},{1,0}};
    vec x;
    fl t;

    void init(fl t0, vec x0)
    {
        t = t0;
        x = x0;//vx->x[0], x -> x[1]
    }

    void step(fl dt)
    {
        x[0] += (A[0][0]*x[0]+A[0][1]*x[1]+gr[0])*dt;//vx
        x[1] += (A[1][0]*x[0]+A[1][1]*x[1]+gr[1])*dt;//integral(vx)
        t += dt;
    }

};

#endif // STATE_H
