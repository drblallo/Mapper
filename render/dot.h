#pragma once
#include <QVector3D>

namespace renderer
 {
    struct Dot
    {
        public:
        //########################################
        //##### Constructors and Conversions #####
        //########################################
            Dot(QVector3D p, QVector3D c) : pos(p), col(c){}
            Dot(QVector3D p) : pos(p), col(255, 255, 255){}
            Dot(float x, float y, float z) : pos(x, y, z), col(255, 255, 255){}
            Dot& operator= ( const Dot & other )
            {
                pos = other.pos;
                col = other.col;
                return *this;
            }

        //########################################
        //#####           Fields             #####
        //########################################
            QVector3D pos;
            QVector3D col;
    };
}
