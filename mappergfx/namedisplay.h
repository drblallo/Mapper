#pragma once
#include "render/texturedobject.h"
#include <QVector2D>
#include <vector>

namespace mappergfx
{
    class LetterDisplay;
    class NamePlacer;
    class ConnectedRegions;
    class NameDisplay //:public renderer::TexturedObject
    {
        public:
            NameDisplay(const mappergfx::NamePlacer *placer, QVector2D offset);
            NameDisplay(QVector2D offset);
            ~NameDisplay();
           // virtual bool hitted(const QVector3D* source, const QVector3D* direction, QVector3D* out) const {return false;}
            void setLetters(std::vector<QOpenGLTexture*> ls) ;
            void setText(const NamePlacer* plc);
            void setScale(QVector3D scale);

        protected:
            QVector2D offset;
            //virtual GLenum getRenderMode() const{return GL_TRIANGLES;}
            //virtual void setBuffer(const std::vector<float>* list);
            //virtual int getVertexCount() const {return vCount;}
            void addBox(std::vector<float> &buffer, std::vector<QVector2D> &corners, char letter);
            //int vCount;
            //virtual void Prerender();
            //virtual void PostRender();
            std::vector<QOpenGLTexture*> letters;
            void addRegionToBuffer(const ConnectedRegions* region, std::vector<float>& buffer);

            std::vector<LetterDisplay*> letterDisplays;

    };
}
