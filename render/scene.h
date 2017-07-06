#pragma once

namespace renderer
{
    class Scene
    {
        public:
        //########################################
        //##### Constructors and Conversions #####
        //########################################
            Scene();
            virtual ~Scene();

        //########################################
        //#####         Privatizing          #####
        //########################################
            Scene(const Scene&) = delete;
            Scene& operator= (const Scene&) = delete;

        //########################################
        //#####           Methods            #####
        //########################################
            void setAsCurrent() ;
            virtual void update(){}

        //########################################
        //#####         Accessors            #####
        //########################################
            bool canBeReplacedBy(const Scene* newScene) const;
            bool isCurrentScene() const;

        protected:
        //########################################
        //#####           Methods            #####
        //########################################
            virtual void setUp() = 0;
            virtual void tearDown() = 0;

    };
}
