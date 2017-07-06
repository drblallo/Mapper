#pragma once

namespace renderer
{
    class PrimitiveRestart
    {
        public:
        //########################################
        //##### Constructors and Conversions #####
        //########################################
            inline PrimitiveRestart() :
                Enabled(false),
                Index(0)
            {}

        //########################################
        //#####         Accessors            #####
        //########################################
            inline bool isEnabled() const { return Enabled;}
            inline int getIndex() const { return Index;}

        //########################################
        //#####           Methods            #####
        //########################################
            inline void setEnabled(bool enabled) {Enabled = enabled;}
            inline void setIndex(int index) {Index = index;}

        private:
        //########################################
        //#####           Fields             #####
        //########################################
            bool Enabled;
            int Index;
    };
}
