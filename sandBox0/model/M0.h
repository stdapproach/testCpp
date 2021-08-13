#ifndef M0_H
#define M0_H

#include <cmath>
#include "IModel.h"

namespace model {

struct M0: IModel {
    /*
     * text decription of the model, usually json
     */
    std::string modelDesription() const override {
        return "this is a simple 1D model of free stown in Gravitational condition";
    }
    /*
     * dim(phaseVector), without time
     */
    size_t phaseVectorSize() const override {
        return 2;
    }
    /*
     * phaseVector's description, without time
     */
    std::vector<std::string> phaseVectorDescription() const override {
        return {"vx[m/s]", "x[m]"};
    }

    /*
     * default Initial Condition (IC) for this system
     */
    vec defaultIC()const override {
        return {0.0,0.0};
    }

    /*
     * check by model either particular IC correct or not.
     */
    std::tuple<bool, std::string/*why not*/> checkCorrectnessIC(const vec& v) const override {
        std::tuple<bool, std::string> res;
        if (v.size() != phaseVectorSize()) {
            return {false, "wrong size of phase vector"};
        }
        const auto absV = std::abs(v[0]);
        if (absV > 1000) {
            return {false, "speed is too fast"};// too fast speed for this model
        } else {
            return {true, ""};
        }
    }

    /*
     * parameter's description, without time
     */
    std::vector<std::string> paramDescription() const override {
        return paramDescr;
    }

    /*
     * size(parameters)
     */
    size_t paramSize() const override {
        return 3;
    }

    /*
     * current value of digital paremeters
     */
    vec params() const override {
        return m_param;
    }

    /*
     * current value of string parameter (json)
     */
    std::string strParam() const override {
        return m_strParam;
    }

    /*
     * model init: set t0, IC, digital parameters and stringParameter
     * usually been invoked from the simulation's beginning
     */
    void init(fl t0, vec IC, vec param, std::string strParam) override {
        T = t0;
        x = IC;
        m_param = param;
        m_strParam = strParam;
    }
    /*
     * change internal state
     * usually been invoked during simulation
     */
    void changeState(fl t1, vec newIC) override {
        T = t1;
        x = newIC;
    }
    /*
     * get current state
     */
    ExtPhaseVector state() const override {
        return {T, x};
    }
    /*calc one step, dt - time step[sec]*/
    void step(fl dt) override {
        vec gr{m_param[1], 0};// gravitational's influnce to phase vector
        x[0] += (A[0][0]*x[0]+A[0][1]*x[1]+gr[0])*dt;//vx
        x[1] += (A[1][0]*x[0]+A[1][1]*x[1]+gr[1])*dt;//integral(vx)
        T += dt;
    }
private:
    /*
     * current time[s]
     */
    fl T;
    /*
     * current phase vector
    */
    vec x;

    /*
     * current text description, usually json
     */
    std::string descr;

    /*
     * parameters of system
     */
    const std::vector<std::string> paramDescr{"m[kg]","g[m/s2]", "RadiusPlanet[km]"};
    vec m_param{1.0, -9.81, 10000};
    std::string m_strParam{"json for non digital parameters"};

    /*state-space matrix A*/
    const std::vector<vec> A{{0,0},{1,0}};
};

}

#endif // M0_H
