#include "nameplacer.h"
#include "provincesmask.h"
#include "mapreader/map.h"
#include "mapreader/province.h"
#include <iostream>
#include "mapreader/subprovince.h"

using namespace mappergfx;
using namespace mapreader;

void insertAllNeighbours(std::vector<bool>* placed, const std::vector<Province*>* provinceList, ConnectedRegions* region, int currentInserting, const ProvincesMask* mask)
{
	std::vector<const Province*> justAdded;
	std::vector<const Province*> support;
	Province* p(provinceList->at(currentInserting));
	justAdded.push_back(p);
	placed->at(currentInserting) = true;
	QRgb col(mask->getColor(currentInserting));
	region->regionIndexes.push_back(currentInserting);
	
	do
	{
		support = justAdded;
		justAdded.clear();

		for (unsigned a = 0; a < support.size(); a++)
		{
			std::vector<const Province*> neighbours;
			support[a]->getNeighbours(&neighbours);

			for (unsigned b = 0; b < neighbours.size(); b++)
			{
				int index(neighbours[b]->getIndex());
				QRgb otherCol(mask->getColor(index));
				if (!placed->at(index) && otherCol == col)
				{
                    std::cout << qRed(col)<< ", " << qGreen(col) << ", " << qBlue(col) << std::endl;
					justAdded.push_back(neighbours[b]);
					region->regionIndexes.push_back(index);
					placed->at(index) = true;
				}
			}
		}

	}
	while (!justAdded.empty());
}

NamePlacer::NamePlacer(const ProvincesMask* mask)
{
	const std::vector<Province*>* provinceList(mask->getMap()->getProvincesList());
	std::vector<bool> placed;
	for (unsigned a = 0; a < provinceList->size(); a++)
		placed.push_back(false);

	for (unsigned a = 1; a < placed.size(); a++)
	{
		division.push_back(ConnectedRegions());
		insertAllNeighbours(&placed, provinceList, &division.back(), a, mask);

        while (placed[a+1] && a+1 < placed.size())
        {
			a++;
        }
	}	

	generateBoxes(mask);
}


std::pair<float, float> rotatePairVector(std::pair<float, float> vector, float angle)
{
    float x((vector.first * std::cos(angle)) -( vector.second * std::sin(angle)));
    float y((vector.first * std::sin(angle)) + (vector.second * std::cos(angle)));
    return std::pair<float, float>(x, y);
}

QVector2D rotateQVector(QVector2D vector, float angle)
{
    float x((vector.x() * std::cos(angle)) - (vector.y()* std::sin(angle)));
    float y((vector.x() * std::sin(angle)) + (vector.y() * std::cos(angle)));
    return QVector2D(x, y);
}

void generateBoundingBox(ConnectedRegions* reg, const ProvincesMask* mask, QVector2D* outMin, QVector2D* outMax, float angle)
{
    const Province* p(mask->getMap()->getProvinceFromIndex(reg->regionIndexes[0]));
    auto box(p->getCornerBox());

    QVector2D min(box[0].first, box[0].second);
    QVector2D max(box[2].first, box[2].second);
    min = rotateQVector(min, angle);
    max = rotateQVector(max, angle);

    for (unsigned b = 0; b < reg->regionIndexes.size(); b++)
    {
        p = (mask->getMap()->getProvinceFromIndex(reg->regionIndexes[b]));
        box = (p->getCornerBox());
        for (int n = 0; n < 4; n++)
        {
            std::pair<float, float> vec(rotatePairVector(box[n], angle));
            if (min.x() > vec.first)
                min.setX(vec.first);
            if (min.y() > vec.second)
                min.setY(vec.second);
            if (max.x() < vec.first)
                max.setX(vec.first);
            if (max.y() < vec.second)
                max.setY(vec.second);
        }
    }
/*
    reg->corners[0] = min;
    reg->corners[1] = QVector2D(min.x(), max.y());
    reg->corners[2] = max;
    reg->corners[3] = QVector2D(max.x(), min.y());*/
    *outMin = min;
    *outMax = max;
}

bool boxContains(const std::pair<float, float>* box, QVector2D point)
{
    if (box[0].first <= point.x() && box[1].first >= point.x() && box[0].second <= point.y() && box[1].second >= point.y())
        return true;
    return false;

}

QVector2D pairToVector(std::pair<float, float> t)
{
    return QVector2D(t.first, t.second);
}

void populateEdges(std::vector<QVector2D>* edges, std::vector<QVector2D>* centers, ConnectedRegions* reg, const ProvincesMask* mask)
{
   for (unsigned a = 0; a < reg->regionIndexes.size(); a++)
   {
        const Province* p(mask->getMap()->getProvinceFromIndex(reg->regionIndexes[a]));
        for (int b = 0; b < p->getSubProvinceCount(); b++)
        {
            const SubProvince* sub(p->getSubProvince(b));
            for (unsigned c = 0; c < sub->getBordersData()->size(); c++)
            {
                NeighbourData data(sub->getBordersData()->at(c));
                if (std::find(reg->regionIndexes.begin(), reg->regionIndexes.end(), data.neighbourIndex) == reg->regionIndexes.end())
                {
                    auto extremes(sub->getCornerBox());
                    QVector2D center(extremes[0].first+extremes[2].first, extremes[0].second+extremes[2].second);
                    center = center/2;
                    centers->push_back(center);
                    edges->push_back(pairToVector(sub->getBorders()->at(data.borderStart)));
                    edges->push_back(pairToVector(sub->getBorders()->at(data.borderEnd)));
                }
            }
        }

   }
   /* for (unsigned a = 0; a < reg->regionIndexes.size(); a++)
    {
        const Province* p(mask->getMap()->getProvinceFromIndex(reg->regionIndexes[a]));
        QVector2D corners[4];

        //corners[0] = QVector2D(p->getBoudingBox()[0].first, p->getBoudingBox()[0].second);
        //corners[1] = QVector2D(p->getBoudingBox()[0].first, p->getBoudingBox()[1].second);
        //corners[2] = QVector2D(p->getBoudingBox()[1].first, p->getBoudingBox()[1].second);
        //corners[3] = QVector2D(p->getBoudingBox()[1].first, p->getBoudingBox()[0].second);

        for (int a = 0; a < 4; a++)
           corners[a] = QVector2D(p->getCornerBox()[a].first, p->getCornerBox()[a].second);

        std::vector<const Province*> ls;
        p->getNeighbours(&ls);


        for (int a = 0; a < 4; a++)
        {
            bool liesInsideOne(false);
            bool fullySurounded(true);
            for (unsigned i = 0; i < ls.size(); i++)
            {
                const Province* otherProv(ls[i]);
                if (std::find(reg->regionIndexes.begin(), reg->regionIndexes.end(), otherProv->getIndex()) == reg->regionIndexes.end())
                {
                    fullySurounded = false;
                    continue;
                }
                if (boxContains(otherProv->getBoudingBox(), corners[a]) || boxContains(otherProv->getBoudingBox(), corners[(a+1)%4]))
                {
                    liesInsideOne = true;
                }
            }

            if (!liesInsideOne && !fullySurounded)
            {
                edges->push_back(corners[a]);
                edges->push_back(corners[(a+1)%4]);
                centers->push_back((corners[a] + corners[(a+2)%4])/2);
            }
        }
    }
    */
}

struct box
{
    QVector2D corners[4];
};

bool areOnSameSide(QVector2D line1, QVector2D line2, QVector2D point1, QVector2D point2)
{
    float val1((line2.x()-line1.x())*(point1.y()-line1.y()));
    val1 -= ((point1.x()-line1.x())*(line2.y()-line1.y()));
    float val2((line2.x()-line1.x())*(point2.y()-line1.y()));
    val2 -= ((point2.x()-line1.x())*(line2.y()-line1.y()));

    if (val1*val2 > 0)
        return true;
    return false;
}

void reduceSingle(QVector2D first, QVector2D second, QVector2D center, QVector2D *min, QVector2D* max, QVector2D originalMin, QVector2D originalMax)
{
    float oldDistance(100000);
    int target(0);
    bool minOnSameSide(areOnSameSide(first, second, center, *min));
    bool maxOnSameSide(areOnSameSide(first, second, center, *max));
    if (first.x() - originalMin.x() < oldDistance && !minOnSameSide)
    {
        oldDistance = first.x() - originalMin.x();
    }
    if (originalMax.x() - first.x() < oldDistance && !maxOnSameSide)
    {
        target = 1;
        oldDistance = originalMax.x() - first.x();
    }
    if (originalMax.y() - first.y() < oldDistance && !maxOnSameSide)
    {
        target = 2;
        oldDistance = originalMax.y() - first.y();
    }
    if (first.y() - originalMin.y() < oldDistance && !minOnSameSide)
    {
        target = 3;
        oldDistance = first.y() - originalMin.y();
    }

    if (target == 0 && !minOnSameSide)
        min->setX(first.x());
    else
    if (target == 1)
        max->setX(first.x());
    else
    if (target == 2)
        max->setY(first.y());
    else
     if (target == 3)
         min->setY(first.y());

}

void reduce(QVector2D first, QVector2D second, QVector2D center, QVector2D *min, QVector2D* max, QVector2D originalMin, QVector2D originalMax)
{
    bool firstIsOut(first.x() < min->x() || first.y() < min->y() || first.y() > max->y() || first.x() > max->x());
    bool secondIsOut(second.x() < min->x() || second.y() < min->y() || second.y() > max->y() || second.x() > max->x());

    if (!firstIsOut)
    {
        reduceSingle(first, second, center, min, max, originalMin, originalMax);
    }

    if (!secondIsOut)
    {
        reduceSingle(second, first, center, min, max, originalMin, originalMax);

    }
}

void searchForBox(ConnectedRegions* reg, const ProvincesMask* mask, box* outBox, float angle)
{
    QVector2D min, max;
    QVector2D originalMin, originalMax;
    generateBoundingBox(reg, mask, &min, &max, angle);

    originalMax = max;
    originalMin = min;
    for (unsigned a = 0; a < reg->edges.size(); a = a + 2)
    {
         QVector2D first(rotateQVector(reg->edges[a], angle));
         QVector2D second(rotateQVector(reg->edges[a+1], angle));
         QVector2D center(rotateQVector(reg->edges[a/2], angle));

        reduce(first, second, center ,&min, &max, originalMin, originalMax);
    }



    outBox->corners[0] = rotateQVector(min, -1*angle);
    outBox->corners[1] = rotateQVector(QVector2D(min.x(), max.y()), -1*angle);
    outBox->corners[2] = rotateQVector(max, -1*angle);
    outBox->corners[3] = rotateQVector(QVector2D(max.x(), min.y()), -1*angle);
}
float evalFuncion(float l1, float l2)
{
    float t(l1/l2);
    if (t < 1)
        t = 1/t;
    return (l1*l2)/(t);
}

void generateBox(ConnectedRegions* reg, const ProvincesMask* mask)
{
    populateEdges(&reg->edges, &reg->centers, reg, mask);
    if (reg->edges.size() < 2)
        return;


    box b, old;
    searchForBox(reg, mask, &old, 0);
    QVector2D v(old.corners[0] - old.corners[1]);
    QVector2D v2(old.corners[1] - old.corners[2]);
    float oldDim(evalFuncion(v.length(), v2.length()));
    oldDim = std::abs(oldDim);
    for (int a = 1; a < 30; a++)
    {
        searchForBox(reg, mask, &b, std::acos(-1)/30.0f*a);
        v = b.corners[0] - b.corners[1];
        v2 = b.corners[1] - b.corners[2];
        float dim(evalFuncion(v.length(), v2.length()));
        if (oldDim < dim)
        {
            oldDim = dim;
            old = b;
        }
    }
    for (int a = 0; a < 4; a++)
        reg->corners[a] = old.corners[a];

}

void NamePlacer::generateBoxes(const ProvincesMask* mask)
{
    std::cout << division.size() << std::endl;
	for (unsigned a = 0; a < division.size(); a++)
	{
		ConnectedRegions* reg(&division[a]);	
		if (reg->regionIndexes.size() == 0)
		{
			for (int c = 0; c < 4; c++)
				reg->corners[c] = QVector2D(0, 0);
			continue;
		}
        if (reg->regionIndexes.size() == 1)
        {
            const Province* p(mask->getMap()->getProvinceFromIndex(reg->regionIndexes[0]));
            auto box(p->getCornerBox());
            for (int c = 0; c < 4; c++)
            {
                reg->corners[c] = QVector2D(box[c].first, box[c].second);
            }
            continue;

        }

        generateBox(reg, mask);
	}	
}
