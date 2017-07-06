#pragma once
#include "renderobject.h"

class QOpenGLTexture;
namespace renderer
{
    class TexturedObject : public RenderObject
    {
        public:
        //########################################
        //##### Constructors and Conversions #####
        //########################################
            TexturedObject(const std::string& vert, const std::string& frag, QOpenGLTexture* texture);
            virtual ~TexturedObject();

        //########################################
        //#####         Privatizing          #####
        //########################################
            TexturedObject(const TexturedObject&) = delete;
            TexturedObject& operator= (const TexturedObject&) = delete;

        protected:
            QOpenGLTexture* texture;
            int textureUniformLocation;
            int uvLocation;
            virtual void setBuffer(const std::vector<QVector3D>* list);
            void setBuffer(const void * pos, int size);
            virtual GLenum getRenderMode() const{return GL_TRIANGLES;}
            virtual void Prerender();
            virtual void PostRender();
    };
}
