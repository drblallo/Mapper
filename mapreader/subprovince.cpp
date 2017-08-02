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

	generateLargestInscribedBox();
}

float areaOfBox(const std::pair<float, float>* box)
{
	std::pair<float, float> edgeOne(box[0].first-box[1].first, box[0].second-box[1].second);
	std::pair<float, float> edgeTwo(box[1].first-box[2].first, box[1].second-box[2].second);

	float lenghtOne((edgeOne.first*edgeOne.first) + (edgeOne.second*edgeOne.second));
	float lenghtTwo((edgeTwo.first*edgeTwo.first) + (edgeTwo.second*edgeTwo.second));
	lenghtOne = std::sqrt(lenghtOne);
	lenghtTwo = std::sqrt(lenghtTwo);

	return lenghtOne * lenghtTwo;
}

std::pair<float, float> rotateVector(std::pair<float, float> vector, float angle)
{
	float x((vector.first * std::cos(angle)) -( vector.second * std::sin(angle)));
	float y((vector.first * std::sin(angle)) + (vector.second * std::cos(angle)));
	return std::pair<float, float>(x, y);
}


void SubProvince::generateBoxWithOrientation(std::pair<float, float>* outBox, float angle)
{
	std::vector<std::pair<float, float> > modifiedBorders;
	for (unsigned a = 0 ;a < border.size(); a++)
		modifiedBorders.push_back(rotateVector(border[a], angle));

	std::pair<float, float> min (modifiedBorders[0]);
	std::pair<float, float> max (modifiedBorders[0]);

	for (unsigned a = 1; a < modifiedBorders.size(); a++)
	{
		std::pair<float, float> curr (modifiedBorders[a]);
		if (min.first > curr.first)
			min.first = curr.first;
		if (min.second > curr.second)
			min.second = curr.second;

		if (max.first < curr.first)
			max.first = curr.first;
		if (max.second < curr.second)
			max.second= curr.second;
		
	}

	std::pair<float, float> omin (min);
	std::pair<float, float> omax (max);
	for (unsigned a = 0; a < border.size(); a++)
	{
		std::pair<float, float> curr (modifiedBorders[a]);
		
		if (curr.first > min.first && curr.second > min.second && curr.first < max.first && curr.second < max.second)
		{
			float distance[4];
			distance[0] = (curr.first - omin.first);
			distance[1] = (curr.second -omin.second);
			distance[2] = (omax.first - curr.first);
			distance[3] = (omax.second- curr.second);

			int minDistance(0);
			for (int b = 1; b < 4; b++)
				if (distance[b] < distance[minDistance])
					minDistance = b;

			if (minDistance == 0)
				min.first = curr.first;
			else
			if (minDistance == 1)
				min.second = curr.second;
			else
			if (minDistance == 2)
				max.first = curr.first;
			else
			if (minDistance == 3)
				max.second = curr.second;
		}
			
	}

	outBox[0] = std::pair<float, float>(min.first, min.second);	
	outBox[1] = std::pair<float, float>(min.first, max.second);	
	outBox[2] = std::pair<float, float>(max.first, max.second);	
	outBox[3] = std::pair<float, float>(max.first, min.second);	


	for (int a = 0; a < 4; a++)
	{
		outBox[a] = rotateVector(outBox[a], -1*angle);
	}


}

void SubProvince::generateLargestInscribedBox()
{
	std::pair<float, float> box[4];
	std::pair<float, float> largestBox[4];
	generateBoxWithOrientation(largestBox, 0);
	for (int a = 1; a < 18; a++)
	{
		generateBoxWithOrientation(box, 10*a);	
		if (areaOfBox(box) > areaOfBox(largestBox))
			for (int b = 0; b < 4; b++)
				largestBox[b] = box[b];
	}
	for (int b = 0; b < 4; b++)
		boxCorners[b] = largestBox[b];
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

void SubProvince::updateNeighbourData()
{
	for (unsigned a = 0; a < borderNeighbours.size(); a++)
	{
		NeighbourData* data(&borderNeighbours[a]);
		data->neighbourIndex = neighbours.find(data->neighbourID)->second->getIndex();
	}
}

float SubProvince::getBoxSize() const
{
	return areaOfBox(getCornerBox());
}
