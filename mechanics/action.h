#pragma once
#include <vector>
#include <mutex>
#include "informativeobject.h"
#include <functional>

using namespace std;
namespace mechanics
{
    class Action : public InformativeObject
    {
        public:
        //########################################
        //##### Constructors and Conversions #####
        //########################################
            Action();

        //########################################
        //#####         Privatizing          #####
        //########################################
            Action(const Action&) = delete;
            Action& operator= (const Action&) = delete;

        //########################################
        //#####           Methods            #####
        //########################################
            static void call(Action* action);
            static void uncall(Action* action);
            static void resolveAllPending();

        //########################################
        //#####         Accessors            #####
        //########################################
            inline static const vector<Action*>* getPendingActions()
            {
                return &pendingActions;
            }
            inline bool isPending()const {return pending;}
            inline void addCallBack(std::function<void(int)> f){callbacks.push_back(f);}
            virtual bool canBeCalled() const;
            virtual bool canBeUncalled() const;
            virtual QString getcannotUncallReason() const {return QString("can always be called");}
            virtual QString getcannotCallReason() const {return QString("can always be uncalled");}

        protected:
            virtual int execute() = 0;
            bool deleteMe;
            virtual ~Action();

        private:
        //########################################
        //#####           Fields             #####
        //########################################
            static vector<Action*> pendingActions;
            static vector<Action*> allActions;
            static mutex pendingActionsMutex;
            static mutex resolveAllMutex;
            static mutex allActionsMutex;

            std::vector<std::function<void(int)> > callbacks;
            bool pending;
    };
}

/*
#pragma once
#include "mechanics/action.h"

using namespace mechanics;

namespace game
{
    class y
    {
        public:
            y() : Action() {}

        protected:
            virtual void execute();
            virtual ~y();
    };
}

using namespace game;

y::~y(){}

y::execute()
{

}

*/
