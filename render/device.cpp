#include "device.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QString>
#include <QOpenGLTexture>
#include <QOpenGLWidget>
#include <iostream>


using namespace renderer;

//########################################
//#####           Fields             #####
//########################################
GLWidget* Device::GraphicWindow(NULL);

//########################################
//#####           Methods            #####
//########################################
Device::Device()
{

}

QOpenGLShaderProgram* Device::createProgram(const std::string& vertex,const std::string& fragment)
{
    QOpenGLShaderProgram* program = new QOpenGLShaderProgram();
    program->addShaderFromSourceFile(QOpenGLShader::Vertex, QString(vertex.c_str()));
    program->addShaderFromSourceFile(QOpenGLShader::Fragment, QString(fragment.c_str()));
    program->link();
    if (!program->isLinked())
        std::cout << " shader could not create\n";
    return program;
}

QOpenGLBuffer Device::createVertexBuffer(QOpenGLBuffer::UsagePattern pattern)
{
    QOpenGLBuffer buffer;
    buffer.create();
    buffer.setUsagePattern(pattern);
    if (!buffer.isCreated())
        std::cout << " buffer could not create\n";
    return buffer;
}

QOpenGLTexture* Device::createTexture(const std::string *textureName)
{
    return new QOpenGLTexture(QImage(textureName->c_str()).mirrored());
}

QOpenGLTexture* Device::createTexture(const QImage *texture, QOpenGLTexture::MipMapGeneration genMipMaps )
{
    return new QOpenGLTexture(*texture, genMipMaps);
}

