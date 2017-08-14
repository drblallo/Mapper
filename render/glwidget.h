#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <vector>
#include <render/renderState/renderstate.h>
#include "transform3d.h"
#include "camera3d.h"
#include <mutex>

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
        void paintGL();
        void teardownGL();
		void setClearColor(QVector4D color);
		void leftMouseDown();

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
        virtual void resizeGL(int width, int height);
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
        void wheelEvent(QWheelEvent *event);
        QMatrix4x4 m_projection;
        Camera3D m_camera;
        renderer::Scene* scene;
        float farPlane;
        QOpenGLDebugLogger* logger;
        std::mutex renderingLock;

       // bool doOne;
        //int count;

};
