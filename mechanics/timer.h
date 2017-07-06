#pragma once
#include "action.h"

namespace mechanics
{
    class Timer : public Action
    {
        public:
        //########################################
        //##### Constructors and Conversions #####
        //########################################
            Timer(int ttl);

        //########################################
        //#####          Accessors           #####
        //########################################
            inline int getTimeToLive() const {return tickLeft;}
            inline int getInitialTTL() const {return startTickConst;}

        //########################################
        //#####           Methods            #####
        //########################################

        protected:
            virtual int OnExpire() = 0;
            virtual void OnTick();
            virtual ~Timer();

        private:
            virtual int execute();

        //########################################
        //#####           Fields             #####
        //########################################
            int tickLeft;
            int startTickConst;


    };
}


/*
#pragma once
#include "mechanics/timer.h"

using namespace mechanics;

namespace game
{
    class y
    {
        public:
            y(int t) : Timer(t) {}

        protected:
            virtual void OnExpire();
            virtual ~y();
    };
}

using namespace game;

y::~y(){}

y::OnExpire()
{

}

*/
