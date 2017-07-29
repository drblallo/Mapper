#pragma once
#include <QVector3D>
#include <vector> 
#include <QColor>
#include <QImage>

namespace mapreader
{
	class Map;
}

namespace mappergfx
{
	class ProvincesMask
	{
		public:
			ProvincesMask(mapreader::Map* m);	
			void setColor(QColor col, long id);
            void setColor(QColor col, int index);
            const std::vector<QVector3D>* getColors() const {return &provincesColor;}
            QRgb getColor(long id) const;
            QRgb getColor(int index) const;
    //		const QImage* getImage() const {return &image;}
            //void updateImage();

		private:
			mapreader::Map* map;
            std::vector<QVector3D> provincesColor;
        //	QImage image;

	};
}
