#include "persistentaction.h"

using namespace mechanics;

PersistentAction::~PersistentAction(){}

int PersistentAction::execute()
{
    int val(triggered());
    deleteMe = false;
    return val;
}

