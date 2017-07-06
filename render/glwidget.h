#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <vector>
#include <render/renderState/renderstate.h>
#include "transform3d.h"
#include "camera3d.h"

class QOpenGLDebugLogger;

namespace renderer
{
    class RenderObject;
    class EngineObject;
    class Scene;
}

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
 // Q_OBJECT

    public:
        friend class renderer::RenderObject;
        friend class renderer::EngineObject;
        friend class renderer::Scene;
    //########################################
    //##### Constructors and Conversions #####
    //########################################
        GLWidget(QWidget* parent);
        ~GLWidget();

    //########################################
    //#####           Methods            #####
    //########################################
        void initializeGL();
        void resizeGL(int width, int height);
        void paintGL();
        void teardownGL();
		void setClearColor(QVector4D color);

    //########################################
    //#####         Accessors            #####
    //########################################
        inline const QMatrix4x4* getProjection() const {return &m_projection;}
        inline Camera3D* getCamera() {return &m_camera;}
        inline const renderer::Scene* getScene() const {return scene;}
        inline float getFarPlane() const {return farPlane;}
        QVector3D getMouseForward();

    protected slots:
        virtual void update();

    private:
        void sortRenderObjects();
        void applyRenderState(const renderer::RenderState* renderState);
        void applyBlending(const renderer::RenderState* renderState);
        void printContextInformation();
        std::vector<renderer::EngineObject*> engineObjects;
        std::vector<renderer::EngineObject*> updateQueue;
        std::vector<renderer::RenderObject*> renderObjects;
        renderer::RenderState renderState;
        void keyPressEvent(QKeyEvent *event);
        void keyReleaseEvent(QKeyEvent *event);
        void mousePressEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);
        QMatrix4x4 m_projection;
        Camera3D m_camera;
        renderer::Scene* scene;
        float farPlane;
        QOpenGLDebugLogger* logger;

       // bool doOne;
        //int count;

};
