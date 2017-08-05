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
            QString getName(int index) const;
            QString getName(long id) const;
            QString getName(QColor col) const;
            void setName(QColor col, QString name);
            const mapreader::Map* getMap() const {return map;}
			mapreader::Map* getMap() {return map;}

		private:
			mapreader::Map* map;
            std::vector<QVector3D> provincesColor;
            std::vector<std::pair<QColor, QString> > names;
    };
}
