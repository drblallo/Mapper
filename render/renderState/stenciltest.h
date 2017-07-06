#pragma once
#include "stenciltestface.h"

namespace renderer
{
    class StencilTest
    {
        public:
        //########################################
        //##### Constructors and Conversions #####
        //########################################
            inline StencilTest() :
                Enabled(false),
                FrontFace(),
                BackFace()
            {}

        //########################################
        //#####         Accessors            #####
        //########################################
            inline bool isEnabled() const {return Enabled;}
            inline StencilTestFace getFrontFace() const {return FrontFace;}
            inline StencilTestFace getBackFace() const {return BackFace;}

        //########################################
        //#####           Methods            #####
        //########################################
            inline void setEnabled(bool enabled) {Enabled = enabled;}
            inline void setFrontFace(StencilTestFace frontFace){FrontFace = frontFace;}
            inline void setBackFace(StencilTestFace backFace){BackFace = backFace;}

        public:
        //########################################
        //#####           Fields             #####
        //########################################
            bool Enabled;
            StencilTestFace FrontFace;
            StencilTestFace BackFace;
    };
}
