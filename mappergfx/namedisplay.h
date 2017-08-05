#pragma once
#include "render/texturedobject.h"
#include <QVector2D>
#include <vector>

namespace mappergfx
{
    class NamePlacer;
    class ConnectedRegions;
    class NameDisplay :public renderer::TexturedObject
    {
        public:
            NameDisplay(const mappergfx::NamePlacer *placer, QOpenGLTexture *texture, QVector2D offset);
            virtual bool hitted(const QVector3D* source, const QVector3D* direction, QVector3D* out) const {return false;}
            static void setLetters(std::vector<QOpenGLTexture*> ls) {letters = ls;}

        protected:
            QVector2D offset;
            virtual GLenum getRenderMode() const{return GL_TRIANGLES;}
            virtual void setBuffer(const std::vector<float>* list);
            virtual int getVertexCount() const {return vCount;}
            void addBox(std::vector<float> &buffer, std::vector<QVector2D> &corners, char letter);
            int vCount;
            virtual void Prerender();
            virtual void PostRender();
            static std::vector<QOpenGLTexture*> letters;
            void addRegionToBuffer(const ConnectedRegions* region, std::vector<float>& buffer, QVector2D& offset);

    };
}
