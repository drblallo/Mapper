#pragma once
#include <QVector3D>
#include <QOpenGLTexture>

namespace mapreader
{
	class Map;
}

namespace mappergfx
{
	class Background;
	class Borders;
	class ProvincesMask;
    class NameDisplay;
	class MapGFX
	{
		public:
			MapGFX(mapreader::Map& m, float borderScale);
			~MapGFX();
			MapGFX& operator=(MapGFX& other) = delete;
			MapGFX(MapGFX& other) = delete;
			void scale(const QVector3D& vec);
			void scale(float x, float y, float z);
			void applyMask(ProvincesMask* mask);
			void setBackground(QString path);
			QVector3D getScale() const;
			void createTexts(const ProvincesMask* mask);
            void setCurrentSelected(int index);

		private:
			mapreader::Map& map;
			Background* background;
			Borders* borders;

            std::vector<NameDisplay*> areas;
            QOpenGLTexture* textTexture;


	};
}
