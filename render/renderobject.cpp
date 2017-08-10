#include "renderobject.h"
#include "device.h"
#include "glwidget.h"
#include <algorithm>
#include <vector>
#include <QOpenGLShaderProgram>
#include <iostream>


using namespace renderer;

//########################################
//##### Constructors and Conversions #####
//########################################
RenderObject::RenderObject(const std::string& vert,const std::string& frag) :
    renderState(),
    transform(),
    shader(NULL),
    canBeDrawn(false),
    hidden(false)
{
    transform.setScale(1);
    transform.setTranslation(1, 1, 1);
    //std::cout << "ere1\n";
    shader = Device::createProgram(vert, frag);
    shader->bind();
    modelToWorld = shader->uniformLocation("modelToWorld");
    worldToCamera = shader->uniformLocation("worldToCamera");
    cameraToView = shader->uniformLocation("cameraToView");
    farPlane = shader->uniformLocation("farPlane");
    shader->release();
    AO.create();
    buffer = Device::createVertexBuffer(QOpenGLBuffer::DynamicDraw);
    Device::getGraphicWindow()->renderObjects.push_back(this);
}

RenderObject::RenderObject(QOpenGLShaderProgram* program) :
    renderState(),
    transform(),
    shader(program),
    canBeDrawn(false),
    hidden(false)

{
    transform.setScale(1);
    transform.setTranslation(1, 1, 1);
    //std::cout << "ere1\n";
    shader->bind();
    modelToWorld = shader->uniformLocation("modelToWorld");
    if (modelToWorld == -1)
        std::cout << "cannot place uniform location model to world\n";
    worldToCamera = shader->uniformLocation("worldToCamera");
    if (worldToCamera== -1)
        std::cout << "cannot place uniform location world to camera\n";
    cameraToView = shader->uniformLocation("cameraToView");
    if (cameraToView== -1)
        std::cout << "cannot place uniform location camera to view \n";
    farPlane = shader->uniformLocation("farPlane");
    if (farPlane == -1)
        std::cout << "cannot place uniform location far plane\n";
    shader->release();
    AO.create();
    buffer = Device::createVertexBuffer(QOpenGLBuffer::DynamicDraw);
    Device::getGraphicWindow()->renderObjects.push_back(this);
}


RenderObject::~RenderObject()
{
    delete shader;
    buffer.destroy();
    AO.destroy();
    std::cout << "destroyed object\n";
    std::vector<RenderObject*>* w(&Device::getGraphicWindow()->renderObjects);
    w->erase(std::remove(w->begin(), w->end(), this), w->end());
}

//########################################
//#####           Methods            #####
//########################################
void RenderObject::setBuffer(const std::vector<QVector3D>* list)
{
    setBuffer(&(list->at(0)), sizeof(QVector3D)*list->size());
}
void RenderObject::setBuffer(const void * pos, int size)
{
    std::cerr << "called render object set buffer\n";
    shader->bind();
    buffer.bind();
    AO.bind();

    buffer.allocate(pos, size);
    shader->enableAttributeArray(0);
    shader->enableAttributeArray(1);
    shader->setAttributeBuffer(0, GL_FLOAT, int(0), 3, 2*int(sizeof(QVector3D)));
    shader->setAttributeBuffer(1, GL_FLOAT, sizeof(QVector3D), 3, 2*int(sizeof(QVector3D)));

    AO.release();
    buffer.release();
    shader->release();
}
void RenderObject::OnMouseDown(QVector3D )
{

}

void RenderObject::OnMouseOver()
{

}

void RenderObject::Render()
{
    Prerender();
    AO.bind();
    shader->setUniformValue(modelToWorld, transform.toMatrix());
    //glDrawArrays(getRenderMode(), 0, getVertexCount());
    QOpenGLContext::currentContext()->functions()->glDrawArrays(getRenderMode(), 0, getVertexCount());
    AO.release();
    PostRender();
}

void RenderObject::Prerender()
{

    shader->bind();
    shader->setUniformValue(cameraToView, *Device::getGraphicWindow()->getProjection());
    shader->setUniformValue(worldToCamera, Device::getGraphicWindow()->getCamera()->toMatrix());
    shader->setUniformValue(farPlane, Device::getGraphicWindow()->getFarPlane());
}

void RenderObject::PostRender()
{
    shader->release();
}
