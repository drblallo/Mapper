#pragma once
namespace renderer
{
    class EngineObject
    {
        public:
        //########################################
        //##### Constructors and Conversions #####
        //########################################
            EngineObject();
            virtual ~EngineObject();

        //########################################
        //#####           Methods            #####
        //########################################
            virtual void Update();
            inline void setActive(bool a) {active = a;}
            inline bool isActive() const {return active;}

        //########################################
        //#####         Privatizing          #####
        //########################################
            EngineObject(const EngineObject&) = delete;
            EngineObject& operator= (const EngineObject&) = delete;

       protected:
            bool active;

    };
}
