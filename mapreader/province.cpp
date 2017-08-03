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
	if (subProvinces.size() == 0)
		largestBox = 0;
	else
		if (p->getBoxSize() > subProvinces[largestBox]->getBoxSize())
			largestBox = subProvinces.size();
			
	subProvinces.push_back(p);
}

const std::pair<float, float>* Province::getCornerBox() const 
{
	if (subProvinces.size() == 0)
		return NULL;
    return subProvinces[largestBox]->getCornerBox();
}

const std::pair<float, float> *Province::getBoudingBox() const
{
    return subProvinces[largestBox]->getBoundingBox();
}


void Province::updateNeighbourData()
{
	for (unsigned a = 0; a < subProvinces.size(); a++)
	{
		subProvinces[a]->updateNeighbourData();
	}
}

//#include <iostream>
void Province::getNeighbours(std::vector<const Province*>* out) const
{
	if (index == 0)
		return;
	for (unsigned a = 0; a < subProvinces.size(); a++)
	{
		SubProvince* s(subProvinces[a]);
		for (unsigned b = 0; b < s->borderNeighbours.size(); b++)
		{
			const Province* p(s->neighbours[s->borderNeighbours[b].neighbourID]);
			bool found(std::find(out->begin(), out->end(), p) != out->end());
			if (!found)
				out->push_back(p);	
		}
	}
}
