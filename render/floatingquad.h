#pragma once
#include "transform3d.h"

namespace renderer
{
    struct FloatingQuad
    {
        public:
            FloatingQuad(QVector4D col)
            {
                for (int a = 0; a < 4; a++)
                    color[a] = col;
            }
            FloatingQuad(Transform3D transf, QVector4D col) : transform(transf)
            {
                for (int a = 0; a < 4; a++)
                    color[a] = col;
            }
            FloatingQuad(Transform3D transf, QVector4D col, QVector4D col2) : transform(transf)
            {
                for (int a = 0; a < 4; a += 2)
                    color[a] = col;
                for (int a = 1; a < 4; a += 2)
                    color[a] = col2;
            }
            FloatingQuad(Transform3D transf, QVector4D col, QVector4D col2, QVector4D col3, QVector4D col4) : transform(transf)
            {
                color[0] = col;
                color[1] = col2;
                color[2] = col3;
                color[3] = col4;
            }
            Transform3D transform;
            QVector4D color[4];
    };
}
