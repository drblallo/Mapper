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
}


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

QString ProvincesMask::getName(int index) const
{
    QColor col(getColor(index));
    return getName(col);
}

QString ProvincesMask::getName(long id) const
{
    QColor col(getColor(id));
    return getName(col);
}

QString ProvincesMask::getName(QColor col) const
{
    for (unsigned a = 0; a < names.size(); a++)
        if (names[a].first == col)
            return names[a].second;
}
void ProvincesMask::setName(QColor col, QString name)
{
    names.push_back(std::pair<QColor, QString>(col, name));
}
