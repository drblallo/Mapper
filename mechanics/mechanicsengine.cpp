#include "mechanicsengine.h"
#include <thread>
#include <QElapsedTimer>
#include <iostream>
#include <wait.h>
#include <chrono>
#include <algorithm>
#include "action.h"

using namespace mechanics;
using namespace std;

MechanicsEngine* MechanicsEngine::engine(NULL);
bool MechanicsEngine::end(false);
int MechanicsEngine::minTickTime(1);


MechanicsEngine::MechanicsEngine() //: tickElapsed(0)
{

}

void MechanicsEngine::StartEngine()
{
    if (engine)
        return;

    engine = new MechanicsEngine();
    std::thread t(&StartThread);
    t.detach();
}

void MechanicsEngine::StartThread()
{
    engine->loop();
}


void MechanicsEngine::loop()
{
    QElapsedTimer t;
    while (!end)
    {
        t.start();
        tick();
        if (!t.hasExpired(minTickTime*1000))
        {
            int time((minTickTime*1000) - t.elapsed());
            std::this_thread::sleep_for(std::chrono::milliseconds(time));
        }
    }
}

void MechanicsEngine::tick()
{
   // tickElapsed++;
    std::lock_guard<std::mutex> t(engineMutex);
    clearUnused();
    for (unsigned int a = 0; a < modules.size(); a++)
        if (
                (find(toRemove.begin(), toRemove.end(), modules[a]) == toRemove.end()) &&
                !modules[a]->blocked
           )
            modules[a]->tick();
    Action::resolveAllPending();
}

void MechanicsEngine::addModule(EngineModule *module)
{
    std::lock_guard<std::mutex> t(pushBackMutex);
    for (unsigned int a = 0; a < modules.size(); a++)
    {
        if (modules[a] == module)
            return;
    }
    modules.push_back(module);
}

void MechanicsEngine::removeModule(EngineModule *module)
{
    std::lock_guard<std::mutex> t(pushBackMutex);
    toRemove.push_back(module);
}

void MechanicsEngine::clearUnused()
{
    std::lock_guard<std::mutex> t(pushBackMutex);
    for (unsigned int a = 0; a < toRemove.size(); a++)
        modules.erase(std::remove(modules.begin(), modules.end(), toRemove[a]), modules.end());
    toRemove.clear();

}
