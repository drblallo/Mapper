#pragma once
#include "engineobject.h"

namespace renderer
{
    class DefaultInputManager : public EngineObject
    {
        public:
        //########################################
        //##### Constructors and Conversions #####
        //########################################
            DefaultInputManager();
            virtual ~DefaultInputManager();

        //########################################
        //#####           Methods            #####
        //########################################
            virtual void Update();
    };
}
