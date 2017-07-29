#include "provincesmask.h"
#include "mapreader/map.h"
#include "mapreader/province.h"

#include <iostream>

using namespace mapreader;
using namespace mappergfx;

ProvincesMask::ProvincesMask(Map* m) : map(m)//, image(m->getTexture()->width(), m->getTexture()->height(), m->getTexture()->format())
{
	for (unsigned a = 0; a < m->getProvincesList()->size(); a++)
	{
        QColor rgb(m->getProvincesList()->at(a)->getColor());
        provincesColor.push_back(QVector3D(rgb.redF() , rgb.greenF() , rgb.blueF()));
	}	

	
//	updateImage();
}

/*void ProvincesMask::updateImage()
{
    //const std::vector<std::vector<int> >* img(map->getRedTexture());
	QRgb* modified[image.height()];
	for (int a = 0; a < image.height(); a++)
		modified[a] = (QRgb*)image.scanLine(a);

	for (int a = 0; a < image.height(); a++)
	{	
		for (int b = 0; b < image.width(); b++)
		{

            unsigned target(map->getIndexOfPixel(b, a));
			if (target >= map->getProvincesList()->size())
				continue;
			Province* p(map->getProvincesList()->at(target));
			int index(p->getIndex());
            modified[a][b] = QColor(provincesColor[index].x(), provincesColor[index].y(), provincesColor[index].z()).rgb();
		}
	}
	std::cout << "ended" << std::endl;

}*/

void ProvincesMask::setColor(QColor col, long id)
{
    int target(map->getProvince(id)->getIndex());
    setColor(col, target);
}

void ProvincesMask::setColor(QColor col, int index)
{
    if (index < provincesColor.size() && index >= 0)
        provincesColor[index] = QVector3D(col.redF(), col.greenF(), col.blueF());
}

QRgb ProvincesMask::getColor(long id) const
{
    int target(map->getProvince(id)->getIndex());
    return getColor(target);
}

QRgb ProvincesMask::getColor(int index) const
{
    if (index < provincesColor.size() && index >= 0)
          return QColor(provincesColor[index].x()*255, provincesColor[index].y()*255, provincesColor[index].z()*255).rgb();
    else
        return QColor(0, 0, 0).rgb();
}
