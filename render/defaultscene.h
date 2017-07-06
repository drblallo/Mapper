#pragma once
#include "scene.h"
#include <vector>
#include <render/defaultinputmanager.h>

namespace renderer
{
    class RenderObject;
    class DefaultScene : public Scene
    {
        public:
        //########################################
        //##### Constructors and Conversions #####
        //########################################
            DefaultScene();
            virtual ~DefaultScene();

        protected:
        //########################################
        //#####           Methods            #####
        //########################################
            virtual void tearDown();
            virtual void setUp();
            std::vector<renderer::RenderObject*> spawned;
            renderer::DefaultInputManager manager;
    };
}
