#pragma once
#include "dottedobject.h"

namespace renderer
{
    class TrisObject : public DottedObject
    {
        public:
            TrisObject(std::vector<Dot>* lines);
            virtual ~TrisObject();

        protected:
            virtual GLenum getRenderMode() const {return GL_TRIANGLES;}
    };
}
