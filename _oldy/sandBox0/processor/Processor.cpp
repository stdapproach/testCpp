#include "Processor.h"
#include "../model/IModel.h"

using namespace sandBox;
using namespace std;

void Processor::init()
{
    cout << "Processor::init\n";
}


void Processor::model_deleter::operator()(model::IModel *ptr)
{
    cout << "model dtr\n";
    delete ptr;
}

