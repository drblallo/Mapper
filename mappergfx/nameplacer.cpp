#include "nameplacer.h"
#include "provincesmask.h"
#include "mapreader/map.h"
#include "mapreader/province.h"
#include <iostream>
#include "mapreader/subprovince.h"

using namespace mappergfx;
using namespace mapreader;

void NamePlacer::insertAllNeighbours
(
        const std::vector<Province*>* provinceList,
        ConnectedRegions* region,
        int currentInserting
)
{
	std::vector<const Province*> justAdded;
	std::vector<const Province*> support;
	Province* p(provinceList->at(currentInserting));
	justAdded.push_back(p);
    placed.at(currentInserting) = true;
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
                if (!placed.at(index) && otherCol == col)
				{
					justAdded.push_back(neighbours[b]);
					region->regionIndexes.push_back(index);
                    placed.at(index) = true;
				}
			}
		}

	}
	while (!justAdded.empty());
}

NamePlacer::NamePlacer(const ProvincesMask* msk) : mask(msk)
{
	const std::vector<Province*>* provinceList(mask->getMap()->getProvincesList());

	for (unsigned a = 0; a < provinceList->size(); a++)
		placed.push_back(false);

	for (unsigned a = 1; a < placed.size(); a++)
	{
		division.push_back(ConnectedRegions());
        insertAllNeighbours(provinceList, &division.back(), a);

        while (placed[a+1] && a+1 < placed.size())
			a++;
	}	

    generateBoxes();
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
}


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
    if (std::abs(first.x() - originalMin.x()) < oldDistance && !minOnSameSide)
    {
        oldDistance = std::abs(first.x() - originalMin.x());
    }
    if (std::abs(originalMax.x() - first.x()) < oldDistance && !maxOnSameSide)
    {
        target = 1;
        oldDistance = std::abs(originalMax.x() - first.x());
    }
    if (std::abs(originalMax.y() - first.y()) < oldDistance && !maxOnSameSide)
    {
        target = 2;
        oldDistance = std::abs(originalMax.y() - first.y());
    }
    if (std::abs(first.y() - originalMin.y()) < oldDistance && !minOnSameSide)
    {
        target = 3;
        oldDistance = std::abs(first.y() - originalMin.y());
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
void rotate( QVector2D* vals, unsigned ammount)
{

    for (unsigned a = 0; a < ammount; a++)
    {
        QVector2D supp(vals[0]);
        for (int b = 0; b < 3; b++)
                vals[b] = vals[b+1];
        vals[3] = supp;
    }
}

void NamePlacer::searchForBox(ConnectedRegions* reg, box* outBox, float angle)
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
         QVector2D center(rotateQVector(reg->centers[a/2], angle));

        reduce(first, second, center ,&min, &max, originalMin, originalMax);
    }



    outBox->corners[0] = rotateQVector(min, -1*angle);
    outBox->corners[1] = rotateQVector(QVector2D(min.x(), max.y()), -1*angle);
    outBox->corners[2] = rotateQVector(max, -1*angle);
    outBox->corners[3] = rotateQVector(QVector2D(max.x(), min.y()), -1*angle);

}
float evalFuncion(float l1, float l2)
{
    float t(std::abs(l1/l2));
    if (t < 1)
        t = 1/t;
    return (l1*l2)/t;
}

bool NamePlacer::divisionContains(QVector2D vec, ConnectedRegions* reg)
{

        bool contains(false);
        for (unsigned c = 0; c < reg->regionIndexes.size(); c++)
        {
            const Province* p(mask->getMap()->getProvinceFromIndex(reg->regionIndexes[c]));
            const std::pair<float, float>* bounding(p->getBoudingBox());
            if (bounding[0].first < vec.x() &&
                bounding[1].first > vec.x() &&
                bounding[0].second < vec.y() &&
                bounding[1].second > vec.y())
            {
                contains = true;
                break;
            }
        }
        return contains;
}

bool NamePlacer::isResonable(ConnectedRegions* reg, box& b)
{

    for (int a = 0; a < 4; a++)
    {
        if (!divisionContains(b.corners[a], reg))
            return false;
        if (!divisionContains((b.corners[a]+b.corners[(a+1)%4])/2, reg))
            return false;
    }
    if (!divisionContains(((b.corners[0]+b.corners[2])/2), reg))
        return false;
    return true;
}

void NamePlacer::generateBox(ConnectedRegions* reg)
{
    populateEdges(&reg->edges, &reg->centers, reg, mask);
    if (reg->edges.size() < 2)
        return;


    box b, old;
    //searchForBox(reg, &old, 0);
    for (int a = 0; a < 4; a++)
        old.corners[a] = reg->corners[a];
    QVector2D v(old.corners[0] - old.corners[1]);
    QVector2D v2(old.corners[1] - old.corners[2]);
    //float oldDim(evalFuncion(v.length(), v2.length()));
    float oldDim(-1);
  //  oldDim = std::abs(oldDim);
    for (int a = 0; a < 30; a++)
    {
        searchForBox(reg, &b, std::acos(-1)/30.0f*a);
        v = b.corners[0] - b.corners[1];
        v2 = b.corners[1] - b.corners[2];
        float dim(evalFuncion(v.length(), v2.length()));
        dim = std::abs(dim);
        if (oldDim < dim && isResonable(reg, b))
        {
            oldDim = dim;
            old = b;
        }
    }
    for (int a = 0; a < 4; a++)
    {
        reg->corners[a] = old.corners[a];
    }

}

void fixOrientation(ConnectedRegions* outBox)
{
    for (int a = 0; a < 4; a++)
        outBox->corners[a] = QVector2D(outBox->corners[a].x(), -1*outBox->corners[a].y());

    QVector2D edge1(outBox->corners[0] - outBox->corners[1]);
    QVector2D edge2(outBox->corners[1] - outBox->corners[2]);

    if (edge1.x()== 0 || edge2.x() == 0)
        return;

    if (edge1.length() > edge2.length())
    {
        float p1 (-1.0f*outBox->corners[0].x()/edge1.x());
        float y1 (outBox->corners[0].y() + (edge1.y()*p1));

        float p2 (-1.0f*outBox->corners[2].x()/edge1.x());
        float y2 (outBox->corners[2].y() + (edge1.y()*p2));

        if (y1 < y2)
           rotate(outBox->corners, 3);
        else
           rotate(outBox->corners, 1);
    }
    else
    {
        float p1 (-1.0f*outBox->corners[1].x()/edge2.x());
        float y1 (outBox->corners[1].y() + (edge2.y()*p1));

        float p2 (-1.0f*outBox->corners[3].x()/edge2.x());
        float y2 (outBox->corners[3].y() + (edge2.y()*p2));

        if (y1 > y2)
           rotate(outBox->corners, 2);
    }
}

void NamePlacer::generateBoxes()
{
    for (unsigned a = 0; a < division.size(); a++) //foreach group of touching province with the same color
	{
        ConnectedRegions* reg(&division[a]);
        reg->name = "ERROR";
        if (reg->regionIndexes.size() == 0) //if there are no provinces inside that, something went wrong
		{
			for (int c = 0; c < 4; c++)
				reg->corners[c] = QVector2D(0, 0);
            fixOrientation(reg);
            continue;
		}
        reg->name = mask->getName(reg->regionIndexes.at(0));

        const Province* p(mask->getMap()->getProvinceFromIndex(reg->regionIndexes[0]));
        auto box(p->getCornerBox());
        for (int c = 0; c < 4; c++)
        {
            reg->corners[c] = QVector2D(box[c].first, box[c].second);
        }


        generateBox(reg); //else generate that box
        fixOrientation(reg);
	}	
}
