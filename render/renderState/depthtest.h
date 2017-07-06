#pragma once

namespace renderer
{
    enum DepthTestFunction
    {
        DepthTestFunctionNever,
        DepthTestFunctionLess,
        DepthTestFunctionEqual,
        DepthTestFunctionLessThanOrEqual,
        DepthTestFunctionGreater,
        DepthTestFunctionNotEqual,
        DepthTestFunctionGreaterThanOrEqual,
        DepthTestFunctionAlways
    };

    class DepthTest
    {
        public:
        //########################################
        //##### Constructors and Conversions #####
        //########################################
            inline DepthTest() :
                Enabled(true),
                Function(DepthTestFunction::DepthTestFunctionLess)
            {}

        //########################################
        //#####         Accessors            #####
        //########################################
            inline bool isEnabled() const {return Enabled;}
            inline DepthTestFunction getDepthTestFunction() const {return Function;}

        //########################################
        //#####           Methods            #####
        //########################################
            inline void setEnabled(bool enabled){Enabled = enabled;}
            inline void setDepthTestFunction(DepthTestFunction function){Function = function;}

        private:
        //########################################
        //#####           Fields             #####
        //########################################
            bool Enabled;
            DepthTestFunction Function;
    };
}
