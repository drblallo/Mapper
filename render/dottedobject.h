#pragma once
#include <QVector3D>
#include <vector>
#include <render/renderobject.h>
#include <string>
#include "dot.h"

class QOpenGLShaderProgram;
namespace renderer
{

    class DottedObject : public RenderObject
    {
        public:
        //########################################
        //##### Constructors and Conversions #####
        //########################################
            DottedObject(const std::vector<Dot>* dots);
            DottedObject(const std::vector<Dot>* dots, QOpenGLShaderProgram* program);
            DottedObject(QOpenGLShaderProgram* program);
            DottedObject(const std::vector<float>* dots, QOpenGLShaderProgram* program, int vertexCount);
            virtual ~DottedObject();

        //########################################
        //#####         Privatizing          #####
        //########################################
            DottedObject(const DottedObject&) = delete;
            DottedObject& operator= (const DottedObject&) = delete;

        //########################################
        //#####           Methods            #####
        //########################################
            void setDots(const std::vector<Dot>* dot);
            void setDots(const std::vector<float>* dots, int dotCount);
            virtual bool hitted(const QVector3D*, const QVector3D*) const {return false;}
            static QOpenGLShaderProgram* getShader();

        protected:
        //########################################
        //#####         Accessors            #####
        //########################################
            virtual GLenum getRenderMode() const ;
            virtual int getVertexCount()const {return vCount;}
            int vCount;

        private:
            static QOpenGLShaderProgram* pshader;

        //########################################
        //#####           Fields             #####
        //########################################
            static std::string vertString;
            static std::string fragString;

    };
}
