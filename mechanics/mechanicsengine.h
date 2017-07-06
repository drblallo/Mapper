#pragma once
#include <vector>
#include "enginemodule.h"
#include <mutex>

namespace mechanics
{
    class EngineModule;
    class MechanicsEngine
    {
        public:
            friend class EngineModule;
        //########################################
        //#####         Privatizing          #####
        //########################################
            MechanicsEngine(const MechanicsEngine&) = delete;
            MechanicsEngine& operator= (const MechanicsEngine&) = delete;

        //########################################
        //#####           Methods            #####
        //########################################
            static void StartEngine();
            static inline MechanicsEngine* getEngine()
            {
                if (!engine)
                    StartEngine();
                return engine;
            }

        private:
            void removeModule(EngineModule* module);
            void addModule(EngineModule* module);
            void clearUnused();

        //########################################
        //##### Constructors and Conversions #####
        //########################################
            MechanicsEngine();
            static MechanicsEngine* engine;
            static void StartThread();
            void loop();
            void tick();
            static bool end;
            static int minTickTime;
            //int tickElapsed;
            std::vector<EngineModule*> modules;
            std::mutex engineMutex;
            std::mutex pushBackMutex;
            std::vector<EngineModule*> toRemove;


    };
}
