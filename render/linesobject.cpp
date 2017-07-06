#include "linesobject.h"

using namespace renderer;
LinesObject::LinesObject(std::vector<Dot>* lines) : DottedObject(lines)
{

}

LinesObject::LinesObject(std::vector<Dot>* lines, QOpenGLShaderProgram *program) : DottedObject(lines, program)
{

}

LinesObject::LinesObject(QOpenGLShaderProgram *program) : DottedObject(program)
{

}

LinesObject::LinesObject(std::vector<float>* lines, QOpenGLShaderProgram *program, int vertexCount) : DottedObject(lines, program, vertexCount)
{

}

LinesObject::~LinesObject()
{

}
