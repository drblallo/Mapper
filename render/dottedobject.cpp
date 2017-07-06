#include "dottedobject.h"
#include <iostream>
#include "device.h"

using namespace renderer;
std::string DottedObject::vertString(":/shaders/simple.vert");
std::string DottedObject::fragString(":/shaders/simple.frag");
QOpenGLShaderProgram* DottedObject::pshader(NULL);

//########################################
//##### Constructors and Conversions #####
//########################################
DottedObject::DottedObject(const std::vector<Dot> *dots) : RenderObject(getShader())
{
    if (!dots)
    {
        std::cout << "no points to spawn\n";
        return;
    }
    if (dots->size() > 0)
        setDots(dots);
    canBeDrawn = true;
}

DottedObject::DottedObject
	(
		const std::vector<Dot>* dots,
	   	QOpenGLShaderProgram* program
	) : RenderObject(program)
{
    if (!dots)
    {
        std::cout << "no points to spawn\n";
        return;
    }
    if (dots->size() > 0)
        setDots(dots);
    canBeDrawn = true;

}

DottedObject::DottedObject(QOpenGLShaderProgram* program) : RenderObject(program)
{
    canBeDrawn = true;
}

DottedObject::DottedObject
	(
		const std::vector<float>* dots,
	   	QOpenGLShaderProgram* program,
	   	int vertexCount
	) : RenderObject(program)
{
    if (!dots)
    {
        std::cout << "no points to spawn\n";
        return;
    }
    if (dots->size() > 0)
        setDots(dots, vertexCount);
    canBeDrawn = true;

}

DottedObject::~DottedObject()
{

}

//########################################
//#####           Methods            #####
//########################################
void DottedObject::setDots(const std::vector<Dot> *dots)
{
    std::cout << "setting up a dotted object of " << dots->size() << "\n";
    vCount = dots->size();
    setBuffer(&(dots->at(0)), dots->size() * sizeof(Dot));
    //std::cout << dots->size() << "\n";
}

void DottedObject::setDots(const std::vector<float> *dots, int dotCount)
{
    std::cout << "setting up a dotted object with " << dotCount << " vertex\n";
    vCount = dotCount;
    setBuffer(&(dots->at(0)), dots->size() * sizeof(float));
    //std::cout << dots->size() << "\n";
}

GLenum DottedObject::getRenderMode() const
{
    return GL_POINTS;
}

QOpenGLShaderProgram* DottedObject::getShader()
{
    if (!pshader)
    {
        pshader = Device::createProgram(vertString, fragString);
    }
    return pshader;
}
