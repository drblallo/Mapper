#include "action.h"
#include <algorithm>
#include <iostream>

using namespace mechanics;


vector<Action*> Action::allActions;
vector<Action*> Action::pendingActions;
mutex Action::pendingActionsMutex;
mutex Action::resolveAllMutex;
mutex Action::allActionsMutex;

//########################################
//##### Constructors and Conversions #####
//########################################
Action::Action() : deleteMe(false), pending(false)
{
    lock_guard<mutex> r(allActionsMutex);
    allActions.push_back(this);
}

Action::~Action()
{
    if (pending)
    {
        uncall(this);
    }
    lock_guard<mutex> r(allActionsMutex);
    allActions.erase(remove(allActions.begin(), allActions.end(), this),allActions.end());
    std::cerr << "removed action\n";
}

//########################################
//#####           Methods            #####
//########################################
void Action::call(Action* act)
{
    lock_guard<mutex> r(allActionsMutex);
    lock_guard<mutex> l(pendingActionsMutex);

    if (find(allActions.begin(), allActions.end(), act) == allActions.end())
        return;

    if (act->pending)
        return;

    if (!act->canBeCalled())
        return;

    pendingActions.push_back(act);
    act->pending = true;
    act->deleteMe = true;
}


void Action::uncall(Action* act)
{
    lock_guard<mutex> l(pendingActionsMutex);
    lock_guard<mutex> r(allActionsMutex);

    if (!act->canBeUncalled())
        return;

    if (find(allActions.begin(), allActions.end(), act) == allActions.end())
        return;

    pendingActions.erase(remove(pendingActions.begin(), pendingActions.end(), act), pendingActions.end());
    act->pending = false;
    act->deleteMe = true;
}

void Action::resolveAllPending()
{
    lock_guard<mutex> l(resolveAllMutex);

    pendingActionsMutex.lock();
    vector<Action*> ls;
    for (unsigned int a = 0; a < pendingActions.size(); a++)
        ls.push_back(pendingActions[a]);
    pendingActions.clear();

    pendingActionsMutex.unlock();

    for (unsigned int a = 0; a < ls.size(); a++)
    {
        ls[a]->pending = false;
        int result(ls[a]->execute());
        for (unsigned int d = 0; d < ls[a]->callbacks.size(); d++)
        {
            ls[a]->callbacks[d](result);
        }
    }

    for (int a = allActions.size()-1; a >= 0; a--)
    {
        if (allActions[a]->deleteMe)
        {
            delete allActions[a];
        }
    }
}

bool Action::canBeCalled() const
{
    return true;
}

bool Action::canBeUncalled() const
{
    return true;
}



