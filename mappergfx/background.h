#pragma once
#include "render/texturedobject.h"
#include "render/engineobject.h"
#include <QOpenGLFunctions_3_3_Core>

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
            virtual int getVertexCount() const;
			virtual GLenum getRenderMode() const {return GL_TRIANGLES;}
			void setProvinceMask(ProvincesMask& mask);
            virtual void Update();
			virtual void OnMouseDown(QVector3D localPoint);
            void setSelected(int index);
            void setBlackSpaceAlpha(float value);
            void setTextureImportace(float value);

			Background(const Background&) = delete;
			Background& operator= (const TexturedObject&) = delete;
			void setBackgroundTexture(QString path);
			

		protected:
			virtual void Prerender();
			virtual void PostRender();
            void addProvinceQuad(QVector4D color, int id, std::vector<float>& list);
			mapreader::Map* map;
			QOpenGLBuffer regionColorBuffer;
			QOpenGLTexture* backgroundTexture;
            int currSelected;
            QOpenGLBuffer colorBuffer;
            QOpenGLFunctions_3_3_Core functions;

	};
}

