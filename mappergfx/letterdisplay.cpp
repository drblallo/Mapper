#include "letterdisplay.h"
#include <QOpenGLTexture>
#include <QOpenGLShader>
using namespace renderer;
using namespace mappergfx;

LetterDisplay::LetterDisplay(QOpenGLTexture* texture) : TexturedObject(":/shaders/letter.vert", ":/shaders/letter.frag", texture), vertexCount(0)
{
    shader->bind();
    shader->enableAttributeArray(0);
    shader->enableAttributeArray(1);
    int val = shader->uniformLocation("tex");
    shader->setUniformValue(val, 0);
    shader->release();

    renderState.blending.setEnabled(true);
    renderState.blending.setDestinationAlphaFactor(DestinationBlendingOneMinusAlpha);
    renderState.blending.setSourceAlphaFactor(SourceBlendingSourceAlpha);
    renderState.depthMask = false;

    transform.setTranslation(0, 0, 0.02f);
    transform.setScale(1, 1, 1);
    transform.setRotation(QQuaternion::fromEulerAngles(0, 0, 0));

    canBeDrawn = true;
}

LetterDisplay::~LetterDisplay()
{

}

void LetterDisplay::setQuads()
{
    LetterDisplay::setBuffer();
    vertexCount = vertices.size()*6;
}

void LetterDisplay::clearVerticies()
{
    vertices.clear();
}

void LetterDisplay::addQuads(std::vector<LetterQuad> extremes)
{
    for (unsigned a = 0; a < extremes.size(); a++)
        vertices.push_back(extremes[a]);
}

void LetterDisplay::addQuads(LetterQuad extremes)
{
    vertices.push_back(extremes);
}

void LetterDisplay::setBuffer()
{
    shader->bind();
    buffer.bind();
    AO.bind();

    if (vertices.size() > 0)
    {
        buffer.allocate(&(vertices.at(0)), vertices.size()*sizeof(LetterQuad));
        shader->enableAttributeArray(0);
        shader->enableAttributeArray(1);
        shader->setAttributeBuffer(0, GL_FLOAT, 0, 3, 5*sizeof(float));
        shader->setAttributeBuffer(1, GL_FLOAT, 3*sizeof(float), 2, 5*sizeof(float));
    }
    else
        buffer.allocate(0, 0);

    AO.release();
    buffer.release();
    shader->release();
}

void LetterDisplay::setTexture(QOpenGLTexture *newTexture)
{
    texture = newTexture;
}

LetterQuad::LetterQuad(
    LetterCorner& q1,
    LetterCorner& q2,
    LetterCorner& q3,
    LetterCorner& q4,
    LetterCorner& q5,
    LetterCorner& q6
    )
{
    letterCorners[0] = q1;
    letterCorners[1] = q2;
    letterCorners[2] = q3;
    letterCorners[3] = q4;
    letterCorners[4] = q5;
    letterCorners[5] = q6;
}
int LetterDisplay::getVertexCount() const
{
    return vertexCount;
}
