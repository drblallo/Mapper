#include "province.h"
#include "subprovince.h"
#include "mapreaderdefines.h"
#define SPEAK

using namespace mapreader;

Province::Province(QRgb& col, Map* m) : map(m), color(col), id(getIdFromColor(col))
{
}

unsigned Province::getIndex() const
{
	return index;
}

long Province::getIdFromColor(QRgb color)
{
	int id(0);
	id = qRed(color) * 300 * 300;	
	id += (qBlue(color) * 300);
	id += qGreen(color);
	return id;
}

long Province::getId() const
{
	return id;
}

const Map* Province::getMap() const
{
	return map;
}

QRgb Province::getColor() const
{
	return color;
}

Province::~Province()
{
	for (int a = subProvinces.size() - 1; a >= 0; a--)
		delete subProvinces[a];
}

void Province::addSubProvince(const std::vector<position>& border, QImage& source, QRgb** map)
{
	SubProvince* p(new SubProvince(this,border, map, source));
	subProvinces.push_back(p);
}
