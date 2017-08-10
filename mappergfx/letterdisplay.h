#pragma once
#include "render/texturedobject.h"
#include <QVector2D>

namespace mappergfx {

    class LetterCorner
    {
        public:
            LetterCorner() {}
            LetterCorner(QVector3D p, QVector2D uv) : pos(p), uvPos(uv){}
            LetterCorner(const LetterCorner& other) : pos(other.pos), uvPos(other.uvPos){}
            LetterCorner& operator=(const LetterCorner& other)
            {
                pos = (other.pos);
                uvPos = (other.uvPos);
                return *this;
            }
            QVector3D pos;
            QVector2D uvPos;
    };

    class LetterQuad
    {
        public:
            LetterQuad(){}
            LetterQuad(const LetterQuad& other) { for (int a = 0; a < 6; a++) letterCorners[a] = other.letterCorners[a];}
            LetterQuad& operator=(const LetterQuad& other) { for (int a = 0; a < 6; a++) letterCorners[a] = other.letterCorners[a]; return *this;}
            LetterQuad
            (
                    LetterCorner &q1,
                    LetterCorner &q2,
                    LetterCorner &q3,
                    LetterCorner &q4,
                    LetterCorner &q5,
                    LetterCorner &q6
                    );

            LetterCorner letterCorners[6];
    };

    class LetterDisplay : public renderer::TexturedObject
    {
        public:
            LetterDisplay(QOpenGLTexture* letterTexture);
            ~LetterDisplay();
            void setQuads();
            void clearVerticies();
            void addQuads(std::vector<LetterQuad> extremes);
            void addQuads(LetterQuad extremes);
            void setTexture(QOpenGLTexture* texture);
            virtual bool hitted(const QVector3D*, const QVector3D*, QVector3D*) const {return false;}

        protected:
            virtual int getVertexCount() const;
            int vertexCount;
            std::vector<LetterQuad> vertices;
            virtual void setBuffer();
            virtual GLenum getRenderMode() const{return GL_TRIANGLES;}
    };
}
