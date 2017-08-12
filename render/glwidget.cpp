#include "glwidget.h"
#include <QDebug>
#include <QString>
#include "device.h"
#include "renderobject.h"
#include <iostream>
#include "input.h"
#include <QKeyEvent>
#include <QOpenGLDebugLogger>
#include "mainwindow.h"
#include "render/scene.h"
#include "renderdefines.h"

using namespace renderer;
GLWidget::GLWidget(QWidget* parent) : QOpenGLWidget(parent), scene(NULL), farPlane(1000)// doOne(false), count(0)
{
    Device::setGraphicWindow(this);
}

GLWidget::~GLWidget()
{
  makeCurrent();
  teardownGL();
}

/*******************************************************************************
 * OpenGL Events
 ******************************************************************************/

void GLWidget::initializeGL()
{
  initializeOpenGLFunctions();
  connect(this, &GLWidget::frameSwapped, this, &GLWidget::update);
  printContextInformation();
  setFocusPolicy(Qt::StrongFocus);
  installEventFilter(this);

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_CULL_FACE);
  //glCullFace(GL_BACK);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  //resizeGL(111, 11111);

   logger = new QOpenGLDebugLogger(this);

  logger->initialize(); // initializes in the current context, i.e. ctx

}

void GLWidget::setClearColor(QVector4D color)
{
	makeCurrent();
	glClearColor(color.x(), color.y(), color.z(), color.w());
}

void GLWidget::update()
{
  //std::lock_guard<std::mutex> m(renderingLock);
  Input::update();
  makeCurrent();
  updateQueue.clear();

  for (unsigned int a = 0; a < engineObjects.size(); a++)
      if (engineObjects[a]->isActive())
         updateQueue.push_back(engineObjects[a]);

  if (scene)
    scene->update();

  for (unsigned int a = 0; a < updateQueue.size(); a++)
      updateQueue[a]->Update();

    if (context()->format().testOption(QSurfaceFormat::DebugContext))
    {
      GLenum error = GL_NO_ERROR;
      do {
          error = glGetError();
          if (error != GL_NO_ERROR)
          {
              const QList<QOpenGLDebugMessage> messages = logger->loggedMessages();
              for (const QOpenGLDebugMessage &message : messages)
                  qDebug() << message;
          }

      } while (error != GL_NO_ERROR);
      }
  QOpenGLWidget::update();
}

void GLWidget::resizeGL(int width, int height)
{
    m_projection.setToIdentity();
    m_projection.perspective(45.0f,float(width) / float(height), 1.0f, farPlane);
}

void GLWidget::paintGL()
{
 // std::lock_guard<std::mutex> m(renderingLock);
  //std::cout << count << " paint \n";
  //count++;
  sortRenderObjects();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//  qDebug() << renderObjects.size() << "\n";

  for (unsigned int a = 0; a < renderObjects.size(); a++)
  {
      if (renderObjects[a]->toBeDrawn() /*&& renderObjects[a]->getState()->depthMask*/)
      {
        applyRenderState(renderObjects[a]->getState());
        renderObjects[a]->Render();
      }
  }
  glDepthMask(GL_TRUE);

}

bool distanceFunction(RenderObject* ogg, RenderObject* ogg2)
{
	QVector3D cameraPos(Device::getGraphicWindow()->getCamera()->translation());
	float d(ogg->getTransform()->translation().distanceToPoint(cameraPos));
    return (d > ogg2->getTransform()->translation().distanceToPoint(cameraPos));
}

void GLWidget::sortRenderObjects()
{
	//move trasparent object to the bottom
	int target(renderObjects.size()-1);
    for (int a = renderObjects.size()-1; a >= 0; a--)
    {
        if (!renderObjects[a]->getState()->depthMask)
        {
			std::swap(renderObjects[a], renderObjects[target]);
			target--;
        }
    }

	//sort by distance trasparent object
	std::sort(renderObjects.begin() + target + 1, renderObjects.end(), distanceFunction);
	
}

void GLWidget::applyRenderState(const RenderState *state)
{
    if (state->depthTest.isEnabled() != renderState.depthTest.isEnabled())
    {
        if (state->depthTest.isEnabled())
        {
           // qDebug() << "enabled depth";
            glEnable(GL_DEPTH_TEST);
        }else
        {
           // qDebug() << "disabled";
            glDisable(GL_DEPTH_TEST);
        }
        renderState.depthTest.setEnabled(state->depthTest.isEnabled());
    }
    if (state->depthTest.isEnabled())
    {
        if (state->depthTest.getDepthTestFunction() != renderState.depthTest.getDepthTestFunction())
        {
            glDepthFunc(state->depthTest.getDepthTestFunction());
            renderState.depthTest.setDepthTestFunction(state->depthTest.getDepthTestFunction());
        }
    }

    if (state->facetCulling.isEnabled() != renderState.facetCulling.isEnabled())
    {
        if (state->facetCulling.isEnabled())
            glEnable(GL_CULL_FACE);
        else
            glDisable(GL_CULL_FACE);

        renderState.facetCulling.setEnabled(state->facetCulling.isEnabled());
    }

    if (state->facetCulling.isEnabled())
    {

        if (state->facetCulling.getCullFace() != renderState.facetCulling.getCullFace())
        {
            switch (state->facetCulling.getCullFace()) {
            case CullFaceBack:
                glCullFace(GL_BACK);
                break;
            case CullFaceFront:
                glCullFace(GL_FRONT);
                break;
            case CullFaceFrontAndBack:
                glCullFace(GL_FRONT_AND_BACK);
                break;
            }

            renderState.facetCulling.setCullFace(state->facetCulling.getCullFace());
        }
    }

    if (state->depthMask != renderState.depthMask)
    {
        if (state->depthMask)
            glDepthMask(GL_TRUE);
        else
            glDepthMask(GL_FALSE);

        renderState.depthMask = (state->depthMask);
    }
    if (state->facetCulling.isEnabled())
    {
        if (state->facetCulling.getCullFace() != renderState.facetCulling.getCullFace())
        {
            glCullFace(state->facetCulling.getCullFace());
            renderState.facetCulling.setCullFace(state->facetCulling.getCullFace());
        }
        if (state->facetCulling.getFrontFaceWiding() != renderState.facetCulling.getFrontFaceWiding())
        {
            glFrontFace(state->facetCulling.getFrontFaceWiding());
            renderState.facetCulling.setFrontFaceWiding(state->facetCulling.getFrontFaceWiding());
        }
    }
    applyBlending(state);

}

QVector3D GLWidget::getMouseForward()
{

    float width = ((2.0f*Input::mouseScreenPosition().x())/this->width())-1.0f;
    float height = ((2.0f*Input::mouseScreenPosition().y())/this->height())-1.0f;

    width = width*1;
    height = height*-1;


    QVector4D clicked(width, height, 0, 1.0f);

    QVector4D ray_eye = m_projection.inverted() * clicked;
    ray_eye = QVector4D(ray_eye.x(), ray_eye.y(), -1.0f, 0.0f);
    QVector4D ray_world = m_camera.toMatrix().inverted() * ray_eye;
    QVector3D actualRay = QVector3D(ray_world.x(), ray_world.y(), ray_world.z());
    return actualRay.normalized();
}

void GLWidget::leftMouseDown()
{
	for (int a = renderObjects.size() - 1; a >= 0; a--)
	{
		QVector3D dir(getMouseForward());
		//QVector3D dir(m_camera.forward());
		QVector3D source(m_camera.translation());
		QVector3D out;
		if (renderObjects[a]->hitted(&source, &dir, &out))
			renderObjects[a]->OnMouseDown(out);
	}
}

void GLWidget::applyBlending(const RenderState *state)
{

    if (state->blending.isEnabled() != renderState.blending.isEnabled())
    {
        if (state->blending.isEnabled())
            glEnable(GL_BLEND);
        else
            glDisable(GL_BLEND);

        renderState.blending.setEnabled(state->blending.isEnabled());
    }
    if (state->blending.isEnabled())
    {
        if (state->blending.getAlphaEquation() != renderState.blending.getAlphaEquation())
        {
            switch (state->blending.getAlphaEquation()) {
            case BlendEquationAdd:

                glBlendEquation(GL_FUNC_ADD);
                break;
            case BlendEquationSubtract:

                glBlendEquation(GL_FUNC_SUBTRACT);
                break;
            default:
                break;
            }
            renderState.blending.setAlphaEquation(state->blending.getAlphaEquation());
        }

        if (
                state->blending.getSourceAlphaFactor() != renderState.blending.getSourceAlphaFactor()
                ||
                state->blending.getDestinationAlphaFactor() != renderState.blending.getDestinationAlphaFactor()
           )
        {
            //std::cout << "modifer\n";
            //glBlendFunc(state->blending.getSourceAlphaFactor(), state->blending.getDestinationAlphaFactor());
            int source;
            switch (state->blending.getSourceAlphaFactor())
            {
                case (SourceBlendingSourceAlpha):
                    source = GL_SRC_ALPHA;
                break;

                default:
                    source = GL_ONE;
            }
            int desination;
            switch (state->blending.getDestinationAlphaFactor())
            {
                case (DestinationBlendingOneMinusAlpha):
                    desination = GL_ONE_MINUS_SRC_ALPHA;
                break;
                 default:
                    desination = GL_ZERO;
            }
            glBlendFunc(source, desination);

            renderState.blending.setSourceAlphaFactor(state->blending.getSourceAlphaFactor());
            renderState.blending.setDestinationAlphaFactor(state->blending.getDestinationAlphaFactor());
        }
    }
}

void GLWidget::teardownGL()
{
}

/*******************************************************************************
 * Private Helpers
 ******************************************************************************/

void GLWidget::printContextInformation()
{
  QString glType;
  QString glVersion;
  QString glProfile;

  glType = (context()->isOpenGLES()) ? "OpenGL ES" : "OpenGL";
  glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));

#define CASE(c) case QSurfaceFormat::c: glProfile = #c; break
  switch (format().profile())
  {
    CASE(NoProfile);
    CASE(CoreProfile);
    CASE(CompatibilityProfile);
  }
#undef CASE

  qDebug() << qPrintable(glType) << qPrintable(glVersion) << "(" << qPrintable(glProfile) << ")";
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
  if (event->isAutoRepeat())
  {
    event->ignore();
  }
  else
  {
    Input::registerKeyPress(event->key());
  }
}

void GLWidget::keyReleaseEvent(QKeyEvent *event)
{
  if (event->isAutoRepeat())
  {
    event->ignore();
  }
  else
  {
    Input::registerKeyRelease(event->key());
  }
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
  Input::registerMousePress(event->button());
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
  Input::registerMouseRelease(event->button());
}
