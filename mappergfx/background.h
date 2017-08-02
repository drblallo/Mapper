#pragma once
#include "render/texturedobject.h"
#include "render/engineobject.h"

namespace mapreader
{
	class Map;
}

class QOpenGLTexture;

namespace mappergfx
{
	class ProvincesMask;
    class Background : public renderer::TexturedObject, renderer::EngineObject
	{
		public:
            Background(mapreader::Map* m, ProvincesMask* mask);
			virtual ~Background();
			virtual bool hitted(const QVector3D*, const QVector3D*, QVector3D*) const;
			virtual int getVertexCount() const {return 6;}
			virtual GLenum getRenderMode() const {return GL_TRIANGLES;}
			void setProvinceMask(ProvincesMask& mask);
            virtual void Update();
			virtual void OnMouseDown(QVector3D localPoint);
            void setSelected(int index);

			Background(const Background&) = delete;
			Background& operator= (const TexturedObject&) = delete;
			void setBackgroundTexture(QString path);
			

		protected:
			virtual void Prerender();
			virtual void PostRender();
			mapreader::Map* map;
			QOpenGLBuffer regionColorBuffer;
			QOpenGLTexture* backgroundTexture;
            int currSelected;

	};
}

