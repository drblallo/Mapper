#pragma once
#include "dottedobject.h"

namespace renderer
{
    class LinesObject : public DottedObject
    {
        public:
            LinesObject(std::vector<Dot>* lines);
            LinesObject(std::vector<float> *lines, QOpenGLShaderProgram* program, int vertexCount);
            LinesObject(std::vector<Dot> *lines, QOpenGLShaderProgram* program);
            LinesObject(QOpenGLShaderProgram* program);
            virtual ~LinesObject();

        protected:
            virtual GLenum getRenderMode() const {return GL_LINES;}
    };
}
