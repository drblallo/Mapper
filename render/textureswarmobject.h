#pragma once
#include "texturedobject.h"
#include <vector>
#include "transform3d.h"
#include "floatingquad.h"

namespace renderer
{


    class TextureSwarmObject : public TexturedObject
    {
        public:
        //########################################
        //##### Constructors and Conversions #####
        //########################################
            TextureSwarmObject(std::vector<FloatingQuad> *textures, QOpenGLTexture* texture);
            virtual ~TextureSwarmObject();

        //########################################
        //#####         Privatizing          #####
        //########################################
            TextureSwarmObject(const TextureSwarmObject&) = delete;
            TextureSwarmObject& operator= (const TextureSwarmObject&) = delete;

        //########################################
        //#####           Methods            #####
        //########################################
            void setFloatingQuads(std::vector<FloatingQuad> *textures);
            virtual bool hitted(const QVector3D*, const QVector3D*) const {return false;}

        protected:
        //########################################
        //#####         Accessors            #####
        //########################################
            virtual GLenum getRenderMode() const {return GL_TRIANGLES;}
            virtual int getVertexCount()const {return vCount;}

        //########################################
        //#####           Fields             #####
        //########################################
            static std::string vertString;
            static std::string fragString;
            int vCount;
    };
}
