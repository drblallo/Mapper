#pragma once

namespace renderer
{
    enum AlphaTestFunction
    {
        AlphaTestFunctionNever,
        AlphaTestFunctionLess,
        AlphaTestFunctionEqual,
        AlphaTestFunctionLessThanOrEqual,
        AlphaTestFunctionGreater,
        AlphaTestFunctionNotEqual,
        AlphaTestFunctionGreaterThanOrEqual,
        AlphaTestFunctionAlways
    };

    class AlphaTest
    {
        public:
        //########################################
        //##### Constructors and Conversions #####
        //########################################
            inline AlphaTest() :
                Enabled(false),
                Function(AlphaTestFunction::AlphaTestFunctionGreater),
                limit(0.5f)
            {}

        //########################################
        //#####         Accessors            #####
        //########################################
            inline bool isEnabled() const {return Enabled;}
            inline AlphaTestFunction getAlphaTestFunction() const {return Function;}
            inline float getLimit() const {return limit;}

        //########################################
        //#####           Methods            #####
        //########################################
            inline void setEnabled(bool enabled){Enabled = enabled;}
            inline void setAlphaTestFunction(AlphaTestFunction function){Function = function;}
            inline void setLimit(float val) {limit = val;}

        private:
        //########################################
        //#####           Fields             #####
        //########################################
            bool Enabled;
            AlphaTestFunction Function;
            float limit;
    };
}
