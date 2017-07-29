#pragma once
#include "render/texturedobject.h"
#include "render/engineobject.h"

namespace mapreader
{
	class Map;
}

namespace mappergfx
{
	class ProvincesMask;
    class Background : public renderer::TexturedObject, renderer::EngineObject
	{
		public:
            Background(mapreader::Map* m, ProvincesMask* mask);
			virtual ~Background();
			virtual bool hitted(const QVector3D*, const QVector3D*) const{return false;}
			virtual int getVertexCount() const {return 6;}
			virtual GLenum getRenderMode() const {return GL_TRIANGLES;}
			void setProvinceMask(ProvincesMask& mask);
            virtual void Update();

			Background(const Background&) = delete;
			Background& operator= (const TexturedObject&) = delete;
			

		protected:
			virtual void Prerender();
			mapreader::Map* map;
			QOpenGLBuffer regionColorBuffer;
	};
}

