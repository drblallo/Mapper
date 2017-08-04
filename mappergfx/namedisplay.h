#pragma once
#include "render/texturedobject.h"
#include <QVector2D>

namespace mappergfx
{
    class ConnectedRegions;
    class NameDisplay :public renderer::TexturedObject
    {
        public:
            NameDisplay(const ConnectedRegions* placer, QOpenGLTexture *texture, QVector2D offset);
            virtual bool hitted(const QVector3D* source, const QVector3D* direction, QVector3D* out) const {return false;}

        protected:
            QVector2D offset;
            virtual GLenum getRenderMode() const{return GL_TRIANGLE_STRIP;}
            virtual void setBuffer(const std::vector<float>* list);
            virtual int getVertexCount() const {return vCount;}
            void addBox(std::vector<float> &buffer, std::vector<QVector2D> &corners);
            int vCount;

    };
}
