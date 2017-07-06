#include "subprovince.h"
#include "province.h"
#include "map.h"
#define SPEAK
#include "mapreaderdefines.h"

using namespace mapreader;

bool SubProvince::isNeinghbourOf(Province& p) const
{
	auto iter(neighbours.find(p.getId()));
	return (iter != neighbours.end());
}

SubProvince::SubProvince
	(
		Province* p,
	   	const std::vector<position>& borderIn,
	   	QRgb** source,
	   	QImage& image
	)
	: parent(p)
{
	for (unsigned a = 0; a < borderIn.size(); a++)
	{
		border.push_back(borderIn[a]);
		addNeighbours(borderIn.at(a).first, borderIn.at(a).second, source, image);
	}

	for (auto t(neighbours.begin()); t != neighbours.end(); t++)
	{
		QRgb id(t->second->getColor());	
		for (unsigned a = 0; a < borderIn.size(); a++)
		{
			unsigned val(a);
			while (touchesNeighbour(borderIn[a].first, borderIn[a].second, id, source, image))	
			{
				a++;
				if (a >= borderIn.size())
					break;
			}
			if (val < a)
			{
				NeighbourData data;
				data.borderStart = val;
				data.borderEnd = a - 1;
				data.neighbourID = t->first;
				WRITE("created a contourn line");
				WRITE(data.borderStart);
				WRITE(data.borderEnd);
				WRITE(data.neighbourID);
				
				borderNeighbours.push_back(data);
			}
		}
	}
}

bool SubProvince::touchesNeighbour(int x, int y, QRgb col, QRgb** source, QImage& image)
{
	for (int a = -1; a < 2; a++)
	{
		for (int b = -1; b < 2; b++)
		{
			if (x+b >= image.width() || x+b < 0 || y+a < 0 || y+a >= image.height())
				continue;

			if (source[y+a][x+b] == col)
				return true;
		}
	}
	return false;
}

void SubProvince::addNeighbours(int x, int y, QRgb** source, QImage& image)
{
	for (int a = -1; a < 2; a++)
	{
		for (int b = -1; b < 2; b++)
		{
			if (x+b >= image.width() || x+b < 0 || y+a < 0 || y+a >= image.height())
				continue;

			if (source[y][x] != source[y+a][x+b])
			{

				const Province* p(parent->getMap()->getProvince(source[y+a][x+b]));
				if (!p)
					continue;
				if (neighbours.find(p->getId()) == neighbours.end())	
				{
					neighbours.insert(std::pair<int,const Province*>(p->getId(), p));
					WRITE("found another neighbour");
				}
			}
		}
	}
}

std::vector<NeighbourData> SubProvince::getBorderWith(long id) const
{
	std::vector<NeighbourData> out;	
	for (unsigned a = 0; a < borderNeighbours.size(); a++)
	{
		if (borderNeighbours[a].neighbourID == id)
			out.push_back(borderNeighbours[a]);
	}
	return out;
}
