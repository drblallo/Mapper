#pragma once
#include <QRect>

namespace renderer
{
    class ScissorTest
    {
        public:
        //########################################
        //##### Constructors and Conversions #####
        //########################################
            inline ScissorTest():
                Enabled(false),
                rectangle(0, 0, 0, 0)
            {}
        //########################################
        //#####         Accessors            #####
        //########################################
            inline bool isEnabled() const {return Enabled;}
            inline const QRect* getRectangle() const {return &rectangle;}

        //########################################
        //#####           Methods            #####
        //########################################
            inline void setEnabled(bool enabled) {Enabled = enabled;}
            inline void setRectangle(const QRect *rect) {rectangle = *rect;}

        private:
        //########################################
        //#####           Fields             #####
        //########################################
            bool Enabled;
            QRect rectangle;

    };
}
