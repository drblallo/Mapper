#include "testQuad.h"
#include "render/device.h"
#include <QOpenGLShaderProgram>
#include <QVector3D>
#include <iostream>
using namespace renderer;

std::string TestQuad::vertString(":/shaders/simple.vert");
std::string TestQuad::fragString(":/shaders/simple.frag");

// Front Verticies
#define VERTEX_FTR  QVector3D( 0.5f,  0.5f,  0.5f), QVector3D( 1.0f, 0.0f, 0.0f )
#define VERTEX_FTL  QVector3D(-0.5f,  0.5f,  0.5f), QVector3D( 0.0f, 1.0f, 0.0f )
#define VERTEX_FBL  QVector3D(-0.5f, -0.5f,  0.5f), QVector3D( 0.0f, 0.0f, 1.0f )
#define VERTEX_FBR  QVector3D( 0.5f, -0.5f,  0.5f), QVector3D( 0.0f, 0.0f, 0.0f )

// Back Verticies
#define VERTEX_BTR  QVector3D( 0.5f,  0.5f, -0.5f), QVector3D( 1.0f, 1.0f, 0.0f )
#define VERTEX_BTL  QVector3D(-0.5f,  0.5f, -0.5f), QVector3D( 0.0f, 1.0f, 1.0f )
#define VERTEX_BBL  QVector3D(-0.5f, -0.5f, -0.5f), QVector3D( 1.0f, 0.0f, 1.0f )
#define VERTEX_BBR  QVector3D( 0.5f, -0.5f, -0.5f), QVector3D( 1.0f, 1.0f, 1.0f )

// Create a colored cube
static const QVector3D sg_vertexes[] = {
  // Face 1 (Front)
    VERTEX_FTR, VERTEX_FTL, VERTEX_FBL,
    VERTEX_FBL, VERTEX_FBR, VERTEX_FTR,
  // Face 2 (Back)
    VERTEX_BBR, VERTEX_BTL, VERTEX_BTR,
    VERTEX_BTL, VERTEX_BBR, VERTEX_BBL,
  // Face 3 (Top)
    VERTEX_FTR, VERTEX_BTR, VERTEX_BTL,
    VERTEX_BTL, VERTEX_FTL, VERTEX_FTR,
  // Face 4 (Bottom)
    VERTEX_FBR, VERTEX_FBL, VERTEX_BBL,
    VERTEX_BBL, VERTEX_BBR, VERTEX_FBR,
  // Face 5 (Left)
    VERTEX_FBL, VERTEX_FTL, VERTEX_BTL,
    VERTEX_FBL, VERTEX_BTL, VERTEX_BBL,
  // Face 6 (Right)
    VERTEX_FTR, VERTEX_FBR, VERTEX_BBR,
    VERTEX_BBR, VERTEX_BTR, VERTEX_FTR
};

#undef VERTEX_BBR
#undef VERTEX_BBL
#undef VERTEX_BTL
#undef VERTEX_BTR

#undef VERTEX_FBR
#undef VERTEX_FBL
#undef VERTEX_FTL
#undef VERTEX_FTR

TestQuad::TestQuad() : RenderObject(vertString, fragString)
{
    setBuffer(&sg_vertexes, sizeof(sg_vertexes));

    renderState.depthTest.setEnabled(true);

    transform.setTranslation(0.0f, 0.0f, -50.0f);
    transform.setScale(50, 50, 50);
    canBeDrawn = true;
}

TestQuad::~TestQuad()
{
}

bool TestQuad::hitted(const QVector3D*, const QVector3D*) const
{
    return false;
}

GLenum TestQuad::getRenderMode() const
{
    return GL_TRIANGLES;
}

int TestQuad::getVertexCount() const
{
    return 36;
}

void TestQuad::Update()
{
   // transform.rotate(1.0f, QVector3D(0.4f, 0.3f, 0.3f));
}

