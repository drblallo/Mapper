#include "textureswarmobject.h"
#include "iostream"

using namespace renderer;

std::string TextureSwarmObject::vertString(":/shaders/simpleTex.vert");
std::string TextureSwarmObject::fragString(":/shaders/simpleTex.frag");

TextureSwarmObject::TextureSwarmObject(
        std::vector<FloatingQuad>* quads,
        QOpenGLTexture* texture
        ) :
    TexturedObject(vertString, fragString, texture)

{
    setFloatingQuads(quads);
   // renderState.facetCulling.setCullFace(CullFaceFrontAndBack);
    renderState.blending.setEnabled(true);
    renderState.blending.setDestinationAlphaFactor(DestinationBlendingOneMinusAlpha);
    renderState.blending.setSourceAlphaFactor(SourceBlendingSourceAlpha);
    renderState.depthMask = false;
    canBeDrawn = true;
}

TextureSwarmObject::~TextureSwarmObject()
{

}

void TextureSwarmObject::setFloatingQuads(std::vector<FloatingQuad> *quads)
{
    std::vector<float> list;

    QVector3D upperLeft(-1, 1, 0);
    QVector3D upperRight(1, 1, 0);
    QVector3D lowerRight(1, -1, 0);
    QVector3D lowerLeft(-1, -1, 0);

    for (unsigned int a = 0; a < quads->size(); a++)
    {
        QMatrix4x4 mat = quads->at(a).transform.toMatrix();
        QVector3D v = mat * upperLeft;

        list.push_back(v.x());
        list.push_back(v.y());
        list.push_back(v.z());

        list.push_back(quads->at(a).color[0].x());
        list.push_back(quads->at(a).color[0].y());
        list.push_back(quads->at(a).color[0].z());
        list.push_back(quads->at(a).color[0].w());

        list.push_back(0);
        list.push_back(1);


        v = mat * lowerLeft;

        list.push_back(v.x());
        list.push_back(v.y());
        list.push_back(v.z());

        list.push_back(quads->at(a).color[2].x());
        list.push_back(quads->at(a).color[2].y());
        list.push_back(quads->at(a).color[2].z());
        list.push_back(quads->at(a).color[2].w());

        list.push_back(0);
        list.push_back(0);

        v = mat * upperRight;

        list.push_back(v.x());
        list.push_back(v.y());
        list.push_back(v.z());

        list.push_back(quads->at(a).color[1].x());
        list.push_back(quads->at(a).color[1].y());
        list.push_back(quads->at(a).color[1].z());
        list.push_back(quads->at(a).color[1].w());

        list.push_back(1);
        list.push_back(1);

        v = mat * upperRight;

        list.push_back(v.x());
        list.push_back(v.y());
        list.push_back(v.z());

        list.push_back(quads->at(a).color[1].x());
        list.push_back(quads->at(a).color[1].y());
        list.push_back(quads->at(a).color[1].z());
        list.push_back(quads->at(a).color[1].w());

        list.push_back(1);
        list.push_back(1);

        v = mat * lowerLeft;

        list.push_back(v.x());
        list.push_back(v.y());
        list.push_back(v.z());

        list.push_back(quads->at(a).color[2].x());
        list.push_back(quads->at(a).color[2].y());
        list.push_back(quads->at(a).color[2].z());
        list.push_back(quads->at(a).color[2].w());

        list.push_back(0);
        list.push_back(0);

        v = mat * lowerRight;

        list.push_back(v.x());
        list.push_back(v.y());
        list.push_back(v.z());

        list.push_back(quads->at(a).color[3].x());
        list.push_back(quads->at(a).color[3].y());
        list.push_back(quads->at(a).color[3].z());
        list.push_back(quads->at(a).color[3].w());

        list.push_back(1);
        list.push_back(0);
    }
    setBuffer(&(list[0]), sizeof(float)*list.size());
    vCount = 6*quads->size();
}
