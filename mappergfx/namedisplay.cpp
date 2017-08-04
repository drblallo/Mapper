#include "namedisplay.h"
#include "nameplacer.h"
#include <QOpenGLShader>
#include <QOpenGLTexture>

using namespace renderer;
using namespace mappergfx;
NameDisplay::NameDisplay(const ConnectedRegions *region, QOpenGLTexture* texture, QVector2D off) :
    TexturedObject(":/shaders/name.vert", ":/shaders/name.frag", texture), offset(off)
{
    std::vector<float> buffer;
    std::vector<QVector2D> corners;

    corners.push_back(region->corners[0] + offset);
    corners.push_back(region->corners[1] + offset);
    corners.push_back(region->corners[2] + offset);
    corners.push_back(region->corners[3] + offset);

    //addBox(&buffer, corners);
    QVector2D minorEdge(corners[1]-corners[0]);
    QVector2D majorEdge((-1*corners[0])+corners[3]);
    QVector2D halfWay((corners[0]+corners[1])/2.0f);
    float val(1.0f/region->name.length());
    QVector2D baseOne(val*majorEdge);
    QVector2D baseTwo(minorEdge.normalized()*baseOne.length());
    for (unsigned a = 0; a < region->name.length(); a++)
    {
        std::vector<QVector2D> letterCorners;
        letterCorners.push_back((halfWay-baseTwo) + (a*baseOne));
        letterCorners.push_back((halfWay+baseTwo) + (a*baseOne));
        letterCorners.push_back((halfWay+baseTwo) + ((a+1)*baseOne));
        letterCorners.push_back((halfWay-baseTwo) + ((a+1)*baseOne));
        addBox(buffer, letterCorners);
    }

    setBuffer(&buffer);


    buffer.clear();
    std::string s(region->name.toStdString());
    for (unsigned a = 0; a < s.length(); a++)
    {
        char c(s[a]);
        buffer.push_back(int(c));
    }

    shader->bind();
    shader->setUniformValueArray("text", &(buffer[0]), buffer.size(), 1);
    shader->setUniformValue("textLenght", (GLint)buffer.size());
    shader->setUniformValueArray("corners", &(corners[0]), 4);
    shader->release();

   // transform.setScale(1, 1, 1);
   // transform.setTranslation(0, 0, -8.98);
   // renderState.facetCulling.setCullFace(renderer::CullFaceFrontAndBack);
    renderState.blending.setEnabled(true);
    renderState.blending.setDestinationAlphaFactor(DestinationBlendingOneMinusAlpha);
    renderState.blending.setSourceAlphaFactor(SourceBlendingSourceAlpha);
    renderState.depthMask = false;
    canBeDrawn = true;
}

void NameDisplay::addBox(std::vector<float>& buffer, std::vector<QVector2D>& corners)
{
    buffer.push_back(corners[0].x());
    buffer.push_back(corners[0].y());
    buffer.push_back(0);

    buffer.push_back(0);
    buffer.push_back(0);

    buffer.push_back(corners[1].x());
    buffer.push_back(corners[1].y());
    buffer.push_back(0);

    buffer.push_back(0);
    buffer.push_back(1);

    buffer.push_back(corners[3].x());
    buffer.push_back(corners[3].y());
    buffer.push_back(0);

    buffer.push_back(1);
    buffer.push_back(0);


    buffer.push_back(corners[2].x());
    buffer.push_back(corners[2].y());
    buffer.push_back(0);

    buffer.push_back(1);
    buffer.push_back(1);

}

void NameDisplay::setBuffer(const std::vector<float>* vector)
{
    shader->bind();
    texture->bind();
    buffer.bind();
    AO.bind();

    int val = shader->uniformLocation("tex");
    shader->setUniformValue(val, 0);
    buffer.allocate(&(vector->at(0)), vector->size()*sizeof(float));
    shader->enableAttributeArray(0);
    shader->enableAttributeArray(1);
    shader->setAttributeBuffer(0, GL_FLOAT, 0, 3, 5*sizeof(float));
    shader->setAttributeBuffer(1, GL_FLOAT, 3*sizeof(float), 2, 5*sizeof(float));

    AO.release();
    buffer.release();
    texture->release();
    shader->release();
    vCount = vector->size()/5;
}
