#include "nameplacer.h"
#include "provincesmask.h"
#include "mapreader/map.h"
#include "mapreader/province.h"
#include <iostream>

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

void generateBox(ConnectedRegions* reg, const ProvincesMask* mask)
{
    const Province* p(mask->getMap()->getProvinceFromIndex(reg->regionIndexes[0]));
    auto box(p->getCornerBox());

    QVector2D min(box[0].first, box[0].second);
    QVector2D max(box[2].first, box[2].second);

    for (unsigned b = 0; b < reg->regionIndexes.size(); b++)
    {
        p = (mask->getMap()->getProvinceFromIndex(reg->regionIndexes[b]));
        box = (p->getCornerBox());
        for (int n = 0; n < 4; n++)
        {
            if (min.x() > box[n].first)
                min.setX(box[n].first);
            if (min.y() > box[n].second)
                min.setY(box[n].second);
            if (max.x() < box[n].first)
                max.setX(box[n].first);
            if (max.y() < box[n].second)
                max.setY(box[n].second);
        }
    }

    reg->corners[0] = min;
    reg->corners[1] = QVector2D(min.x(), max.y());
    reg->corners[2] = max;
    reg->corners[3] = QVector2D(max.x(), min.y());
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
