#pragma once
#include "renderState/renderstate.h"
#include "transform3d.h"
#include <string>
#include "QOpenGLFunctions"
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include "engineobject.h"

class GLWidget;
class QVector3D;
class QOpenGLShaderProgram;
namespace renderer
{
    class RenderObject
    {
		friend class GLWidget;
        public:
        //########################################
        //##### Constructors and Conversions #####
        //########################################
            RenderObject(const std::string& vert, const std::string& frag);
            RenderObject(QOpenGLShaderProgram* program);
            virtual ~RenderObject();

        //########################################
        //#####         Privatizing          #####
        //########################################
            RenderObject(const RenderObject&) = delete;
            RenderObject& operator= (const RenderObject&) = delete;

        //########################################
        //#####         Accessors            #####
        //########################################
            inline Transform3D* getTransform() {return &transform;}
            inline const Transform3D* getTransform() const {return &transform;}
            inline bool toBeDrawn() const {return canBeDrawn && !hidden;}
            inline bool visible() const {return !hidden;}

        //########################################
        //#####           Methods            #####
        //########################################
            virtual void OnMouseDown(QVector3D localPoint);
            virtual void OnMouseOver();
            virtual void Prerender();
            virtual void PostRender();
            virtual void Render();
            inline const RenderState* getState() const {return &renderState;}
            inline void hide(bool h){hidden = h;}
            virtual bool hitted(const QVector3D* source, const QVector3D* direction, QVector3D* out) const = 0;

        protected:
        //########################################
        //#####           Fields             #####
        //########################################
            RenderState renderState;
            Transform3D transform;
            QOpenGLShaderProgram* shader;
            QOpenGLVertexArrayObject AO;
            QOpenGLBuffer buffer;
            bool canBeDrawn;
            int modelToWorld;
            int worldToCamera;
            int cameraToView;
            int farPlane;
            bool hidden;

        //########################################
        //#####           Methods            #####
        //########################################
            virtual void setBuffer(const std::vector<QVector3D>* list);
            virtual void setBuffer(const void* pos, int size);
            inline const QOpenGLShaderProgram* getShader() const {return shader;}
            virtual GLenum getRenderMode() const = 0;
            virtual int getVertexCount() const = 0;
            inline const QOpenGLBuffer* getBuffer() const {return &buffer;}

    };
}
