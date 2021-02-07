#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <iostream>
#include "ISandBox.h"

namespace model {
struct IModel;
}

namespace sandBox {

struct Processor: ISandBox {
    void init() override;

    struct model_deleter {
        void operator()(model::IModel* ptr);
    };

    std::unique_ptr<model::IModel, model_deleter> model;
    ~Processor()=default;
};
}

#endif // PROCESSOR_H
