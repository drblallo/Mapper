#include "texturedobject.h"
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <iostream>

using namespace renderer;
//########################################
//##### Constructors and Conversions #####
//########################################
TexturedObject::TexturedObject(
        const std::string& vert,
        const std::string& frag,
        QOpenGLTexture *tex
        ) : RenderObject(vert, frag), texture(tex)
{

}

TexturedObject::~TexturedObject()
{

}


void TexturedObject::Prerender()
{
    RenderObject::Prerender();
    texture->bind();
    //shader->setUniformValue(textureUniformLocation, texture);
}

void TexturedObject::PostRender()
{
    texture->release();
    RenderObject::PostRender();
}

void TexturedObject::setBuffer(const std::vector<QVector3D>* list)
{
    setBuffer(&(list->at(0)), sizeof(QVector3D)*list->size());
}
void TexturedObject::setBuffer(const void * pos, int size)
{
    shader->bind();
    texture->bind();
    buffer.bind();
    AO.bind();

    buffer.allocate(pos, size);
    shader->enableAttributeArray(0);
    shader->enableAttributeArray(1);
    shader->enableAttributeArray(2);
    shader->setAttributeBuffer(0, GL_FLOAT, 0, 3, 9*sizeof(float));
    shader->setAttributeBuffer(1, GL_FLOAT, 3*sizeof(float), 4, 9*sizeof(float));
    shader->setAttributeBuffer(2, GL_FLOAT, 7*sizeof(float), 2, 9*sizeof(float));

    AO.release();
    buffer.release();
    texture->release();
    shader->release();
}
