#include "enginemodule.h"
#include "mechanicsengine.h"

using namespace mechanics;
EngineModule::EngineModule() : blocked(false)
{
    lock_guard<mutex> t(executionMutex);
    MechanicsEngine::getEngine()->addModule(this);
}

EngineModule::~EngineModule()
{
    if (executionMutex.try_lock())
    {
        MechanicsEngine::getEngine()->removeModule(this);
        executionMutex.unlock();
        return;
    }
    else
    {
        throw new exception();
    }
}

void EngineModule::tick()
{
    lock_guard<mutex> t(executionMutex);
    execute();
}
