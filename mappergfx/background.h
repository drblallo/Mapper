#pragma once
#include "render/texturedobject.h"

namespace mapreader
{
	class Map;
}

namespace mappergfx
{
	class ProvincesMask;
	class Background : public renderer::TexturedObject
	{
		public:
			Background(mapreader::Map* m);
			virtual ~Background();
			virtual bool hitted(const QVector3D*, const QVector3D*) const{return false;}
			virtual int getVertexCount() const {return 6;}
			virtual GLenum getRenderMode() const {return GL_TRIANGLES;}
			void setProvinceMask(ProvincesMask& mask);

			Background(const Background&) = delete;
			Background& operator= (const TexturedObject&) = delete;
			

		protected:
			virtual void Prerender();
			mapreader::Map* map;
			QOpenGLBuffer regionColorBuffer;
	};
}

