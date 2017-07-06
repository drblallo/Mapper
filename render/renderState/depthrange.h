#pragma once

namespace renderer
{
    class DepthRange
    {
        public:
        //########################################
        //##### Constructors and Conversions #####
        //########################################
            inline DepthRange() :
                Near(0.0),
                Far(1.0)
            {}

        //########################################
        //#####         Accessors            #####
        //########################################
            inline double getNear() const {return Near;}
            inline double getFar() const {return Far;}

        //########################################
        //#####           Methods            #####
        //########################################
            inline void setNear(double newNear){Near = newNear;}
            inline void setFar(double newFar){Far = newFar;}

        private:
        //########################################
        //#####           Fields             #####
        //########################################
            double Near;
            double Far;
    };
}
