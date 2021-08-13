#ifndef ISANDBOX_H
#define ISANDBOX_H

#include <memory>

namespace model {
    struct IModel;
}

namespace sandBox {

struct ISandBox {
    virtual void init() = 0;
    virtual ~ISandBox() = default;
};
}

#endif // ISANDBOX_H
