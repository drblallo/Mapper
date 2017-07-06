#pragma once
#include "render/renderobject.h"
#include <string>

namespace renderer
{
    class TestQuad : public RenderObject
    {
        public:
            TestQuad();
            virtual ~TestQuad();
            virtual bool hitted(const QVector3D* source, const QVector3D* direction) const;
            static std::string vertString;
            static std::string fragString;
            virtual void Update();

        protected:
            virtual int getVertexCount() const;
            virtual GLenum getRenderMode() const;
    };
}
