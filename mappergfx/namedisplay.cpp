#include "namedisplay.h"
#include "nameplacer.h"
#include <iostream>
#include <QOpenGLShader>
#include <QOpenGLTexture>
#include "letterdisplay.h"
#include "fonttotexturearray.h"

using namespace renderer;
using namespace mappergfx;


NameDisplay::NameDisplay(const NamePlacer *region, QVector2D off) :
    /*TexturedObject(":/shaders/name.vert", ":/shaders/name.frag", letters[0]),*/ offset(off)
{

    mappergfx::FontToTextureArray text("arial.ttf");
    for (int a = 0; a < 26; a++)
        letters.push_back(text.getTexture(a));
    for (int a = 0; a < 26; a++)
        letterDisplays.push_back(new LetterDisplay(letters[a]));


    setText(region);

}

NameDisplay::NameDisplay(QVector2D off) : offset(off)
{
    mappergfx::FontToTextureArray text("arial.ttf");
    for (int a = 0; a < 26; a++)
        letters.push_back(text.getTexture(a));
    for (int a = 0; a < 26; a++)
        letterDisplays.push_back(new LetterDisplay(letters[a]));
}

void NameDisplay::addRegionToBuffer(const ConnectedRegions* region, std::vector<float>& buffer)
{
    std::vector<QVector2D> corners;

    corners.push_back(region->corners[0] + offset);
    corners.push_back(region->corners[1] + offset);
    corners.push_back(region->corners[2] + offset);
    corners.push_back(region->corners[3] + offset);

    QVector2D minorEdge(corners[1]-corners[0]);
    QVector2D majorEdge((-1*corners[0])+corners[3]);

    QVector2D halfWay((corners[0]+corners[1])/2.0f);
    float val(1.0f/(region->name.length()+2));

    QVector2D baseOne(val/2.0f*majorEdge);
    for (int a = 0; a < region->name.length(); a++)
    {
        QVector2D baseTwo(minorEdge.normalized()*baseOne.length());
        QVector2D center(halfWay+(val*majorEdge*(a+1))+baseOne);
        addLetterToBuffer(region->name[a], baseOne, baseTwo, center, buffer);
    }
}

void NameDisplay::addLetterToBuffer(QChar s, QVector2D baseVector, QVector2D heightVector, QVector2D center, std::vector<float>& buffer)
{
        if (s == ' ' || !s.isLetter() || !s.isUpper())
            return;
        std::vector<QVector2D> letterCorners;
        letterCorners.push_back(center - heightVector - baseVector);
        letterCorners.push_back(center + heightVector - baseVector);
        letterCorners.push_back(center + heightVector + baseVector);
        letterCorners.push_back(center - heightVector + baseVector);
        addBox(buffer, letterCorners, s.toLatin1());

}

NameDisplay::~NameDisplay()
{
    for (int a = 0; a < 26; a++)
        delete letterDisplays[a];
}

void NameDisplay::addBox(std::vector<float>& buffer, std::vector<QVector2D>& corners, char letter)
{
    QVector3D v(corners[0].x(), corners[0].y(), 0);
    buffer.push_back(v.x());
    buffer.push_back(v.y());
    buffer.push_back(v.z());

    buffer.push_back(0);
    buffer.push_back(0);
    buffer.push_back(int(letter-'A'));
    LetterCorner q1(v, QVector2D(0, 0));

    v = QVector3D(corners[1].x(), corners[1].y(), 0);
    buffer.push_back(v.x());
    buffer.push_back(v.y());
    buffer.push_back(v.z());

    buffer.push_back(0);
    buffer.push_back(1);
    buffer.push_back(int(letter-'A'));
    LetterCorner q2(v, QVector2D(0, 1));

    v = QVector3D(corners[3].x(), corners[3].y(), 0);
    buffer.push_back(v.x());
    buffer.push_back(v.y());
    buffer.push_back(v.z());

    buffer.push_back(1);
    buffer.push_back(0);
    buffer.push_back(int(letter-'A'));
    LetterCorner q3(v, QVector2D(1, 0));


    buffer.push_back(corners[3].x());
    buffer.push_back(corners[3].y());
    buffer.push_back(0);

    buffer.push_back(1);
    buffer.push_back(0);
    buffer.push_back(int(letter-'A'));
    LetterCorner q4(v, QVector2D(1, 0));

    buffer.push_back(corners[1].x());
    buffer.push_back(corners[1].y());
    buffer.push_back(0);

    buffer.push_back(0);
    buffer.push_back(1);
    buffer.push_back(int(letter-'A'));
    LetterCorner q5(q2);

    v = QVector3D(corners[2].x(), corners[2].y(), 0);
    buffer.push_back(v.x());
    buffer.push_back(v.y());
    buffer.push_back(v.z());

    buffer.push_back(1);
    buffer.push_back(1);
    buffer.push_back(int(letter-'A'));
    LetterCorner q6(v, QVector2D(1, 1));

    LetterQuad quad(q1, q2, q3, q4, q5, q6);
    letterDisplays[int(letter-'A')]->addQuads(quad);

}

void NameDisplay::setScale(QVector3D scale)
{
    for (int a = 0; a < 26; a++)
        letterDisplays[a]->getTransform()->setScale(scale);
}

void NameDisplay::setText(const NamePlacer* region)
{
    std::vector<float> buffer;

    for (int a = 0; a < 26; a++)
        letterDisplays[a]->clearVerticies();
    for (int a = 0; a < region->getRegionCount(); a++)
        addRegionToBuffer(region->getRegion(a), buffer);
    for (int a = 0; a < 26; a++)
        letterDisplays[a]->setQuads();
   // setBuffer(&buffer);

}

void NameDisplay::setLetters(std::vector<QOpenGLTexture *> ls)
{
    letters = ls;
    for (int a = 0; a < 26; a++)
     {
        letterDisplays[a]->setTexture(ls[a]);
    }
}

/*void NameDisplay::setBuffer(const std::vector<float>* vector)
{
    shader->bind();
    buffer.bind();
    AO.bind();

    buffer.allocate(&(vector->at(0)), vector->size()*sizeof(float));
    shader->enableAttributeArray(0);
    shader->enableAttributeArray(1);
    shader->enableAttributeArray(2);
    shader->setAttributeBuffer(0, GL_FLOAT, 0, 3, 6*sizeof(float));
    shader->setAttributeBuffer(1, GL_FLOAT, 3*sizeof(float), 2, 6*sizeof(float));
    shader->setAttributeBuffer(2, GL_FLOAT, 5*sizeof(float), 1, 6*sizeof(float));

    AO.release();
    buffer.release();
    shader->release();
    vCount = vector->size()/6;
}*/

/*void NameDisplay::Prerender()
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
*/
