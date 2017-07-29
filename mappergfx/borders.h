#pragma once
#include "render/texturedobject.h"

namespace mapreader
{
    class Map;
}

namespace mappergfx
{
	class ProvincesMask;
    class Borders : public renderer::TexturedObject
    {
        public:
            Borders(mapreader::Map* m, float scale, ProvincesMask* mask);
            virtual ~Borders();
            virtual bool hitted(const QVector3D *, const QVector3D *) const {return false;}
            virtual GLenum getRenderMode() {return GL_TRIANGLES;}
            virtual int getVertexCount() const {return vertexCount;}
			void setProvinceMask(ProvincesMask* mask);
			virtual void setBuffer(const void * pos, int size);

            Borders(const Borders&) = delete;
            Borders& operator= (const Borders&) = delete;

        private:
            mapreader::Map* map;
            void setUpData();
            int vertexCount;
			float scale;

    };
}
