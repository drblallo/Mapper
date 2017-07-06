#pragma once
#include <mutex>

using namespace std;
namespace mechanics
{
    class EngineModule
    {
        public:
            friend class MechanicsEngine;
        //########################################
        //##### Constructors and Conversions #####
        //########################################
            EngineModule();
            virtual ~EngineModule();

            inline void setBlocked(bool b){blocked = b;}

        //########################################
        //#####         Privatizing          #####
        //########################################
            EngineModule(const EngineModule&) = delete;
            EngineModule& operator= (const EngineModule&) = delete;

        protected:
        //########################################
        //#####           Methods            #####
        //########################################
            bool blocked;
            mutex executionMutex;
            virtual void execute() = 0;

        private:
            void tick();

    };
}
