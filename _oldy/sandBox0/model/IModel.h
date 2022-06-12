#ifndef IMODEL_H
#define IMODEL_H

#include <string>
#include <vector>
#include <tuple>

namespace model {

struct IModel {
    typedef double fl;
    typedef std::vector<fl> vec;
    /*extended phase vector*/
    typedef std::tuple<fl /*time*/, vec /*phaseVec*/> ExtPhaseVector;

    /*
     * text decription of the model, usually json
     */
    virtual std::string modelDesription() const = 0;

    /*
     * phaseVector's description, without time
     */
    virtual std::vector<std::string> phaseVectorDescription() const = 0;

    /*
     * dim(phaseVector), without time
     */
    virtual size_t phaseVectorSize() const = 0;

    /*
     * default Initial Condition (IC) for this system
     */
    virtual vec defaultIC()const = 0;

    /*
     * check by model either particular IC correct or not.
     */
    virtual std::tuple<bool, std::string/*why not*/> checkCorrectnessIC(const vec& v) const = 0;

    /*
     * parameter's description, without time
     */
    virtual std::vector<std::string> paramDescription() const = 0;

    /*
     * size(parameters)
     */
    virtual size_t paramSize() const = 0;

    /*
     * current value of digital paremeters
     */
    virtual vec params() const = 0;

    /*
     * current value of string parameter (json)
     */
    virtual std::string strParam() const = 0;

    /*
     * model init: set t0, IC, digital parameters and stringParameter
     * usually been invoked from the simulation's beginning
     */
    virtual void init(fl t0, vec IC, vec param, std::string strParam) = 0;

    /*
     * change internal state
     * usually been invoked during simulation
     */
    virtual void changeState(fl t1, vec newIC) = 0;

    /*
     * get current state
     */
    virtual ExtPhaseVector state() const = 0;

    /*calc one step, dt - time step[sec]*/
    virtual void step(fl dt) = 0;


    //virtual void init() = 0;
    virtual ~IModel() = default;
};

}

#endif // IMODEL_H
