#include "namedisplay.h"
#include "nameplacer.h"
#include <iostream>
#include <QOpenGLShader>
#include <QOpenGLTexture>

using namespace renderer;
using namespace mappergfx;

std::vector<QOpenGLTexture*> NameDisplay::letters;

NameDisplay::NameDisplay(const NamePlacer *region, QVector2D off) :
    TexturedObject(":/shaders/name.vert", ":/shaders/name.frag", letters[0]), offset(off)
{
    std::vector<float> buffer;

    for (int a = 0; a < region->getRegionCount(); a++)
        addRegionToBuffer(region->getRegion(a), buffer, off);
    setBuffer(&buffer);



   // transform.setScale(1, 1, 1);
   // transform.setTranslation(0, 0, -8.98);
   // renderState.facetCulling.setCullFace(renderer::CullFaceFrontAndBack);
    renderState.blending.setEnabled(true);
    renderState.blending.setDestinationAlphaFactor(DestinationBlendingOneMinusAlpha);
    renderState.blending.setSourceAlphaFactor(SourceBlendingSourceAlpha);
    renderState.depthMask = false;

    shader->bind();;
    std::vector<int> t;
    for (int a = 0; a < 26; a++)
        t.push_back(a+1);
    shader->setUniformValueArray("letters", &(t[0]), 26);

    /*for (int a = 0; a < 26; a++)
    {
        QString str("letters[");
        str.append(QString::number(a));
        str.append("]");
        int loc = shader->uniformLocation(str);
        if (loc < 0)
            std::cout << "error" << std::endl;
        shader->setUniformValue(loc, a+1);
    }*/
    shader->release();

    canBeDrawn = true;
}

void NameDisplay::addRegionToBuffer(const ConnectedRegions* region, std::vector<float>& buffer, QVector2D& offset)
{
    std::vector<QVector2D> corners;

    corners.push_back(region->corners[0] + offset);
    corners.push_back(region->corners[1] + offset);
    corners.push_back(region->corners[2] + offset);
    corners.push_back(region->corners[3] + offset);

    //addBox(&buffer, corners);
    QVector2D minorEdge(corners[1]-corners[0]);
    QVector2D majorEdge((-1*corners[0])+corners[3]);
    QVector2D halfWay((corners[0]+corners[1])/2.0f);
    float val(1.0f/(region->name.length()+2));
    QVector2D baseOne(val*majorEdge);
    QVector2D baseTwo(minorEdge.normalized()*baseOne.length());
    std::string s(region->name.toStdString());
    for (unsigned a = 0; a < region->name.length(); a++)
    {
        if (s[a] == ' ' || !region->name[a].isLetter() || !region->name[a].isUpper())
            continue;
        std::vector<QVector2D> letterCorners;
        letterCorners.push_back((halfWay-baseTwo) + ((a+1)*baseOne));
        letterCorners.push_back((halfWay+baseTwo) + ((a+1)*baseOne));
        letterCorners.push_back((halfWay+baseTwo) + ((a+2)*baseOne));
        letterCorners.push_back((halfWay-baseTwo) + ((a+2)*baseOne));
        addBox(buffer, letterCorners, s[a]);
    }

}

void NameDisplay::addBox(std::vector<float>& buffer, std::vector<QVector2D>& corners, char letter)
{
    buffer.push_back(corners[0].x());
    buffer.push_back(corners[0].y());
    buffer.push_back(0);

    buffer.push_back(0);
    buffer.push_back(0);
    buffer.push_back(int(letter-'A'));

    buffer.push_back(corners[1].x());
    buffer.push_back(corners[1].y());
    buffer.push_back(0);

    buffer.push_back(0);
    buffer.push_back(1);
    buffer.push_back(int(letter-'A'));

    buffer.push_back(corners[3].x());
    buffer.push_back(corners[3].y());
    buffer.push_back(0);

    buffer.push_back(1);
    buffer.push_back(0);
    buffer.push_back(int(letter-'A'));


    buffer.push_back(corners[3].x());
    buffer.push_back(corners[3].y());
    buffer.push_back(0);

    buffer.push_back(1);
    buffer.push_back(0);
    buffer.push_back(int(letter-'A'));

    buffer.push_back(corners[1].x());
    buffer.push_back(corners[1].y());
    buffer.push_back(0);

    buffer.push_back(0);
    buffer.push_back(1);
    buffer.push_back(int(letter-'A'));

    buffer.push_back(corners[2].x());
    buffer.push_back(corners[2].y());
    buffer.push_back(0);

    buffer.push_back(1);
    buffer.push_back(1);
    buffer.push_back(int(letter-'A'));

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
    shader->enableAttributeArray(2);
    shader->setAttributeBuffer(0, GL_FLOAT, 0, 3, 6*sizeof(float));
    shader->setAttributeBuffer(1, GL_FLOAT, 3*sizeof(float), 2, 6*sizeof(float));
    shader->setAttributeBuffer(2, GL_FLOAT, 5*sizeof(float), 1, 6*sizeof(float));

    AO.release();
    buffer.release();
    texture->release();
    shader->release();
    vCount = vector->size()/6;
}

void NameDisplay::Prerender()
{
    TexturedObject::Prerender();
    for (int a = 0; a < 26; a++)
        letters[a]->bind(a+1);
}

void NameDisplay::PostRender()
{
    for (int a = 0; a < 26; a++)
        letters[a]->release();
    TexturedObject::PostRender();
}
